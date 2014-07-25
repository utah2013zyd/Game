#include "FCMapInfo.h"
using namespace Orz;


FCMapInfo::FCMapInfo(void)
:_enable(false),_ground(0.0f)
{

}
FCMapInfo::~FCMapInfo(void)
{

}

float FCMapInfo::getGround(void) const
{
	return _ground;
}

void FCMapInfo::setGround(float ground)
{
	_ground = ground;
}
bool FCMapInfo::isEnable(void) const
{
	return _enable;// = true;
}
void FCMapInfo::enable(void)
{
	_enable = true;
}
void FCMapInfo::disable(void)
{
	_enable = false;
}