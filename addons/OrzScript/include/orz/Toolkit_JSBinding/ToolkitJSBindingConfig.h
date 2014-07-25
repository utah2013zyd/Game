#ifndef __Orz_JSBinding_JSBindingConfig_h__
#define __Orz_JSBinding_JSBindingConfig_h__
#include <orz/Toolkit_Base/Global.h>
#include <orz/Toolkit_Base/EventSystem.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( TOOLKIT_JSBINDING_EXPORTS )
#		define _ToolkitJSBindingExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _ToolkitJSBindingExport
#		else
#			define _ToolkitJSBindingExport __declspec( dllimport )
#		endif
#	endif
#define _ToolkitJSBindingPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _ToolkitJSBindingExport  __attribute__ ((visibility("default")))
#       define _ToolkitJSBindingPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _ToolkitJSBindingExport
#       define _ToolkitJSBindingPrivate
#   endif

#endif

class _ToolkitJSBindingExport boost::noncopyable_::noncopyable;
#include <orz/Toolkit_JSBinding/ToolkitJSBindingDeclare.h>
#endif