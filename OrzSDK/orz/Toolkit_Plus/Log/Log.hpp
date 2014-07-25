#ifndef __Orz_ToolkitPlus_Log_hpp_
#define __Orz_ToolkitPlus_Log_hpp_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogEnv.h>
namespace Orz
{
//! 用于构建日志输出数据的类
class Log
	: public std::stringstream
{
public:
	//! 默认构造函数
	ORZ_PLUS_INLINE
	Log()
		: std::stringstream(std::stringstream::in | std::stringstream::out)
		, m_ol(OL_DEBUG)
		, m_usingDoubleBuff(false)
	{
	}

	ORZ_PLUS_INLINE
	Log(OutLevel _ol, bool _usingDoubleBuff=false)
		: std::stringstream(std::stringstream::in | std::stringstream::out)
		, m_ol(_ol)
		, m_usingDoubleBuff(_usingDoubleBuff)
	{
	}
	
	//! 析构函数
	ORZ_PLUS_INLINE
	~Log()
	{
	}

public:
	ORZ_PLUS_INLINE
	void			initArg			(OutLevel _ol, bool _usingDoubleBuff)
	{
		m_ol = _ol;
		m_usingDoubleBuff = _usingDoubleBuff;
	}


	ORZ_PLUS_INLINE
	void			put				(const char* _str, ... ) throw()
	{
		va_list ap;
		char buf[32768];

		va_start(ap, _str);
		vsnprintf(buf, 32768, _str, ap);
		va_end(ap);

		*this << buf;
	}

	ORZ_PLUS_INLINE
	OutLevel		getLevel		() const
	{
		return m_ol;
	}

	ORZ_PLUS_INLINE
	bool			usingDoubleBuff	() const
	{
		return m_usingDoubleBuff;
	}

	ORZ_PLUS_INLINE
	void			clean			()
	{
		this->str("");
		this->clear();
	}

private:
	OutLevel		m_ol;
	bool			m_usingDoubleBuff;
};
}
#endif
