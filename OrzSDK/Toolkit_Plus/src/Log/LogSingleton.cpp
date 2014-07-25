#include <orz/Toolkit_Plus/Log/LogSingleton.h>
using namespace Orz;

LogSingleton::~LogSingleton()
{
	clear();
}

LogSingleton& LogSingleton::instance()
{
	static LogSingleton singleton;
	return singleton;
}

void LogSingleton::set(OrzAsioLogger* _logger)
{
	if (m_logger)
		return;

	m_logger = _logger;
}

OrzAsioLogger* LogSingleton::get()
{
	return m_logger;
}

void LogSingleton::clear()
{
	if (m_logger)
	{
		delete m_logger;
		m_logger = 0;
	}
}


