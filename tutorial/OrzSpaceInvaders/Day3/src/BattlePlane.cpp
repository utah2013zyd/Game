#include "BattlePlane.h"
using namespace Orz;


//(Day2)战斗机构造函数
BattlePlane::BattlePlane(Ogre::SceneManager * sm):
_node(NULL),
_leftDown(false),
_rightDown(false),
_bulletManager(sm, 20)
{
	using namespace Ogre;

	//(Day2)监听键盘消息
	IInputManager::getSingleton().addKeyListener(this);



    //(Day2)通过飞机模型创建一个实体
	Ogre::Entity *ent = sm->createEntity( "razor", "razor.mesh" );

	//(Day2)创建一个节点
	_node = sm->getRootSceneNode()->createChildSceneNode();

	//设置位置
	_node->setPosition(0, -320, 0);
	//(Day2)在创建一个节点用于调整飞机模型方向
	Ogre::SceneNode * sn = _node->createChildSceneNode();
	//(Day2)roll PI
	sn->roll(Ogre::Radian(Ogre::Math::PI));

	//(Day2)设置朝向
	sn->setDirection(0.0f, 1.0f ,0.0f );
	
	//(Day2)最后在节点上面挂接实体
	sn->attachObject( ent );



	//(Day4)创建粒子系统
	_thrusters = sm->createParticleSystem( "ParticleSys1", 200 );

	//(Day4)设置粒子系统的纹理
	_thrusters ->setMaterialName( "Examples/Flare" );
	_thrusters ->setDefaultDimensions( 25, 25 );


	//(Day4)设置点发射器1&2
	ParticleEmitter *emit1 = _thrusters ->addEmitter( "Point" );
	ParticleEmitter *emit2 = _thrusters ->addEmitter( "Point" );

	//(Day4)设置发射器1的参数
	emit1->setAngle( Degree(3) );
	emit1->setTimeToLive( 0.2f );
	emit1->setEmissionRate( 70 );

	emit1->setParticleVelocity( 50 );

	emit1->setDirection(- Vector3::UNIT_Y);
	emit1->setColour( ColourValue::White, ColourValue::Red);        

	//(Day4)设置发射器2的参数
	emit2->setAngle( Degree(3) );
	emit2->setTimeToLive( 0.2f );
	emit2->setEmissionRate( 70 );

	emit2->setParticleVelocity( 50 );

	emit2->setDirection( -Vector3::UNIT_Y );
	emit2->setColour( ColourValue::White, ColourValue::Red );

	//(Day4)设置两个发射器位置
	emit1->setPosition( Vector3( 5.7f, 0.0f, 0.0f ) );
	emit2->setPosition( Vector3( -18.0f, 0.0f, 0.0f ) );

	//(Day4)设置发射速度
	emit1->setParticleVelocity( 70 );
	emit2->setParticleVelocity( 70 );
	
	//(Day4)把发射器挂接到飞机节点上。
	_node->createChildSceneNode( Vector3( 13.0f, -75.0f, 0.0f ) )->attachObject(_thrusters);

	
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
	else if(evt.getKey() == KC_SPACE)//(Day4)如果释放空格键 发射子弹
		_bulletManager.fire(_node->getPosition(), Ogre::Vector3(0.0f, 10.0f, 0.0f));
	return true;
}

void BattlePlane::update(void)
{
	//(Day4)更新子弹
	_bulletManager.update();
	//(Day2)在每一frame中更新飞机位置
	if(_leftDown && _rightDown)
		return;
	if(_leftDown && _node->getPosition().x > -530.0f)
		_node->translate(-10,0,0);
	if(_rightDown&& _node->getPosition().x < 530.0f)
		_node->translate(10,0,0);
}