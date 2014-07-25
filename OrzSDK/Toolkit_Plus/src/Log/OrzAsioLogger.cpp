#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include "Log/LoggerImpl.h"
using namespace Orz;


typedef boost::shared_ptr<FreeList<Log> > LogFLShrPtr;

OrzAsioLogger::OrzAsioLogger(const char* _config, uint32 _buffCount, uint32 _buffQueueSize, 
							 uint32 _doubleBuffQueueSize, uint32 _logPoolClr, uint32 _logPoolPer) throw()
	: m_impl(new LoggerImpl(_config, _buffCount, _buffQueueSize, _doubleBuffQueueSize))
{
	sLogFL.set(LogFLShrPtr(new FreeList<Log>(_logPoolClr, _logPoolPer)));
}

OrzAsioLogger::~OrzAsioLogger() throw()
{
	if (m_impl)
	{
		delete m_impl;
	}
}

bool OrzAsioLogger::boost()
{
	return m_impl->boost();
}

void OrzAsioLogger::shutdown()
{
	m_impl->shutdown();
}

void OrzAsioLogger::join()
{
	m_impl->join();
}

void OrzAsioLogger::out(OutLevel _ol, const char *_str, ...) throw()
{
	if (_str == 0)
		return;

	va_list arg_ptr;
	va_start(arg_ptr, _str); 

	LogShrPtr ld = allocLog(_ol, false);
	*ld << std::string(_str, strlen(_str));

	while (1)
	{
		char* str = 0;
		str = va_arg(arg_ptr, char*);
		if (!str)
			break;

		*ld << std::string(str, strlen(str));
	}
	va_end(arg_ptr);
	m_impl->outBuff(ld);
}

void OrzAsioLogger::outDBuff(OutLevel _ol, const char* _str, ... ) throw()
{
	if (_str == 0)
		return;

	va_list arg_ptr;
	va_start(arg_ptr, _str); 

	LogShrPtr ld = allocLog(_ol, true);
	*ld << std::string(_str, strlen(_str));

	while (1)
	{
		char* str = 0;
		str = va_arg(arg_ptr, char*);
		if (!str)
			break;

		*ld << std::string(str, strlen(str));
	}
	va_end(arg_ptr);
	m_impl->out(ld);
}

void OrzAsioLogger::outByte(OutLevel _ol, const char* _str, uint32 _len, ... ) throw()
{
	if (_str == 0)
		return;

	va_list arg_ptr;
	va_start(arg_ptr, _len); 

	if (_len == 0)
		_len = (uint32)strlen(_str);

	LogShrPtr ld = allocLog(_ol, false);
	*ld << std::string(_str, _len);

	while (1)
	{
		char* str = 0;
		str = va_arg(arg_ptr, char*);
		if (!str)
			break;

		uint32 len = 0;
		len = va_arg(arg_ptr, uint32);
		if (len == 0)
			len = (uint32)strlen(str);

		*ld << std::string(str, len);
	}
	va_end(arg_ptr);
	m_impl->outBuff(ld);
}

void OrzAsioLogger::outByteDBuff(OutLevel _ol, const char* _str, uint32 _len, ... ) throw()
{
	if (_str == 0)
		return;

	va_list arg_ptr;
	va_start(arg_ptr, _len); 

	if (_len == 0)
		_len = (uint32)strlen(_str);

	LogShrPtr ld = allocLog(_ol, true);
	*ld << std::string(_str, _len);

	while (1)
	{
		char* str = 0;
		str = va_arg(arg_ptr, char*);
		if (!str)
			break;

		uint32 len = 0;
		len = va_arg(arg_ptr, uint32);
		if (len == 0)
			len = (uint32)strlen(str);

		*ld << std::string(str, len);
	}
	va_end(arg_ptr);
	m_impl->out(ld);
}

void OrzAsioLogger::out(LogShrPtr _data) throw()
{
	if (_data->usingDoubleBuff())
	{
		m_impl->out(_data);
	}
	else
	{
		m_impl->outBuff(_data);
	}
}

void OrzAsioLogger::outFormat(OutLevel _ol, bool _usingDoubleBuff, const char* _str, ... ) throw()
{
	va_list ap;
	char buf[32768];

	va_start(ap, _str);
	vsnprintf(buf, 32768, _str, ap);
	va_end(ap);

	LogShrPtr ld = allocLog(_ol, _usingDoubleBuff);
	*ld << std::string(buf);

	if (_usingDoubleBuff)
	{
		m_impl->out(ld);
	}
	else
	{
		m_impl->outBuff(ld);
	}
}
