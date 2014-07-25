#ifndef __Orz_OrzAsio_AsioManagerImpl_h__
#define __Orz_OrzAsio_AsioManagerImpl_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Agent/Agent.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/DynLib/MTDynLibMgr.h>
#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
namespace Orz
{
class _OrzOrzAsioPrivate AsioManagerImpl
{
public:
	ORZ_PLUS_INLINE
	AsioManagerImpl(uint32 _dynLibMgrHash, uint32 _pluginsMgrHash)
		: m_bHasInited(false)
		, m_isBlock(false)
		, m_dynLibMgr(_dynLibMgrHash)
		, m_pluginsMgr(_pluginsMgrHash, &m_dynLibMgr)
		, m_end(false)
	{ 
	}

	ORZ_PLUS_INLINE
	~AsioManagerImpl(void)
	{
	}

public:
	bool				initialize			(IServiceManager* _svcMgr)
	{
		if (m_bHasInited)
		{
			sLogger->out(OL_DEBUG, "OrzAsio system has inited!", 0);
			return false;
		}

		if (!_svcMgr)
		{
			return false;
		}

		m_svcMgr = _svcMgr;
		return setup();
	}

	ORZ_PLUS_INLINE
	bool				boost				(bool _isBlock)              
	{
		if (!m_svcMgr)
		{
			return false;
		}

		m_pluginsMgr.boost();

		m_isBlock = _isBlock;
		sLogger->out(OL_INFO, "OrzAsio system started!", 0);
		if (m_isBlock)
		{
			run();
		}
		else
		{
			if (m_netAgent.start() == 0)
			{
				m_netAgent.post(boost::bind(&AsioManagerImpl::run, this));
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	ORZ_PLUS_INLINE
	void				join				()
	{
		m_pluginsMgr.join();
		if (!m_isBlock)
		{
			m_netAgent.join();
		}
	}

	ORZ_PLUS_INLINE
	IServiceManager*	getSvcMgr			()
	{
		return m_svcMgr;
	}

	ORZ_PLUS_INLINE
	MTDynLibMgr*		getDynLibMgr		()
	{
		return &m_dynLibMgr;
	}

	ORZ_PLUS_INLINE
	MTPluginMgr*		getPluginsMgr		()
	{
		return &m_pluginsMgr;
	}

	ORZ_PLUS_INLINE
	void				shutdown			(void)
	{
		sLogger->out(OL_INFO, "OrzAsio system shutting down...", 0);
		m_pluginsMgr.shutdown();
		if (!m_svcMgr || m_end)
		{
			return;
		}

		m_svcMgr->shutdown();
		if (!m_isBlock)
		{
			m_netAgent.stop();
		}
		else
		{
			m_pluginsMgr.join();
		}

		sPkFList.clear();
		m_end = true;
	}

private:
	void				run					()
	{
		m_svcMgr->run();
	}

	bool				setup				()
	{
		m_bHasInited = true;
		return true;
	}

private:
	bool									m_bHasInited;
	bool									m_isBlock;
	Agent									m_netAgent;
	IServiceManager*						m_svcMgr;
	MTDynLibMgr								m_dynLibMgr;
	MTPluginMgr								m_pluginsMgr;
	bool									m_end;
};
}
#endif
