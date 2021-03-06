#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{

        //这里power是当前可以用的能量值。你可以全部用光，也可以用一部分



        Orz::FCMap* map = Orz::FCMap::getActiveMap();//得到地图

        int x = getX();

        int y = getY();



        int enemyX = map->getFighter(!_fighterID)->getX();//得到敌人的X

        int enemyY = map->getFighter(!_fighterID)->getY();//得到敌人的Y

    if(rand()%2)

                return getAction(Sleep, West, power);

        if(rand()%2)

        {

                if(x<enemyX)

                {

                        if(rand()%2)

                        {

                                //走

                                return getAction(Move,East,10.0);

                        }

                        else

                        {

                                //打

                                return getAction(Fire,East,power);

                        }



                }

                else if(x>enemyX)

                {

                        if(rand()%2)

                        {

                                //走

                                return getAction(Move,West,10.0);

                        }

                        else

                        {

                                //打

                                return getAction(Fire,West,power);

                        }

                }

        }

        else

        {

                if(y<enemyY)

                {

                        if(rand()%2)

                        {

                                //走

                                return getAction(Move,North,10.0);

                        }

                        else

                        {

                                //打

                                return getAction(Fire,South,power);

                        }

                }

                else if(y>enemyY)

                {

                        if(rand()%2)

                        {

                                //走

                                return getAction(Move,South,10.0);

                        }

                        else

                        {

                                //打

                                return getAction(Fire,North,power);

                        }

                }

        }



        if(x==enemyX)

        {

                if(y<enemyY)

                {

                        return getAction(Fire,South,power);



                }

                else

                {

                        return getAction(Fire,North,power);



                }

        }

        else

        {

                if(x<enemyX)

                {

                        return getAction(Fire,East,power);



                }

                else

                {

                        return getAction(Fire,West,power);

                }

        }

}




const std::string & FCFighterFactory::getTypeName() const
{
	static const std::string typeName(PROJECT_NAME);
	return typeName;
}


FCFighterFactory::pointer_type FCFighterFactory::createInstance(const std::string& instanceName, parameter_type parameter)
{
	int pos = 0;
	int x = 0;
	int y = 0;
	int id= 0;
	if(parameter)
	{
		parameter_iterator it = parameter->find("pos");
		if(it!= parameter->end())
		{
			pos = boost::get<int>(it->second);
			x = pos/16;
			y = pos%16;
		}

		
		it = parameter->find("id");
		if(it!= parameter->end())
		{
			id = boost::get<int>(it->second);
		}
	}


	return pointer_type(new FCFighter(instanceName, x, y, id));



}
void FCFighter::radar(ActionReturnPtr action)
{
	std::stringstream ss;

	
	if(action->get<3>()==this->_fighterID)
		ss<<"我";
	else
		ss<<"我的对手(id="<<action->get<3>()<<")，";
	
	if(action->get<0>() == Fire || action->get<0>() == Move)
	{
		ss<<"正在以("<<action->get<2>()<<")能量，向";
		
		switch(action->get<1>())
		{
		case East:
			ss<<"东，";
			break;
		case West:
			ss<<"西，";
			break;
		case North:
			ss<<"北，";
			break;
		case South:
			ss<<"南，";
			break;

		}
	}
	switch(action->get<0>())
	{
	case Fire:
		ss<<"开火。";
		break;
	case Move:
		ss<<"移动。";
		break;
	case Sleep:
		ss<<"休息。";
		break;

	case Die:
		ss<<"被摧毁。";
		break;

	case Hurt:
		ss<<"被击中。";
		break;

	}
	std::cout<<ss.str()<<std::endl;
	
}
