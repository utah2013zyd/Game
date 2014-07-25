#ifndef __ShrewMouseFrameListener_H__
#define __ShrewMouseFrameListener_H__

#include "ExampleFrameListener.h"
#include <sstream>
class ShrewMouseManager;
class ShrewMouseFrameListener: public ExampleFrameListener
{
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
		bool bufferedJoy = false );

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
	{

		if(mKeyboard->isKeyDown(OIS::KC_SPACE))
		{
			std::stringstream ss;
			ss<<this->mCamera->getPosition()<<this->mCamera->getOrientation()<<std::endl;
			OutputDebugStringA(ss.str().c_str());
			return true;
		}
		return ExampleFrameListener::processUnbufferedKeyInput(evt);
	
	}
private:
	ShrewMouseManager * _miceManager;

};
#endif
