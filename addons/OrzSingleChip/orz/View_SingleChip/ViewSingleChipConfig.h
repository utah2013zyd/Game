#ifndef __Orz_ViewSingleChip_SingleChipConfig_h__
#define __Orz_ViewSingleChip_SingleChipConfig_h__
#include <orz/Toolkit_Base/Global.h>


#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( VIEW_SINGLECHIP_EXPORTS )
#		define _OrzViewSingleChipExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzViewSingleChipExport
#		else
#			define _OrzViewSingleChipExport __declspec( dllimport )
#		endif
#	endif
#define _OrzViewSingleChipPrivate

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _OrzViewSingleChipExport  __attribute__ ((visibility("default")))
#       define _OrzViewSingleChipPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzViewSingleChipExport
#       define _OrzViewSingleChipPrivate
#   endif

#endif

namespace Orz
{
	typedef std::vector<unsigned char>  MsgBuffer;
}

class _OrzViewSingleChipExport boost::noncopyable_::noncopyable;
#include <orz/View_SingleChip/ViewSingleChipPreDeclare.h>
#endif