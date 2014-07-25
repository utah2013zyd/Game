#include "FCGraph.h"

using namespace Orz;

FCGraph::FCGraph(int width, int height):_width(width), _height(height)
{
	_graph.resize(width * height);
}
FCGraph::~FCGraph(void)
{

}
float FCGraph::LeastCostEstimate( void* stateStart, void* stateEnd )
{
	int xStart, yStart, xEnd, yEnd;
	FCPather::pos2XY( stateStart, &xStart, &yStart, _width, _height);
	FCPather::pos2XY( stateEnd, &xEnd, &yEnd, _width, _height);

	/* Compute the minimum path cost using distance measurement. It is possible
	   to compute the exact minimum path using the fact that you can move only 
	   on a straight line or on a diagonal, and this will yield a better result.
	*/
	int dx = xStart - xEnd;
	int dy = yStart - yEnd;
	return (float) sqrt( (double)(dx*dx) + (double)(dy*dy) );
}
void FCGraph::AdjacentCost( void* pos, std::vector< micropather::StateCost > *neighbors )
{
	int x, y;
	const int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };
	const int dy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const float cost[8] = { 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f, 1.0f, 1.41f };

	FCPather::pos2XY( pos, &x, &y, _width, _height );

	for( int i=0; i<8; ++i ) {
		int nx = x + dx[i];
		int ny = y + dy[i];

		/*int pass = Passable( nx, ny );
		if ( pass > 0 ) {
			if ( pass == 1 || doorsOpen ) 
			{*/
				// Normal floor
			
		if (    nx >= 0 && nx < _width
			 && ny >= 0 && ny < _height )
		{
			if(cost[i] == 1.0f)
			{
				micropather::StateCost nodeCost = { FCPather::XY2pos( nx, ny , _width, _height), cost[i]* _graph.at(nx*_width + ny).getValue() };
				neighbors->push_back( nodeCost );
			}
		}
	}
}
void  FCGraph::PrintStateInfo( void* state )
{

}

void FCGraph::addBlocker(int x, int y, Orz::EventHandler * blocker)
{
	_graph.at(y*_width + x).addBlocker(blocker);
}


void FCGraph::removeBlocker(int x, int y, Orz::EventHandler * blocker)
{
	_graph.at(y*_width + x).removeBlocker(blocker);
}

void FCGraph::setWay(int x, int y, FCPather::Type type, float value)
{
	_graph.at(y*_width + x).setWay(type, value);
}


boost::shared_ptr<micropather::MicroPather>  FCGraph::createPather( void )
{
	return boost::shared_ptr<micropather::MicroPather>(new micropather::MicroPather(this, 20));
}

int FCGraph::getWidth(void) const 
{
	return _width;
}
int FCGraph::getHeight(void) const 
{
	return _height;
}
		