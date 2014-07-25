#ifndef __Orz_ViewOgre_OgreWindow_h__
#define __Orz_ViewOgre_OgreWindow_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)
#include <orz/Framework_Base/System/WindowInterface.h>

namespace Orz
{

class OgreWindowInterface : public WindowInterface , public Ogre::WindowEventListener
{
public:
	OgreWindowInterface(Ogre::RenderWindow * renderWin):_renderWin(renderWin)
	{
	
	}
	virtual size_t getHandle()
	{

		size_t windowHnd = 0;
		_renderWin->getCustomAttribute("WINDOW", &windowHnd);
		return windowHnd;
	}

	virtual size_t getWidth()
	{
		return _renderWin->getWidth();
	}

	virtual size_t getHeight()
	{
		return _renderWin->getHeight();
	}

	//void onSizeChange(WindowListener * listener)
	//{
	//	listener->onSizeChange(getWidth(), getHeight());
	//}

	virtual void windowResized(Ogre::RenderWindow* rw)
	{
		_sizeChange(getWidth(), getHeight());
		//visitor(boost::bind(&OgreWindowInterface::onSizeChange, this, _1));
	}
	
	virtual void windowClosed(Ogre::RenderWindow* rw)
	{
		_closed();
	}
private:
	Ogre::RenderWindow * _renderWin;
};

class _OrzViewOgrePrivate OgreWindow//, public Ogre::WindowEventListener, public WindowListener
{

public:
	
	virtual void preRender(void){}
	virtual void postRender(void){}
	void render(void)
	{
		preRender();
		Ogre::Root::getSingleton().renderOneFrame();
		postRender();
	}
	OgreWindow(void):_renderWin(NULL)
	{
	}
	virtual ~OgreWindow(void){}
	inline Ogre::RenderWindow * getRenderWindow(void)
	{
		return _renderWin;
	}
protected:
	Ogre::RenderWindow * _renderWin;
};

class _OrzViewOgrePrivate OgreWindowIn: public OgreWindow
{
public:



	
	virtual void preRender(void)
	{
		Ogre::WindowEventUtilities::messagePump();
	}

	
	virtual ~OgreWindowIn(void)
	{

		
		_winInterface->removeListener(_listener);
		Ogre::WindowEventUtilities::removeWindowEventListener(_renderWin, _winInterface.get());
		Ogre::Root::getSingleton().getRenderSystem()->destroyRenderWindow(_renderWin->getName());
	}
	
	

	OgreWindowIn(WindowListener* listener):_winInterface(), _listener(listener)
	{
		try
		{
		
			
			_renderWin = Ogre::Root::getSingleton().getAutoCreatedWindow();//createRenderWindow("OgreWindowIn", 800, 600, false, 0);//->initialise(true, SystemInterface::getSingleton().getInfo().getAppName());
		
		}
		catch(std::exception & e)
		{
			std::cout<<e.what()<<"!!!!!!!!!!!"<<std::endl;
		}

		_winInterface.reset(new OgreWindowInterface(_renderWin));
		_winInterface->addListener(_listener);
		Ogre::WindowEventUtilities::addWindowEventListener(_renderWin, _winInterface.get());

		
		WeakWindowPtr win = WindowPtr(_winInterface);
		
		SystemInterface::getSingleton().setParame<WeakWindowPtr>("WINDOW", win);
	
	}
private:
	
	boost::shared_ptr<OgreWindowInterface> _winInterface;
	WindowListener* _listener;
};

class _OrzViewOgrePrivate OgreWindowOut: public OgreWindow//, public Ogre::WindowEventListener, public WindowListener
{

public:
	

	OgreWindowOut(WeakWindowPtr win, WindowListener* listener):OgreWindow(), _win(win), _listener(listener)
	{


		
		WindowPtr wp = _win.lock();
		wp->addListener(_listener);
		Ogre::NameValuePairList params;
		params["externalWindowHandle"] =  boost::lexical_cast<std::string>(static_cast<unsigned int>(wp->getHandle()));//handle;
				
			
		_renderWin = Ogre::Root::getSingleton().createRenderWindow("Orz Out",//SystemInterface::getSingleton().getInfo().getAppName(), 
			static_cast<unsigned int>(wp->getWidth()),
			static_cast<unsigned int>(wp->getHeight()),//SystemInterface::getSingleton().getInfo().renderHeight, 
			false, 
			&params);
	
	}
	virtual ~OgreWindowOut(void)
	{
		if(WindowPtr wp = _win.lock())
		{
			wp->removeListener(_listener);
		}
	}
	
protected:
	WeakWindowPtr _win;
	WindowListener* _listener;
};
}
#endif
