#ifndef __Orz_ViewOgre_OgreGraphicsManagerImpl_h__
#define __Orz_ViewOgre_OgreGraphicsManagerImpl_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#include <orz/Framework_Base/FrameworkBasePreDeclare.h>
#include <orz/Framework_Base/System/WindowInterface.h>
namespace Orz {
    

class _OrzViewOgrePrivate OgreGraphicsManagerImpl: public WindowListener
{
public:
	OgreGraphicsManagerImpl(void);
	~OgreGraphicsManagerImpl(void);
	Ogre::Camera * getCamera(void);
	Ogre::SceneManager * getSceneManager(void);
	Ogre::RenderWindow * getRenderWindow(void);
	bool update(TimeType interval);
	bool init(void);
	void shutdown(void);
private:


	virtual void onSizeChange(size_t width, size_t height);
	virtual void onClosed(void){_exit = true;}
	OgreWindowPtr createRenderWindowIn(void);
	OgreWindowPtr createRenderWindowOut(WeakWindowPtr win);
	void setupResources(void);
	void clearResources(void);

	void createSceneManager(void);
	void destroySceneManager(void);

	void createCamera(void);
	void destroyCamera(void);

	void createViewports(void);
	void destroyViewports(void);


	void loadResources(void);



	void createSimpleScene(void);
	void destroySimpleScene(void);


	boost::scoped_ptr<Ogre::Root> _root;
	OgreWindowPtr _window;
	Ogre::SceneManager * _sceneManager;
	Ogre::Camera * _camera;
	Ogre::Viewport* _viewport;

	
private:
	bool _init;
	bool _exit;
	
	
};
}

#endif

