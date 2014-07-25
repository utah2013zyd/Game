#include <Log/LoggerImpl.h>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
#include <log4cxx/logmanager.h> 
#include <log4cxx/xml/domconfigurator.h> 
#include <log4cxx/patternlayout.h> 
#include <log4cxx/rolling/rollingfileappender.h> 
#include <log4cxx/rolling/fixedwindowrollingpolicy.h> 
#include <log4cxx/rolling/filterbasedtriggeringpolicy.h> 
#include <log4cxx/filter/levelrangefilter.h> 
#include <log4cxx/helpers/pool.h> 
#include <log4cxx/logger.h> 
#include <log4cxx/propertyconfigurator.h> 
#include <log4cxx/dailyrollingfileappender.h> 
#include <log4cxx/helpers/stringhelper.h>

using namespace Orz;
using namespace log4cxx; 
using namespace log4cxx::rolling; 
using namespace log4cxx::xml; 
using namespace log4cxx::filter; 
using namespace log4cxx::helpers;


LoggerImpl::LoggerImpl(const char* _config, uint32 _buffCount, uint32 _buffQueueSize, uint32 _doubleBuffQueueSize) throw()
	: m_queueBuffs(_buffCount == 0 ? 1 : _buffCount, NumChooser(0, (_buffCount == 0 ? 1 : _buffCount) - 1, (uint32)&_buffCount, false))
	, m_hasInited(false)
{
	if (_config)
	{
		PropertyConfigurator::configure(File(_config));
		m_hasInited = true;
	}
}

LoggerImpl::~LoggerImpl() throw()
{
}

bool LoggerImpl::boost()
{
	if (!m_hasInited)
	{
		return false;
	}

	for (uint32 i=0; i<m_queueBuffs.size(); ++i)
	{
		if (m_queueBuffs[i].start() != 0)
		{
			return false;
		}
	}

	if (m_doubleBuff.start() != 0)
	{
		return false;
	}
	return true;
}

void LoggerImpl::shutdown()
{
	m_doubleBuff.stop();
	for (uint32 i=0; i<m_queueBuffs.size(); ++i)
	{
		m_queueBuffs[i].stop();
	}
	LogManager::shutdown();
}

void LoggerImpl::join()
{
	m_doubleBuff.join();
	for (uint32 i=0; i<m_queueBuffs.size(); ++i)
	{
		m_queueBuffs[i].join();
	}
}

void LoggerImpl::out(LogShrPtr _data) throw()
{
	m_queueBuffs.select().post(boost::bind(&LoggerImpl::outBuff, this, _data));
}

void LoggerImpl::outBuff(LogShrPtr _data) throw()
{
	m_doubleBuff.post(boost::bind(&LoggerImpl::output, this, _data));
}

void LoggerImpl::output(LogShrPtr _data) throw()
{
	LoggerPtr logger = Logger::getRootLogger();

	if (logger != 0 && _data)
	{
		OutLevel ol = _data->getLevel();
		switch (ol)
		{
		case OL_DEBUG:	return logger->debug(_data->str().c_str());
		case OL_INFO:	return logger->info(_data->str().c_str());
		case OL_WARN:	return logger->warn(_data->str().c_str());
		case OL_ERROR:	return logger->error(_data->str().c_str());
		case OL_FATAL:	return logger->fatal(_data->str().c_str());
		default:		return logger->trace(_data->str().c_str());
		}
	}
}
