#ifndef __Orz_FrameworkBase_ToolkitBaseConfig_h__
#define __Orz_FrameworkBase_ToolkitBaseConfig_h__
#include <orz/Toolkit_Base/Global.h>

#include <orz/Toolkit_Base/LogManager.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( FRAMEWORK_BASE_EXPORTS )
#		define _OrzFrameworkBaseExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzFrameworkBaseExport
#		else
#			define _OrzFrameworkBaseExport __declspec( dllimport )
#		endif
#	endif
#define _OrzToolkitBasePrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzFrameworkBaseExport  __attribute__ ((visibility("default")))
#       define _OrzToolkitBasePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzFrameworkBaseExport
#       define _OrzToolkitBasePrivate
#   endif

#endif

const Orz::TimeType WORLD_UPDATE_INTERVAL = 0.015f;

class _OrzFrameworkBaseExport boost::noncopyable_::noncopyable;
#include <orz/Framework_Base/FrameworkBasePreDeclare.h>
#endif