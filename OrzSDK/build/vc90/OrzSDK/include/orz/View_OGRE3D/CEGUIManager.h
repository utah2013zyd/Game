#ifndef __Orz_ViewOgre_CEGUIManager_h__
#define __Orz_ViewOgre_CEGUIManager_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#include <orz/Toolkit_Base/Singleton.h>

#include <orz/Framework_Base/System/SystemInterface.h>


namespace Orz{


class CEGUIManagerImpl;
template class _OrzViewOgreExport boost::scoped_ptr<CEGUIManagerImpl>;

class _OrzViewOgreExport CEGUIManager : public Singleton<CEGUIManager>
{

public:
	CEGUIManager(void);
	~CEGUIManager(void);
	
	bool init(void);
	void shutdown(void);
	//!!change funtion name enableMouse(bool enable)
	
	void mouseEvent(bool enable);
	//!!change funtion name enableKey(bool enable)
	void keyEvent(bool enable);

	CEGUI::Renderer * getRenderer(void);
	CEGUI::System * getSystem(void);
private:
	
	boost::scoped_ptr<CEGUIManagerImpl> _impl;
};


SYSTEMUNIT_BEGIN(CEGUIManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(CEGUIManager) 

}

//\
// \
//SYSTEMUNIT_FACTORY(Interface, CEGUIManager)

#endif