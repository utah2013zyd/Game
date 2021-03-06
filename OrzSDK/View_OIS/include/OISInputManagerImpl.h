﻿#ifndef __Orz_ViewOIS_OISInputManagerImpl_h__
#define __Orz_ViewOIS_OISInputManagerImpl_h__
#include <orz/View_OIS/ViewOISConfig.h>
#include <orz/Framework_Base/System/SystemInterface.h>
#include <orz/Framework_Base/System/WindowInterface.h>
#include <orz/View_OIS/OISInputManager.h>

#include <Ogre/OIS/OIS.h>

namespace Orz
{

	class _OrzViewOISPrivate OISInputManagerImpl:
		public OIS::KeyListener,public OIS::MouseListener, public OIS::JoyStickListener, public Orz::KeyListener, public Orz::WindowListener

	{
	public:
		OISInputManagerImpl(void):_inputManager(NULL)
		{
		}
		bool init(IInputManager * inputManager)
		{	

			_inputManager = inputManager;
			if(!SystemInterface::getSingleton().hasParame("WINDOW"))
				return false;

			WeakWindowPtr win;
			try
			{
				win = SystemInterface::getSingleton().getParame<WeakWindowPtr>("WINDOW");
			}
			catch(...)
			{
				std::cout<<"bool init(void) error"<<std::endl;
			}
			WindowPtr wp;
			if(wp = win.lock())
			{
				std::cout<<wp->getHandle()<<std::endl;
				// use *r
			}
			else
			{
				return false;
			}


			OIS::ParamList paramList;    

			wp->addListener(this);
			paramList.insert( std::make_pair( std::string( "WINDOW" ), boost::lexical_cast<std::string>(wp->getHandle()) ) );

			if(SystemInterface::getSingleton().hasParame("w32_mouse"))
			{
				if(SystemInterface::getSingleton().getParame<bool>("w32_mouse"))
				{
					paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
					paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
				}
			}
			_oisManager = OIS::InputManager::createInputSystem(paramList);
			if(_oisManager->numKeyboards() > 0)
			{
				_keyboard = static_cast<OIS::Keyboard*>(_oisManager->createInputObject( OIS::OISKeyboard, true ));
				_keyboard->setEventCallback(this);
			}
			if(_oisManager->numMice() > 0)
			{
				_mouse = static_cast<OIS::Mouse*>(_oisManager->createInputObject( OIS::OISMouse, true ));
				_mouse->setEventCallback(this);
				const OIS::MouseState &ms = _mouse->getMouseState();
				ms.width = static_cast<int>(wp->getWidth());
				ms.height = static_cast<int>(wp->getHeight());
			}
			if(_oisManager->numJoySticks() > 0)			
			{	
				_joystick = static_cast<OIS::JoyStick*>(_oisManager->createInputObject(OIS::OISJoyStick, true));
				_joystick->setEventCallback(this);
			}
			_inputManager->addKeyListener(this);

			return true;
		}
		void shutdown(void)
		{	
			_inputManager = NULL;
			if(_oisManager)
			{
				if(_mouse)
				{	
					_oisManager->destroyInputObject( _mouse );
					_mouse = NULL;
				}
				if(_keyboard)
				{
					_oisManager->destroyInputObject( _keyboard );
					_keyboard = NULL;
				}
				if(_joystick)
				{
					_oisManager->destroyInputObject(_joystick);
					_joystick = NULL;
				}
				OIS::InputManager::destroyInputSystem(_oisManager);
				_oisManager = NULL;//y::TDiJoyConnecter& Orz::Joy::TDiJoyConnecter::getSingleton(): Assertion `_singleton' failed.

			}


		}

		bool update(TimeType)
		{

			if(_keyboard)
				_keyboard->capture();
			if(_mouse)
				_mouse->capture();
			if(_joystick)	
				_joystick->capture();
			return true;

		}
		~OISInputManagerImpl(void)
		{

		}



	
	private:
		
		virtual void onSizeChange(size_t width, size_t height)
		{
			const OIS::MouseState &ms = _mouse->getMouseState();
			ms.width = width;
			ms.height = height;
		}
		virtual bool onKeyPressed(const KeyEvent & evt)
		{
			if(evt.getText() == 27)
			{
				SystemInterface::getSingleton().exit();
				return true;
			}
			return false;
		}


		virtual bool onKeyReleased(const KeyEvent & evt)
		{
			return false;
		}


		virtual bool keyPressed( const OIS::KeyEvent &arg)
		{
			if(_inputManager)
				_inputManager->_keyPressed(KeyEvent(static_cast<Orz::KeyCode>(arg.key) ,arg.text));
			return true;
		}

		virtual bool keyReleased( const OIS::KeyEvent &arg )
		{
			if(_inputManager)
				_inputManager->_keyReleased(KeyEvent(static_cast<Orz::KeyCode>(arg.key) ,arg.text));
			return true;
		}

		virtual bool mouseMoved( const OIS::MouseEvent &arg )
		{
			if(_inputManager)
				_inputManager->_mouseMoved(MouseEvent(arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, arg.state.Z.rel));
			return true;
		}



		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
		{
			if(_inputManager)
				_inputManager->_mousePressed(MouseEvent( arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, static_cast<MouseButtonID>(id)));
			return true;
		}


		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
		{
			if(_inputManager)
				_inputManager->_mouseReleased(MouseEvent( arg.state.X.rel, arg.state.Y.rel,  arg.state.X.abs, arg.state.Y.abs, static_cast<MouseButtonID>(id)));
			return true;
		}

		virtual bool buttonPressed(const OIS::JoyStickEvent &arg, int button)
		{
			//std::cout  << axis<<"         " << arg.state.mAxes[axis].abs << std::endl;
			//std::cout << "button pressed: " << button << std::endl;
			if(_inputManager)
				_inputManager->_joystickPressed(JoyStickEvent(button));
			return true;
		}

		virtual bool buttonReleased(const OIS::JoyStickEvent &arg, int button)
		{
			if(_inputManager)
				_inputManager->_joystickReleased(JoyStickEvent(button));
			return true;
		}

		// d pad left/right 0
		// d pad up/down 1
		// right joystick up/down 2
		// right joystick left/right 3
		// RT 4
		virtual bool axisMoved(const OIS::JoyStickEvent &arg, int axis)
		{
			//std::cout  << axis<<"         " << arg.state.mAxes[axis].abs << std::endl;
			if(_inputManager)
				_inputManager->_joystickAxisMoved(JoyStickEvent(axis, arg.state.mAxes[axis].abs));
			return true;
		}



	private:

		IInputManager * _inputManager;
		OIS::InputManager  * _oisManager;
		OIS::Mouse* _mouse;
		OIS::Keyboard* _keyboard;
		OIS::JoyStick* _joystick;

	};


}

#endif
