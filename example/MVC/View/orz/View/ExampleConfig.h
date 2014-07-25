#ifndef __Orz_View_ExampleConfig_h__
#define __Orz_View_ExampleConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN
#	if defined( VIEW_EXPORTS )
#		define _OrzViewExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewExport
#		else
#			define _OrzViewExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewExport  __attribute__ ((visibility("default")))
#       define _OrzViewPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewExport
#       define _OrzViewPrivate
#   endif

#endif


class _OrzViewExport boost::noncopyable_::noncopyable;
#include <orz/View_Fmod/ViewFmodPreDeclare.h>
#endif