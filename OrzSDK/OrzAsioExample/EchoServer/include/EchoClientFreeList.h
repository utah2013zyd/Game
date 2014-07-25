#ifndef __Orz_EchoServer_EchoClientFreeList_h_
#define __Orz_EchoServer_EchoClientFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <EchoServerPreDeclare.h>
namespace Orz
{
class EchoClientFreeList
{
	EchoClientFreeList() {}
	EchoClientFreeList& operator=(const EchoClientFreeList& _other);
public:
	~EchoClientFreeList() {}

public:
	static EchoClientFreeList& instance()
	{
		static EchoClientFreeList singleton;
		return singleton;
	}

public:
	ORZ_PLUS_INLINE
	void				set				(IFreeListShrPtr _freeList)
	{
		if (m_freeList)
			return;

		m_freeList = _freeList;
	}

	ORZ_PLUS_INLINE
	EchoClient*			alloc			()
	{
		if (!m_freeList)
			return 0;

		return (EchoClient*)m_freeList->alloc();
	}

	ORZ_PLUS_INLINE
	void				free			(IClient* _ptr)
	{
		if (!m_freeList)
			return;

		m_freeList->free(_ptr);
	}

private:
	IFreeListShrPtr			m_freeList;
};

#define sEClFList EchoClientFreeList::instance()

class EchoClientDeleter
{
public:
	ORZ_PLUS_INLINE EchoClientDeleter() {}
	ORZ_PLUS_INLINE ~EchoClientDeleter() {}

	ORZ_PLUS_INLINE void operator()(IClient* _ptr)
	{
		sEClFList.free(_ptr);
	}
};

ORZ_PLUS_INLINE
EchoClientShrPtr allocEchoClient()
{
	EchoClientShrPtr ecl(sEClFList.alloc(), EchoClientDeleter());
	ecl->clear();
	return ecl;
}
}
#endif
