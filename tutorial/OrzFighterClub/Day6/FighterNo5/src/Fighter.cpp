#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FCFighter::ActionReturnPtr FCFighter::thinking(int power)
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
        //记录是否是在攻击障碍。
        static BOOL bIsFireStock = FALSE;
        //记录相邻时攻击次数。
        static int iCount = 0;
        static BOOL bFirst = TRUE;

        //记录敌人此回合之前三次所占位置。
        static std::vector<int> xArr;
        static std::vector<int> yArr;

        //记录休息次数，以解决无限休息问题。
        static int iStop = 0;

        //最小攻击能量
        int nMinPower = 90;
        int iFw = 3;//攻击范围

        if (iStop > 10 && iStop < 20)
        {
                int i = rand()%5;

                for (int x0 = 0; x0 < 16; x0 ++)
                {
                        if (pMap->getBlocker(x0, y))
                        {
                                if (x0 > x)
                                {
                                        switch (i)
                                        {
                                        case 1:
                                                return getAction(Fire, East, 10.0f);
                                        case 2:
                                                return getAction(Fire, East, 10.0f);
                                        }
                                }

                                switch (i)
                                {
                                case 1:
                                        return getAction(Fire, West, 10.0f);
                                case 2:
                                        return getAction(Fire, West, 10.0f);
                                }
                        }
                }

                for (int y0 = 0; y0 < 16; y0 ++)
                {
                        if (pMap->getBlocker(x, y0))
                        {
                                if (y0 > y)
                                {
                                        switch (i)
                                        {
                                        case 1:
                                                return getAction(Fire, South, 10.0f);
                                        case 2:
                                                return getAction(Fire, South, 10.0f);
                                        }
                                }

                                switch (i)
                                {
                                case 1:
                                        return getAction(Fire, North, 10.0f);
                                case 2:
                                        return getAction(Fire, North, 10.0f);
                                }
                        }
                }
        }

        // //解决由于同种思想而引起的无限等待问题。
        if (iStop > 30 && iStop <50)
        {
                if (x > enemyX && x -1 == enemyX)
                {
                        if (power < nMinPower)
                        {
                                return getAction(Sleep, West, power);
                        }
                        return getAction(Move, West, 10.0);
                }
                else if ( x == enemyX)
                {
                        if (!bIsFire  && power > nMinPower)
                        {
                                if (y > enemyY)
                                {
                                        bIsFire = TRUE;
                                        return getAction(Fire, North, power*4/5);
                                }
                                else
                                {
                                        bIsFire = TRUE;
                                        return getAction(Fire, South, power*4/5);
                                }
                        }
                        else
                        {
                                iStop = 0;//清零
                                bIsFire = FALSE;
                                return getAction(Move, East, power);//全力逃跑。
                        }

                }
                else if(x > enemyX)
                {
                        return getAction(Move, West, 10.0);
                }

                if (x < enemyX && x +1 == enemyX)
                {
                        if (power < nMinPower)
                        {
                                return getAction(Sleep, East, power);
                        }
                        return getAction(Move, East, 10.0);
                }
                else if ( x == enemyX)
                {
                        if (!bIsFire  && power > nMinPower)
                        {
                                if (y > enemyY)
                                {
                                        bIsFire = TRUE;
                                        return getAction(Fire, North, power*4/5);
                                }
                                else
                                {
                                        bIsFire = TRUE;
                                        return getAction(Fire, South, power*4/5);
                                }
                        }
                        else
                        {
                                iStop = 0;//清零
                                bIsFire = FALSE;
                                return getAction(Move, West, power);//全力逃跑。
                        }
                }
                else if(x < enemyX)
                {
                        return getAction(Move, East, 10.0);
                }
        }
        //end..

        //如果相邻，直接攻击次
        if(x + 1 == enemyX && y == enemyY)
        {
                if(iCount < 2)
                {
                        iCount ++;

                        return getAction(Fire, East,power*4/5);
                }
                iCount = 0;
        }
        else if(x - 1 == enemyX && y == enemyY)
        {
                if(iCount < 2)
                {
                        iCount ++;

                        return getAction(Fire, West,power*4/5);
                }
                iCount = 0;
        }
        else if(y - 1 == enemyY && x == enemyX)
        {
                if(iCount < 2)
                {
                        iCount ++;

                        return getAction(Fire, North,power*4/5);
                }
                iCount = 0;
        }
        else if(y + 1 == enemyY && x == enemyX)
        {
                if(iCount < 2)
                {
                        iCount ++;

                        return getAction(Fire, South,power*4/5);
                }
                iCount = 0;
        }

        if (enemyX < 0 || enemyX >15 || enemyY< 0 || enemyY >15)
        {}
        else
        {
                xArr.push_back(enemyX);
                yArr.push_back(enemyY);
        }

        if (xArr.size() > 3)
        {
                //保持数组元素为最近三个。
                xArr.erase(xArr.begin());
                yArr.erase(yArr.begin());
        }

		/************************************************************************/
        /* 基本战略：设定攻击范围，如果敌人不在攻击范围，靠近之，移动到最佳位置，如果在，等待蓄积能量攻击。*/
        /************************************************************************/

        //处在攻击范围外,向敌人靠近
        if (abs(x - enemyX) > iFw || abs(y - enemyY) > iFw)
        {
                //计算移动方向。进入攻击范围最近点
                int iBestX, iBestY;

                getBestXY(x, y, enemyX, enemyY, iBestX, iBestY, iFw);

                //移动前先清空所有格范围内障碍，免得碰墙。
                if (pMap->getBlocker(x,y))
                {
                        bIsFireStock = TRUE;
                        return getAction(Fire,West, 10.0f);
                }

                //如果此时共线，优先处理。
                if (enemyX == x)
                {
                        //处在敌人的攻击点上。但是不是最佳攻击点。移动

                        //首先判断敌人上次所在地点。
                        if (xArr.size() < 3)
                        {
                                switch(rand()%2)
                                {
                                case 0:
                                        return getAction(Move, East, power*2/5);
                                case 1:
                                        return getAction(Move, West, power*2/5);
                                }
                        }

                        if (x - 1 == xArr[0])
                        {
                                if (pMap->getBlocker(x + 1 > 15 ? 15:x+1, y))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, East, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, East, power*3/5);
                        }
                        else
                        {
                                if (pMap->getBlocker(x - 1 < 0 ? 0:x-1, y))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, West, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, West, power*3/5);
                        }
                }

                if (enemyY == y)
                {
                        //处在敌人的攻击点上。但是不是最佳攻击点。移动

                        //首先判断敌人上次所在地点。
                        if (yArr.size() < 3)
                        {
                                switch(rand()%2)
                                {
                                case 0:
                                        return getAction(Move, South, power*2/5);
                                case 1:
                                        return getAction(Move, North, power*2/5);
                                }
                        }

                        if (y - 1 == yArr[0])
                        {
                                if (pMap->getBlocker(x , y + 1 > 15 ? 15:y+1))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, South, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, South, power*3/5);
                        }
                        else
                        {
                                if (pMap->getBlocker(x , y - 1>0 ? y-1:0))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, North, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, North, power*3/5);
                        }
                }

                if (iBestX > x)
                {
                        if ( iBestX - 1 > x)
                        {
                                //最佳位置在左面。
                                if (pMap->getBlocker(x + 1 > 15 ? 15:x+1, y))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, East, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, East, 10.0f);
                        }
                        else if (iBestX - 1 == x )
                        {
                                //处在标准射击位置线上。向敌人移动
                                if (iBestY > y)
                                {
                                        //在上面
                                        if (pMap->getBlocker(x, y + 1 > 15 ? 15:y+1))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, South, 10.0f);
                                        }

                                        iStop = 0;
                                        return getAction(Move, North, 10.0f);
                                }
                                else if (iBestY < y)
                                {
                                        //在下面
                                        if (pMap->getBlocker(x, y-1<0 ? 0:y-1))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, North, 10.0f);
                                        }

                                        iStop = 0;
                                        return getAction(Move, South, 10.0f);
                                }
                        }
                }
                else
                {
                        if (iBestX+1 < x)
                        {
                                //最佳位置在右面
                                if (pMap->getBlocker(x - 1 < 0 ? 0:x-1, y))
                                {
                                        bIsFireStock = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, West, 10.0f);
                                }

                                iStop = 0;
                                return getAction(Move, West, 10.0f);
                        }
                        else if (iBestX + 1 == x)
                        {
                                //处在标准射击位置线上。向敌人移动
                                if (iBestY > y)
                                {
                                        //在上面
                                        if (pMap->getBlocker(x, y + 1 > 15 ? 15:y+1))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, South, 10.0f);
                                        }

                                        iStop = 0;
                                        return getAction(Move, North, 10.0f);
                                }
                                else if (iBestY < y)
                                {
                                        //在下面
                                        if (pMap->getBlocker(x, y-1<0 ? 0:y-1))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, North, 10.0f);
                                        }

                                        iStop = 0;
                                        return getAction(Move, South, 10.0f);
                                }
                        }
                }
        }
        else
        {
                //在敌人攻击范围之内，伺机攻击之。

                //移动前先清空所有格范围内障碍，免得碰墙。
                if (pMap->getBlocker(x,y))
                {
                        bIsFireStock = TRUE;
                        iStop = 0;
                        return getAction(Fire,West, 10.0f);
                }

                if (x == enemyX)
                {
                        //同在x轴上
                        if (!bIsFire)
                        {
                                if (y > enemyY)
                                {
                                        //在下面
                                        if (power < nMinPower)
                                        {
                                                if (x - 1 == xArr[0])
                                                {
                                                        if (x +2 > 15)
                                                        {
                                                                iStop = 0;
                                                                return getAction(Move, West, power*3/5);
                                                        }

                                                        if (pMap->getBlocker(x + 1 > 15 ? 15:x+1, y))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, East, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, East, power*2/5);
                                                }
                                                else
                                                {
                                                        if (x -2 < 0)
                                                        {
                                                                bIsFire = FALSE;
                                                                iStop = 0;
                                                                return getAction(Move, East, power*3/5);
                                                        }

                                                        if (pMap->getBlocker(x - 1 < 0 ? 0:x -1, y))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, West, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, West, power*2/5);
                                                }
                                        }

                                        bIsFire = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, North, power*4/5);
                                }
                                else
                                {
                                        //在上面
                                        if (power < nMinPower)
                                        {
                                                if (x - 1 == xArr[0])
                                                {
                                                        if (x +2 > 15)
                                                        {
                                                                bIsFire = FALSE;
                                                                iStop = 0;
                                                                return getAction(Move, West, power*3/5);
                                                        }

                                                        if (pMap->getBlocker(x + 1 > 15 ? 15:x+1, y))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, East, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, East, power*2/5);
                                                }
                                                else
                                                {
                                                        if (x -2 < 0)
                                                        {
                                                                bIsFire = FALSE;
                                                                iStop = 0;
                                                                return getAction(Move, East, power*3/5);
                                                        }

                                                        if (pMap->getBlocker(x - 1 < 0 ? 0 : x-1, y))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, West, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, West, power*2/5);
                                                }
                                        }

                                        bIsFire = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, South, power*4/5);
                                }
                        }

                        if (bIsFire)
                        {
                                if (x - 1 == xArr[0])
                                {
                                        if (x +2 > 15)
                                        {
                                                bIsFire = FALSE;
                                                iStop = 0;
                                                return getAction(Move, West, power*3/5);
                                        }

                                        if (pMap->getBlocker(x + 1 > 15 ? 15:x+1, y))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, East, power*2/5);
                                        }

                                        bIsFire = FALSE;
                                        iStop = 0;
                                        return getAction(Move, East, power*3/5);
                                }
                                else
                                {
                                        if (x -2 < 0)
                                        {
                                                bIsFire = FALSE;
                                                iStop = 0;
                                                return getAction(Move, East, power*3/5);
                                        }

                                        if (pMap->getBlocker(x - 1 < 0 ? 0 : x-1, y))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, West, power*2/5);
                                        }

                                        bIsFire = FALSE;
                                        iStop = 0;
                                        return getAction(Move, West, power*3/5);
                                }
                        }

                }

                if (y == enemyY)
                {
                        //同在y轴上
                        if (!bIsFire)
                        {
                                if (x > enemyX)
                                {
                                        //在右面
                                        if (power < nMinPower)
                                        {
                                                if (y - 1 == yArr[0])
                                                {
                                                        if (pMap->getBlocker(x, y + 1 > 15 ? 15:y+1))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, South, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, South, power*3/5);
                                                }
                                                else
                                                {
                                                        if (pMap->getBlocker(x, y-1< 0 ? 0:y-1))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, North, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, North, power*3/5);
                                                }
                                        }

                                        bIsFire = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, West, power*4/5);
                                }
                                else
                                {
                                        //在左面
                                        if (power < nMinPower)
                                        {
                                                if (y - 1 == yArr[0])
                                                {
                                                        if (pMap->getBlocker(x , y + 1 > 15 ? 15:y+1))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, South, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, South, power*3/5);
                                                }
                                                else
                                                {
                                                        if (pMap->getBlocker(x , y-1<0 ? 0:y-1))
                                                        {
                                                                bIsFireStock = TRUE;
                                                                iStop = 0;
                                                                return getAction(Fire, North, power*2/5);
                                                        }

                                                        iStop = 0;
                                                        return getAction(Move, North, power*3/5);
                                                }
                                        }

                                        bIsFire = TRUE;
                                        iStop = 0;
                                        return getAction(Fire, East, power*4/5);
                                }
                        }

                        if (bIsFire)
                        {
                                if (y - 1 == yArr[0])
                                {
                                        if (pMap->getBlocker(x , y + 1 > 15 ? 15:y+1))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, South, power*2/5);
                                        }

                                        bIsFire = FALSE;
                                        iStop = 0;
                                        return getAction(Move, South, power*3/5);
                                }
                                else
                                {
                                        if (pMap->getBlocker(x - 1 < 0 ? 0 : x -1, y))
                                        {
                                                bIsFireStock = TRUE;
                                                iStop = 0;
                                                return getAction(Fire, North, power*2/5);
                                        }

                                        bIsFire = FALSE;
                                        iStop = 0;
                                        return getAction(Move, North, power*3/5);//尽力逃跑
                                }
                        }

                }

                iStop ++;

                return getAction(Sleep, West, power);
        }

        return getAction(Sleep, West, power);

		
}


