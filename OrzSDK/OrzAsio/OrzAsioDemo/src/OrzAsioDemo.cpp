#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <orz/OrzAsio/NetEngine/Packet.hpp>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <EchoConnection.h>

// for test
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
using namespace Orz;

#define HASH_COUNTER_BASE 10
void hashCounterHdr(uint32 _index, HashCounter<uint32>* _hc, Agent* _agt)
{
	if (_index >= HASH_COUNTER_BASE/2 - 1)
	{
		for (uint32 i=0; i<HASH_COUNTER_BASE*10; ++i)
		{
			_hc->inc(_index);
		}
	}
	else
	{
		for (uint32 i=0; i<HASH_COUNTER_BASE*10; ++i)
		{
			_hc->dec(_index);
		}
	}
	_agt->stop();
}

void test_HashCounter()
{
	HashCounter<uint32> hc(HASH_COUNTER_BASE);
	hc.setHash();

	DynArray<Agent> threads(HASH_COUNTER_BASE);
	for (uint32 i=0; i<HASH_COUNTER_BASE; ++i)
	{
		threads[i].start();
		threads[i].post(boost::bind(&hashCounterHdr, i, &hc, &(threads[i])));
	}

	for (uint32 i=0; i<threads.size(); ++i)
	{
		threads[i].join();
	}

	printf("HashCounter size: %d\n", hc.get());
	getchar();
}


#define HASH_MAP_BASE 10
void hashMapHdr(uint32 _index, HashMap<uint32, int32, uint32>* _hm, Agent* _agt)
{
	for (uint32 i=0; i<HASH_MAP_BASE*10; ++i)
	{
		_hm->add(_index*10+i, i, _index*10+i);
	}
	printf("Thread id: %d, hm add size: %d\n", _index, _hm->size());

	for (uint32 i=0; i<HASH_MAP_BASE*10; ++i)
	{
		_hm->erase(_index*10+i, _index*10+i);
	}
	printf("Thread id: %d, hm erase size: %d\n", _index, _hm->size());
	_agt->stop();
}

void hashShrMapHdr(uint32 _index, HashShrMap<uint32, int32, uint32>* _hsm, Agent* _agt)
{
	for (uint32 i=0; i<HASH_MAP_BASE*10; ++i)
	{
		_hsm->add(_index*10+i, i, _index*10+i);
	}
	printf("Thread id: %d, hsm add size: %d\n", _index, _hsm->size());

	for (uint32 i=0; i<HASH_MAP_BASE*10; ++i)
	{
		_hsm->erase(_index*10+i, _index*10+i);
	}
	printf("Thread id: %d, hsm erase size: %d\n", _index, _hsm->size());
	_agt->stop();
}

void test_HashMap()
{
	HashMap<uint32, int32, uint32> hm(HASH_MAP_BASE);
	hm.setHash();

	DynArray<Agent> threads(HASH_MAP_BASE);
	for (uint32 i=0; i<HASH_MAP_BASE; ++i)
	{
		threads[i].start();
		threads[i].post(boost::bind(&hashMapHdr, i, &hm, &(threads[i])));
	}

	for (uint32 i=0; i<threads.size(); ++i)
	{
		threads[i].join();
	}

	printf("HashMap size: %d\n\n", hm.size());


	// hash shr map
	HashShrMap<uint32, int32, uint32> hsm(HASH_MAP_BASE);
	hsm.setHash();

	for (uint32 i=0; i<HASH_MAP_BASE; ++i)
	{
		threads[i].start();
		threads[i].post(boost::bind(&hashShrMapHdr, i, &hsm, &(threads[i])));
	}

	for (uint32 i=0; i<threads.size(); ++i)
	{
		threads[i].join();
	}

	printf("HashShrMap size: %d\n", hsm.size());
	getchar();
}


#define LOGGER_THREAD_NUM 10
void loggerHdr(uint32 _index, Agent* _agt)
{
	for (uint32 i=0; i<1; ++i)
	{
		sLogger->outFormat(OL_INFO, false, "Test outFormat1 time: [%llu], threadId: [%u]", now(), _index);
		sLogger->outFormat(OL_INFO, false, "Test outFormat2 time: [%llu], threadId: [%u]", now(), _index);
		sLogger->outFormat(OL_INFO, false, "Test outFormat3 time: [%llu], threadId: [%u]", now(), _index);
	}
	_agt->stop();
}

