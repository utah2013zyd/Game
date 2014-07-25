#ifndef __Orz_ToolkitPlus_Query_h_
#define __Orz_ToolkitPlus_Query_h_
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
namespace Orz
{
//! ���ڹ���SQL������
class Query
	: public std::stringstream
{
public:
	//! Ĭ�Ϲ��캯��
	ORZ_PLUS_INLINE
	Query()
		: std::stringstream(std::stringstream::in | std::stringstream::out)
	{
	}
	
	//! ��������
	ORZ_PLUS_INLINE
	~Query()
	{
	}

public:
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
	void			clean			()
	{
		this->str("");
		this->clear();
	}
};
}
#endif
