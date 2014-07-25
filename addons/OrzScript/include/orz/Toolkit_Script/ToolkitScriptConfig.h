#ifndef __Orz_ToolkitScript_ToolkitScriptConfig_h__
#define __Orz_ToolkitScript_ToolkitScriptConfig_h__
#include <orz/Toolkit_Base/Global.h>
#include <orz/Toolkit_Base/EventSystem.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( TOOLKIT_SCRIPT_EXPORTS )
#		define _OrzToolkitScriptExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzToolkitScriptExport
#		else
#			define _OrzToolkitScriptExport __declspec( dllimport )
#		endif
#	endif
#define _OrzToolkitScriptPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzToolkitScriptExport  __attribute__ ((visibility("default")))
#       define _OrzToolkitScriptPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzToolkitScriptExport
#       define _OrzToolkitScriptPrivate
#   endif

#endif

class _OrzToolkitScriptExport boost::noncopyable_::noncopyable;
#include <orz/Toolkit_Script/ToolkitScriptDeclare.h>
#endif