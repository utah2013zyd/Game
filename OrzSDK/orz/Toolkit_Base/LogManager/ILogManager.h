#ifndef __Orz_ToolkitBase_ILogManager_h__
#define __Orz_ToolkitBase_ILogManager_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{
class _OrzToolkitBaseExport ILogManager : public Singleton<ILogManager>
{
public:
	inline void utf8(const std::string & text)
	{
		std::wstring w_text;
		utf8::utf8to16(text.begin(), text.end(), std::back_inserter(w_text));
		unicode(w_text);
	}
	virtual void unicode(const std::wstring & text) = 0;
};

}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, LogManager)



#endif
