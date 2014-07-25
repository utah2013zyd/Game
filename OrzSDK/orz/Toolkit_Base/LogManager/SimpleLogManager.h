#ifndef __Orz_ToolkitBase_SimpleLogManager_h__
#define __Orz_ToolkitBase_SimpleLogManager_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/LogManager/ILogManager.h>
namespace Orz
{
class _OrzToolkitBaseExport SimpleLogManager : public ILogManager
{
public:
	virtual void unicode(const std::wstring & text);
};

}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, LogManager)



#endif
