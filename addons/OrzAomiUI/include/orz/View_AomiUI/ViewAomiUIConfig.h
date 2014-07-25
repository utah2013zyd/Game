#ifndef __Orz_ViewAomiUI_ViewAomiUIConfig_h__
#define __Orz_ViewAomiUI_ViewAomiUIConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN
#	if defined( VIEW_AOMIUI_EXPORTS )
#		define _OrzViewAomiUIExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewAomiUIExport
#		else
#			define _OrzViewAomiUIExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewAomiUIPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewAomiUIExport  __attribute__ ((visibility("default")))
#       define _OrzViewAomiUIPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewAomiUIExport
#       define _OrzViewAomiUIPrivate
#   endif

#endif

class _OrzViewAomiUIExport boost::noncopyable_::noncopyable;
#include <orz/View_AomiUI/ViewAomiUIPreDeclare.h>
#endif