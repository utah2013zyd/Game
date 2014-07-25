#include <DynLib/MTDynLibMgrImpl.h>
#include <DynLib/MTDynLibImpl.h>
using namespace Orz;

//-----------------------------------------------------------------------
MTDynLibMgrImpl::MTDynLibMgrImpl(uint32 _dynLibsHash)
	: m_libList(_dynLibsHash)
	, m_dlIdr(10000, 1)
{
	m_libList.setHash();
}

//-----------------------------------------------------------------------
MTDynLibMgrImpl::~MTDynLibMgrImpl()
{
	uint32 size = m_libList.arraySize();
	for (uint32 i=0; i<size; ++i)
	{
		ThreadShrMap<MTDynLibId, MTDynLibShrPtr>& tMap = m_libList.threadMap(i);
		tMap.beginWrite();
		std::map<MTDynLibId, MTDynLibShrPtr>& m = tMap.map();
		std::map<MTDynLibId, MTDynLibShrPtr>::iterator itr = m.begin();
		for (; itr!=m.end(); ++itr)
		{
			itr->second->unload();
		}
		tMap.endWrite();
	}

    m_libList.clear();
}

//-----------------------------------------------------------------------
MTDynLibShrPtr MTDynLibMgrImpl::load(const std::string& _filename)
{
	MTDynLibId dlid = m_dlIdr.generate();
	if (dlid == m_dlIdr.errObj())
	{
		return MTDynLibShrPtr();
	}

	MTDynLibShrPtr pLib = MTDynLibShrPtr(new MTDynLib(_filename, dlid));
	if (pLib->load())
	{
		m_libList.add(dlid, pLib, dlid);
		return pLib;
	}
	else
	{
		m_dlIdr.release(dlid);
	}
	return MTDynLibShrPtr();
}

//-----------------------------------------------------------------------
void MTDynLibMgrImpl::unload(MTDynLibShrPtr _lib)
{
	MTDynLibId dlid = _lib->getId();
	m_libList.erase(dlid, dlid);
	_lib->unload();
	_lib.reset();
	m_dlIdr.release(dlid);
}
