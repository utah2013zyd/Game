#ifndef __Orz_ViewOgreNewt_OgreNewtManager_h__
#define __Orz_ViewOgreNewt_OgreNewtManager_h__

#include <orz/View_OgreNewt/ViewOgreNewtConfig.h>
#include <orz/Toolkit_Base/Singleton.h>

#include <orz/Framework_Base/System/SystemInterface.h>
namespace OgreNewt
{
class World;
}


namespace Orz
{

typedef boost::scoped_ptr<OgreNewt::World> OgreNewtWorldPtr;
class _OrzViewOgreNewtonExport OgreNewtManager : public Singleton<OgreNewtManager>
{
public:
	OgreNewtManager(void);
	~OgreNewtManager(void);
	bool init(void);
	void shutdown(void);
	bool update(TimeType i);
	OgreNewt::World * getNewtWorld(void);
private:
	OgreNewtWorldPtr _world;
	float _time;
	float _speed;

};

SYSTEMUNIT_BEGIN(OgreNewtManager)
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(OgreNewtManager)
}




#endif