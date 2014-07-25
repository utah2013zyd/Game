#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
		
	  static int xwestShoot = 0, xeastShoot = 0, yNorthShoot = 0, ySouthShoot = 0; // �ڸ��������Ѿ�������ڵ�����

        static int roundBlock[4];                // ��λ����Χ�ķ�����

        static bool bPosFirst = true;        // ��λ���Ƿ��һ�β鿴��Χ�������

        // bEastDir, bWestDirΪ��ʶλ, ��Ϊ���ʱ��, ��ʾ�Ѿ���ɨ������������ש��

        static bool bEastDir = false, bWestDir = false, bWEClear = false; 

        static int myMaxPower = 100, nCount = 0, theMaxPower = 250, lastPower;

        static bool bLastSleep = false;                        // ��ʾ��һ֡�Ƿ�����Ϣ

        int i;



        Orz::FCMap* map = Orz::FCMap::getActiveMap();



        // �ҵ�λ��

        int myPosX = getX();

        int myPosY = getY();



        // �õ����˵�λ��

        int enemyPosX = map->getFighter(!_fighterID)->getX();

        int enemyPosY = map->getFighter(!_fighterID)->getY();



        if( bLastSleep && lastPower == power)

        {

                theMaxPower = power;

        }



        bLastSleep = false;

        lastPower = power;



        // �����Ⱦ��������е�ש��, �����Լ������޻���ֵ

        // ÿ��λ�ü���ĸ������ש����, ����ש���������ڵ�����

        // ֻͨ���������������ƶ���ɨ����������ש��

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



        // �����ĸ������ש���������ڵ�

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



        // ���е�����, ��ʾ��λ�õ��ڵ��Ѿ��������, ���������ƶ����¸�λ�ü��������ڵ�

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



        // ���е�����, ��ʾ�Ѿ���ɨ������������ש��, ���ڿ�ʼ���ۻ���

        // ����ʵ��, �������ͬһ��ֱ�����ƿ�.

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



        // ��������һ��

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
