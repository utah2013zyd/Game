#include "FCMap.h"
#include "FCObject.h"

using namespace Orz;

FCMap* FCMap::_activeMap = NULL;
FCMap* FCMap::getActiveMap(void)
{
	return _activeMap;
}
FCMap::FCMap(int width, int height):_graph(new FCGraph(width, height))
{
  _fighter[0] = NULL;//
  _fighter[1] = NULL;//

}
FCMap::~FCMap(void)
{
	inactive();
}

FCPatherPtr FCMap::createPather(void)
{
	return FCPatherPtr(new FCPather(_graph->createPather(), _graph->getWidth(), _graph->getHeight()));
}
bool FCMap::isActive(void)
{
	return this == _activeMap;
}

void FCMap::active(void)
{
	_activeMap = this;
}
void FCMap::inactive(void)
{
	if(_activeMap == this)
		_activeMap = NULL;
}


	
Orz::EventHandler * FCMap::getBlocker(int x, int y) const
{
	return 	_graph->getBlocker(x, y);
}

static int Rand(void)
{
	return rand();
}

bool FCMap::hasFighter(int id, int x, int y) const
{
	if(id == 0 || id ==1)
	{
		if(_fighter[id]->getX() == x && _fighter[id]->getY() == y )
			return true;
	}
	return false;//	_graph->getBlocker(x, y);
}
void FCMap::addBlocker(int x, int y, Orz::EventHandler * blocker)
{
	_graph->addBlocker(x, y, blocker);
	//_graph->setBlocker)
}
void FCMap::removeBlocker(int x, int y, Orz::EventHandler * blocker)
{
	_graph->removeBlocker(x, y, blocker);
}


void FCMap::setWay(int x, int y, FCPather::Type type, float value)
{
	_graph->setWay(x, y, type, value);
}


void FCMap::setFighter(int id, Orz::FCObject * fighter)
{
	if(id ==0 || id == 1)
		_fighter[id]= fighter;
}
Orz::FCObject * FCMap::getFighter(int id) const
{
	if(id == 0 || id == 1)
		return _fighter[id];
	return NULL;
}
