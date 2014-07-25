#include "FCMap.h"

using namespace Orz;

FCMap* FCMap::_activeMap = NULL;
FCMap* FCMap::getActiveMap(void)
{
	return _activeMap;
}
FCMap::FCMap(int width, int height):_graph(new FCGraph(width, height))
{
  

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
