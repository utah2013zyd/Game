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

//����������ڳ�ʼ������
void FCObject::doEnable(void)
{
}

//����֮ǰ����
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


//����֮ǰ����
void FCObject::doFrame(void)
{	

	
}
	

		

