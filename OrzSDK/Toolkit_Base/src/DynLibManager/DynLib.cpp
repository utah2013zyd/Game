#include <orz/Toolkit_Base/DynLibManager/DynLib.h>


#include "DynLibManager/DynLibImpl.h"
using namespace Orz;


  
DynLib::DynLib( const std::string & name ):_impl(new DynLibImpl(name))
{
	
}

DynLib::~DynLib(void)
{

}
//DynLib::DynLib(const DynLib & dl):_impl(new DynLibImpl(*(dl._impl)))
//{
//}
//DynLib& DynLib::operator = (const DynLib& dl)
//{
//	(*_impl) = *(dl._impl);
//	return (*this);
//}
//void DynLib::swap(DynLib& dl) throw()
//{
//	_impl.swap(dl._impl);
//}

void DynLib::load(void)
{
	_impl->load();
}
   
void DynLib::unload(void)
{
	_impl->unload();
}

const std::string & DynLib::getName(void) const
{
	return _impl->getName();
}

void* DynLib::getSymbol(const std::string & strName ) const throw()
{
	return _impl->getSymbol(strName);
}
