
#ifndef __Orz_TestLogManager_h__
#define __Orz_TestLogManager_h__
#include <orz/Toolkit_Base/LogManager/ILogManager.h>
namespace Orz
{
class _OrzToolkitBaseExport TestLogManager : public ILogManager
{
public:
	virtual void unicode(const std::wstring & text);
	const std::wstring & get(int i);

	size_t size(void);
	void clear(void);
private:
	std::vector<std::wstring> _logs;
};

}

#endif