#ifndef __Orz_ControllerQt_QtLogManager_h__
#define __Orz_ControllerQt_QtLogManager_h__
#include <orz/Toolkit_Base/LogManager.h>
#include <QWidget>

#include <orz/Framework_Base/System/SystemInterface.h>
#include "LogWindow.h"
class QtLogManager:public Orz::ILogManager
{
	
public:
	static QtLogManager& getSingleton(void);
	static QtLogManager* getSingletonPtr(void);

	void unicode(const std::wstring & unicodet);

	void addWindow(LogWindow * window);
	void removeWindow(LogWindow * window);
private:
	std::vector<LogWindow *> _windows;
};

namespace Orz
{
	
SYSTEMUNIT_BEGIN(QtLogManager) 
SYSTEMUNIT_END(QtLogManager) 

}
#endif