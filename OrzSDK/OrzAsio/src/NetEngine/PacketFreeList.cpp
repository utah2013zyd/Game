#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/IPacket.h>
using namespace Orz;


PacketDeleter::PacketDeleter()
{
}

PacketDeleter::~PacketDeleter()
{
}

void PacketDeleter::operator()(IPacket* _ptr)
{
	if (_ptr)
	{
		_ptr->clear();
	}
	sPkFList.free(_ptr);
}

PacketFreeList::PacketFreeList()
{
}

PacketFreeList::~PacketFreeList()
{
}

PacketFreeList& PacketFreeList::instance()
{
	static PacketFreeList singleton;
	return singleton;
}

void PacketFreeList::set(IFreeListShrPtr _freeList)
{
	if (m_freeList)
	{
		return;
	}

	m_freeList = _freeList;
}

IPacket* PacketFreeList::alloc()
{
	if (!m_freeList)
	{
		return 0;
	}

	return (IPacket*)m_freeList->alloc();
}

IPacketShrPtr PacketFreeList::select()
{
	return IPacketShrPtr(sPkFList.alloc(), PacketDeleter());
}

void PacketFreeList::free(IPacket* _ptr)
{
	if (!m_freeList)
	{
		return;
	}

	m_freeList->free(_ptr);
}

void PacketFreeList::clear()
{
	m_freeList = IFreeListShrPtr();
}
