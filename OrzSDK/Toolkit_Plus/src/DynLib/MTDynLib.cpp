#include <orz/Toolkit_Plus/DynLib/MTDynLib.h>
#include <DynLib/MTDynLibImpl.h>
using namespace Orz;
MTDynLib::MTDynLib(const std::string& _name, MTDynLibId _dlid) 
	: m_impl(new MTDynLibImpl(_name, _dlid))
{
}

MTDynLib::MTDynLib(const MTDynLib& _dl) 
	: m_impl(new MTDynLibImpl(*(_dl.m_impl)))
{
}

MTDynLib& MTDynLib::operator=(const MTDynLib& _dl)
{
	(*m_impl) = *(_dl.m_impl);
	return (*this);
}

MTDynLib::~MTDynLib(void)
{
}

void MTDynLib::swap(MTDynLib& _dl) throw()
{
	m_impl.swap(_dl.m_impl);
}

bool MTDynLib::load(void)
{
	return m_impl->load();
}
   
bool MTDynLib::unload(void)
{
	return m_impl->unload();
}

const std::string& MTDynLib::getName(void) const
{
	return m_impl->getName();
}

MTDynLibId MTDynLib::getId(void) const
{
	return m_impl->getId();
}

void* MTDynLib::getSymbol(const std::string& strName) const throw()
{
	return m_impl->getSymbol(strName);
}

std::string MTDynLib::dynLibError(void)
{
	return m_impl->dynLibError();
}
