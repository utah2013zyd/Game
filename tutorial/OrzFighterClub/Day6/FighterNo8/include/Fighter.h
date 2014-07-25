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
	//�����������
	ActionReturnPtr increaseMaxPower(const float& power,int& x,int& y,int& enemyX,int& enemyY);
	//AI�ƶ�
	ActionReturnPtr moveAI(const float& power,bool& bFire,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y);
	//AI����
	ActionReturnPtr fireAI(const float& power,int& x,int& y,int& enemyX,int& enemyY);
	//����Enemy
	ActionReturnPtr moveToEnemy(const float& power,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y);
	
};



//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ��
class FCFighterFactory: public IFactory<Actor>
{
public:

	virtual const std::string & getTypeName() const;

	virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
};
}




#endif