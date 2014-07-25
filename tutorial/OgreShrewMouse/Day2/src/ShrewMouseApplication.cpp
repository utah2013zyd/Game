#include "ShrewMouseApplication.h"
#include "ShrewMouseFrameListener.h"
#include "ShrewMouseManager.h"
using namespace Ogre;

ShrewMouseApplication::ShrewMouseApplication(void):_miceManager(NULL)
{
	
}
ShrewMouseApplication::~ShrewMouseApplication(void)
{
	//�����������ͷŵ��������
	if(_miceManager)
		delete _miceManager;
}


void ShrewMouseApplication::createScene(void)
{

	//���ó���������
	mSceneMgr->setAmbientLight(ColourValue(0.5, 0.5, 0.5));

	//����һ���ƹ�
	Light* l = mSceneMgr->createLight("MainLight");
	//���õƹ�λ��
	l->setPosition(20,80,50);
	
	//���������λ��
	mCamera->setPosition(94.8765f, 194.043f, 373.974f);
	//����������Ƕ�
	mCamera->setOrientation(Ogre::Quaternion(0.953763f, -0.300446f, -0.00865638f, -0.00272705f));
	//����һ�����������������Ogre3D�����г�����������Ϊ�����ṩ��
	_miceManager = new ShrewMouseManager(mSceneMgr);
}
void ShrewMouseApplication::createFrameListener(void)
{
	//�������Լ���FrameListener���������ṩ��Ogre3Dͼ������
    mFrameListener= new ShrewMouseFrameListener(_miceManager, mWindow, mCamera);
    mRoot->addFrameListener(mFrameListener);
}