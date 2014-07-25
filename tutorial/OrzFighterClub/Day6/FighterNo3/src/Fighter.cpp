#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}


FighterBase::ActionReturnPtr FCFighter::Life_AI(const float& power,int& x,int& y,int& enemyX,int& enemyY )
{
	static int x_Imove=1;

	static int y_Imove=1;

	Orz::FCMap* map = Orz::FCMap::getActiveMap();//�õ���ͼ

	float dis=(x-enemyX)*(x-enemyX)+(y-enemyY)*(y-enemyY);

	/*std::cout<<"wo "<<x<<": "<<y<<": "<<"enemy "<<enemyX<<": "<<enemyY<<std::endl;

	std::cout<<"dis----"<<dis<<"  "<<x_Imove<<" "<<y_Imove<<std::endl;*/



	if (this->getThePower()<power)

	{   

		static bool rand_shu=true;

		std::cout<<rand_shu<<std::endl;

		if (x==enemyX)

		{//����ƶ�

			for(int xi=-1; xi<2; ++xi)//����X����

			{

				if((x+xi)>=0 && (x+xi)<16){

					if(map->getBlocker(x+xi,y))//������ϰ���

					{

						if((x+xi)<x)//������

							return getAction(Fire, West, 10.0);//��������

						else//���������

							return getAction(Fire, East, 10.0);//�򶫿���

					}

				}

			}

			//����һֱ������һֱ��

			if(rand_shu){

				if (x==15)

				{

					rand_shu=false;

					return getAction(Move,West,10.0);

				}

				return getAction(Move,East,10.0);

			}else{

				if (x==0){

					rand_shu=true;

					return getAction(Move,East,10.0);

				}

				return getAction(Move,West,10.0);

			}

		} 

		else if (y==enemyY)

		{

			for(int yi=-1; yi<2; ++yi)//����X����

			{

				if ((y+yi)>=0 && (y+yi)<16)

				{

					if(map->getBlocker(x,(y+yi)))//������ϰ���

					{

						if((y+yi)<y)//������

							return getAction(Fire, North, 10.0);//�򱱿���

						else//������

							return getAction(Fire, South, 10.0);//���Ͽ���

					}

				}

			}

			//һֱ���ϻ���

			if(rand_shu){

				if (y==15)

				{

					rand_shu=false;

					return getAction(Move,North,10.0);

				}

				return getAction(Move,South,10.0);

			}else{

				if (y==0)

				{

					rand_shu=true;

					return getAction(Move,South,10.0);

				}

				return getAction(Move,North,10.0);

			}

		} 

		else{

			//��Ϣ

			return getAction(Sleep, West, power);

		}

	} 

	else

	{//��Ѱ����

		if (x==enemyX || y==enemyY || (x_Imove%16)==0 || (y_Imove%16)==0)

		{

			if (x==enemyX || (x_Imove%16)==0)

			{//



				if (y>enemyY)

				{

					for(int y3=1; y3<(y-enemyY); ++y3)//����Y����

					{

						if(map->getBlocker(x,(y-y3)))//������ϰ���

						{

							return getAction(Fire, North, 10.0);//�򱱿���

						}

					}

					x_Imove+=14;

					return getAction(Fire, North, power);//�򱱿���

				} 

				else{

					for(int y4=1; y4<(enemyY-y); ++y4)//����Y����

					{

						if(map->getBlocker(x,(y+y4)))//������ϰ���

						{

							//if((y-y3)<y)//������

							return getAction(Fire, South, 10.0);//���Ͽ���                                }

						}

						x_Imove+=14;

						return getAction(Fire, South, power);//���Ͽ���

					}//end

				} 
			}
			else if(y==enemyY || (y_Imove%16)==0)

			{

				if (x>enemyX){

					for(int x3=1; x3<(x-enemyX); ++x3)//����Y����

					{

						if(map->getBlocker(x-x3,y))//������ϰ���

						{

							//if((y-y3)<y)//������

							return getAction(Fire, West, 10.0);//��������

						}

					}

					y_Imove+=14;

					return getAction(Fire, West, power);//��������

				} else{

					for(int x4=1; x4<(enemyX-x); ++x4)//����Y����

					{

						if(map->getBlocker(x+x4,y))//������ϰ���

						{

							return getAction(Fire, East, 10.0);//�򶫿���

						}

					}

					y_Imove+=14;

					return getAction(Fire, East,power);//�򶫿���

				}//end



			}

		} 

		else

		{//���ٵ���

			if (((x-enemyX)*(x-enemyX))>((y-enemyY)*(y-enemyY)))

			{

				if (y>enemyY)

				{

					if(y>0){

						if(map->getBlocker(x,(y-1)))//������ϰ���

						{

							return getAction(Fire, North, 10.0);//�򱱿���

						}

					}

					++y_Imove;

					return getAction(Move, North, 10.0);//���ƶ�

				} else

				{

					if(y<15){

						if(map->getBlocker(x,(y+1)))//������ϰ���

						{

							return getAction(Fire,South, 10.0);//���Ͽ���

						}

					}

					++y_Imove;

					return getAction(Move, South, 10.0);//�����ƶ�

				}



			} 

			else

			{

				if (x>enemyX)

				{

					if(x>0 ){

						if(map->getBlocker(x-1,y))//������ϰ���

						{

							return getAction(Fire, West, 10.0);//��������

						}

					}

					++x_Imove;

					return getAction(Move, West, 10.0);//�����ƶ�

				} else

				{

					if(x<15){

						if(map->getBlocker(x+1,y))//������ϰ���

						{

							return getAction(Fire, East, 10.0);//�򶫿���

						}

					}

					++x_Imove;

					return getAction(Move, East, 10.0);//���ƶ�

				}



			}

		}//

	}

	return getAction(Sleep, West, power);//˯����





}

FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
	//����power�ǵ�ǰ�����õ�����ֵ�������ȫ���ù⣬Ҳ������һ����



	Orz::FCMap* map = Orz::FCMap::getActiveMap();//�õ���ͼ

	int y = getY();//�õ���ǰX

	int x = getX();//�õ���ǰY

	int enemyX = map->getFighter(!_fighterID)->getX();//�õ����˵�X

	int enemyY = map->getFighter(!_fighterID)->getY();//�õ����˵�Y

	float dis=(x-enemyX)*(x-enemyX)+(y-enemyY)*(y-enemyY);

	//�ж�dis

	int case_dis;

	if(dis>121){

		case_dis=0;

	}else if (dis >64 && dis<=121)

	{

		case_dis=1;

	}else if(dis>25 && dis<=64){

		case_dis=2;

	}else if(dis>4 && dis<=25){

		case_dis=3;

	}else{

		case_dis=4;

	}

	//�����߼�

	switch(case_dis){

case 0:

	return Life_AI(100.0f,x,y,enemyX,enemyY);



case 1:

	return Life_AI(75,x,y,enemyX,enemyY);



case 2:

	return Life_AI(50,x,y,enemyX,enemyY);



case 3:

	return Life_AI(25.0f,x,y,enemyX,enemyY);



case 4:

	if (this->getThePower()<50)

	{

		return getAction(Sleep, West, power);//˯����

	}

	else{



		if (x==enemyX || y==enemyY)

		{

			if (x==enemyX)

			{//



				if (y>enemyY)

				{

					for(int y3=1; y3<(y-enemyY); ++y3)//����Y����

					{

						if(map->getBlocker(x,(y-y3)))//������ϰ���

						{

							return getAction(Fire, North, 10.0);//�򱱿���

						}

					}



					return getAction(Fire, North, power);//�򱱿���

				} 

				else

				{

					for(int y4=1; y4<(enemyY-y); ++y4)//����Y����

					{

						if(map->getBlocker(x,(y+y4)))//������ϰ���

						{

							return getAction(Fire, South, 10.0);//���Ͽ���

						}

					}

					return getAction(Fire, South, power);//���Ͽ���

				}//end



			} 

			else

			{

				if (x>enemyX)

				{

					for(int x3=1; x3<(x-enemyX); ++x3)//����Y����

					{

						if(map->getBlocker(x-x3,y))//������ϰ���

						{

							return getAction(Fire, West, 10.0);//��������

						}

					}

					return getAction(Fire, West, power);//��������

				} 

				else

				{

					for(int x4=1; x4<(enemyX-x); ++x4)//����Y����

					{

						if(map->getBlocker(x+x4,y))//������ϰ���

						{

							return getAction(Fire, East, 10.0);//�򶫿���

						}

					}



					return getAction(Fire, East,power);//�򶫿���

				}//end



			}

		} 

		else

		{//���ٵ���

			if (((x-enemyX)*(x-enemyX))>((y-enemyY)*(y-enemyY)))

			{

				if (y>enemyY)

				{

					if(map->getBlocker(x,(y-1)))//������ϰ���

					{

						return getAction(Fire, North, 10.0);//�򱱿���

					}

					return getAction(Move, North, 10.0);//���ƶ�

				} 

				else

				{

					if(map->getBlocker(x,(y+1)))//������ϰ���

					{

						return getAction(Fire,South, 10.0);//���Ͽ���

					}

					return getAction(Move, South, 10.0);//�����ƶ�

				}



			} 

			else

			{

				if (x>enemyX)

				{

					if(map->getBlocker(x-1,y))//������ϰ���

					{

						return getAction(Fire, West, 10.0);//��������

					}

					return getAction(Move, West, 10.0);//�����ƶ�

				} 

				else

				{

					if(map->getBlocker(x+1,y))//������ϰ���

					{

						return getAction(Fire, East, 10.0);//�򶫿���

					}

					return getAction(Move, East, 10.0);//���ƶ�

				}



			}

		}



	}

	}



	return getAction(Sleep, West, power);//˯����




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
