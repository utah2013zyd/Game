#ifndef __Orz_ViewOgre_OgreGraphicsManager_h__
#define __Orz_ViewOgre_OgreGraphicsManager_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Framework_Base/System/SystemInterface.h>
namespace Orz{


class OgreGraphicsManagerImpl;
template class _OrzViewOgreExport boost::scoped_ptr<OgreGraphicsManagerImpl>;

class _OrzViewOgreExport OgreGraphicsManager : public Singleton<OgreGraphicsManager>
{

public:
	OgreGraphicsManager(void);
	~OgreGraphicsManager(void);
	bool update(TimeType interval);
	bool init(void);
	void shutdown(void);
	Ogre::Camera * getCamera(void);
	Ogre::SceneManager * getSceneManager(void);
	Ogre::Viewport * getViewport(void);
	Ogre::RenderWindow * getRenderWindow(void);
private:
	
	boost::scoped_ptr<OgreGraphicsManagerImpl> _impl;
};


SYSTEMUNIT_BEGIN(OgreGraphicsManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(OgreGraphicsManager) 

}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, OgreGraphicsManager) 

#endif