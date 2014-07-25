#ifndef __Orz_ToolkitTrigger_ToolkitTriggerConfig_h__
#define __Orz_ToolkitTrigger_ToolkitTriggerConfig_h__
#include <orz/Toolkit_Base/Global.h>
#include <orz/Toolkit_Base/EventSystem.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( TOOLKIT_TRIGGER_EXPORTS )
#		define _OrzToolkitTriggerExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzToolkitTriggerExport
#		else
#			define _OrzToolkitTriggerExport __declspec( dllimport )
#		endif
#	endif
#define _OrzToolkitTriggerPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzToolkitTriggerExport  __attribute__ ((visibility("default")))
#       define _OrzToolkitTriggerPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzToolkitTriggerExport
#       define _OrzToolkitTriggerPrivate
#   endif

#endif

#define __OIS__

class _OrzToolkitTriggerExport boost::noncopyable_::noncopyable;
#include <orz/Toolkit_Trigger/ToolkitTriggerDeclare.h>
#endif