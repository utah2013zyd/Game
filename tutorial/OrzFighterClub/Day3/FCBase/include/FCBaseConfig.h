#ifndef __Orz_FCBase_FCBaseConfig_h__
#define __Orz_FCBase_FCBaseConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( FCBASE_EXPORTS )
#		define _FCBaseExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _FCBaseExport
#		else
#			define _FCBaseExport __declspec( dllimport )
#		endif
#	endif
#	define _FCBasePrivate
#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _FCBaseExport  __attribute__ ((visibility("default")))
#       define _FCBasePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _FCBaseExport
#       define _FCBasePrivate
#   endif

#endif


#endif