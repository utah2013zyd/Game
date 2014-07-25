#include "Fighter.h"
#include "FCMap.h"

using namespace Orz;

FCFighter::FCFighter(const std::string & name, int x, int y, int id):FighterBase(name, x, y, id)
{}
FighterBase::ActionReturnPtr FCFighter::thinking(int power)
{
		
	Orz::FCMap* map = Orz::FCMap::getActiveMap();
	int y = getY();
	int x = getX();

	for(int x2=0; x2<16; ++x2)
	{
		if(map->getBlocker(x2,y))
		{
			if(x2<x)
					return getAction(Fire, West, power);
			else
					return getAction(Fire, East, power);
		}
	}
	int fighterX = map->getFighter(!_fighterID)->getX();
	int fighterY = map->getFighter(!_fighterID)->getY();
	if(x < fighterX)
	{
		return getAction(Move, East, power);
	}else if(x > fighterX)
	{
		return getAction(Move, West, power);
	}else
	{
		if(y> fighterY)
			return getAction(Fire, North, power);
		else
			return getAction(Fire, South, power);
	}
	return getAction(Sleep, West, power);
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
