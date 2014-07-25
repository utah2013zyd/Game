#include "PlayerController.h"
#include <iostream>
using namespace Orz;

PlayerController::PlayerController(const std::string & name, const std::string & fighterName): Actor(name), _fighterName(fighterName)
{
	IInputManager::getSingleton().addKeyListener(this);
	IInputManager::getSingleton().addMouseListener(this);
}

PlayerController::~PlayerController(void)
{
	IInputManager::getSingleton().removeKeyListener(this);
	IInputManager::getSingleton().removeMouseListener(this);
}

bool PlayerController::onMousePressed(const MouseEvent & evt)
{
	if(evt.getButton() == MB_Left)
	{
		Event* evt = FCEvents::createEvent(FCEvents::FIRE);
		evt->setSender(this->shared_from_this());
		evt->setReceiver(((FCFighter*)IDManager::getPointer(_fighterName, ACTOR))->shared_from_this());
		getWorld()->broadcasting(evt);
	}
	return true;
}

bool PlayerController::onMouseReleased(const Orz::MouseEvent &evt)
{
	return true;
}

bool PlayerController::onMouseMoved(const Orz::MouseEvent &evt)
{
	return true;
}

bool PlayerController::onKeyPressed(const Orz::KeyEvent &evt)
{
	FCFighter* player = (FCFighter*)IDManager::getPointer(_fighterName, ACTOR);
	switch(evt.getKey())
	{
	case KC_A:
		player->yaw(1);
		break;
	case KC_D:
		player->yaw(-1);
		break;
	case KC_W:
		player->pitch(-1);
		break;
	case KC_S:
		player->pitch(1);
		break;
	case KC_Q:
		player->roll(-1);
		break;
	case KC_E:
		player->roll(1);
		break;
	case KC_SPACE:
		//player->forward(1);
		break;
	}

	return true;
}

bool PlayerController::onKeyReleased(const Orz::KeyEvent &evt)
{
	FCFighter* player = (FCFighter*)IDManager::getPointer(_fighterName, ACTOR);
	switch(evt.getKey())
	{
	case KC_A:
	case KC_D:
		player->yaw(0);
		break;

	case KC_W:
	case KC_S:
		player->pitch(0);
		break;

	case KC_Q:
	case KC_E:
		player->roll(0);
		break;
	case KC_SPACE:
		//player->forward(1);
		break;
	}

	return true;
}

const std::string & PlayerControllerFactory::getTypeName() const
{
	static const std::string typeName("PlayerController");
	return typeName;
}
PlayerControllerFactory::pointer_type PlayerControllerFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	std::string playerName;

	if(parameter)
	{

		parameter_iterator it = parameter->find("playerName");

		if(it!= parameter->end())
		{
			playerName = VariantData<std::string>::get(it->second);
						std::cout << "into create instance"<<std::endl;
		}
	}

	return pointer_type(new PlayerController(instanceName, playerName));
}