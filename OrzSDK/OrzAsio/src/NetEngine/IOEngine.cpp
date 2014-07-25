#include <orz/OrzAsio/NetEngine/IOEngine.h>
using namespace Orz;


IOEngine::IOEngine(uint32 _threadPoolSize)
	: m_work(m_ioSvc)
	, m_threadPool(_threadPoolSize == 0 ? 1:_threadPoolSize)
{
}

IOEngine::~IOEngine()
{
}

void IOEngine::boost()
{
	for (uint32 i=0; i<m_threadPool.size(); ++i)
	{
		m_threadPool[i] = ThreadShrPtr(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioSvc)));
	}
}

boost::asio::io_service& IOEngine::ioService()
{
	return m_ioSvc;
}

void IOEngine::shutdown()
{
	m_ioSvc.stop();
}

void IOEngine::join()
{
	for (uint32 i=0; i<m_threadPool.size(); ++i)
	{
		if (m_threadPool[i])
		{
			m_threadPool[i]->join();
		}
	}
}
