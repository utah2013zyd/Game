#include <orz/Toolkit_Plus/Agent/Agent.h>
using namespace Orz;

Agent::Agent()
	: m_work(new boost::asio::io_service::work(m_workSvc))
{
}

Agent::~Agent()
{
}

int32 Agent::start()
{
	if (m_workThread)
	{
		return 1;
	}

	ThreadShrPtr tptr(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_workSvc)));
	m_workThread = tptr;
	
	return 0;
}

boost::asio::io_service& Agent::ioService()
{
	return m_workSvc;
}

void Agent::join()
{
	if (m_workThread)
	{
		m_workThread->join();
		m_workThread = ThreadShrPtr();
		m_workSvc.reset();
	}
}

void Agent::stop()
{
	m_workSvc.stop();
}

ThreadShrPtr Agent::getThread()
{
	return m_workThread;
}
