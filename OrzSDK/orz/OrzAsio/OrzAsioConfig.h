/* OrzAsioConfig.h -- config of the 'OrzAsio'

  Copyright (C) 2008-2009 ��С�� (Nous)

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
 *	Copyright (C) 2008-2009 ��С�� (Nous)
 *
 *	\section intro ���
 *
 *	��ӭ����OrzAsio��API�ĵ���
 *	�����������ҵ�����OrzAsio��һЩʹ�úͽ�����Ϣ�������԰�������õ�ʹ��OrzAsio����������������
 *	
 *	OrzAsio��Ŀ����Ϊ�˳�Ϊһ����Ч������չ�����õ��첽����⣬���û��ṩ��Ը�Ч�������ȶ����������档��������ײ����boost::asio��
 *	һ����Ч������չ����ƽ̨������⣬�������ϸ�Ľ������
 *	<A HREF="boost::asio�ĵ���http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio.html" >www.boost.org/doc/libs/1_40_0/doc/html</A> 
 *
 *	OrzAsioҲʹ��������һЩ�����⣬����Toolkit_Plus��һ�����߿⣬��Ϊ�������߿���֧��OrzAsio�Լ���ʹ��OrzAsio���û������õĿ�������
 *	OpenSSL��<A HREF="http://www.openssl.org/" >www.openssl.org</A>�������������簲ȫ�⣬OrzAsio��Ҫʹ������SSL������Ϊ��ȫ���ӵĻ�����
 *
 *	������Ϣ�������վ��<A HREF="http://ogre3d.cn/" >ogre3d.cn</A>
 *
 *	\section orzasio ���ֵ�
 *
 *	�û�ʹ��OrzAsioһ��ֻ��Ҫ�Ӵ����¼����ࣺAsioSingleton��AsioManager��IConnection��IPacket��SessionPxy��
 *	Packet��PacketFreeList��ServiceManager��SessionDeputy�����Բ���API�ĵ���ʱ�����ص��ע���ϼ�������ĵ���
 *
 *	\section orzasioexample Helloworld!
 *	
 *	Helloworld����ʹ��OrzAsio����һ������7500�˿ڵ�TCP�첽���������û�����ʹ��telnet������ʷ�������7500�˿�
 *	ÿ��һ��telnet�ɹ����ӵ���������ʱ���������ͻ��ڿ���̨�ϴ�ӡ��һ��"Hello world!"�ַ�������telnet�����ж�ʱ��
 *	�������ͻ��ڿ���̨�ϴ�ӡ��һ��"Goodbye world!"�ַ�����
 *	������Կ�windows��linuxƽ̨��
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
		// ����ڴ�й¶
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
 *	������main.cpp�еĴ��룬����ʹ����EchoConnection�࣬�������£�
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
 *	��windows�������Ƚ������û�����"ctrl+c"���߹رճ����Ҫ���õĻص�������������boost::bind��AsioManager::shutdown�����󶨵�����ص������ϣ�
 *	����win32apiSetConsoleCtrlHandler�����ã�����������ر�ʱ�Ϳ����Զ�����AsioManager::shutdown���Ӷ����OrzAsio�����ݲ��ر�OrzAsio����ȫ���˳���
 *
 *	linux�����������ź���ʵ���������ܡ�
 *	
 *	��������try���棺
 *	\code
 	sAsioIns.set(new AsioManager(10,10));
 *	\endcode
 *	����һ��AsioManager������AsioSingleton�������С�
 *
 *	\code
	sLogIns.set(new OrzAsioLogger("log4cxx.properties", 3,100,300));
 *	\endcode
 *	������һ��OrzAsioLogger���󣬲�����LogSingleton�������У�ʹ��log4cxx.properties�ļ���Ϊlo4cxx�������ļ���
 *	�����3����loggerʹ�õ��߳�����100����ÿ���̵߳Ķ��л������Ĵ�С��300��ʾ�����������Ķ��д�С�������3x100 == 300
 *
 *	\code
	NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
 *	\endcode
 *	������һ��10x100�������������
 *
 *	\code
	ServiceManager svcMgr(1,0,0, 1,0,0, 2,0,0,1, 10,10,1,1,1,1, pkFL);
 *	\endcode
 *	����ServiceManager������1��Tcp��������1��Tcp��������Tcp������ӵ��2���̣߳�10x10��tcpsocket����أ�ͬʱ�����100��tcpsocket��
 *	
 *	\code
	EchoConnection connection(&svcMgr, 90);
 *	\endcode
 *	������һ��EchoConnection�������ڴ���������Ϣ��90�����������90��tcp����ͬʱ���ӵ��������ϡ�
 *
 *	\code
	sAsioMgr->initialize(&svcMgr)
 *	\endcode
 *	��ʼ��AsioManager
 *
 *	\code
	sSvcMgr(ServiceManager)->listen(7500, &connection, 10);
	sAsioMgr->boost(true);	// windows
	sAsioMgr->boost(false);	// linux
 *	\endcode
 *	����7500�˿ڣ��ȴ��ͻ������ӣ������øղŴ�����EchoConnection����Ϊ�ص�����
 *	Ȼ����windows��ʹ������ģʽ��linux��ʹ�÷�����ģʽ������OrzAsio���������ڸ���ƽ̨���˳�����ʽ�����ġ�
 *
 *	�����ٿ�EchoConnection��
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
 *	�ֱ��ڻص�����EchoConnection::OnConnected�����ӽ�������ã���EchoConnection::OnDisconnected�������жϺ���ã��У�
 *	��ӡHello world!��Goodbye world!��Ϣ������̨
 *
 *	������������Ȼ����windows�´򿪿���̨����ʼ->����->����cmd->���س������������� telnet <ip��ַ> <�˿�>������telnet 127.0.0.1 7500���س���
 *	���һ����������ῴ������̨�������Hello world!���ر�telnet���ڣ�����ڷ���������̨�Ͽ���Goodbye world!��Ϣ
 *
 *	�����ʾ���������OrzAsioExample
 */

class _OrzOrzAsioExport boost::noncopyable_::noncopyable;
class _OrzOrzAsioExport boost::detail::shared_count;
class _OrzOrzAsioExport boost::any;
#include <orz/OrzAsio/OrzAsioPreDeclare.h>
#endif
