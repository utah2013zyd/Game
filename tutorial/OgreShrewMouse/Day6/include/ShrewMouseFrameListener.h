#ifndef __ShrewMouseFrameListener_H__
#define __ShrewMouseFrameListener_H__

#include "ExampleFrameListener.h"
#include "ShrewMouseManager.h"
#include <sstream>
class ShrewMouseFrameListener: public ExampleFrameListener,public OIS::KeyListener
{
public:
	// Constructor takes a RenderWindow because it uses that to determine input context
	ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
		bool bufferedJoy = false );






	bool keyPressed( const OIS::KeyEvent &arg );
	bool keyReleased( const OIS::KeyEvent &arg );		



	bool frameEnded(const FrameEvent& evt)
	{
		ExampleFrameListener::frameEnded(evt);
		if(_miceManager)
			_miceManager->update(evt.timeSinceLastFrame);
		return !_exit;
	}

	void updateScore(void);

private:
	ShrewMouseManager * _miceManager;
	bool _exit;
	int _score;

};
#endif
