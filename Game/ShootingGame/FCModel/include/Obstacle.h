#pragma once
#include "FCConfig.h"

namespace Orz{
	class Obstacle : public Actor
	{
	public:
		Obstacle(const std::string & name = IDManager::BLANK, Ogre::Vector3 pos = Ogre::Vector3::ZERO, float scale = 1.0, const std::string & materialName = "");
		~Obstacle(void);
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);

		Ogre::Vector3 getPosition(){ return _pos; }
		float getScale(){ return _scale; }

	private:
		void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16);

	private:
		Ogre::Vector3 _pos;
		float _scale;
		std::string _materialName;

		Ogre::SceneNode* _node;
		Ogre::Entity* _entity;

	};

	class ObstacleFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};
}