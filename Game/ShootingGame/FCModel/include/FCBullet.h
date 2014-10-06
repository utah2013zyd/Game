#include "FCConfig.h"
#include "FCBulletLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{

	class FCBullet :public Actor, private FSMLogic<FCBullet, FCBulletLogic>
	{
	public:
		FCBullet(const std::string & name = IDManager::BLANK, const std::string & ownerName = IDManager::BLANK, int queryFlag = 0x0, double speed = 150.0, double range = 200);
		virtual ~FCBullet(void);

		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//在每一个frame调用
		void doExecute(Event *evt);

		//void fire(void);
		bool isActive(void);
		void fly(void);
		bool isOutReach(void);
		bool checkHit(void);
		void explode(void);
		void setInactive(void);
		double getRange(void) { return _range;}
		Ogre::Vector3 getPosition(void) { return _node->getPosition(); }
		Ogre::Quaternion getOrientation(void) { return _node->getOrientation(); }
	private:
		Ogre::SceneNode* _node;
		Ogre::Entity* _entity;
		Ogre::ParticleSystem* _thrusters;
		double _speed;
		Ogre::Vector3 _direction;
		double _distance;     //distance that already fly
		double _range;        //effective range
		std::string _ownerName;      //name of fighter
		bool _active;                //if bullet has been activated
		Ogre::SphereSceneQuery * _query;
		int _queryFlag;
		

	};

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应。
	class FCBulletFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};


}