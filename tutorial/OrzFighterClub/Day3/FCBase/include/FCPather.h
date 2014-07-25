#ifndef __Orz_FCBase_FCPather_h__
#define __Orz_FCBase_FCPather_h__


#include "FCBaseConfig.h"
#include "FCMapInfo.h"

namespace micropather
{
class MicroPather;
}

namespace Orz
{

class _FCBaseExport FCPather
{
public:

	enum Type
	{
		NO_WAY,
		WAY
	};
	FCPather(boost::shared_ptr<micropather::MicroPather> pather, int width, int height);
	virtual ~FCPather(void);
	bool solve(int x, int y);
	void reset(int x, int y);

	std::pair<int, int> getNext(void);
	bool isEnd(void);

	
	
	static void pos2XY( void* pos, int* x, int* y, int width, int height);
	static void* XY2pos( int x, int y, int width, int height);

private:
	std::vector<void*> _path;
	int _where;
	float _totalCost;
	void * _pos;
	boost::shared_ptr<micropather::MicroPather> _pather;
	int _width;
	int _height;
	
};

typedef boost::shared_ptr<FCPather> FCPatherPtr;

}
#endif