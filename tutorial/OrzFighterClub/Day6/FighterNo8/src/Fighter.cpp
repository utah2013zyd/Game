#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
        Orz::FCMap* map = Orz::FCMap::getActiveMap();
        //记录是否fire
        static bool bFire = false;
        // 记录上次Enemy位置
        static int i_old_x = 0,i_old_y = 0;
        int uFireArea = 3;

        int enemyX = map->getFighter(!_fighterID)->getX();
        int enemyY = map->getFighter(!_fighterID)->getY();

        int y = getY();
        int x = getX();

        if(rand()%2 == 0 && !(abs(x - enemyX) > uFireArea && abs(y - enemyY) > uFireArea))
        {
                //保持能量和移动没有限制
                if (x != enemyX && y != enemyY)
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        if ( x+1 < 16 && map->getBlocker(x+1 ,y))
                                return getAction(Fire, East, 10.);
                        else if ( x-1 > 0 && map->getBlocker(x-1 ,y))
                                return getAction(Fire, West, 10.);
                        else if ( y+1 < 16 && map->getBlocker(x ,y+1))
                                return getAction(Fire, South, 10.);
                        else if ( y-1 > 0 && map->getBlocker(x ,y-1))
                                return getAction(Fire, North, 10.);
                        //增大能力
                        return increaseMaxPower(power,x,y,enemyX,enemyY);
                }
        }
        //转移阵地移动
        if (bFire)
        {
                bFire = false;
                if (x == enemyX || y == enemyY)
                {
                        return moveAI(power,bFire,x,y,enemyX,enemyY,i_old_x,i_old_y);
                }
                else
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        return getAction(Sleep, West, power);
                }
        }
        //控制在攻击范围之内
        if (abs(x - enemyX) > uFireArea || abs(y - enemyY) > uFireArea)
        {
                //同一直线避开
                if (x == enemyX || y == enemyY)
                {
                        return moveAI(10.f,bFire,x,y,enemyX,enemyY,i_old_x,i_old_y);
                }
                else
                {
                        return moveToEnemy(10.f,x,y,enemyX,enemyY,i_old_x,i_old_y);

                }
        }
        else if (abs(x - enemyX) <= uFireArea && abs(y - enemyY) <= uFireArea)
        {
                if (power < 50)
                {
                        i_old_x = enemyX ; i_old_y = enemyY;
                        return increaseMaxPower(power,x,y,enemyX,enemyY);
                }

                i_old_x = enemyX;i_old_y = enemyY;

                if (x == enemyX || y == enemyY)
                {
                        bFire = true;
                        return fireAI(power,x,y,enemyX,enemyY);
                }
                else
                {
                        return increaseMaxPower(power,x,y,enemyX,enemyY);
                }
        }

        return increaseMaxPower(power,x,y,enemyX,enemyY);
}
FighterBase::ActionReturnPtr FCFighter::increaseMaxPower(const float& power,int& x,int& y,int& enemyX,int& enemyY)
{
        Orz::FCMap* map = Orz::FCMap::getActiveMap();

        if ( getMaxPower() - power >= 25.f)
                return getAction(Sleep, West, power);
        else
        {
                //增加MaxPower
                for (int i = 1; i < 16 ; i ++)
                {
                        if ( x+i < 16 && map->getBlocker(x+i ,y))
                                return getAction(Fire, East, 10.);
                        else if ( x-i > 0 && map->getBlocker(x-i ,y))
                                return getAction(Fire, West, 10.);
                        else if ( y+i < 16 && map->getBlocker(x ,y+i))
                                return getAction(Fire, South, 10.);
                        else if ( y-i > 0 && map->getBlocker(x ,y-i))
                                return getAction(Fire, North, 10.);
                }
                //远离emeny
                switch (rand() % 2)
                {
                case 0:
                        {
                                if ( x < enemyX)
                                        return getAction(Move, West, 10.); 
                                else
                                        return getAction(Move, East, 10.); 
                        }
                        break;
                case 1:
                        {
                                if ( y < enemyY)
                                        return getAction(Move, South, 10.); 
                                else
                                        return getAction(Move, North, 10.); 
                        }
                        break;
                }
        }
        return getAction(Sleep, South, power);
}
FighterBase::ActionReturnPtr FCFighter::moveAI(const float& power,bool& bFire,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y)
{
        Orz::FCMap* map = Orz::FCMap::getActiveMap();
        bFire = true;
        if ( x+1 < 16 && map->getBlocker(x+1 ,y))
                return getAction(Fire, East, 10.);
        else if ( x-1 > 0 && map->getBlocker(x-1 ,y))
                return getAction(Fire, West, 10.);
        else if ( y+1 < 16 && map->getBlocker(x ,y+1))
                return getAction(Fire, South, 10.);
        else if ( y-1 > 0 && map->getBlocker(x ,y-1))
                return getAction(Fire, North, 10.);

        bFire = false;
        if (x == enemyX)
        {
                if (x + 1 == i_old_x)
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        return getAction(Move, West, power);
                }
                else if (x - 1 == i_old_x)
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        return getAction(Move, East, power);
                }
                i_old_x = enemyX;i_old_y = enemyY;
                switch (rand() % 2)
                {
                case 0:
                        return getAction(Move, East, power);
                        break;
                case 1:
                        return getAction(Move, West, power);
                        break;
                }
        }
        else if (y == enemyY)
        {
                if (y + 1 == i_old_y)
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        return getAction(Move, North, power);
                }
                else if (y - 1 == i_old_y)
                {
                        i_old_x = enemyX;i_old_y = enemyY;
                        return getAction(Move, South, power);
                }
                i_old_x = enemyX;i_old_y = enemyY;

                switch (rand() % 2)
                {
                case 0:
                        return getAction(Move, North, power);
                        break;
                case 1:
                        return getAction(Move, South, power);
                        break;
                }
        }

        return getAction(Sleep, South, power);
}
FighterBase::ActionReturnPtr FCFighter::fireAI(const float& power,int& x,int& y,int& enemyX,int& enemyY)
{
        if (x == enemyX)
        {
                if (y > enemyY)
                        return getAction(Fire, North, power); 
                else
                        return getAction(Fire, South, power); 
        }
        else if ( y == enemyY)
        {
                if (x > enemyX)
                        return getAction(Fire, West, power); 
                else
                        return getAction(Fire, East, power); 
        }
        return getAction(Sleep, South, power);
}
FighterBase::ActionReturnPtr FCFighter::moveToEnemy(const float& power,int& x,int& y,int& enemyX,int& enemyY,int& i_old_x,int& i_old_y)
{
        Orz::FCMap* map = Orz::FCMap::getActiveMap();

        if (x + 1 == i_old_x || x + 1 == enemyX || x - 1 == i_old_x || x - 1 == enemyX)
        {
                i_old_x = enemyX;i_old_y = enemyY;
                if (y > enemyY)
                        return getAction(Move, South, power);
                else
                        return getAction(Move, North, power);
        }
        else if (y + 1 == i_old_y || y + 1 == enemyY || y - 1 == i_old_y || y - 1 == enemyY)
        {
                i_old_x = enemyX;i_old_y = enemyY;
                if (x > enemyX)
                        return getAction(Move, East, power);
                else
                        return getAction(Move, West, power);
        }

        i_old_x = enemyX;i_old_y = enemyY;
        switch (rand() % 2)
        {
        case 0:
                {
                        if ( x > enemyX)
                        {
                                if ( x-1 > 0 && map->getBlocker(x-1 ,y))
                                        return getAction(Fire, West, 10.);
                                return getAction(Move, West, 10.); 
                        }
                        else
                        {
                                if ( x+1 < 16 && map->getBlocker(x+1 ,y))
                                        return getAction(Fire, East, 10.);
                                return getAction(Move, East, 10.); 
                        }
                }
                break;
        case 1:
                {
                        if ( y > enemyY)
                        { 
                                if ( y+1 < 16 && map->getBlocker(x ,y+1))
                                        return getAction(Fire, South, 10.);
                                return getAction(Move, South, 10.); 
                        }
                        else
                        { 
                                if ( y-1 > 0 && map->getBlocker(x ,y-1))
                                        return getAction(Fire, North, 10.);
                                return getAction(Move, North, 10.); 
                        }
                }
                break;
        }
        return getAction(Sleep, South, power);
}
FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}




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
