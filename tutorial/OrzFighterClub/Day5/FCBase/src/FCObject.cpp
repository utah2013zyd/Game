#include "FCObject.h"
#include "FCMap.h"
#include "FCEvents.h"



using namespace Orz;
FCObject::FCObject(const std::string & name, int x, int y):Actor(name),_x(x), _y(y)//_start(false)
{
	
}
FCObject::~FCObject(void)
{
	

}

//这个函数会在初始化调用
void FCObject::doEnable(void)
{
}

//销毁之前调用
void FCObject::doDisable(void)
{
	//_otdES.reset();
}
	


int FCObject::getX(void)
{
	return _x;
}
int FCObject::getY(void)
{
	return _y;
}


//销毁之前调用
void FCObject::doFrame(void)
{	

	
}
	

		

