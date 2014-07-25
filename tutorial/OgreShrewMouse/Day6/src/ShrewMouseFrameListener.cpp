#include "ShrewMouseFrameListener.h"
#include "ShrewMouseManager.h"

ShrewMouseFrameListener::ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys, bool bufferedMouse,
												 bool bufferedJoy):ExampleFrameListener(win, cam, bufferedKeys, bufferedMouse, bufferedJoy),_miceManager(miceManager)
{
	
	_exit = false;
	mKeyboard->setEventCallback(this);
	showDebugOverlay(false);
	_score = 0;
	OverlayManager::getSingleton().getByName("ShrewMouse")->show();
	updateScore();
}


void ShrewMouseFrameListener::updateScore(void)
{

	OverlayElement* score = OverlayManager::getSingleton().getOverlayElement("ShrewMouse/Score");
	score->setCaption(Ogre::String("Score: =") + Ogre::StringConverter::toString(_score));
}



bool ShrewMouseFrameListener::keyPressed( const OIS::KeyEvent &arg )
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
	{
		bool ret = _miceManager->hit(who);
		if(ret)
		{
			_score++;
			updateScore();
		}
	}
	return false;
}
bool ShrewMouseFrameListener::keyReleased( const OIS::KeyEvent &arg )
{

	if(arg.key == OIS::KC_ESCAPE || arg.key == (OIS::KC_Q) )
		_exit =true;
	return true;
}	