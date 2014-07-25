#ifndef __Orz_ToolkitPlus_LoggerImpl_h_
#define __Orz_ToolkitPlus_LoggerImpl_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Agency.hpp>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogEnv.h>
#include <orz/Toolkit_Plus/Log/Log.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
namespace Orz 
{
class _OrzToolkitPlusPrivate LoggerImpl
{
public:
	LoggerImpl(const char* _config, uint32 _buffCount, uint32 _buffQueueSize, uint32 _doubleBuffQueueSize) throw();
	~LoggerImpl() throw();

public:
	bool					boost					();
	void					shutdown				();
	void					join					();
	void					out						(LogShrPtr _data) throw();
	void					outBuff					(LogShrPtr _data) throw();
	void					output					(LogShrPtr _data) throw();

private:
	Agency<Agent>			m_queueBuffs;
	Agent					m_doubleBuff;
	bool					m_hasInited;
};

}

#endif
