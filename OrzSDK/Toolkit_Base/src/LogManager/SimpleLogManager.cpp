#include <orz/Toolkit_Base/LogManager/SimpleLogManager.h>
using namespace Orz;



void SimpleLogManager::unicode(const std::wstring & text)
{
	std::wcerr<<text<<std::endl;
}


