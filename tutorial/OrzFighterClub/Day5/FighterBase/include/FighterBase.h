#ifndef __Orz_FighterBase_FighterBase_h__
#define __Orz_FighterBase_FighterBase_h__

#include "FighterBaseConfig.h"
#include "Bullets.h"

#include "FCMovable.h"
#include "FighterLogic.h"

namespace Orz
{
	// FSMLogic<FighterBase, FCFighterLogic> 表示FCFighterLogic是FighterBase的逻辑
class _FighterBaseExport FighterBase: public FCMovable, private FSMLogic<FighterBase, FCFighterLogic>
{
	public:
		enum ACTION
		{
			Fire,
			Move,
			Sleep,
			Hurt,
			Die
		};

		enum DIRECTOR
		{
			East,
			South ,
			West,
			North 
		};
		typedef boost::tuple<ACTION, DIRECTOR, float, int> ActionReturn;//action director power fighter_id
		typedef boost::shared_ptr<ActionReturn> ActionReturnPtr;
		ActionReturnPtr getAction(const ACTION action, const DIRECTOR director = East , float power = 10.f) const;

		FighterBase(const std::string & name = IDManager::BLANK, int x = 0, int y = 0, int id = 0);
		virtual ~FighterBase(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用
		void doExecute(Event * evt = NULL);
		Ogre::Vector3 getPosition(void) const;
		bool nextPosition();
		void setPosition(const Ogre::Vector3 & pos);
		
		virtual bool canMove(int x, int y);
		Ogre::Vector3 getTarget(void) const;
		void sendFightingEvent(FighterBase::ActionReturnPtr arp);
		Orz::EventHandler * getTargetBlocker(void) const;
		
		virtual ActionReturnPtr thinking(int power);
		void move(DIRECTOR dir, float speed, bool isSleep = false);
		void updateMove(void);
		void fire(DIRECTOR dir, float power);
		void updateFire(void);

		void addPower(float power);
		void setMaxPower(float power);
		float getMaxPower(void) const;
		float getThePower(void) const;
		
		virtual void radar(ActionReturnPtr action){}
	protected:

		
		void resetPosition(void);

		Ogre::Vector3 XY2Position(int x, int y);
		Ogre::SceneNode * _node;
		Ogre::Entity * _entity;
		
		std::pair<int, int> _toWhere;
		//Ogre::Vector3 _toPos;
		boost::shared_ptr<BulletManager> _bulletManager;
		bool _start;
		bool _run; 
		bool _fired;
		TimeType _fireTime;

		bool _moved;
		TimeType _moveTime;
		TimeType _moveAllTime;
		const int _fighterID;
		float _power;
		float _maxPower;
		float _health;
};

} 

#endif