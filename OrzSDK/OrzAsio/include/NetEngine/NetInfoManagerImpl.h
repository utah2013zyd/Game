#ifndef __Orz_OrzAsio_NetInfoManagerImpl_h__
#define __Orz_OrzAsio_NetInfoManagerImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Counter.hpp>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
namespace Orz 
{
class _OrzOrzAsioPrivate NetInfoManagerImpl
{
	typedef boost::shared_ptr<HashCounter<NetPort> > HashCounterStrPtr;
public:
	ORZ_PLUS_INLINE
	NetInfoManagerImpl(uint32 _cBase)
		: m_counter(_cBase)
		, m_inConnections(_cBase)
		, m_outConnections(_cBase)
	{
		m_counter.setHash();
		m_inConnections.setHash();
		m_outConnections.setHash();
	}

	ORZ_PLUS_INLINE
	~NetInfoManagerImpl()
	{
	}

public:
	ORZ_PLUS_INLINE
	uint32				getSocketNum				() const
	{
		return m_counter.get();
	}

	ORZ_PLUS_INLINE
	uint32				getInConnectionNum			() const
	{
		uint32 r = 0;
		uint32 arraySize = m_inConnections.arraySize();
		for (uint32 i=0; i<arraySize; ++i)
		{
			ThreadShrMap<NetPort, HashCounterStrPtr, std::less<NetPort> >& tm = m_inConnections.threadMap(i);
			DynShrLock lock(tm.mutex());
			std::map<NetPort, HashCounterStrPtr, std::less<NetPort> >& m = tm.map();
			std::map<NetPort, HashCounterStrPtr, std::less<NetPort> >::iterator itr = m.begin();
			for (; itr!=m.end(); ++itr)
			{
				if (itr->second)
				{
					r += itr->second->get();
				}
			}
		}
		return r;
	}

	ORZ_PLUS_INLINE
	uint32				getOutConnectionNum			() const
	{
		uint32 r = 0;
		uint32 arraySize = m_outConnections.arraySize();
		for (uint32 i=0; i<arraySize; ++i)
		{
			ThreadShrMap<NetPort, HashCounterStrPtr, std::less<NetPort> >& tm = m_outConnections.threadMap(i);
			DynShrLock lock(tm.mutex());
			std::map<NetPort, HashCounterStrPtr, std::less<NetPort> >& m = tm.map();
			std::map<NetPort, HashCounterStrPtr, std::less<NetPort> >::iterator itr = m.begin();
			for (; itr!=m.end(); ++itr)
			{
				if (itr->second)
				{
					r += itr->second->get();
				}
			}
		}
		return r;
	}

	ORZ_PLUS_INLINE
	uint32				getInConnectionNum			(NetPort _port) const
	{
		HashCounterStrPtr incNum = m_inConnections.get(_port, _port);
		if (!incNum)
		{
			return 0;
		}
		else
		{
			return (uint32)incNum->get();
		}
	}

	ORZ_PLUS_INLINE
	uint32				getOutConnectionNum			(NetPort _port) const
	{
		HashCounterStrPtr outNum = m_outConnections.get(_port, _port);
		if (!outNum)
		{
			return 0;
		}
		else
		{
			return (uint32)outNum->get();
		}
	}

	ORZ_PLUS_INLINE
	void				increaseSocket				()
	{
		NetPort i = 0;
		PtrIntType pit = (PtrIntType)&i;
		m_counter.inc((NetPort)pit);
	}

	ORZ_PLUS_INLINE
	void				decreaseSocket				()
	{
		NetPort i = 0;
		PtrIntType pit = (PtrIntType)&i;
		m_counter.dec((NetPort)pit);
	}

	ORZ_PLUS_INLINE
	void				increaseIn					(NetPort _port)
	{
		m_inConnections.lockWrite(_port, _port);
		HashCounterStrPtr incNum = m_inConnections.get(_port, _port, false);
		if (incNum)
		{
			incNum->inc(_port);
		}
		else
		{
			HashCounterStrPtr hc(new HashCounter<NetPort>(m_inConnections.base()));
			hc->setHash();
			hc->inc(_port);
			m_inConnections.add(_port, hc, _port, false);
		}
		m_inConnections.unlockWrite(_port, _port);
		increaseSocket();
	}

	ORZ_PLUS_INLINE
	void				decreaseIn					(NetPort _port)
	{
		m_inConnections.lockWrite(_port, _port);
		HashCounterStrPtr incNum = m_inConnections.get(_port, _port, false);
		if (incNum)
		{
			incNum->dec(_port);
		}
		m_inConnections.unlockWrite(_port, _port);
		decreaseSocket();
	}

	ORZ_PLUS_INLINE
	void				increaseOut					(NetPort _port)
	{
		m_outConnections.lockWrite(_port, _port);
		HashCounterStrPtr outNum = m_outConnections.get(_port, _port, false);
		if (outNum)
		{
			outNum->inc(_port);
		}
		else
		{
			HashCounterStrPtr hc(new HashCounter<NetPort>(m_outConnections.base()));
			hc->setHash();
			hc->inc(_port);
			m_outConnections.add(_port, hc, _port, false);
		}
		m_outConnections.unlockWrite(_port, _port);
		increaseSocket();
	}

	ORZ_PLUS_INLINE
	void				decreaseOut					(NetPort _port)
	{
		m_outConnections.lockWrite(_port, _port);
		HashCounterStrPtr outNum = m_outConnections.get(_port, _port, false);
		if (outNum)
		{
			outNum->dec(_port);
		}
		m_outConnections.unlockWrite(_port, _port);
		decreaseSocket();
	}

private:
	HashCounter<NetPort>							m_counter;
	mutable HashShrMap<NetPort, HashCounterStrPtr>	m_inConnections;
	mutable HashShrMap<NetPort, HashCounterStrPtr>	m_outConnections;
};
}
#endif
