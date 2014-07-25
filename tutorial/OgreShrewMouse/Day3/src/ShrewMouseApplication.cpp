#include "ShrewMouseApplication.h"
#include "ShrewMouseFrameListener.h"
#include "ShrewMouseManager.h"
using namespace Ogre;

ShrewMouseApplication::ShrewMouseApplication(void):_miceManager(NULL)
{
	
}
ShrewMouseApplication::~ShrewMouseApplication(void)
{
	if(_miceManager)
		delete _miceManager;
}


void ShrewMouseApplication::createScene(void)
{
    // Set ambient light
    
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	mCamera->setPosition(94.8765, 194.043, 373.974);
	mCamera->setOrientation(Ogre::Quaternion(0.953763, -0.300446, -0.00865638, -0.00272705));
	_miceManager = new ShrewMouseManager(mSceneMgr);
	

}
void ShrewMouseApplication::createFrameListener(void)
{
    mFrameListener= new ShrewMouseFrameListener(_miceManager, mWindow, mCamera);
    mRoot->addFrameListener(mFrameListener);
}