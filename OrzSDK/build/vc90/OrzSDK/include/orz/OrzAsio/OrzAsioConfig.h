/* OrzAsioConfig.h -- config of the 'OrzAsio'

  Copyright (C) 2008-2009 熊小磊 (Nous)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Please note that the OrzAsio is based in part on the work of the
  openssl and the boost. This means that if you use the OrzAsio in your product, 
  you must acknowledge somewhere in your documentation that you've used those code. 
  See the README files in the openssl and the boost for further informations.
*/

#ifndef __Orz_OrzAsio_OrzAsioConfig_h__
#define __Orz_OrzAsio_OrzAsioConfig_h__
#ifndef ORZ_PLUS_CONFIG_BIG_ENDIAN
#	define ORZ_PLUS_CONFIG_BIG_ENDIAN
#endif
#include <orz/Toolkit_Plus/GlobalPlus.h>
#include <boost/asio/ssl.hpp>

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#	if defined( ORZASIO_EXPORTS )
#		define _OrzOrzAsioExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzOrzAsioExport
#		else
#			define _OrzOrzAsioExport __declspec( dllimport )
#		endif
#	endif
#define _OrzOrzAsioPrivate

#elif ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_PLUS_GCC_VISIBILITY )
#       define _OrzOrzAsioExport  __attribute__ ((visibility("default")))
#       define _OrzOrzAsioPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzOrzAsioExport
#       define _OrzOrzAsioPrivate
#   endif

#endif

