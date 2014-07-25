#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
		
	  static int xwestShoot = 0, xeastShoot = 0, yNorthShoot = 0, ySouthShoot = 0; // 在各个方向已经发射的炮弹数量

        static int roundBlock[4];                // 该位置周围的方块数

        static bool bPosFirst = true;        // 该位置是否第一次查看周围方块情况

        // bEastDir, bWestDir为标识位, 都为真的时候, 表示已经横扫了整个场景的砖块

        static bool bEastDir = false, bWestDir = false, bWEClear = false; 

        static int myMaxPower = 100, nCount = 0, theMaxPower = 250, lastPower;

        static bool bLastSleep = false;                        // 表示上一帧是否是休息

        int i;



        Orz::FCMap* map = Orz::FCMap::getActiveMap();



        // 我的位置

        int myPosX = getX();

        int myPosY = getY();



        // 得到敌人的位置

        int enemyPosX = map->getFighter(!_fighterID)->getX();

        int enemyPosY = map->getFighter(!_fighterID)->getY();



        if( bLastSleep && lastPower == power)

        {

                theMaxPower = power;

        }



        bLastSleep = false;

        lastPower = power;



        // 首先先尽快打掉所有的砖块, 增加自己的上限火力值

        // 每个位置检查四个方向的砖块数, 根据砖块数发送炮弹数量

        // 只通过向东西两个方向移动横扫整个场景的砖块

        if(!(bEastDir && bWestDir) && bPosFirst)

        {

                bPosFirst = false;

                if(map->getBlocker(myPosX,myPosY))

                        roundBlock[0]++;;

                if(!bWEClear)

                        for (i = myPosX + 1; i < 16; ++i)

                        {

                                if(map->getBlocker(i,myPosY))

                                        roundBlock[0]++;

                        }



                for (i = myPosY - 1; i >= 0; --i)

                {

                        if(map->getBlocker(myPosX, i))

                                roundBlock[1]++;

                }



                if(!bWEClear)

                        for (i = myPosX - 1; i >= 0; --i)

                        {

                                if(map->getBlocker(i,myPosY))

                                        roundBlock[2]++;

                        }



                for (i = myPosY + 1; i < 16; ++i)

                {

                        if(map->getBlocker(myPosX, i))

                                roundBlock[3]++;

                }

        }



        // 根据四个方向的砖块数发送炮弹

        if(!(bEastDir && bWestDir) && !bPosFirst)

        {

                if(roundBlock[0] > 0 && xeastShoot < roundBlock[0])

                {

                        ++xeastShoot;

                        return getAction(Fire, East, 10);

                }

                if(roundBlock[1] > 0 && yNorthShoot < roundBlock[1])

                {

                        ++yNorthShoot;

                        return getAction(Fire, North, 10);

                }

                if(roundBlock[2] > 0 && xwestShoot < roundBlock[2])

                {        

                        ++xwestShoot;

                        return getAction(Fire, West, 10);

                }

                if(roundBlock[3] > 0 && ySouthShoot < roundBlock[3])

                {

                        ++ySouthShoot;

                        return getAction(Fire, South, 10);

                }

                bWEClear = true;

                bPosFirst = true;

        }



        // 运行到这里, 表示该位置的炮弹已经发送完毕, 东西方向移动到下个位置继续发送炮弹

        if(!(bEastDir && bWestDir) && bPosFirst)

        {

                roundBlock[0] = roundBlock[1] = roundBlock[2] = roundBlock[3] = 0;

                xeastShoot = xwestShoot = yNorthShoot = ySouthShoot = 0;

                if(myPosX == 0)

                        bWestDir = true;

                if(!bWestDir && myPosX >= 0)

                        return getAction(Move, West, 10);

                else if(!bEastDir && myPosX < 15)

                        return getAction(Move, East, 10);

                bEastDir = true;

        }



        // 运行到这里, 表示已经横扫了整个场景的砖块, 现在开始积累火力

        // 积累实力, 如果敌我同一条直线则移开.

        if (bEastDir && bWestDir && power <= theMaxPower)

        {

                

                if(abs(myPosX - enemyPosX) >= 1 && abs(myPosY - enemyPosY) >= 1)

                {

                        bLastSleep = true;

                        return getAction(Sleep);

                }

                else if (myPosX == enemyPosX)

                {

                        if(myPosX < 15)

                                return getAction(Move, East, 10);

                        else

                                return getAction(Move, West, 10);

                }

                else if (myPosY == enemyPosY)

                {

                        if(myPosY < 15)

                                return getAction(Move, South, 10);

                        else

                                return getAction(Move, North, 10);

                }

        }



        // 发动致命一击

        if((bEastDir && bWestDir) && (myPosX == enemyPosX || myPosY == enemyPosY))

        {

                if(myPosX == enemyPosX)

                {

                        if(myPosY >= enemyPosY)

                                return getAction(Fire, North, power);

                        else

                                return getAction(Fire, South, power);

                }

                else if (myPosY == enemyPosY)

                {

                        if(myPosX >= enemyPosX)

                                return getAction(Fire, West, power);

                        else

                                return getAction(Fire, East, power);

                }

        }



        else if((bEastDir && bWestDir))

        {

                if (abs(myPosX - enemyPosX) > abs(myPosY - enemyPosY) )

                {

                        if (myPosY > enemyPosY)

                                return getAction(Move, South, 10);

                        else

                                return getAction(Move, North, 10);

                }

                else

                {

                        if (myPosX > enemyPosX)

                                return getAction(Move, West, 10);

                        else

                                return getAction(Move, East, 10);

                }

        }



        bLastSleep = true;

        return getAction(Sleep);


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
