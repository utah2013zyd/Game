#include "FCPather.h"
#include "micropather.h"
using namespace Orz;

FCPather::FCPather(boost::shared_ptr<micropather::MicroPather> pather, int width, int height):
	_pather(pather),
	_path(),
	_totalCost(0.0f), 
	_width(width),
	_height(height),
	_where(0)
{
	_pos = FCPather::XY2pos(0, 0, width, height);
}
FCPather::~FCPather(void)
{
	
}
		
bool FCPather::solve(int x, int y)
{
	int result = 0;
	void * to = XY2pos( x, y, _width, _height);
	result = _pather->Solve( _pos, to, &_path, &_totalCost );
	if(result == micropather::MicroPather::SOLVED)
	{
		_pos = to;

		std::vector<void*>::iterator it;
		for(it = _path.begin(); it != _path.end(); ++it)
		{
			int x, y;
			FCPather::pos2XY(*it, &x, &y, _width, _height);
		}
		return true;
	}
	return false;
}
void FCPather::reset(int x, int y)
{
	_pather->Reset();
	_pos = FCPather::XY2pos(x, y, _width, _height);
	_where = 0;
}


std::pair<int, int> FCPather::getNext(void)
{
	std::pair<int, int> xy;
	
	FCPather::pos2XY(_path.at(_where), &xy.first, &xy.second, _width, _height);
	++_where;
	return xy;
}
bool FCPather::isEnd(void)
{
	return _where == _path.size();
}



void FCPather::pos2XY( void* pos, int* x, int* y, int width, int height) 
{
	int index = (int)pos;
	*y = index / width;
	*x = index - *y * width;
}

void* FCPather::XY2pos( int x, int y, int width, int height)
{
	return (void*) ( y*width + x );
}