void FCFighter::getBestXY(int x, int y, int enemyX, int enemyY, int &iBestX, int &iBestY, int iFW)
{
        std::vector<int> xArr, yArr;

        //1
        int iTempX = enemyX + iFW;

        if (iTempX >= 0 && iTempX < 16)
        {
                xArr.push_back(iTempX);
        }
        else
        {
                xArr.push_back(15);
        }

        xArr.push_back(enemyX);

        yArr.push_back(enemyY);

        int iTempY = enemyY + iFW;

        if (iTempY >= 0 && iTempY <16)
        {
                yArr.push_back(iTempY);
        }
        else
        {
                yArr.push_back(15);
        }

        //2
        iTempX = enemyX - iFW;

        if (iTempX >= 0 && iTempX <16)
        {
                xArr.push_back(iTempX);
        }
        else
        {
                xArr.push_back(0);
        }

        xArr.push_back(enemyX);

        yArr.push_back(enemyY);

        iTempY = enemyY - iFW;

        if (iTempY >= 0 && iTempY <16)
        {
                yArr.push_back(iTempY);
        }
        else
        {
                yArr.push_back(0);
        }

        if (xArr.size() != yArr.size() || xArr.size() < 4)
        {
                std::cout<<"攻击范围计算错误!"<<"\n";

                return;
        }

        std::map<double, int> dMap;
        double dDis = 0;
        dMap.clear();

        for (int i = 0 ; i < xArr.size(); i ++)
        {
                dDis = sqrtf((yArr[i] - y)*(yArr[i] - y)+(xArr[i] - x)*(xArr[i] - x));

                dMap[dDis] = i;
        }

        if (dMap.size() < 0)
        {
                return;
        }

        std::map<double, int>::iterator iter = dMap.begin();
        int iPos = iter->second;

        iBestX = xArr[iPos];
        iBestY = yArr[iPos];
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
