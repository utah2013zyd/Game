// EchoServer.cpp : Defines the entry point for the console application.
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <PacketBuilder.h>
#include <Packet.h>
#include <EchoConnection.h>
#include <orz/Toolkit_Plus/Toolkit/Rander.hpp>
#include <orz/OrzAsio/NetEngine/SSLContext.h>
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

	if (argc < 10)
    {
		std::cerr << "Usage: exe <port num> <start port> <tcp acceptor num> <tcp service num> <tcp acceptor thread num>" 
				  << " <packet clr num> <packet per num> <tcp socket clr num> <tcp socket per num> <has ssl 0 == hasn't !0 == has>"
				  << " <ssl port - if hasn't ssl then this is none> <log4cxx conf file path=log4cxx.properties>\n";
		return 1;
    }

	uint32 port_num = (uint32)atoi(argv[1]);
	uint16 start_port = (uint16)atoi(argv[2]);
	uint32 tcp_acceptor_num = (uint32)atoi(argv[3]);
	uint32 tcp_service_num = (uint32)atoi(argv[4]);
	uint32 tcp_acceptor_thread_num  = (uint32)atoi(argv[5]);
	uint32 packet_clr_num = (uint32)atoi(argv[6]);
	uint32 packet_per_num = (uint32)atoi(argv[7]);
	uint32 tcp_socket_clr_num = (uint32)atoi(argv[8]);
	uint32 tcp_socket_per_num = (uint32)atoi(argv[9]);
	bool hasSSL = (int32)atoi(argv[10]) == 0 ? false : true;

	uint16 ssl_port = 0;
	if (hasSSL)
	{
		ssl_port = (uint16)atoi(argv[11]);
	}
	char log4cxx_conf_path[512] = "log4cxx.properties";
	if (hasSSL)
	{
		if (argc == 13)
		{
			memcpy(log4cxx_conf_path, argv[12], sizeof(argv[12]));
		}
	}
	else
	{
		if (argc == 12)
		{
			memcpy(log4cxx_conf_path, argv[11], sizeof(argv[11]));
		}
	}

	typedef boost::shared_ptr<FreeList<NetPk> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger(log4cxx_conf_path));

		NetPacketFLShrPtr pkFL(new FreeList<NetPk>(packet_clr_num, packet_per_num));

		ServiceManager svcMgr(tcp_acceptor_num,1,1, tcp_service_num,1,1, tcp_acceptor_thread_num,1,1, 10, 
							  tcp_socket_clr_num,tcp_socket_per_num, 10,10, 10,10, pkFL, IPacketParserShrPtr(new PacketBuilder));
		EchoConnection connection(3000);
		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			console_ctrl_function = boost::bind(&AsioManager::shutdown, sAsioMgr);
			SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
			// listen and recvfrom
			// max 3000 connections at this connection
			for (uint32 i=0; i<port_num; ++i)
			{
				sSvcMgr(ServiceManager)->listen(start_port+(uint16)i, &connection, 2);
				sLogger->out(OL_INFO, " Listen port: ", d2s((uint32)(start_port+(uint16)i)).c_str(), 0); 
			}
			sSvcMgr(ServiceManager)->recvfrom(start_port, &connection, 5, 2);

			if (hasSSL)
			{
				SSLContextShrPtr ct = sSvcMgr(ServiceManager)->createContext(boost::asio::ssl::context::sslv23);
				if (ct)
				{
					ct->getContext().set_options(boost::asio::ssl::context::default_workarounds
												 | boost::asio::ssl::context::no_sslv2
												 | boost::asio::ssl::context::single_dh_use);
					ct->getContext().set_password_callback(boost::bind(&SSLContext::getPassword, ct.get()));
					ct->getContext().use_certificate_chain_file("mycert.pem");
					ct->getContext().use_private_key_file("mykey.pem", boost::asio::ssl::context::pem);
					ct->getContext().use_tmp_dh_file("dh512.pem");

					ContextId cxtid = sSvcMgr(ServiceManager)->registerContext(ct, 10, 100);
					sSvcMgr(ServiceManager)->sslListen(ssl_port, &connection, cxtid, 2);
					sLogger->out(OL_INFO, " SSL Listen port: ", d2s(ssl_port).c_str(), 0);
				}
			}
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

