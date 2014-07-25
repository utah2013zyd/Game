#ifndef __Orz_ViewOIS_ViewOISConfig_h__
#define __Orz_ViewOIS_ViewOISConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( VIEW_OIS_EXPORTS )
#		define _OrzViewOISExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewOISExport
#		else
#			define _OrzViewOISExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewOISPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewOISExport  __attribute__ ((visibility("default")))
#       define _OrzViewOISPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewOISExport
#       define _OrzViewOISPrivate
#   endif

#endif

#define __OIS__

class _OrzViewOISExport boost::noncopyable_::noncopyable;
#include <orz/View_OIS/ViewOISPreDeclare.h>
#endif