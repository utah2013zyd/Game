#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}


FighterBase::ActionReturnPtr FCFighter::Life_AI(const float& power,int& x,int& y,int& enemyX,int& enemyY )
{
	static int x_Imove=1;

	static int y_Imove=1;

	Orz::FCMap* map = Orz::FCMap::getActiveMap();//得到地图

	float dis=(x-enemyX)*(x-enemyX)+(y-enemyY)*(y-enemyY);

	/*std::cout<<"wo "<<x<<": "<<y<<": "<<"enemy "<<enemyX<<": "<<enemyY<<std::endl;

	std::cout<<"dis----"<<dis<<"  "<<x_Imove<<" "<<y_Imove<<std::endl;*/



	if (this->getThePower()<power)

	{   

		static bool rand_shu=true;

		std::cout<<rand_shu<<std::endl;

		if (x==enemyX)

		{//随机移动

			for(int xi=-1; xi<2; ++xi)//遍历X方向

			{

				if((x+xi)>=0 && (x+xi)<16){

					if(map->getBlocker(x+xi,y))//如果有障碍物

					{

						if((x+xi)<x)//在左面

							return getAction(Fire, West, 10.0);//向西开火

						else//如果再右面

							return getAction(Fire, East, 10.0);//向东开火

					}

				}

			}

			//控制一直向西或一直向东

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

			for(int yi=-1; yi<2; ++yi)//遍历X方向

			{

				if ((y+yi)>=0 && (y+yi)<16)

				{

					if(map->getBlocker(x,(y+yi)))//如果有障碍物

					{

						if((y+yi)<y)//在上面

							return getAction(Fire, North, 10.0);//向北开火

						else//在下面

							return getAction(Fire, South, 10.0);//向南开火

					}

				}

			}

			//一直向南或向北

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

			//休息

			return getAction(Sleep, West, power);

		}

	} 

	else

	{//搜寻开火

		if (x==enemyX || y==enemyY || (x_Imove%16)==0 || (y_Imove%16)==0)

		{

			if (x==enemyX || (x_Imove%16)==0)

			{//



				if (y>enemyY)

				{

					for(int y3=1; y3<(y-enemyY); ++y3)//遍历Y方向

					{

						if(map->getBlocker(x,(y-y3)))//如果有障碍物

						{

							return getAction(Fire, North, 10.0);//向北开火

						}

					}

					x_Imove+=14;

					return getAction(Fire, North, power);//向北开火

				} 

				else{

					for(int y4=1; y4<(enemyY-y); ++y4)//遍历Y方向

					{

						if(map->getBlocker(x,(y+y4)))//如果有障碍物

						{

							//if((y-y3)<y)//在下面

							return getAction(Fire, South, 10.0);//向南开火                                }

						}

						x_Imove+=14;

						return getAction(Fire, South, power);//向南开火

					}//end

				} 
			}
			else if(y==enemyY || (y_Imove%16)==0)

			{

				if (x>enemyX){

					for(int x3=1; x3<(x-enemyX); ++x3)//遍历Y方向

					{

						if(map->getBlocker(x-x3,y))//如果有障碍物

						{

							//if((y-y3)<y)//在左面

							return getAction(Fire, West, 10.0);//向西开火

						}

					}

					y_Imove+=14;

					return getAction(Fire, West, power);//向西开火

				} else{

					for(int x4=1; x4<(enemyX-x); ++x4)//遍历Y方向

					{

						if(map->getBlocker(x+x4,y))//如果有障碍物

						{

							return getAction(Fire, East, 10.0);//向东开火

						}

					}

					y_Imove+=14;

					return getAction(Fire, East,power);//向东开火

				}//end



			}

		} 

		else

		{//跟踪敌人

			if (((x-enemyX)*(x-enemyX))>((y-enemyY)*(y-enemyY)))

			{

				if (y>enemyY)

				{

					if(y>0){

						if(map->getBlocker(x,(y-1)))//如果有障碍物

						{

							return getAction(Fire, North, 10.0);//向北开火

						}

					}

					++y_Imove;

					return getAction(Move, North, 10.0);//向北移动

				} else

				{

					if(y<15){

						if(map->getBlocker(x,(y+1)))//如果有障碍物

						{

							return getAction(Fire,South, 10.0);//向南开火

						}

					}

					++y_Imove;

					return getAction(Move, South, 10.0);//向南移动

				}



			} 

			else

			{

				if (x>enemyX)

				{

					if(x>0 ){

						if(map->getBlocker(x-1,y))//如果有障碍物

						{

							return getAction(Fire, West, 10.0);//向西开火

						}

					}

					++x_Imove;

					return getAction(Move, West, 10.0);//向西移动

				} else

				{

					if(x<15){

						if(map->getBlocker(x+1,y))//如果有障碍物

						{

							return getAction(Fire, East, 10.0);//向东开火

						}

					}

					++x_Imove;

					return getAction(Move, East, 10.0);//向东移动

				}



			}

		}//

	}

	return getAction(Sleep, West, power);//睡觉了





}

FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
	//这里power是当前可以用的能量值。你可以全部用光，也可以用一部分



	Orz::FCMap* map = Orz::FCMap::getActiveMap();//得到地图

	int y = getY();//得到当前X

	int x = getX();//得到当前Y

	int enemyX = map->getFighter(!_fighterID)->getX();//得到敌人的X

	int enemyY = map->getFighter(!_fighterID)->getY();//得到敌人的Y

	float dis=(x-enemyX)*(x-enemyX)+(y-enemyY)*(y-enemyY);

	//判断dis

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

	//处理逻辑

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

		return getAction(Sleep, West, power);//睡觉了

	}

	else{



		if (x==enemyX || y==enemyY)

		{

			if (x==enemyX)

			{//



				if (y>enemyY)

				{

					for(int y3=1; y3<(y-enemyY); ++y3)//遍历Y方向

					{

						if(map->getBlocker(x,(y-y3)))//如果有障碍物

						{

							return getAction(Fire, North, 10.0);//向北开火

						}

					}



					return getAction(Fire, North, power);//向北开火

				} 

				else

				{

					for(int y4=1; y4<(enemyY-y); ++y4)//遍历Y方向

					{

						if(map->getBlocker(x,(y+y4)))//如果有障碍物

						{

							return getAction(Fire, South, 10.0);//向南开火

						}

					}

					return getAction(Fire, South, power);//向南开火

				}//end



			} 

			else

			{

				if (x>enemyX)

				{

					for(int x3=1; x3<(x-enemyX); ++x3)//遍历Y方向

					{

						if(map->getBlocker(x-x3,y))//如果有障碍物

						{

							return getAction(Fire, West, 10.0);//向西开火

						}

					}

					return getAction(Fire, West, power);//向西开火

				} 

				else

				{

					for(int x4=1; x4<(enemyX-x); ++x4)//遍历Y方向

					{

						if(map->getBlocker(x+x4,y))//如果有障碍物

						{

							return getAction(Fire, East, 10.0);//向东开火

						}

					}



					return getAction(Fire, East,power);//向东开火

				}//end



			}

		} 

		else

		{//跟踪敌人

			if (((x-enemyX)*(x-enemyX))>((y-enemyY)*(y-enemyY)))

			{

				if (y>enemyY)

				{

					if(map->getBlocker(x,(y-1)))//如果有障碍物

					{

						return getAction(Fire, North, 10.0);//向北开火

					}

					return getAction(Move, North, 10.0);//向北移动

				} 

				else

				{

					if(map->getBlocker(x,(y+1)))//如果有障碍物

					{

						return getAction(Fire,South, 10.0);//向南开火

					}

					return getAction(Move, South, 10.0);//向南移动

				}



			} 

			else

			{

				if (x>enemyX)

				{

					if(map->getBlocker(x-1,y))//如果有障碍物

					{

						return getAction(Fire, West, 10.0);//向西开火

					}

					return getAction(Move, West, 10.0);//向西移动

				} 

				else

				{

					if(map->getBlocker(x+1,y))//如果有障碍物

					{

						return getAction(Fire, East, 10.0);//向东开火

					}

					return getAction(Move, East, 10.0);//向东移动

				}



			}

		}



	}

	}



	return getAction(Sleep, West, power);//睡觉了




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