#else	// posix ...
#include <pthread.h>	// we need pthread lib in posix

int main(int argc, char* argv[])
{
	if (argc < 10)
    {
		std::cerr << "Usage: exe <port num> <start port> <tcp acceptor num> <tcp service num> <tcp acceptor thread num>" 
				  << " <packet clr num> <packet per num> <tcp socket clr num> <tcp socket per num> <has ssl 0 == hasn't !0 == has>"
				  << " <ssl port - if hasn't ssl then this is none> <log4cxx conf file path=log4cxx.properties>\n";
		return 1;
    }

	uint32 port_num = (uint32)atoi(argv[1]);
	uint16 start_port = (uint16)atoi(argv[2]);
	uint32 tcp_acceptor_num = (uint32)atoi(argv[3]);
	uint32 tcp_service_num = (uint32)atoi(argv[4]);
	uint32 tcp_acceptor_thread_num  = (uint32)atoi(argv[5]);
	uint32 packet_clr_num = (uint32)atoi(argv[6]);
	uint32 packet_per_num = (uint32)atoi(argv[7]);
	uint32 tcp_socket_clr_num = (uint32)atoi(argv[8]);
	uint32 tcp_socket_per_num = (uint32)atoi(argv[9]);
	bool hasSSL = (int32)atoi(argv[10]) == 0 ? false : true;

	uint16 ssl_port = 0;
	if (hasSSL)
	{
		ssl_port = (uint16)atoi(argv[11]);
	}
	char log4cxx_conf_path[512] = "log4cxx.properties";
	if (hasSSL)
	{
		if (argc == 13)
		{
			memcpy(log4cxx_conf_path, argv[12], sizeof(argv[12]));
		}
	}
	else
	{
		if (argc == 12)
		{
			memcpy(log4cxx_conf_path, argv[11], sizeof(argv[11]));
		}
	}

	typedef boost::shared_ptr<FreeList<NetPk> > NetPacketFLShrPtr;

	try
	{
		sAsioIns.set(new AsioManager(10,10));
		sLogIns.set(new OrzAsioLogger(log4cxx_conf_path));

		NetPacketFLShrPtr pkFL(new FreeList<NetPk>(packet_clr_num, packet_per_num));
		ServiceManager svcMgr(tcp_acceptor_num,1,1, tcp_service_num,1,1, tcp_acceptor_thread_num,1,1, 1, 
							  tcp_socket_clr_num,tcp_socket_per_num, 10,10, 10,10, pkFL, IPacketParserShrPtr(new PacketBuilder));
		EchoConnection connection(3000);
		sPluginsMgr->addPlugin(PluginWrap(sLogger));

		if (sAsioMgr->initialize(&svcMgr))
		{
			// Block all signals for background thread.
			sigset_t new_mask;
			sigfillset(&new_mask);
			sigset_t old_mask;
			pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

			// listen and recvfrom
			for (uint32 i=0; i<port_num; ++i)
			{
				sSvcMgr(ServiceManager)->listen(start_port+(uint16)i, &connection, 2);
			}
			sSvcMgr(ServiceManager)->recvfrom(start_port, &connection, 5, 2);

			if (hasSSL)
			{
				SSLContextShrPtr ct = sSvcMgr(ServiceManager)->createContext(boost::asio::ssl::context::sslv23);
				if (ct)
				{
					ct->getContext().set_options(boost::asio::ssl::context::default_workarounds
												 | boost::asio::ssl::context::no_sslv2
												 | boost::asio::ssl::context::single_dh_use);
					ct->getContext().set_password_callback(boost::bind(&SSLContext::getPassword, ct.get()));
					ct->getContext().use_certificate_chain_file("mycert.pem");
					ct->getContext().use_private_key_file("mykey.pem", boost::asio::ssl::context::pem);
					ct->getContext().use_tmp_dh_file("dh512.pem");

					ContextId cxtid = sSvcMgr(ServiceManager)->registerContext(ct, 10, 100);
					sSvcMgr(ServiceManager)->sslListen(ssl_port, &connection, cxtid, 2);
				}
			}

			// Run server in background thread.
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
