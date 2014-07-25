#ifndef __ShrewMouseFrameListener_H__
#define __ShrewMouseFrameListener_H__

#include "ExampleFrameListener.h"
#include "ShrewMouseManager.h"
#include <sstream>
class ShrewMouseFrameListener: public ExampleFrameListener, public OIS::KeyListener
{
private:
	ShrewMouseManager * _miceManager;
	bool _exit;
public:
	ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys, bool bufferedMouse,
		bool bufferedJoy ):ExampleFrameListener(win, cam, bufferedKeys, bufferedMouse, bufferedJoy),_miceManager(miceManager)
	{
		_exit = false;
		mKeyboard->setEventCallback(this);
	}

	bool keyPressed( const OIS::KeyEvent &arg )
	{
		int who = -1;
		switch(arg.key)
		{
		case OIS::KC_NUMPAD1:
			who = 6;
			break;
		case OIS::KC_NUMPAD2:
			who = 7;
			break;
		case OIS::KC_NUMPAD3:
			who = 8;
			break;
		case OIS::KC_NUMPAD4:
			who = 3;
			break;
		case OIS::KC_NUMPAD5:
			who = 4;
			break;
		case OIS::KC_NUMPAD6:
			who = 5;
			break;
		case OIS::KC_NUMPAD7:
			who = 0;
			break;
		case OIS::KC_NUMPAD8:
			who = 1;
			break;
		case OIS::KC_NUMPAD9:
			who = 2;
			break;
		}
		if(who != -1)
			return _miceManager->hit(who);
		
		 return false;
	}
	bool keyReleased( const OIS::KeyEvent &arg )
	{

		if(arg.key == OIS::KC_ESCAPE || arg.key == (OIS::KC_Q) )
			_exit =true;
		return true;
	}	

	bool frameEnded(const FrameEvent& evt)
	{
		ExampleFrameListener::frameEnded(evt);
		if(_miceManager)
			_miceManager->update(evt.timeSinceLastFrame);
		return !_exit;
	}


};
#endif
