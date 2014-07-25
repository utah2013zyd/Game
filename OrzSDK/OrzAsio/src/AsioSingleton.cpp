#include <orz/OrzAsio/AsioSingleton.h>
using namespace Orz;


AsioSingleton::AsioSingleton()
	: m_asioMgr(0)
{
}

AsioSingleton::~AsioSingleton()
{
	clear();
}

AsioSingleton& AsioSingleton::instance()
{
	static AsioSingleton singleton;
	return singleton;
}

void AsioSingleton::set(AsioManager* _asioMgr)
{
	if (m_asioMgr)
	{
		return;
	}

	m_asioMgr = _asioMgr;
}

AsioManager* AsioSingleton::get()
{
	return m_asioMgr;
}

void AsioSingleton::clear()
{
	if (m_asioMgr)
	{
		delete m_asioMgr;
		m_asioMgr = 0;
	}
}
