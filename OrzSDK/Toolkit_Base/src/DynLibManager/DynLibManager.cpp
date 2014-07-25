//#include <orz/Toolkit_Base/DynLibManager/DynLibManager.h>
//
//#include "DynLibManager/DynLibManagerImpl.h"
//using namespace Orz;
//
//template<> DynLibManager* Singleton<DynLibManager>::_singleton = NULL;
//
//DynLibManager::DynLibManager(void):_impl(new DynLibManagerImpl())
//{}
//
//DynLibManager::~DynLibManager()
//{
//
//}
//
//     
//DynLibPtr DynLibManager::load(const std::string & filename)
//{
//	return _impl->load(filename);
//}
//
//void DynLibManager::unload(DynLibPtr lib)
//{
//	_impl->unload(lib);
//}
//
//
//
//  
