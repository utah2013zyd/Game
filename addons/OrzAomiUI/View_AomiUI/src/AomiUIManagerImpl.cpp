#include "AomiUIManagerImpl.h"
#include <orz/View_OGRE3D/OgreGraphicsManager.h>

#include <orz/Framework_Base/System/SystemInterface.h>
#include <stdlib.h>
#include <direct.h>

using namespace Orz;




std::string getCurrentWorkingDirectory()
{
	std::string workingDirectory = "";
	char currentPath[_MAX_PATH];
	getcwd(currentPath, _MAX_PATH);
	workingDirectory = currentPath;

	return workingDirectory + "\\";
}



AomiUIManagerImpl::AomiUIManagerImpl(void):
	_enableMouseEvent(false)
	//,_enableKeyEvent(false)
{

}
AomiUIManagerImpl::~AomiUIManagerImpl(void)
{
	
}


bool AomiUIManagerImpl::init(void)
{
	
	/*_viewport = OgreGraphicsManager::getSingleton().getCamera()->getViewport();
	if(Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNonPOW2TexturesLimited())
	{
		std::cerr << "Error! Rendering device has limited/no support for NPOT textures, falling back to 512x512 dimensions.\n";
		_width = _height = 512;
	}
	else
	{
		_width = _viewport->getActualWidth();
		_height = _viewport->getActualHeight();
	}*/
	//createOverlayAndMaterials();
	mouseEvent(true);
//	keyEvent(true);
	
	setupAwesomium();

	return true;

}


void AomiUIManagerImpl::setupAwesomium(void)
{
	_core.reset(new Awesomium::WebCore(Awesomium::LOG_VERBOSE));
	if(SystemInterface::getSingleton().hasParame("AomiUI_MediaDirectory"))
	{
		try
		{
			std::string path = SystemInterface::getSingleton().getParame<std::string>("AomiUI_MediaDirectory");
			_core->setBaseDirectory(getCurrentWorkingDirectory() + path); 
		}
		catch(...)
		{
			_core->setBaseDirectory(getCurrentWorkingDirectory() + "..\\media");
		}
	}
	else
	{
		_core->setBaseDirectory(getCurrentWorkingDirectory() + "..\\media");
		
	}
	

	_core->setCustomResponsePage(404, "404response.html");

}
void AomiUIManagerImpl::shutdown(void)
{

	
	_views.clear();
	
	_core.reset();
	//!!
}
bool AomiUIManagerImpl::update(TimeType i)
{
	draw();
	_core->update();
	return true;
}



void AomiUIManagerImpl::draw()
{
	
	std::map<std::string, AomiViewPtr>::iterator it;// = _views.find(name);
	for(it = _views.begin(); it != _views.end(); ++it)
	{
		it->second->draw();
	}
	
}
//AomiView * AomiUIManagerImpl::createAomiView(const std::string & name, int width, int height)
//{
//
//	AomiViewPtr ptr(new AomiView(_core.get(), name, width, height));
//	_views[name] = ptr;
//	return ptr.get();
//	 
//
//}
OverlayView* AomiUIManagerImpl::createOverlayView(const std::string & name, int width, int height)
{	
	OverlayViewPtr ptr(new OverlayView(_core.get(), name, width, height));
	_views[name] = ptr;
	return ptr.get();
}

void AomiUIManagerImpl::destroyView(const std::string & name)
{
	std::map<std::string, AomiViewPtr>::iterator it = _views.find(name);
	if(it != _views.end())
	{
		
		it->second.reset();
		_views.erase(it);
	}
}

bool AomiUIManagerImpl::onMousePressed(const MouseEvent & evt)
{
	if(evt.getButton() == MB_Right)
	{
		return false; 
	}
	
	std::map<std::string, AomiViewPtr>::iterator it;
	for(it = _views.begin(); it != _views.end(); ++it)
	{
		it->second->injectMouseDown(evt.getButton());
	}
	return false; 
	
}
bool AomiUIManagerImpl::onMouseReleased(const MouseEvent & evt)
{

	std::map<std::string, AomiViewPtr>::iterator it;
	for(it = _views.begin(); it != _views.end(); ++it)
	{
		it->second->injectMouseUp(evt.getButton());
	}
	
	return false; 
	
}
bool AomiUIManagerImpl::onMouseMoved(const MouseEvent & evt)
{

	std::map<std::string, AomiViewPtr>::iterator it;
	if(evt.getZ())
	{
		for(it = _views.begin(); it != _views.end(); ++it)
		{
			it->second->injectMouseWheel(evt.getZ());
		}
	}
	else
	{
		for(it = _views.begin(); it != _views.end(); ++it)
		{
			it->second->injectMouseMove(evt.getAbsX(), evt.getAbsY());
		}
	}
	return false; 
	
}