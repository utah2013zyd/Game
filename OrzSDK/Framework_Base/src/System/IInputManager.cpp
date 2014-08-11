
#include <orz/Framework_Base/System/IInputManager.h>
//#include "System/IInputManagerImpl.h"

using namespace Orz;

template<> IInputManager* Singleton<IInputManager>::_singleton = NULL;


IInputManager::IInputManager(void)//:_impl(new IInputManagerImpl())
{}
IInputManager::~IInputManager(void)
{

}

//void visitor(IInputManager::MouseVisitorFun fun)
//{
//	for(MouseListenerList::iterator it = _mouseListener.begin(); it != _mouseListener.end(); ++it)
//	{
//		fun(*it);
//	}
//}
//
//void visitor(IInputManager::KeyVisitorFun fun)
//{
//	for(KeyListenerList::iterator it = _keyListener.begin(); it != _keyListener.end(); ++it)
//	{
//		fun(*it);
//	}
//}


void IInputManager::addMouseListener(MouseListener * listener)
{
	_mouseListener.push_back(listener);
}
void IInputManager::removeMouseListener(MouseListener * listener)
{
	_mouseListener.erase(std::remove(_mouseListener.begin(), _mouseListener.end(), listener), _mouseListener.end());
}
void IInputManager::addKeyListener(KeyListener * listener)
{
	_keyListener.push_back(listener);
}
void IInputManager::removeKeyListener(KeyListener * listener)
{
	_keyListener.erase(std::remove(_keyListener.begin(), _keyListener.end(), listener), _keyListener.end());
}
void IInputManager::addJoyStickListener(JoyStickListener * listener)
{
	_joystickListener.push_back(listener);
}
void IInputManager::removeJoyStickListener(JoyStickListener * listener)
{
	_joystickListener.erase(std::remove(_joystickListener.begin(), _joystickListener.end(), listener), _joystickListener.end());
}

void IInputManager::_mousePressed(const MouseEvent & evt)
{
	bool ret = false;
	BOOST_FOREACH(MouseListener * listener, _mouseListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onMousePressed(evt) || ret;
		}
	}
}
void IInputManager::_mouseReleased(const MouseEvent & evt)
{

	bool ret = false;
	BOOST_FOREACH(MouseListener * listener, _mouseListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onMouseReleased(evt) || ret;
		}
	}

}
void IInputManager::_mouseMoved(const MouseEvent & evt)
{

	bool ret = false;
	BOOST_FOREACH(MouseListener * listener, _mouseListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onMouseMoved(evt) || ret;
		}
	}


}

void IInputManager::_keyPressed(const KeyEvent & evt)
{

	
	bool ret = false;
	BOOST_FOREACH(KeyListener * listener, _keyListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onKeyPressed(evt) || ret;
		}
	}


	
}
void IInputManager::_keyReleased(const KeyEvent & evt)
{
	bool ret = false;
	BOOST_FOREACH(KeyListener * listener, _keyListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onKeyReleased(evt) || ret;
		}
	}	
}

void IInputManager::_joystickPressed(const JoyStickEvent & evt)
{
	bool ret = false;
	BOOST_FOREACH(JoyStickListener * listener, _joystickListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onButtonPressed(evt) || ret;
		}
	}
}

void IInputManager::_joystickReleased(const Orz::JoyStickEvent &evt)
{
	bool ret = false;
	BOOST_FOREACH(JoyStickListener * listener, _joystickListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onButtonReleased(evt) || ret;
		}
	}
}

void IInputManager::_joystickAxisMoved(const Orz::JoyStickEvent &evt)
{
	bool ret = false;
	BOOST_FOREACH(JoyStickListener * listener, _joystickListener)
	{
		if(!ret || listener->getAlways())
		{
			ret = listener->onAxisMoved(evt) || ret;
		}
	}
}
//void IInputManager::mouseVisitor(MouseVisitorFun fun)
//{
//	_impl->visitor(fun);
//}
//
//void IInputManager::keyVisitor(KeyVisitorFun fun)
//{
//	_impl->visitor(fun);
//}
//

//MouseListener::MouseListener(void)
//{
//	
//}
//
//
//
//KeyListener::KeyListener(void)
//{
//	
//}
//KeyListener::~KeyListener(void)
//{
//	
//}