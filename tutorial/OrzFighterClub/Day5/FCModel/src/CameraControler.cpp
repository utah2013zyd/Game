#include "CameraControler.h"

using namespace Orz;


CameraControler::CameraControler(Ogre::Camera * camera):
_camera(camera), 
_translateVector(Ogre::Vector3::ZERO), 
_rotX(0.0f),
_rotY(0.0f),
_moveScale(0.0f)
{
	IInputManager::getSingleton().addKeyListener(this);
	IInputManager::getSingleton().addMouseListener(this);

}
CameraControler::~CameraControler(void)
{
	
	IInputManager::getSingleton().removeMouseListener(this);
	IInputManager::getSingleton().removeKeyListener(this);
}
bool CameraControler::onMousePressed(const MouseEvent & evt)
{
	
	return true;
}
bool CameraControler::onMouseReleased(const MouseEvent & evt)
{

	return true;
}
void CameraControler::log(void)
{
	ILogManager::getSingleton().utf8("Camera:");
}
void CameraControler::update(void)
{
	_camera->yaw(_rotX);
	_camera->pitch(_rotY);
	_camera->moveRelative(_translateVector);
	_rotX = Ogre::Degree(0);
	_rotY = Ogre::Degree(0);

}
bool CameraControler::onMouseMoved(const MouseEvent & evt)
{
	_rotX += Ogre::Degree(evt.getX() * -0.13);
	_rotY += Ogre::Degree(evt.getY() * -0.13);
	return true;
}

bool CameraControler::onKeyPressed(const KeyEvent & evt)
{
	switch(evt.getKey())
	{
	case KC_A:
		_translateVector.x = -1.0f;
		break;
	case KC_D:
		_translateVector.x = 1.0f;
		break;
	case KC_S:
		_translateVector.z = 1.0f;
		break;
	case KC_W:
		_translateVector.z = -1.0f;
		break;
	case KC_P:
		log();
		break;

	
	}
	return true;
}
bool CameraControler::onKeyReleased(const KeyEvent & evt)
{
	switch(evt.getKey())
	{
	case KC_A:
	case KC_D:
		_translateVector.x = 0.0f;
		break;
	case KC_S:
	case KC_W:
		_translateVector.z = 0.0f;
		break;

	
	}
	return true;
}
