#include "OgreGraphicsManagerImpl.h"
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)
#include <orz/Framework_Base/System.h>
#include "OgreWindow.h"

using namespace Orz;
OgreGraphicsManagerImpl::OgreGraphicsManagerImpl(void):
_root(),
_window(),
_sceneManager(NULL),
_camera(NULL),
_viewport(NULL),
_init(false),
_exit(false)
{

}
OgreGraphicsManagerImpl::~OgreGraphicsManagerImpl(void)
{

}
Ogre::Camera * OgreGraphicsManagerImpl::getCamera(void)
{

	return _camera;
}
Ogre::SceneManager * OgreGraphicsManagerImpl::getSceneManager(void)
{
	return _sceneManager;
}
Ogre::RenderWindow * OgreGraphicsManagerImpl::getRenderWindow(void)
{

	return _window->getRenderWindow();
}
bool OgreGraphicsManagerImpl::update(TimeType interval)
{
	if(_init)
		_window->render();
	return !_exit;
}
bool OgreGraphicsManagerImpl::init(void)
{
	
	_root.reset(new Ogre::Root());

	
	//if(!_root->restoreConfig())
	if(!_root->showConfigDialog())
	{
		//SystemInterface::getSingleton().exit();
		return false;
	}
	
	setupResources();
	
	if(SystemInterface::getSingleton().hasParame("WINDOW"))
	{
		Ogre::Root::getSingleton().initialise(false, "Orz Window");
		WeakWindowPtr win;
		try
		{
			win = SystemInterface::getSingleton().getParame<WeakWindowPtr>("WINDOW");
		}
		catch(...)
		{
			std::cout<<"bool init(void) error"<<std::endl;
		}

		_window = createRenderWindowOut(win);
		//!!_window->addListener(this);
	}
	else
	{
		Ogre::Root::getSingleton().initialise(true, "Orz Window");
		_window = createRenderWindowIn();
		//!!_window->addListener(this);
	}
	




	loadResources();
	createSceneManager();
	createCamera();
	createViewports();

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	
	createSimpleScene();



	_init = true;
	//createRenderWindow();

	return true;
}
void OgreGraphicsManagerImpl::shutdown(void)
{
	_init = false;
	destroySimpleScene();
	clearResources();
	destroyViewports();
	destroyCamera();
	destroySceneManager();
	_window.reset();
	_root.reset();
}

OgreWindowPtr OgreGraphicsManagerImpl::createRenderWindowIn(void)
{
	return OgreWindowPtr(new OgreWindowIn(this));
}

OgreWindowPtr OgreGraphicsManagerImpl::createRenderWindowOut(WeakWindowPtr win)
{
	//new OgreWindowOut(win)
	return boost::shared_ptr<OgreWindowOut>(new OgreWindowOut(win, this));
}

void OgreGraphicsManagerImpl::onSizeChange(size_t width, size_t height)
{
	assert(_camera);
	getRenderWindow()->windowMovedOrResized();
	_camera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));
}

void OgreGraphicsManagerImpl::setupResources(void)
{
	Ogre::ConfigFile cf;
	cf.load("resources.cfg");
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}


void OgreGraphicsManagerImpl::clearResources(void)
{
	//Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup()
}

void OgreGraphicsManagerImpl::createSceneManager(void)
{
	_sceneManager = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
}
void OgreGraphicsManagerImpl::destroySceneManager(void)
{
	Ogre::Root::getSingleton().destroySceneManager(_sceneManager);
	_sceneManager = 0;
}


void OgreGraphicsManagerImpl::createCamera(void)
{
	_camera = _sceneManager->createCamera("PlayerCam");
	_camera->setNearClipDistance(5);
}
void OgreGraphicsManagerImpl::destroyCamera(void)
{

}

void OgreGraphicsManagerImpl::createViewports(void)
{
	_viewport = _window->getRenderWindow()->addViewport(_camera);
	_camera->setAspectRatio(
		Ogre::Real(_viewport->getActualWidth()) / Ogre::Real(_viewport->getActualHeight()));
	_viewport->setBackgroundColour(Ogre::ColourValue::Green);

}
void OgreGraphicsManagerImpl::destroyViewports(void)
{

}


void OgreGraphicsManagerImpl::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}



void OgreGraphicsManagerImpl::createSimpleScene(void)
{

	
}
void OgreGraphicsManagerImpl::destroySimpleScene(void)
{

}