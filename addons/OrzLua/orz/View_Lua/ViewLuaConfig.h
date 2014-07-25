#ifndef __Orz_ViewLua_LuaConfig_h__
#define __Orz_ViewLua_LuaConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( VIEW_LUA_EXPORTS )
#		define _OrzViewLuaExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewLuaExport
#		else
#			define _OrzViewLuaExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewLuaPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewLuaExport  __attribute__ ((visibility("default")))
#       define _OrzViewLuaPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewLuaExport
#       define _OrzViewLuaPrivate
#   endif

#endif

class _OrzViewLuaExport boost::noncopyable_::noncopyable;
#include <orz/View_Lua/ViewLuaPreDeclare.h>
#endif