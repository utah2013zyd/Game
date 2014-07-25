#ifndef __Orz_FCBase_FCGraph_h__
#define __Orz_FCBase_FCGraph_h__


#include "FCBaseConfig.h"
#include "micropather.h"
#include "FCMapInfo.h"
#include "FCMapBlock.h"
namespace Orz
{
class _FCBasePrivate FCGraph: public micropather::Graph
{
public:
	FCGraph(int width, int height);
	virtual ~FCGraph(void);

	virtual float LeastCostEstimate( void* stateStart, void* stateEnd );
	virtual void AdjacentCost( void* state, std::vector< micropather::StateCost > *adjacent );
	virtual void  PrintStateInfo( void* state );


	void addBlocker(int x, int y, Orz::EventHandler * blocker = NULL);
	
	 Orz::EventHandler * getBlocker(int x, int y) const;
	void removeBlocker(int x, int y, Orz::EventHandler * blocker = NULL);
	void setWay(int x, int y, FCPather::Type type = FCPather::WAY, float value = 0.0f);


	boost::shared_ptr<micropather::MicroPather> createPather(void);
	
	int getWidth(void) const;
	int getHeight(void) const;
		
private:
	int _width;
	int _height;
	std::vector<FCMapBlock> _graph;
	//boost::shared_ptr<micropather::MicroPather> _pather;
};


}
#endif