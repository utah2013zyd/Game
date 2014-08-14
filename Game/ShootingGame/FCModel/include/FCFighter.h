#ifndef __Orz_FC_FCFighter_h__
#define __Orz_FC_FCFighter_h__
#include "FCConfig.h"
#include "FCFighterLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{

	class FCFighter : public Actor, protected FSMLogic<FCFighter, FCFighterLogic>
	{
	private:
		typedef std::vector<ActorPtr> BulletsList;
	public:
		FCFighter(const std::string & name = IDManager::BLANK, Ogre::Vector3 initPos = Ogre::Vector3(0.0, 0.0, 0.0), int queryFlag = 0x0, double speedLimit = 100, double rotateAngle = 70);
		virtual ~FCFighter(void);
		
		virtual void doEnable(void);
		virtual void doDisable(void);
		virtual void doFrame(void);							//called every frame
		virtual void doExecute(Event *evt);
		
		void roll(double z);
		void yaw(double y);
		void pitch(double x);
		void speedControl(double speed);
		void forward(void);
		void fire(void);
		void crush(void);
		void disappear();
		bool checkHit(void);
		Ogre::Matrix3 getAxis(){return _node->getLocalAxes(); }

		Ogre::Vector3 getPosition(){ return _node->getPosition(); }
		double getRotateAngle(){ return _rotateAngle; }
		Ogre::SceneNode* getNode(){ return _node; }
	protected:
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

		//fly
		Ogre::Quaternion _rotateQua;
		Ogre::Real _rotProgress;
		Ogre::Real _rotFactor;
		bool _rotating;
	};

}

#endif 