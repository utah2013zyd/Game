#include "CameraController.h"

using namespace Orz;


CameraController::CameraController(Ogre::Camera * camera):
_camera(camera), 
_translateVector(Ogre::Vector3::ZERO), 
_rotX(0.0f),
_rotY(0.0f),
_moveScale(0.0f)
{
	/*Ogre::SceneManager * sm = OgreGraphicsManager::getSingleton().getSceneManager();
	_node = sm->getRootSceneNode()->createChildSceneNode("cameraNode");
	_node->attachObject(_camera);
	_camera->setPosition(0, 0, 0);
	*///IInputManager::getSingleton().addKeyListener(this);
	//IInputManager::getSingleton().addMouseListener(this);

}
CameraController::~CameraController(void)
{
	
	//IInputManager::getSingleton().removeMouseListener(this);
//	IInputManager::getSingleton().removeKeyListener(this);
}
//bool CameraController::onMousePressed(const MouseEvent & evt)
//{
//	
//	return true;
//}
//bool CameraController::onMouseReleased(const MouseEvent & evt)
//{
//
//	return true;
//}
void CameraController::log(void)
{
	std::cout<<"Camera: Position,"<<_camera->getPosition()<<"."<<_camera->getOrientation()<<std::endl;
}
void CameraController::update(void)
{
	_camera->yaw(_rotX);
	_camera->pitch(_rotY);
	//_camera->moveRelative(_translateVector);
	_rotX = Ogre::Degree(0);
	_rotY = Ogre::Degree(0);
	FCFighter* temp = (FCFighter*)IDManager::getPointer("player", 0);

	_camera->setPosition(temp->getPosition()+temp->getAxis() * Ogre::Vector3(0.f, 5.f, -60.f));
	//Ogre::Quaternion quat = Ogre::Vector3::UNIT_Z.getRotationTo(temp->getPosition() - _camera->getPosition());
	//_camera->setOrientation(temp->getNode()->getOrientation());
	
	_camera->setDirection(temp->getPosition()-_camera->getPosition());
//	_camera->lookAt(temp->getPosition());
	//temp->getNode()->addChild(_node);
	//_node->setPosition(Ogre::Vector3(0.0, 5.0f, -60.0f));
	//_camera->lookAt(temp->getPosition());

}
//bool CameraController::onMouseMoved(const MouseEvent & evt)
//{
//	_rotX += Ogre::Degree(evt.getX() * -0.13f);
//	_rotY += Ogre::Degree(evt.getY() * -0.13f);
//	return true;
//}

//bool CameraController::onKeyPressed(const KeyEvent & evt)
//{
//	switch(evt.getKey())
//	{
//	case KC_A:
//		std::cout<<"done here!"<<std::endl;
//		_translateVector.x = -1.0f;
//		break;
//	case KC_D:
//		_translateVector.x = 1.0f;
//		break;
//	case KC_S:
//		_translateVector.z = 1.0f;
//		break;
//	case KC_W:
//		_translateVector.z = -1.0f;
//		break;
//	case KC_P:
//		log();
//		break;
//	}
//	return true;
//}
//bool CameraController::onKeyReleased(const KeyEvent & evt)
//{
//	switch(evt.getKey())
//	{
//	case KC_A:
//	case KC_D:
//		_translateVector.x = 0.0f;
//		break;
//	case KC_S:
//	case KC_W:
//		_translateVector.z = 0.0f;
//		break;
//	}
//	return true;
//}
