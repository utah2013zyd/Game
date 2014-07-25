// ActOGServer.cpp : Defines the entry point for the console application.
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <packet/packet_builder.h>
#include <GameConnection.h>
#include <orz/Toolkit_Plus/Log/LogWrap.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
using namespace Orz;

//------------------------------------------------------------------------
//	a flash action online game demo, like dnf you can fight with you friend in game.using blow url to play the game
//	http://202.106.63.196/core_engine/pb_client/PowerBear.html
//	
//	wasd to move character, j l to attack and use skill, ¡ý¡ú + j == skill1 (¡ý¡ú : when face right)
//------------------------------------------------------------------------


void testPkt()
{
	typedef boost::shared_ptr<FreeList<net_pk> > NetPacketFLShrPtr;
	NetPacketFLShrPtr pkFL(new FreeList<net_pk>(5, 10));
	sPkFList.set(pkFL);
	IPacketShrPtr pk = allocPacket();
	pk->setMsg(M_JUMP);
	pk->put("teststring", 10);
	pk->put("teststr2", 8);
	StringPtr ms;
	pk->get(ms, pk->getSize());
	printf("pk: %s\n", ms.ptr());
	printf("pk size: %d\n", pk->getSize());
	printf("msg: %s\n", g_msgs[pk->getMsg()].c_str());

	IPacketParserShrPtr pkpr(new packet_builder);

	char str[PKT_MAX_SIZE] = "001400ACTION1#nous|1&4&5&1";
	std::vector<IPacketShrPtr> pks;
	pkpr->parse(str, 2, pks);
	pkpr->parse(str+2, 1, pks);
	pkpr->parse(str+3, 1, pks);
	pkpr->parse(str+4, 2, pks);
	pkpr->parse(str+6, 1, pks);
	pkpr->parse(str+7, 2, pks);
	pkpr->parse(str+9, 17, pks);

	for (uint32 i=0; i<pks.size(); ++i)
	{
		StringPtr msi;
		pks[i]->get(msi, pks[i]->getSize());
		printf("pks[i]: %s\n", msi.ptr());
		printf("pks[i] size: %d\n", pks[i]->getSize());
		printf("msg: %s\n", g_msgs[pks[i]->getMsg()].c_str());
		Args args_list;
		packet_seer psr;
		pks[i]->reset();
		psr.analyse_packet(pks[i], args_list, g_spts);
		std::vector<Arg>& args = args_list.argList;
		for (uint32 i=0; i<args.size(); ++i)
		{
			printf("args[%d]: arg: %s\n", i, args[i].arg.c_str());
		}
	}

	getchar();
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

int main(int argc, char* argv[])
{
	// ¼ì²âÄÚ´æÐ¹Â¶
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//testPkt();
	//return 0;

	typedef boost::shared_ptr<FreeList<net_pk> > NetPacketFLShrPtr;

	try
	{
		boost::shared_ptr<AsioManager> orzAsioMgr(boost::shared_ptr<AsioManager>(new AsioManager));
		sAsioIns.set(orzAsioMgr);

		sLogIns.setCurrentAppender(ADRT_PERDAY);
		OrzAsioLoggerShrPtr orzAsioLogger(new OrzAsioLogger("log4cxx.properties"));
		sLogIns.set(boost::shared_ptr<OrzAsioLogger>(orzAsioLogger));

		NetPacketFLShrPtr pkFL(new FreeList<net_pk>(50, 100));
		ServiceManager svcMgr(1,0,0, 3,0,0, 3,0,0, 1, 100,50, 1,1, 1,1, IPacketParserShrPtr(new packet_builder));
		GameConnection connection(&svcMgr, 20000);
		if (sAsioMgr->setup(&svcMgr, orzAsioLogger, ADRT_PERDAY, pkFL))
		{
			console_ctrl_function = boost::bind(&GameConnection::shutdown, &connection);
			SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

			// listen
			if (connection.load_config("gs.conf") == 0)
			{
				svcMgr.listen(7004, &connection, 200);
				sAsioMgr->start(true);
			}
		}
		sAsioMgr->shutdown();
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

#else	// posix ...
#include <pthread.h>	// we need pthread lib in posix

int main(int argc, char* argv[])
{
	typedef boost::shared_ptr<FreeList<net_pk> > NetPacketFLShrPtr;

	try
	{
		boost::shared_ptr<AsioManager> orzAsioMgr(boost::shared_ptr<AsioManager>(new AsioManager));
		sAsioIns.set(orzAsioMgr);

		sLogIns.setCurrentAppender(ADRT_PERDAY);
		OrzAsioLoggerShrPtr orzAsioLogger(new OrzAsioLogger("log4cxx.properties"));
		sLogIns.set(boost::shared_ptr<OrzAsioLogger>(orzAsioLogger));

		NetPacketFLShrPtr pkFL(new FreeList<net_pk>(50, 100));
		ServiceManager svcMgr(1,0,0, 50,0,0, 1,0,0, 1, 100,50, 1,1, 1,1, IPacketParserShrPtr(new packet_builder));
		GameConnection connection(&svcMgr, 20000);
		if (sAsioMgr->setup(&svcMgr, orzAsioLogger, ADRT_PERDAY, pkFL))
		{
			// Block all signals for background thread.
			sigset_t new_mask;
			sigfillset(&new_mask);
			sigset_t old_mask;
			pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

			// listen 
			if (connection.load_config("gs.conf") == 0)
			{
				svcMgr.listen(7004, &connection, 200);
				// Run server in background thread.
				sAsioMgr->start(false);
			}

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

