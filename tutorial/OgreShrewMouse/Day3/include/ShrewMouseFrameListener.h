#ifndef __ShrewMouseFrameListener_H__
#define __ShrewMouseFrameListener_H__

#include "ExampleFrameListener.h"
#include "ShrewMouseManager.h"
#include <sstream>
class ShrewMouseFrameListener: public ExampleFrameListener
{
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
		bool bufferedJoy = false );

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
	{

		

		if( mKeyboard->isKeyDown(OIS::KC_ESCAPE) || mKeyboard->isKeyDown(OIS::KC_Q) )
			return false;

		// Return true to continue rendering
		return true;
	}

	virtual bool processUnbufferedMouseInput(const FrameEvent& evt)
	{

		

		return true;
	}


	bool frameEnded(const FrameEvent& evt)
	{
		ExampleFrameListener::frameEnded(evt);
		if(_miceManager)
			_miceManager->update(evt.timeSinceLastFrame);
		return true;
	}

private:
	ShrewMouseManager * _miceManager;

};
#endif
