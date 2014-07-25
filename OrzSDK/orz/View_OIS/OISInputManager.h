#ifndef __Orz_ViewOIS_InputManager_h__
#define __Orz_ViewOIS_InputManager_h__
#include <orz/View_OIS/ViewOISConfig.h>
#include <orz/Framework_Base/System.h>

namespace Orz
{
class OISInputManagerImpl;
template class _OrzViewOISExport boost::scoped_ptr<OISInputManagerImpl>;

class _OrzViewOISExport OISInputManager: public IInputManager
{
	friend class OISInputManagerImpl;
public:
	OISInputManager(void);
	bool init(void);		///初始化
	void shutdown(void);///关闭
	bool update(TimeType interval);

	virtual ~OISInputManager(void);

	static OISInputManager& getSingleton(void);
	static OISInputManager* getSingletonPtr(void);


private:
	
	boost::scoped_ptr<OISInputManagerImpl> _oisImpl;
	
};

SYSTEMUNIT_BEGIN(OISInputManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(OISInputManager)


}


//\
// \
//SYSTEMUNIT_FACTORY(Interface, InputManager)


#endif
