#ifndef __Orz_ToolkitPlus_LogSingleton_h_
#define __Orz_ToolkitPlus_LogSingleton_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Log/OrzAsioLogger.h>
namespace Orz
{
//! Log系统的单件包装类
/*!
 *	\note 单件
 example:
 \code
	OrzAsioLogger* orzAsioLogger = new OrzAsioLogger("log4cxx.properties", 3,100,300);
	sLogIns.set(orzAsioLogger);
	...
	sLogger->out(OL_DEBUG, "Log - Id:[", d2s(1).c_str(), "]!", 0);
	sLogger->outByte(OL_DEBUG, "Log - Id:[", 0, d2s(1).c_str(), d2s(1).size(), "]!", 0, 0);
 \endcode
 */
class _OrzToolkitPlusExport LogSingleton
{
	LogSingleton() : m_logger(0) {}
	LogSingleton& operator=(const LogSingleton& _other);
public:
	~LogSingleton();

public:
	static LogSingleton& instance();

public:
	void				set					(OrzAsioLogger* _logger);
	OrzAsioLogger*		get					();
	void				clear				();

private:
	OrzAsioLogger*		m_logger;
};

#define sLogIns	LogSingleton::instance()
#define sLogger LogSingleton::instance().get()
}
#endif