/*! \mainpage OrzAsio 0.4.0 API documentation
 *
 *	\image html OrzAsioLogo.PNG
 *
 *	Copyright (C) 2008-2009 熊小磊 (Nous)
 *
 *	\section intro 简介
 *
 *	欢迎来到OrzAsio的API文档。
 *	在这里，你可以找到关于OrzAsio的一些使用和介绍信息，它可以帮助你更好的使用OrzAsio来开发你的网络程序。
 *	
 *	OrzAsio的目的是为了成为一个高效、可扩展、易用的异步网络库，给用户提供相对高效、灵活和稳定的网络引擎。它的网络底层基于boost::asio，
 *	一个高效、可扩展、跨平台的网络库，对其更详细的介绍请见
 *	<A HREF="boost::asio文档：http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio.html" >www.boost.org/doc/libs/1_40_0/doc/html</A> 
 *
 *	OrzAsio也使用了另外一些辅助库，例如Toolkit_Plus，一个工具库，作为基础工具库来支持OrzAsio以及让使用OrzAsio的用户来更好的开发程序；
 *	OpenSSL（<A HREF="http://www.openssl.org/" >www.openssl.org</A>），著名的网络安全库，OrzAsio主要使用它的SSL部分作为安全连接的基础。
 *
 *	更多信息请访问网站：<A HREF="http://ogre3d.cn/" >ogre3d.cn</A>
 *
 *	\section orzasio 入手点
 *
 *	用户使用OrzAsio一般只需要接触以下几个类：AsioSingleton、AsioManager、IConnection、IPacket、SessionPxy、
 *	Packet、PacketFreeList、ServiceManager、SessionDeputy，所以查阅API文档的时候，请重点关注以上几个类的文档。
 *
 *	\section orzasioexample Helloworld!
 *	
 *	Helloworld程序，使用OrzAsio建立一个监听7500端口的TCP异步服务器，用户可以使用telnet命令访问服务器的7500端口
 *	每当一个telnet成功连接到服务器上时，服务器就会在控制台上打印出一行"Hello world!"字符串；当telnet连接中断时，
 *	服务器就会在控制台上打印出一行"Goodbye world!"字符串。
 *	代码可以跨windows和linux平台。
 *
 *	\code
 *	// main.cpp
 *	#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
	#include <orz/Toolkit_Plus/Log/LogSingleton.h>
	#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
	#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
	#include <orz/OrzAsio/NetEngine/Packet.hpp>
	#include <orz/OrzAsio/AsioSingleton.h>
	#include <orz/OrzAsio/NetEngine/ServiceManager.h>
	#include <EchoConnection.h>
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

	int main(void)
	{
		// 检测内存泄露
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		typedef boost::shared_ptr<FreeList<NetPacket> > NetPacketFLShrPtr;

		try
		{
			sAsioIns.set(new AsioManager(10,10));
			sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));

			NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
			ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,1, 10,10,1,1,1,1, pkFL);
			EchoConnection connection(&svcMgr, 90);

			if (sAsioMgr->initialize(&svcMgr))
			{
				console_ctrl_function = boost::bind(&AsioManager::shutdown, sAsioMgr);
				SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

				// listen 7500
				sSvcMgr(ServiceManager)->listen(7500, &connection, 10);
				sAsioMgr->boost(true);
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

	#else
	#include <pthread.h>	// we need pthread lib in posix

	int main(void)
	{
		typedef boost::shared_ptr<FreeList<NetPacket> > NetPacketFLShrPtr;

		try
		{
			sAsioIns.set(new AsioManager(10,10));
			sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));

			NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
			ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,1, 10,10,1,1,1,1, pkFL);
			EchoConnection connection(&svcMgr, 90);

			if (sAsioMgr->initialize(&svcMgr))
			{
				// Block all signals for background thread.
				sigset_t new_mask;
				sigfillset(&new_mask);
				sigset_t old_mask;
				pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

				// listen 7500
				sSvcMgr(ServiceManager)->listen(7500, &connection, 10);
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
	// end of file main.cpp
 *
 *	\endcode
 *
 *	以上是main.cpp中的代码，其中使用了EchoConnection类，代码如下：
 *	\code
	// EchoConnection.h
	#ifndef __Orz_EchoConnection_h__
	#define __Orz_EchoConnection_h__
	#include <orz/OrzAsio/OrzAsioConfig.h>
	#include <orz/OrzAsio/NetEngine/ServiceManager.h>
	#include <orz/OrzAsio/NetEngine/IConnection.h>
	namespace Orz
	{
	class EchoConnection
		: public IConnection
	{
	public:
		EchoConnection(ServiceManager* _svcMgr, uint32 _maxInc=1);
		~EchoConnection();

	public:
		void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
		void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
		void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);
		void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

	private:
		ServiceManager*									m_svcMgr;
	};
	}
	#endif
	// end of file EchoConnection.h


	// EchoConnection.cpp
	#include <EchoConnection.h>
	#include <orz/OrzAsio/NetEngine/ServiceManager.h>
	using namespace Orz;

	EchoConnection::EchoConnection(ServiceManager* _svcMgr, uint32 _maxInc)
		: IConnection(_maxInc)
		, m_svcMgr(_svcMgr)
	{
	}

	EchoConnection::~EchoConnection()
	{
	}

	void EchoConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
	{
		printf("Hello world!\n");
	}

	void EchoConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
	{
	}

	void EchoConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
	}

	void EchoConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		printf("Goodbye world!\n");
	}
	// end of file EchoConnection.cpp

 *	\endcode
 *
 *	在windows下首先先建立当用户按下"ctrl+c"或者关闭程序后要调用的回调函数，程序用boost::bind将AsioManager::shutdown方法绑定到这个回调函数上，
 *	并用win32apiSetConsoleCtrlHandler来设置，这样，程序关闭时就可以自动调用AsioManager::shutdown，从而清除OrzAsio的数据并关闭OrzAsio，安全的退出。
 *
 *	linux下则是利用信号来实现上述功能。
 *	
 *	接下来看try里面：
 *	\code
 	sAsioIns.set(new AsioManager(10,10));
 *	\endcode
 *	创建一个AsioManager并放入AsioSingleton单件类中。
 *
 *	\code
	sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));
 *	\endcode
 *	创建了一个OrzAsioLogger对象，并放入LogSingleton单件类中，使用log4cxx.properties文件作为lo4cxx的配置文件，
 *	后面的3设置logger使用的线程数，100代表每个线程的队列缓冲区的大小，300表示二级缓冲区的队列大小，最好让3x100 == 300
 *
 *	\code
	NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
 *	\endcode
 *	创建了一个10x100的网络封包对象池
 *
 *	\code
	ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,1, 10,10,1,1,1,1, pkFL);
 *	\endcode
 *	创建ServiceManager，其中1个Tcp接收器、1个Tcp服务器、Tcp接收器拥有2个线程，10x10个tcpsocket对象池（同时最多有100个tcpsocket）
 *	
 *	\code
	EchoConnection connection(&svcMgr, 90);
 *	\endcode
 *	创建了一个EchoConnection对象，用于处理网络消息，90代表最多允许90个tcp连接同时连接到服务器上。
 *
 *	\code
	sAsioMgr->initialize(&svcMgr)
 *	\endcode
 *	初始化AsioManager
 *
 *	\code
	sSvcMgr(ServiceManager)->listen(7500, &connection, 10);
	sAsioMgr->boost(true);	// windows
	sAsioMgr->boost(false);	// linux
 *	\endcode
 *	监听7500端口，等待客户的连接，并设置刚才创建的EchoConnection来作为回调对象。
 *	然后在windows下使用阻塞模式、linux下使用非阻塞模式来启动OrzAsio，这是由于各自平台的退出程序方式决定的。
 *
 *	我们再看EchoConnection：
 *	\code
	void EchoConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
	{
		printf("Hello world!\n");
	}
	...
	void EchoConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		printf("Goodbye world!\n");
	}
 *	\endcode
 *	分别在回调函数EchoConnection::OnConnected（连接建立后调用）和EchoConnection::OnDisconnected（连接中断后调用）中，
 *	打印Hello world!和Goodbye world!信息到控制台
 *
 *	启动服务器，然后再windows下打开控制台（开始->运行->输入cmd->按回车），输入命令 telnet <ip地址> <端口>（例如telnet 127.0.0.1 7500）回车，
 *	如果一且正常，则会看到控制台上输出：Hello world!；关闭telnet窗口，则会在服务器控制台上看到Goodbye world!信息
 *
 *	更多的示例代码请见OrzAsioExample
 */

class _OrzOrzAsioExport boost::noncopyable_::noncopyable;
class _OrzOrzAsioExport boost::detail::shared_count;
class _OrzOrzAsioExport boost::any;
#include <orz/OrzAsio/OrzAsioPreDeclare.h>
#endif
