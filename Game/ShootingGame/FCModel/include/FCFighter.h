#ifndef __Orz_FC_FCFighter_h__
#define __Orz_FC_FCFighter_h__
#include "FCConfig.h"
#include "FCFighterLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{

	class FCFighter : public Actor, private FSMLogic<FCFighter, FCFighterLogic>
	{
	private:
		typedef std::vector<ActorPtr> BulletsList;
	public:
		FCFighter(const std::string & name = IDManager::BLANK, Ogre::Vector3 initPos = Ogre::Vector3(0.0, 0.0, 0.0), int queryFlag = 0x0, double speedLimit = 50, double rotateAngle = 30);
		virtual ~FCFighter(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用
		void doExecute(Event *evt);
		
		void roll(double z);
		void yaw(double y);
		void pitch(double x);
		void forward(void);
		void fire(void);
		void crush(void);
		void disappear();
		bool checkHit(void);
		Ogre::Matrix3 getAxis(){return _node->getLocalAxes(); }

		Ogre::Vector3 getPosition(){ return _node->getPosition(); }
		double getRotateAngle(){ return _rotateAngle; }
		Ogre::SceneNode* getNode(){ return _node; }
	private:
		Ogre::SceneNode * _node;
		Ogre::Entity * _entity;
		//OgreNewt::CollisionPtr _col;
		//OgreNewt::Body* _body;
		BulletsList _bullets;
		Ogre::Vector3 _initPos;
		double _speedLimit;
		double _power;
		double _rotateAngle;
		int _queryFlag;
		Ogre::Vector3 _rotateVec;
		double _lifePoint;
		Ogre::SphereSceneQuery * _query;

	
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应。
	class FCFighterFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 