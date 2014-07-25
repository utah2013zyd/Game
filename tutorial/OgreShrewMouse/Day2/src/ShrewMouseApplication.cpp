#include "ShrewMouseApplication.h"
#include "ShrewMouseFrameListener.h"
#include "ShrewMouseManager.h"
using namespace Ogre;

ShrewMouseApplication::ShrewMouseApplication(void):_miceManager(NULL)
{
	
}
ShrewMouseApplication::~ShrewMouseApplication(void)
{
	//在最后别忘了释放地鼠管理器
	if(_miceManager)
		delete _miceManager;
}


void ShrewMouseApplication::createScene(void)
{

	//设置场景环境光
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//创建一个灯光
	Light* l = mSceneMgr->createLight("MainLight");
	//设置灯光位置
	l->setPosition(20,80,50);
	
	//设置摄像机位置
	mCamera->setPosition(94.8765f, 194.043f, 373.974f);
	//设置摄像机角度
	mCamera->setOrientation(Ogre::Quaternion(0.953763f, -0.300446f, -0.00865638f, -0.00272705f));
	//创建一个地鼠管理器，并把Ogre3D引擎中场景管理器作为参数提供。
	_miceManager = new ShrewMouseManager(mSceneMgr);
}
void ShrewMouseApplication::createFrameListener(void)
{
	//把我们自己的FrameListener派生类型提供给Ogre3D图形引擎
    mFrameListener= new ShrewMouseFrameListener(_miceManager, mWindow, mCamera);
    mRoot->addFrameListener(mFrameListener);
}