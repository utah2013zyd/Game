#include "ShrewMouseFrameListener.h"
#include "ShrewMouseManager.h"

ShrewMouseFrameListener::ShrewMouseFrameListener(ShrewMouseManager * miceManager, RenderWindow* win, Camera* cam, bool bufferedKeys, bool bufferedMouse,
		bool bufferedJoy):ExampleFrameListener(win, cam, bufferedKeys, bufferedMouse, bufferedJoy),_miceManager(miceManager)
{
}
