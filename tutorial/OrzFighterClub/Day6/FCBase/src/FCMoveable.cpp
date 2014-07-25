#include "FCMovable.h"
#include "FCMap.h"



using namespace Orz;
FCMovable::FCMovable(const std::string & name, int x, int y):FCObject(name, x, y)//,_pos(pos),_start(false)
{
	
}
FCMovable::~FCMovable(void)
{
	

}

bool FCMovable::canMove(int x, int y)
{
	if(x < 0 || x> 15 || y < 0 || y >15)
		return false;
	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		if(map->getBlocker(x, y ))
			return false;
	/*	if(map->hasFighter(!_id, x, y))
			return true;*/
	}
	return true;
}
bool FCMovable::patherSolve(int x, int y)
{
	FCMap * map = FCMap::getActiveMap();
	if(map)
	{
		_pather = map->createPather();
		_pather->reset(_x, _y);
		_pather->solve(x, y);
		_pather->getNext();
	}
	return true;
}
//这个函数会在初始化调用
void FCMovable::doEnable(void)
{

	FCObject::doEnable();
	
}

//销毁之前调用
void FCMovable::doDisable(void)
{
	FCObject::doDisable();
	
}
	




//销毁之前调用
void FCMovable::doFrame(void)
{	

	
	FCObject::doFrame();
	
	
}
	

		

