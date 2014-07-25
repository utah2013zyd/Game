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
	 ActionReturnPtr  Life_AI(const float& power,int& x,int& y,int& enemyX,int& enemyY );
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