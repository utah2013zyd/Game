#ifndef __Orz_ViewOgre_ViewOGRE3DConfig_h__
#define __Orz_ViewOgre_ViewOGRE3DConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN
#	if defined( VIEW_OGRE3D_EXPORTS )
#		define _OrzViewOgreExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewOgreExport
#		else
#			define _OrzViewOgreExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewOgrePrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewOgreExport  __attribute__ ((visibility("default")))
#       define _OrzViewOgrePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewOgreExport
#       define _OrzViewOgrePrivate
#   endif

#endif


class _OrzViewOgreExport boost::noncopyable_::noncopyable;
#include <orz/View_OGRE3D/ViewOgrePreDeclare.h>
#endif