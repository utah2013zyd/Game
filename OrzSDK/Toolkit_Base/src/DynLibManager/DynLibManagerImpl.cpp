//
//#include "DynLibManager/DynLibManagerImpl.h"
//
//#include "DynLibManager/DynLibImpl.h"
//using namespace Orz;
//
////-----------------------------------------------------------------------
//DynLibManagerImpl::DynLibManagerImpl()
//{
//}
////-----------------------------------------------------------------------
//DynLibPtr DynLibManagerImpl::load( const std::string & filename)
//{
//	DynLibList::iterator i = _libList.find(filename);
//	if (i != _libList.end())
//	{
//		return i->second;
//	}
//	else
//	{
//		DynLibPtr pLib = DynLibPtr(new DynLib(filename));
//		pLib->load();
//    	_libList[filename] = pLib;
//        return pLib;
//	}
//}
////-----------------------------------------------------------------------
//void DynLibManagerImpl::unload(DynLibPtr lib)
//{
//
//	DynLibList::iterator i = _libList.find(lib->getName());
//	if (i != _libList.end())
//	{
//		_libList.erase(i);
//	}
//	lib->unload();
//	lib.reset();
//
//}
////-----------------------------------------------------------------------
//DynLibManagerImpl::~DynLibManagerImpl()
//{
//    // Unload & delete resources in turn
//    for( DynLibList::iterator it = _libList.begin(); it != _libList.end(); ++it )
//    {
//        it->second->unload();
//       
//    }
//
//    // Empty the list
//    _libList.clear();
//}
