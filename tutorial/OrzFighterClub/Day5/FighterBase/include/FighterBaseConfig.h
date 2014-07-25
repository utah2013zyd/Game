#ifndef __Orz_FighterBase_FighterBaseConfig_h__
#define __Orz_FighterBase_FighterBaseConfig_h__
#include <orz/Toolkit_Base/Global.h>
#include <orz/Framework_Base/System.h>
#include <orz/Framework_Base/System.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_OGRE3D/CEGUIManager.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/Framework_Base/Logic.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#	if defined( FIGHTERBASE_EXPORTS )
#		define _FighterBaseExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _FighterBaseExport
#		else
#			define _FighterBaseExport __declspec( dllimport )
#		endif
#	endif
#	define _FighterBasePrivate
#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX  

// Enable GCC symbol visibility
#   if defined( ORZ_GCC_VISIBILITY )
#       define _FighterBaseExport  __attribute__ ((visibility("default")))
#       define _FighterBasePrivate __attribute__ ((visibility("hidden")))
#   else
#       define _FighterBaseExport
#       define _FighterBasePrivate
#   endif

#endif

#include <orz/Toolkit_Base/EventSystem.h>
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)




#pragma warning(push)
#pragma warning(disable:4251)
#include <CEGUI.h>
#pragma warning(pop)


#endif