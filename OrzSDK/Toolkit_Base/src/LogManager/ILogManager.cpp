#include <orz/Toolkit_Base/LogManager/ILogManager.h>
using namespace Orz;


template<> ILogManager* Singleton<ILogManager>::_singleton = NULL;

//void LogManager::unicode(const std::wstring & text)
//{
//	std::wcerr<<text<<std::endl;
//}
//

