#include "BattlePlane.h"
using namespace Orz;


//(Day2)ս�������캯��
BattlePlane::BattlePlane(Ogre::SceneManager * sm):
_node(NULL),
_leftDown(false),
_rightDown(false)
{
	using namespace Ogre;

	//(Day2)����������Ϣ
	IInputManager::getSingleton().addKeyListener(this);



    //(Day2)ͨ���ɻ�ģ�ʹ���һ��ʵ��
	Ogre::Entity *ent = sm->createEntity( "razor", "razor.mesh" );

	//(Day2)����һ���ڵ�
	_node = sm->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(200, 60,250));

	//(Day2)����λ��
//	_node->setPosition(0, -320, 0);
	//(Day2)�ڴ���һ���ڵ����ڵ����ɻ�ģ�ͷ���
	Ogre::SceneNode * sn = _node->createChildSceneNode();
	//(Day2)roll PI
//	sn->roll(Ogre::Radian(Ogre::Math::PI));
	//(Day2)���ó���
//	sn->setDirection(0.0f, 1.0f ,0.0f );
	sn->setScale(0.1, 0.1, 0.1);	
	//(Day2)����ڽڵ�����ҽ�ʵ��
	sn->attachObject( ent );
	
}

BattlePlane::~BattlePlane(void)
{
	//(Day2)ע�����̼���
	IInputManager::getSingleton().removeKeyListener(this);
}

bool BattlePlane::onKeyPressed(const KeyEvent & evt)
{

	//(Day2)������̰�����Ϣ
	if(evt.getKey() == KC_LEFT)
		_leftDown = true;
	else if(evt.getKey() == KC_RIGHT)
		_rightDown = true;
	return true;
}
bool BattlePlane::onKeyReleased(const KeyEvent & evt)
{

	
	//(Day2)��������ͷ���Ϣ
	if(evt.getKey() == KC_LEFT)
		_leftDown = false;
	else if(evt.getKey() == KC_RIGHT)
		_rightDown = false;
	return true;
}

void BattlePlane::update(void)
{
	//(Day2)��ÿһframe�и��·ɻ�λ��
	if(_leftDown && _rightDown)
		return;
	if(_leftDown && _node->getPosition().x > -530.0f)
		_node->translate(-10,0,0);
	if(_rightDown&& _node->getPosition().x < 530.0f)
		_node->translate(10,0,0);
}