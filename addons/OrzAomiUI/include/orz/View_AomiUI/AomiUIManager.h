#ifndef __Orz_ViewAomiUI_AomiUIManager_h__
#define __Orz_ViewAomiUI_AomiUIManager_h__

#include <orz/View_AomiUI/ViewAomiUIConfig.h>
#include <orz/View_AomiUI/AomiView.h>
#include <orz/View_AomiUI/OverlayView.h>
#include <orz/Toolkit_Base/Singleton.h>

#include <orz/Framework_Base/System.h>



namespace Orz
{


class AomiUIManagerImpl;
template class _OrzViewAomiUIExport boost::scoped_ptr<AomiUIManagerImpl>;

class _OrzViewAomiUIExport AomiUIManager : public Singleton<AomiUIManager>
{
public:
	AomiUIManager(void);
	~AomiUIManager(void);
	bool init(void);
	void shutdown(void);
	bool update(TimeType i);

	
	//AomiView * createAomiView(const std::string & name, int width, int height);
	OverlayView * createOverlayView(const std::string & name, int width, int height);
	
	void destroyView(const std::string & name);


private:
	boost::scoped_ptr<AomiUIManagerImpl> _impl;
};

SYSTEMUNIT_BEGIN(AomiUIManager) 
SYSTEMUNIT_INIT 
SYSTEMUNIT_UPDATE 
SYSTEMUNIT_SHUTDOWN 
SYSTEMUNIT_END(AomiUIManager)



}

#endif