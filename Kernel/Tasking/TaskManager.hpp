#pragma once
#include <Libkernel/Assert.hpp>
#include <Filesystem/VFS/VFS.hpp>
#include <Memory/vmm.hpp>
#include "../Memory/vmm.hpp"

#include "Process.hpp"
#include "Thread.hpp"

#include <Drivers/PIT.hpp>

#include <Macaronlib/List.hpp>
#include <Macaronlib/Singleton.hpp>

namespace Kernel::Tasking {

using namespace Drivers;

#define DEFAULT_BSS_SIZE 4096
#define DEFAULT_HEAP_SIZE 4096

class ProcessStorage;
class Process;
class Thread;

class TaskManager : public Singleton<TaskManager>, public TickReciever {
    friend class Process;
public:
    TaskManager();
    bool run();

    // syscalls handlers
    void sys_exit_handler(int error_code);
    int sys_fork_handler();
    int sys_execve_handler(const char* filename, const char* const* argv, const char* const* envp);

    void create_process(const String& filepath);

    Thread* cur_thread();
    Process* cur_process();

public:
    void on_tick(Trapframe* tf) override;

public:
    // TODO: support kernel processes / threads
    ProcessStorage* m_process_storage {};
    List<Thread*> m_threads {};
    List<Thread*>::Iterator<ListNode<Thread*>> m_cur_thread { m_threads.end() };
};

}
