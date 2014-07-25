#include <orz/Toolkit_Plus/DynLib/MTDynLibMgr.h>
#include <DynLib/MTDynLibMgrImpl.h>
using namespace Orz;

MTDynLibMgr::MTDynLibMgr(uint32 _dynLibsHash)
	: m_impl(new MTDynLibMgrImpl(_dynLibsHash))
{
}

MTDynLibMgr::~MTDynLibMgr()
{
	if (m_impl)
	{
		delete m_impl;
	}
}

MTDynLibShrPtr MTDynLibMgr::load(const std::string& _filename)
{
	return m_impl->load(_filename);
}

void MTDynLibMgr::unload(MTDynLibShrPtr _lib)
{
	m_impl->unload(_lib);
}

  
