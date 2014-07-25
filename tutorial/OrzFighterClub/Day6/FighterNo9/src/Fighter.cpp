#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}

FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{

	//得到当前地图
	Orz::FCMap *pMap = Orz::FCMap::getActiveMap();

	if (pMap == NULL)
	{
		return getAction(Sleep, West, power);
	}

	//获取自己坐标
	int x = getX();
	int y = getY();

	//获取对手当前x,y坐标
	int enemyX = pMap->getFighter(!_fighterID)->getX();
	int enemyY = pMap->getFighter(!_fighterID)->getY();

	//记录是否进行过一次攻击。
	static BOOL bIsFire = FALSE;
	//用来标记是否到头。
	static int iFlagX = 0;
	static int iFlagY = 0;

	int ptMy[2];
	int ptEmemy[2];

	ptMy[0] = x;
	ptMy[1] = y;

	ptEmemy[0] = enemyX;
	ptEmemy[1] = enemyY;

	//最小攻击能量
	int nMinPower = 80;
	float fBlockPower = 25.0f;

	if (pMap->getBlocker(x, y))
	{
		return getAction(Fire, North, fBlockPower);
	}

	if (x != enemyX && y != enemyY)
	{
		if (abs(x - enemyX) > 2 || abs(y - enemyY) > 2)
		{
			//沿y轴远离敌人。
			switch(CrossPruct(x, y, enemyX, enemyY))
			{
			case 0://东北、西北
				{
					if (y == 15)
					{
						iFlagX = 1;
					}
					else if (y == 0)
					{
						iFlagX = 0;
					}

					if (iFlagX == 0)
					{
						return ScanAI(x, y, power, North);
					}
					else
					{
						return ScanAI(x, y, power, South);
					}
				}
				break;
			case 1://东南、西南
				{
					if (x == 15)
					{
						iFlagY = 1;
					}
					else if (x == 0)
					{
						iFlagY = 0;
					}

					if (iFlagY == 0)
					{
						return ScanAI(x, y, power, East);
					}
					else
					{
						return ScanAI(x, y, power, West);
					}
				}
				break;
			default:
				break;
			}
		}

		return getAction(Sleep, West, power);
	}
	else
	{
		if (x == enemyX && !bIsFire && power > nMinPower)
		{
			if (y > enemyY)
			{
				//在下面。
				bIsFire = TRUE;
				return getAction(Fire, North, power);
			}
			else
			{
				bIsFire = TRUE;
				return getAction(Fire, South, power);
			}
		}

		if (y == enemyY && !bIsFire && power > nMinPower)
		{
			if (x > enemyX)
			{
				//在右面。
				bIsFire = TRUE;
				return getAction(Fire, West, power);
			}
			else
			{
				bIsFire = TRUE;
				return getAction(Fire, East, power);
			}
		}

		//此处应根据敌人方向判断。
		DIRECTOR dirArr0[]= {West, East};
		DIRECTOR dirArr1[]= {South,North};
		switch(CrossPruct(x, y, enemyX, enemyY))
		{
		case 5://北
			{
				DIRECTOR dir;

				switch(rand()%2)
				{
				case 0:
					{
						dir = East;
					}
					break;
				case 1:
					{
						dir = West;
					}
					break;
				default:
					break;
				}

				int xTemp = 0;
				int xTempTemp = 0;

				if (dir == West)
				{
					xTemp = x -1>0 ? x-1:0;
					xTempTemp = x +1>15 ? 15:x+1;
				}
				else
				{
					xTemp = x +1>15 ? 15:x+1;
					xTempTemp = x -1>0 ? x-1:0;
				}

				if (pMap->getBlocker(xTemp, y))
				{
					return getAction(Fire, dir, power);
				}
				else if (x == 15)
				{
					DIRECTOR dirTemp;
					if (pMap->getBlocker(xTempTemp, y))
					{
						if (dir == dirArr0[0])
						{
							dirTemp = dirArr0[1];
						}
						else 
						{
							dirTemp = dirArr0[0];
						}

						return getAction(Fire, dirTemp, fBlockPower);
					}

					bIsFire = FALSE;
					return getAction(Move, dirTemp, power);
				}

				bIsFire = FALSE;
				return getAction(Move, dir, power);
			}
			break;
		case 4://西
			{
				DIRECTOR dir;

				switch(rand()%2)
				{
				case 0:
					{
						dir = North;
					}
					break;
				case 1:
					{
						dir = South;
					}
					break;
				default:
					break;
				}

				int yTemp = 0;
				int yTempTemp = 0;

				if (dir == North)
				{
					yTemp = y -1>0 ? y-1:0;
					yTempTemp = y +1>15 ? 15:y+1;
				}
				else
				{
					yTemp = y +1>15 ? 15:y+1;
					yTempTemp = y -1>0 ? y-1:0;
				}

				if (pMap->getBlocker(x, yTemp))
				{
					return getAction(Fire, dir, power);
				}
				else if (x == 15)
				{
					DIRECTOR dirTemp;
					if (pMap->getBlocker(x, yTempTemp))
					{
						if (dir == dirArr1[0])
						{
							dirTemp = dirArr1[1];
						}
						else 
						{
							dirTemp = dirArr1[0];
						}

						return getAction(Fire, dirTemp, fBlockPower);
					}

					bIsFire = FALSE;
					return getAction(Move, dirTemp, power);
				}

				bIsFire = FALSE;
				return getAction(Move, dir, power);
			}
			break;
		default:
			break;
		}
	}

	bIsFire = FALSE;
	return getAction(Sleep, West, power);
}
int FCFighter::CrossPruct(int x,int y, int enemyX, int enemyY)
{
	int xT = enemyX - x;
	int yT = enemyY - y;

	//北方向.
	double dResult = (xT * 0 + yT * (-1))/sqrtf(xT*xT + yT*yT);
	double dResult0 = (xT * (-1) + yT * 0)/sqrtf(xT*xT + yT*yT);

	if (dResult > 0 && dResult0 > 0)
	{
		return 0;//西北
	}
	else if (dResult > 0 && dResult0 < 0)
	{
		return 0;//东北
	}
	else if (dResult < 0 && dResult0 < 0)
	{
		return 1;//东南
	}
	else if(dResult < 0 && dResult0 > 0)
	{
		return 1;//西南
	}
	else if (dResult == 0)
	{
		return 4;//正北
	}
	else if (dResult0 == 0)
	{
		return 5;//正西
	}
	else if (dResult == -1)
	{
		return 6;//正南
	}
	else if (dResult0 == -1)
	{
		return 7;//正东
	}

	return -1;
}
FighterBase::ActionReturnPtr FCFighter::ScanAI(int x, int y, int &power,const DIRECTOR Rector)
{
	Orz::FCMap *pMap= Orz::FCMap::getActiveMap();

	float fBlockPower = 20.0f;

	//记录石块数目，以减少子弹浪费。
	//0:东,1:西,2:南,3:北
	static int BlockNum[4] = {0, 0, 0, 0};

	//标记石块是否打完。
	static BOOL bIsZero = TRUE;

	//战场没有石块
	BOOL bIsNull = TRUE;

	//根据记录石块数目发射响应炮弹。
	if(BlockNum[0] != 0)
	{
		BlockNum[0] --;
		bIsZero = FALSE;

		return getAction(Fire, East, fBlockPower);
	}

	if(BlockNum[1] != 0)
	{
		BlockNum[1] --;
		bIsZero = FALSE;

		return getAction(Fire, West, fBlockPower);
	}

	if(BlockNum[2] != 0)
	{
		BlockNum[2] --;
		bIsZero = FALSE;

		return getAction(Fire, South, fBlockPower);
	}

	if(BlockNum[3] != 0)
	{
		BlockNum[3] --;
		bIsZero = FALSE;

		return getAction(Fire, North, fBlockPower);
	}

	if (BlockNum[0] == 0 && BlockNum[1] == 0 && BlockNum[2] == 0 && BlockNum[3] == 0 && !bIsZero)
	{
		if (power < 100)
		{
			std::cout<<"赤战最大能量："<<getMaxPower()<<"\n";
			std::cout<<"赤战当前能量："<<power<<"\n";
			return getAction(Sleep, Rector, power); 
		}
		else
		{
			std::cout<<"赤战最大能量："<<getMaxPower()<<"\n";
			std::cout<<"赤战当前能量："<<power<<"\n";
			bIsZero = TRUE;
			return getAction(Move, Rector, 35.0f);
		}
	}

	int i = rand()%4;

	for (int x0 = 0; x0 < 16; x0 ++)
	{
		if (pMap->getBlocker(x0, y))
		{
			if (x0 > x)
			{
				BlockNum[0] ++;

				continue;
			}

			BlockNum[1] ++;

			continue;
		}
	}

	for (int y0 = 0; y0 < 16; y0 ++)
	{
		if (pMap->getBlocker(x, y0))
		{
			if (y0 > y)
			{
				BlockNum[2] ++;

				continue;
			}

			BlockNum[3] ++;

			continue;
		}
	}

	for (int i = 0; i < 16; i ++)
	{
		for (int j = 0; j < 16; j ++)
		{
			if (pMap->getBlocker(i, j))
			{
				bIsNull = FALSE;
			}
		}
	}

	if (BlockNum[0] == 0 && BlockNum[1] == 0 && BlockNum[2] == 0 && BlockNum[3] == 0)
	{
		bIsZero = FALSE;
	}

	if (bIsNull)
	{
		if (power < 200)
		{
			bIsZero = TRUE;
			return getAction(Sleep, Rector, power);
		}
	}

	return getAction(Sleep, Rector, power);

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
