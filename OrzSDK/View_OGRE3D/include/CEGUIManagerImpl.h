#ifndef __Orz_ViewOgre_CEGUIManagerImpl_h__
#define __Orz_ViewOgre_CEGUIManagerImpl_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#include <orz/Framework_Base/FrameworkBasePreDeclare.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/System/SystemInterface.h>

#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#include <Ogre/OgreCEGUIRenderer.h>
#include <Ogre/OgreCEGUIResourceProvider.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4251)
#include <CEGUI/CEGUIImageset.h>
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUILogger.h>
#include <CEGUI/CEGUISchemeManager.h>
#include <CEGUI/CEGUIWindowManager.h>
#include <CEGUI/CEGUIWindow.h>
#include <CEGUI/CEGUIPropertyHelper.h>
#include <CEGUI/elements/CEGUICombobox.h>
#include <CEGUI/elements/CEGUIListbox.h>
#include <CEGUI/elements/CEGUIListboxTextItem.h>
#include <CEGUI/elements/CEGUIPushButton.h>
#include <CEGUI/elements/CEGUIScrollbar.h>
#pragma warning(pop)
#include "OgreGraphicsManagerImpl.h"
namespace Orz {
    

class _OrzViewOgrePrivate CEGUIManagerImpl: public Orz::KeyListener,public Orz::MouseListener, public WindowListener
{
public:
	CEGUIManagerImpl(void):
	_enableMouseEvent(false),
	_enableKeyEvent(false)
	{
		assert(OgreGraphicsManager::getSingletonPtr());
	}
	~CEGUIManagerImpl(void)
	{
	
	}
	
	
	void onSizeChange(size_t width, size_t height)
	{

		_renderer->setDisplaySize(CEGUI::Size(static_cast<float>(width), static_cast<float>(height)));
		//GUIRenderer->setDisplaySize(CEGUI::Size(Ogre::Real(vp->getActualWidth()), Ogre::Real(vp->getActualHeight())));
		//CEGUI::System::getSingleton().
	}
	bool init(void)
	{


		
		if(!SystemInterface::getSingleton().hasParame("WINDOW"))
			return false;

		WeakWindowPtr win;
		try
		{
			win = SystemInterface::getSingleton().getParame<WeakWindowPtr>("WINDOW");//win = get_any_ptr<WeakWindowPtr>(SystemInterface::getSingleton().getParame("WINDOW"));
		}
		catch(...)
		{
			std::cout<<"bool init(void) error"<<std::endl;
		}
		WindowPtr wp;
		if(wp = win.lock())
		{
			wp->addListener(this);
			// use *r
		}
		else
		{
			return false;
		}



		_enableMouseEvent = false;
		_enableKeyEvent = false;


		_renderer.reset(new CEGUI::OgreCEGUIRenderer(
			OgreGraphicsManager::getSingleton().getRenderWindow(), 
            Ogre::RENDER_QUEUE_OVERLAY, false, 3000, 
			OgreGraphicsManager::getSingleton().getSceneManager()));


		_system.reset(new CEGUI::System(_renderer.get()));

		CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
		return true;
		
	}
	void shutdown(void)
	{
	
	
		_system.reset();
		_renderer.reset();
	}
	CEGUI::Renderer * getRenderer(void)
	{
		return _renderer.get();
	}

	CEGUI::System * getSystem(void)
	{
		return _system.get();
	}

	
	void mouseEvent(bool enable)
	{
		if(_enableMouseEvent == enable)
			return;
		_enableMouseEvent = enable;
		if(_enableMouseEvent)
			Orz::IInputManager::getSingleton().addMouseListener(this);
		else
			Orz::IInputManager::getSingleton().removeMouseListener(this);
	}
	void keyEvent(bool enable)
	{
		if(_enableKeyEvent == enable)
			return;
		_enableKeyEvent = enable;
		if(_enableKeyEvent)
			Orz::IInputManager::getSingleton().addKeyListener(this);
		else
			Orz::IInputManager::getSingleton().removeKeyListener(this);

	}

	virtual bool onMousePressed(const MouseEvent & evt)
	{
		return CEGUI::System::getSingleton().injectMouseButtonDown(convertOrzMouseButtonToCegui(evt.getButton()));
	}
	virtual bool onMouseReleased(const MouseEvent & evt)
	{
		return CEGUI::System::getSingleton().injectMouseButtonUp(convertOrzMouseButtonToCegui(evt.getButton()));
	}
	virtual bool onMouseMoved(const MouseEvent & evt)
	{
		if(SystemInterface::getSingleton().hasParame("w32_mouse") && SystemInterface::getSingleton().getParame<bool>("w32_mouse"))
		{
			CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(evt.getAbsX()), static_cast<float>(evt.getAbsY()));//injectMouseMove( static_cast<float>(evt.getX()), static_cast<float>(evt.getY()) );
		}
		else
			CEGUI::System::getSingleton().injectMouseMove(static_cast<float>(evt.getX()), static_cast<float>(evt.getY()));//injectMouseMove( static_cast<float>(evt.getX()), static_cast<float>(evt.getY()) );
		
		int z = evt.getZ();
		
		if(z != 0)
		{
			CEGUI::System::getSingleton().injectMouseWheelChange(static_cast<float>(evt.getZ()));
		}
		return false;
	}

	virtual bool onKeyPressed(const KeyEvent & evt)
	{
		bool ret = CEGUI::System::getSingleton().injectKeyDown( evt.getKey() ) ;
		return CEGUI::System::getSingleton().injectChar( evt.getText() )|| ret;
	}
	virtual bool onKeyReleased(const KeyEvent & evt)
	{
		return CEGUI::System::getSingleton().injectKeyUp( evt.getKey() );
	}

private:
	boost::scoped_ptr<CEGUI::OgreCEGUIRenderer> _renderer;
	boost::scoped_ptr<CEGUI::System> _system;
	bool _enableMouseEvent;
	bool _enableKeyEvent;
	static CEGUI::MouseButton convertOrzMouseButtonToCegui(MouseButtonID id)
	{
		switch (id)
		{
		case MB_Left: return CEGUI::LeftButton;
		case MB_Right: return CEGUI::RightButton;
		case MB_Middle:	return CEGUI::MiddleButton;
		case MB_Button3: return CEGUI::X1Button;
		default: return CEGUI::LeftButton;
		}
	}

};
}

#endif

