#ifndef __Orz_ViewFmod_ViewFmodConfig_h__
#define __Orz_ViewFmod_ViewFmodConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( VIEW_FMOD_EXPORTS )
#		define _OrzViewFmodExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewFmodExport
#		else
#			define _OrzViewFmodExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewFmodPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewFmodExport  __attribute__ ((visibility("default")))
#       define _OrzViewFmodPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewFmodExport
#       define _OrzViewFmodPrivate
#   endif

#endif


class _OrzViewFmodExport boost::noncopyable_::noncopyable;
#include <orz/View_Fmod/ViewFmodPreDeclare.h>
#endif