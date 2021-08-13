#include "ServerConnection.hpp"
#include <libc/syscalls.hpp>
#include <libsys/Log.hpp>
#include <wisterialib/extras.hpp>
#include <wisterialib/posix/defines.hpp>

namespace IPC {

ServerConnection::ServerConnection(const String& endpoint)
    : m_pid(getpid())
{
    m_socket_fd = socket(AF_LOCAL, SOCK_STREAM, 1);
    int result = bind(m_socket_fd, endpoint.cstr());
    if (result < 0) {
        return;
    }
    m_alive = true;
}

void ServerConnection::pump()
{
    if (m_last_ipch_nead_read) {
        if (!read_by_header(m_last_ipch)) {
            return;
        }
        m_last_ipch_nead_read = false;
    }

    IPCHeader ipch;
    while (read(m_socket_fd, &ipch, sizeof(IPCHeader)) > 0) {
        // client attempts to initialize a coonection
        if (ipch.pid_to == -1) {
            IPCHeader ipchr;
            ipchr.pid_from = m_pid;
            ipchr.pid_to = ipch.pid_from;
            ipchr.size = 0;
            write(m_socket_fd, &ipchr, sizeof(IPCHeader));
            continue;
        }

        // skip messages that are not directed to us
        if (ipch.pid_to != m_pid) {
            lseek(m_socket_fd, ipch.size, SEEK_CUR);
            continue;
        }

        if (!read_by_header(ipch)) {
            m_last_ipch = ipch;
            m_last_ipch_nead_read = true;
            return;
        }
    }
}

void ServerConnection::send_data(void* data, size_t bytes, int pid_to)
{
    IPCHeader ipch;
    ipch.pid_from = m_pid;
    ipch.pid_to = pid_to;
    ipch.size = bytes;

    write(m_socket_fd, &ipch, sizeof(IPCHeader));
    write(m_socket_fd, data, bytes);
}

bool ServerConnection::read_by_header(const IPCHeader& ipch)
{
    auto memory = Vector<unsigned char>(ipch.size);
    int bytes = read(m_socket_fd, memory.data(), ipch.size);
    if (bytes <= 0) {
        return false;
    }
    m_message_queue.push_back({ ipch.pid_from, move(memory) });
    return true;
}

};