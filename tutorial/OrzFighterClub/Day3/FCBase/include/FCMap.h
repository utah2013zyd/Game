#ifndef __Orz_FCBase_FCMap_h__
#define __Orz_FCBase_FCMap_h__


#include "FCBaseConfig.h"
#include "FCGraph.h"
#include "FCMapInfo.h"
#include "FCPather.h"
namespace Orz
{
class _FCBaseExport FCGraph;
class _FCBaseExport FCPather;
class _FCBaseExport FCMap
{
public:
	
public:
	FCMap(int width, int height);
	virtual ~FCMap(void);
	FCPatherPtr createPather(void);
	void active(void);
	void inactive(void);
	bool isActive(void);

	void addBlocker(int x, int y, Orz::EventHandler * blocker = NULL);
	
	void removeBlocker(int x, int y, Orz::EventHandler * blocker = NULL);
	void setWay(int x, int y, FCPather::Type type = FCPather::WAY,float value = 1.0f);

	


private:
	FCMapInfo _info;
	boost::shared_ptr<FCGraph> _graph;
	int _width;
	int _height;
private:
	static FCMap * _activeMap;
public:
	static FCMap * getActiveMap();
	
};


}
#endif