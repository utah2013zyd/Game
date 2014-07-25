#ifndef __Orz_ToolkitBase_ToolkitBaseConfig_h__
#define __Orz_ToolkitBase_ToolkitBaseConfig_h__
#include <orz/Toolkit_Base/Global.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( TOOLKIT_BASE_EXPORTS )
#		define _OrzToolkitBaseExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzToolkitBaseExport
#		else
#			define _OrzToolkitBaseExport __declspec( dllimport )
#		endif
#	endif
#define _OrzToolkitBasePrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzToolkitBaseExport  __attribute__ ((visibility("default")))
#       define _OrzToolkitBasePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzToolkitBaseExport
#       define _OrzToolkitBasePrivate
#   endif

#endif


class _OrzToolkitBaseExport boost::noncopyable_::noncopyable;
#include <orz/Toolkit_Base/ToolkitBasePreDeclare.h>
#endif
