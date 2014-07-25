/* ToolkitPlusConfig.h -- config of the 'Toolkit_Plus'

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

  Please note that the Toolkit_Plus is based in part on the work of the
  boost, the libiconv and the log4cxx. This means that if you use the 
  Toolkit_Plus in your product, you must acknowledge somewhere in your 
  documentation that you've used those code. See the README files in the 
  boost, the libiconv and the log4cxx for further informations.
*/

#ifndef __Orz_ToolkitPlus_ToolkitPlusConfig_h__
#define __Orz_ToolkitPlus_ToolkitPlusConfig_h__
#ifndef ORZ_PLUS_CONFIG_BIG_ENDIAN
#	define ORZ_PLUS_CONFIG_BIG_ENDIAN
#endif
#include <orz/Toolkit_Plus/GlobalPlus.h>


#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#	if defined( TOOLKIT_PLUS_EXPORTS )
#		define _OrzToolkitPlusExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzToolkitPlusExport
#		else
#			define _OrzToolkitPlusExport __declspec( dllimport )
#		endif
#	endif
#define _OrzToolkitPlusPrivate

#elif ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_PLUS_GCC_VISIBILITY )
#       define _OrzToolkitPlusExport  __attribute__ ((visibility("default")))
#       define _OrzToolkitPlusPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzToolkitPlusExport
#       define _OrzToolkitPlusPrivate
#   endif

#endif

/*! \mainpage Toolkit_Plus 0.4.0 API documentation
 *
 *	\image html OrzAsioLogo.PNG
 *
 *	Copyright (C) 2008-2009 ��С�� (Nous)
 *
 *	\section intro ���
 *
 *	��ӭ����OrzAsio��Toolkit_Plus�ӿ�API�ĵ���
 *	�����������ҵ�����OrzAsio��Toolkit_Plus�ӿ��һЩʹ�úͽ�����Ϣ�������԰�������õ�ʹ��OrzAsio����������������
 *	
 *	OrzAsio��Ŀ����Ϊ�˳�Ϊһ����Ч������չ�����õ��첽����⣬���û��ṩ��Ը�Ч�������ȶ����������棻�������ӿ�Toolkit_Plus
 *	������Ϊ�������߿���֧��OrzAsio�Լ���ʹ��OrzAsio���û������õĿ�����������������˽���̵߳����װģ�顢���ݿ���ʽӿ�ģ�顢
 *	��̬�����ģ�顢�ַ���ת��ģ�顢��־ģ�顢�������ģ�顢OrzAsio�����Ŀ�ƽ̨�ͻ���Ԥ������Ϣ���Լ�������ģ�飨Toolkit����
 *	�ں�����30��ʵ�á��̰߳�ȫ���󲿷֣�����Ч�ĸ��ֹ����ࡣ
 *
 *	Toolkit_Plus�ӿ��е�ģ�鶼��Ϊ�����ڶ��߳��¸�Ч����ȫ����������ж���ƣ��󲿷��̰߳�ȫ��ģ�飬�ڶ��߳������е�Ч�ʺ͵��߳��¼�����ȫһ����
 *	���һ��ܱ�֤�̰߳�ȫ�Ϳ���ʱ����൥�߳����ԣ����̵߳Ĳ��ֶ��û�������͸�����������û���������Ч����ȫ�Ķ��̳߳���
 *	
 *	Ϊ�˴ﵽ����Ŀ�ģ�Toolkit_Plus�ӿ��е�ģ��ܶ඼ʹ����lock-free��wait-free����ϣ�㷨�ȼ�����
 *	������Ϣ�������վ��<A HREF="http://ogre3d.cn/" >ogre3d.cn</A>
 *
 *	\section toolkit_plus ���ֵ�
 *
 *	Toolkit_Plus�ĸ���ģ���ʹ�ý��ܣ������������ʸ��Ե�API�ĵ���
 */

class _OrzToolkitPlusExport boost::noncopyable_::noncopyable;
class _OrzToolkitPlusExport boost::detail::shared_count;
class _OrzToolkitPlusExport boost::asio::io_service;
#include <orz/Toolkit_Plus/ToolkitPlusPreDeclare.h>
#endif
