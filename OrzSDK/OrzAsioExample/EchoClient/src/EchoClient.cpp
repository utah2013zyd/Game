// EchoClient.cpp : Defines the entry point for the console application.
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <PacketBuilder.h>
#include <Packet.h>
#include <EchoConnection.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
using namespace Orz;

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

int main(int argc, char* argv[])
{
	// ¼ì²âÄÚ´æÐ¹Â¶
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if (argc != 3)
    {
		std::cerr << "Usage: exe <host> <port>\n";
		return 1;
    }

	std::string outIp(argv[1]);
	uint16 port = (uint16)atoi(argv[2]);
	
	typedef boost::shared_ptr<FreeList<NetPk> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger("log4cxx.properties"));

		NetPacketFLShrPtr pkFL(new FreeList<NetPk>(10, 100));
		ServiceManager svcMgr(1,1,1, 1,1,1, 1,1,1, 1, 10,10, 10,10, 10,10, pkFL, IPacketParserShrPtr(new PacketBuilder));
		EchoConnection connection(outIp, port, 5);
		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			console_ctrl_function = boost::bind(&EchoConnection::shutdown, &connection);
			SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

			sSvcMgr(ServiceManager)->connect(outIp.c_str(), port, &connection);
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

int main(int argc, char* argv[])
{
	if (argc != 3)
    {
		std::cerr << "Usage: exe <host> <port>\n";
		return 1;
    }

	std::string outIp(argv[1]);
	uint16 port = (uint16)atoi(argv[2]);
	
	typedef boost::shared_ptr<FreeList<NetPk> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger("log4cxx.properties"));

		NetPacketFLShrPtr pkFL(new FreeList<NetPk>(10, 100));
		ServiceManager svcMgr(1,1,1, 1,1,1, 1,1,1, 1, 10,10, 10,10, 10,10, pkFL, IPacketParserShrPtr(new PacketBuilder));
		EchoConnection connection(outIp, port, 5);
		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			// Block all signals for background thread.
			sigset_t new_mask;
			sigfillset(&new_mask);
			sigset_t old_mask;
			pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

			sSvcMgr(ServiceManager)->connect(outIp.c_str(), port, &connection);
			sAsioMgr->boost(true);

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
