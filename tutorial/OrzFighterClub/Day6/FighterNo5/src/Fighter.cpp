#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FCFighter::ActionReturnPtr FCFighter::thinking(int power)
{
		
	//�õ���ǰ��ͼ
        Orz::FCMap *pMap = Orz::FCMap::getActiveMap();

        if (pMap == NULL)
        {
                return getAction(Sleep, West, power);
        }

        //��ȡ�Լ�����
        int x = getX();
        int y = getY();

        //��ȡ���ֵ�ǰx,y����
        int enemyX = pMap->getFighter(!_fighterID)->getX();
        int enemyY = pMap->getFighter(!_fighterID)->getY();

        //��¼�Ƿ���й�һ�ι�����
        static BOOL bIsFire = FALSE;
        //��¼�Ƿ����ڹ����ϰ���
        static BOOL bIsFireStock = FALSE;
        //��¼����ʱ����������
        static int iCount = 0;
        static BOOL bFirst = TRUE;

        //��¼���˴˻غ�֮ǰ������ռλ�á�
        static std::vector<int> xArr;
        static std::vector<int> yArr;

        //��¼��Ϣ�������Խ��������Ϣ���⡣
        static int iStop = 0;

        //��С��������
        int nMinPower = 90;
        int iFw = 3;//������Χ

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

        // //�������ͬ��˼�����������޵ȴ����⡣
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
                                iStop = 0;//����
                                bIsFire = FALSE;
                                return getAction(Move, East, power);//ȫ�����ܡ�
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
                                iStop = 0;//����
                                bIsFire = FALSE;
                                return getAction(Move, West, power);//ȫ�����ܡ�
                        }
                }
                else if(x < enemyX)
                {
                        return getAction(Move, East, 10.0);
                }
        }
        //end..

        //������ڣ�ֱ�ӹ�����
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
                //��������Ԫ��Ϊ���������
                xArr.erase(xArr.begin());
                yArr.erase(yArr.begin());
        }

		/************************************************************************/
        /* ����ս�ԣ��趨������Χ��������˲��ڹ�����Χ������֮���ƶ������λ�ã�����ڣ��ȴ��������������*/
        /************************************************************************/

        //���ڹ�����Χ��,����˿���
        if (abs(x - enemyX) > iFw || abs(y - enemyY) > iFw)
        {
                //�����ƶ����򡣽��빥����Χ�����
                int iBestX, iBestY;

                getBestXY(x, y, enemyX, enemyY, iBestX, iBestY, iFw);

                //�ƶ�ǰ��������и�Χ���ϰ��������ǽ��
                if (pMap->getBlocker(x,y))
                {
                        bIsFireStock = TRUE;
                        return getAction(Fire,West, 10.0f);
                }

                //�����ʱ���ߣ����ȴ���
                if (enemyX == x)
                {
                        //���ڵ��˵Ĺ������ϡ����ǲ�����ѹ����㡣�ƶ�

                        //�����жϵ����ϴ����ڵص㡣
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
                        //���ڵ��˵Ĺ������ϡ����ǲ�����ѹ����㡣�ƶ�

                        //�����жϵ����ϴ����ڵص㡣
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
                                //���λ�������档
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
                                //���ڱ�׼���λ�����ϡ�������ƶ�
                                if (iBestY > y)
                                {
                                        //������
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
                                        //������
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
                                //���λ��������
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
                                //���ڱ�׼���λ�����ϡ�������ƶ�
                                if (iBestY > y)
                                {
                                        //������
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
                                        //������
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
                //�ڵ��˹�����Χ֮�ڣ��Ż�����֮��

                //�ƶ�ǰ��������и�Χ���ϰ��������ǽ��
                if (pMap->getBlocker(x,y))
                {
                        bIsFireStock = TRUE;
                        iStop = 0;
                        return getAction(Fire,West, 10.0f);
                }

                if (x == enemyX)
                {
                        //ͬ��x����
                        if (!bIsFire)
                        {
                                if (y > enemyY)
                                {
                                        //������
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
                                        //������
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
                        //ͬ��y����
                        if (!bIsFire)
                        {
                                if (x > enemyX)
                                {
                                        //������
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
                                        //������
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
                                        return getAction(Move, North, power*3/5);//��������
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
                std::cout<<"������Χ�������!"<<"\n";

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
		ss<<"��";
	else
		ss<<"�ҵĶ���(id="<<action->get<3>()<<")��";
	
	if(action->get<0>() == Fire || action->get<0>() == Move)
	{
		ss<<"������("<<action->get<2>()<<")��������";
		
		switch(action->get<1>())
		{
		case East:
			ss<<"����";
			break;
		case West:
			ss<<"����";
			break;
		case North:
			ss<<"����";
			break;
		case South:
			ss<<"�ϣ�";
			break;

		}
	}
	switch(action->get<0>())
	{
	case Fire:
		ss<<"����";
		break;
	case Move:
		ss<<"�ƶ���";
		break;
	case Sleep:
		ss<<"��Ϣ��";
		break;

	case Die:
		ss<<"���ݻ١�";
		break;

	case Hurt:
		ss<<"�����С�";
		break;

	}
	std::cout<<ss.str()<<std::endl;
	
}
