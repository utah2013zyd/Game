/* ToolkitPlusConfig.h -- config of the 'Toolkit_Plus'

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
 *	Copyright (C) 2008-2009 熊小磊 (Nous)
 *
 *	\section intro 简介
 *
 *	欢迎来到OrzAsio的Toolkit_Plus子库API文档。
 *	在这里，你可以找到关于OrzAsio的Toolkit_Plus子库的一些使用和介绍信息，它可以帮助你更好的使用OrzAsio来开发你的网络程序。
 *	
 *	OrzAsio的目的是为了成为一个高效、可扩展、易用的异步网络库，给用户提供相对高效、灵活和稳定的网络引擎；而它的子库Toolkit_Plus
 *	则是作为基础工具库来支持OrzAsio以及让使用OrzAsio的用户来更好的开发程序。它包含：带私有线程的类封装模块、数据库访问接口模块、
 *	动态库管理模块、字符集转换模块、日志模块、插件管理模块、OrzAsio依赖的跨平台和基本预处理信息，以及工具箱模块（Toolkit），
 *	内含超过30个实用、线程安全（大部分）、高效的各种工具类。
 *
 *	Toolkit_Plus子库中的模块都是为了能在多线程下高效、安全、方便的运行而设计，大部分线程安全的模块，在多线程下运行的效率和单线程下几乎完全一样，
 *	而且还能保证线程安全和开发时候的类单线程特性（多线程的部分对用户基本上透明），便于用户开发出高效、安全的多线程程序。
 *	
 *	为了达到上述目的，Toolkit_Plus子库中的模块很多都使用了lock-free、wait-free、哈希算法等技术。
 *	更多信息请访问网站：<A HREF="http://ogre3d.cn/" >ogre3d.cn</A>
 *
 *	\section toolkit_plus 入手点
 *
 *	Toolkit_Plus的各类模块的使用介绍，请根据需求访问各自的API文档。
 */

class _OrzToolkitPlusExport boost::noncopyable_::noncopyable;
class _OrzToolkitPlusExport boost::detail::shared_count;
class _OrzToolkitPlusExport boost::asio::io_service;
#include <orz/Toolkit_Plus/ToolkitPlusPreDeclare.h>
#endif
