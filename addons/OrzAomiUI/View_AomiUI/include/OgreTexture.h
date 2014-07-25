#ifndef __Orz_ViewAomiUI_OgreTexture_h__
#define __Orz_ViewAomiUI_OgreTexture_h__

#include <orz/View_AomiUI/ViewAomiUIConfig.h> 
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Framework_Base/System/IInputManager.h>
#include <Ogre/Ogre.h>

#include <orz/View_AomiUI/KeyboardHook.h>


namespace Orz
{


class _OrzViewAomiUIPrivate OgreTexture
{

public:

	OgreTexture(Awesomium::WebCore* core, const std::string & name, int width, int height)\
	{
	
	}
	
	virtual ~OgreTexture(void)
	{}
	
};





}

#endif