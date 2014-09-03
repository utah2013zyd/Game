#include "FCConfig.h"
#include "FCFighter.h"
#include "FCFighterLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>

#pragma once

namespace Orz
{
	class PlayerPlane: public FCFighter, public Ogre::CompositorInstance::Listener
	{
	public:
		PlayerPlane(const std::string & name = IDManager::BLANK, Ogre::Vector3 initPos = Ogre::Vector3(0.0, 0.0, 0.0), int queryFlag = 0x0, double speedLimit = 200);
		virtual ~PlayerPlane(void);

		//override from FCFighter
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);							//called every frame
		void doExecute(Event *evt);
		void forward(void) override;

		//override from Ogre::CompositorInstance::Listener
		void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
		void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
	private:
		Ogre::ParticleSystem* _star;
		Ogre::SceneNode* _camNode;

		//rotation
		const Ogre::Degree _rotateAcceleration;
		const Ogre::Degree _rotateDeacceleration;
		Ogre::Quaternion _rotateSpeed;
		Ogre::Quaternion _rotateLastSpeed;
		Ogre::Real _rotProgress;
		Ogre::Real _rotFactor;
		bool _rotating;
		bool _underAttack;
		float _attackTimeElapsed;
		Ogre::CompositorInstance* _redBlur;
		Ogre::GpuProgramParametersSharedPtr fpParams;

	};

	class PlayerPlaneFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};
}