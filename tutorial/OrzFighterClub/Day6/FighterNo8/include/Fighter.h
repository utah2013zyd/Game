#ifndef __Orz_FCFighter_FCFighter_h__
#define __Orz_FCFighter_FCFighter_h__
#include "FighterBase.h"
#include <string>
namespace Orz
{

class FCFighter: public FighterBase
{
public:
	FCFighter(const std::string & name = IDManager::BLANK, int x = 0, int y = 0, int id = 0);
	virtual ActionReturnPtr thinking(int power);
	void radar(ActionReturnPtr action);
	//增大最大能力
	ActionReturnPtr increaseMaxPower(const float& power,int& x,int& y,int& enemyX,int& enemyY);
	//AI移动
	ActionReturnPtr moveAI(const float& power,bool& bFire,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y);
	//AI开火
	ActionReturnPtr fireAI(const float& power,int& x,int& y,int& enemyX,int& enemyY);
	//靠近Enemy
	ActionReturnPtr moveToEnemy(const float& power,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y);
	
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