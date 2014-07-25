#ifndef __Orz_OgreNewt_OgreNewtConfig_h__
#define __Orz_OgreNewt_OgreNewtConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( VIEW_OGRENEWT_EXPORTS )
#		define _OrzViewOgreNewtonExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewOgreNewtonExport
#		else
#			define _OrzViewOgreNewtonExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewOgreNewtonPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewOgreNewtonExport  __attribute__ ((visibility("default")))
#       define _OrzViewOgreNewtonPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewOgreNewtonExport
#       define _OrzViewOgreNewtonPrivate
#   endif

#endif

class _OrzViewOgreNewtonExport boost::noncopyable_::noncopyable;
#include <orz/View_OgreNewt/ViewOgreNewtPreDeclare.h>
#endif