void test_Logger()
{
	sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));
	sLogger->boost();

	sLogger->out(OL_INFO, "test out", "111~", 0);
	sLogger->outByte(OL_DEBUG, "test outByte", 0, "test outByte", 5, 0);
	sLogger->outDBuff(OL_INFO, "test outDBuff", 0);
	sLogger->outByteDBuff(OL_DEBUG, "test outByteDBuff", 0, 0);
	
	LogShrPtr log = allocLog(OL_INFO, false);
	*log << "\ntest out with Log1, [" << 11 << "]\n";
	log->put("Test out with log2, %d\n", 11);
	*log << "test out with Log3, [" << 11 << "]\n";
	log->put("Test out with log4, %d\n", 11);
	sLogger->out(log);

	log = allocLog(OL_INFO, false);
	*log << "\ntest out using dbuff with Log1, [" << 11 << "]\n";
	log->put("Test out using dbuff with log2, %d\n", 11);
	*log << "test out using dbuff with Log3, [" << 11 << "]\n";
	log->put("Test out using dbuff with log4, %d\n\n", 11);
	sLogger->out(log);

	getchar();

	DynArray<Agent> threads(LOGGER_THREAD_NUM);
	for (uint32 i=0; i<LOGGER_THREAD_NUM; ++i)
	{
		threads[i].start();
		threads[i].post(boost::bind(&loggerHdr, i, &(threads[i])));
	}

	for (uint32 i=0; i<threads.size(); ++i)
	{
		threads[i].join();
	}

	getchar();

	sLogger->shutdown();
	sLogger->join();
}


#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN

boost::function0<void> console_ctrl_function;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
	switch (ctrl_type)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		console_ctrl_function();
		return TRUE;
	default:
		return FALSE;
	}
}

int main(void)
{
	// ¼ì²âÄÚ´æÐ¹Â¶
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//test_HashCounter();
	//test_HashMap();
	//test_Logger();
	//return 0;

	typedef boost::shared_ptr<FreeList<NetPacket> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));

		NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
		ServiceManager svcMgr(1,1,1, 1,1,3, 2,2,2,1, 10,10,10,10,10,10, pkFL);
		EchoConnection connection(&svcMgr, 3000);

		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			console_ctrl_function = boost::bind(&AsioManager::shutdown, sAsioMgr);
			SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

			// listen and SSL listen
			sSvcMgr(ServiceManager)->listen(7000, &connection);
			sAsioMgr->boost(true);
		}
		sAsioMgr->shutdown();
		sAsioMgr->join();
	}
	catch (OrzAsioException& _e)
	{
		std::vector<const char*> errs = _e.what();
		for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
		{
			sLogger->out(OL_DEBUG, *itr, 0);
		}
	}
	return 0;
}

#else
#include <pthread.h>	// we need pthread lib in posix

int main(void)
{
	//test_HashCounter();
	//test_HashMap();
	//test_Logger();
	//return 0;

	typedef boost::shared_ptr<FreeList<NetPacket> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));

		NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
		ServiceManager svcMgr(1,1,1, 1,1,3, 2,2,2,1, 10,10,10,10,10,10, pkFL);
		EchoConnection connection(&svcMgr, 3000);

		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			// Block all signals for background thread.
			sigset_t new_mask;
			sigfillset(&new_mask);
			sigset_t old_mask;
			pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

			// listen and SSL listen
			sSvcMgr(ServiceManager)->listen(7000, &connection);
			sAsioMgr->boost(false);

			// Restore previous signals.
			pthread_sigmask(SIG_SETMASK, &old_mask, 0);
		
			// Wait for signal indicating time to shut down.
			sigset_t wait_mask;
			sigemptyset(&wait_mask);
			sigaddset(&wait_mask, SIGINT);
			sigaddset(&wait_mask, SIGQUIT);
			sigaddset(&wait_mask, SIGTERM);
			pthread_sigmask(SIG_BLOCK, &wait_mask, 0);
			int sig = 0;
			sigwait(&wait_mask, &sig);
		}
		sAsioMgr->shutdown();
		sAsioMgr->join();
	}
	catch (OrzAsioException& _e)
	{
		std::vector<const char*> errs = _e.what();
		for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
		{
			sLogger->out(OL_DEBUG, *itr, 0);
		}
	}
	return 0;
}

#endif
