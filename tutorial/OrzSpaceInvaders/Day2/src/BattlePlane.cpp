#include "BattlePlane.h"
using namespace Orz;


//(Day2)战斗机构造函数
BattlePlane::BattlePlane(Ogre::SceneManager * sm):
_node(NULL),
_leftDown(false),
_rightDown(false)
{
	using namespace Ogre;

	//(Day2)监听键盘消息
	IInputManager::getSingleton().addKeyListener(this);



    //(Day2)通过飞机模型创建一个实体
	Ogre::Entity *ent = sm->createEntity( "razor", "razor.mesh" );

	//(Day2)创建一个节点
	_node = sm->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(200, 60,250));

	//(Day2)设置位置
//	_node->setPosition(0, -320, 0);
	//(Day2)在创建一个节点用于调整飞机模型方向
	Ogre::SceneNode * sn = _node->createChildSceneNode();
	//(Day2)roll PI
//	sn->roll(Ogre::Radian(Ogre::Math::PI));
	//(Day2)设置朝向
//	sn->setDirection(0.0f, 1.0f ,0.0f );
	sn->setScale(0.1, 0.1, 0.1);	
	//(Day2)最后在节点上面挂接实体
	sn->attachObject( ent );
	
}

BattlePlane::~BattlePlane(void)
{
	//(Day2)注销键盘监听
	IInputManager::getSingleton().removeKeyListener(this);
}

bool BattlePlane::onKeyPressed(const KeyEvent & evt)
{

	//(Day2)处理键盘按下消息
	if(evt.getKey() == KC_LEFT)
		_leftDown = true;
	else if(evt.getKey() == KC_RIGHT)
		_rightDown = true;
	return true;
}
bool BattlePlane::onKeyReleased(const KeyEvent & evt)
{

	
	//(Day2)处理键盘释放消息
	if(evt.getKey() == KC_LEFT)
		_leftDown = false;
	else if(evt.getKey() == KC_RIGHT)
		_rightDown = false;
	return true;
}

void BattlePlane::update(void)
{
	//(Day2)在每一frame中更新飞机位置
	if(_leftDown && _rightDown)
		return;
	if(_leftDown && _node->getPosition().x > -530.0f)
		_node->translate(-10,0,0);
	if(_rightDown&& _node->getPosition().x < 530.0f)
		_node->translate(10,0,0);
}