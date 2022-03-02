#include "Ext2Inode.hpp"

#include <Filesystem/Base/VNode.hpp>

namespace Kernel::FS::EXT2 {

Ext2Inode::Ext2Inode(FS* fs, uint32_t inode, inode_t* inode_struct)
    : VNode(fs, inode)
    , m_inode_struct(inode_struct)
{
    if (!m_inode_struct) {
        m_inode_struct = new inode_t {};
    }
}

Ext2Inode::~Ext2Inode()
{
    delete m_inode_struct;
}

uint32_t Ext2Inode::size() const
{
    return m_inode_struct->size;
}

void Ext2Inode::lookup_derived(Dentry& dentry)
{
    dentry.set_vnode(static_cast<Ext2*>(fs())->finddir(*this, dentry.name()));
}

}