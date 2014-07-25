#ifndef __Orz_Example_Exception__
#define __Orz_Example_Exception__
#include <orz/Toolkit_Base/Exception.h>

namespace Orz
{
class ExceptionTest
{
public:
	void throwExcpetion()
	{
		throw ORZ_EXCEPTION(Exception::ERR_INTERNAL_ERROR)<<errno_info_text("Excpetion Test!");
	}

	void throwIOExpection()
	{
		throw ORZ_EXCEPTION(Exception::ERR_CANNOT_WRITE_TO_FILE)<<errno_info_text("IOExcpetion Test!");
	}

};

inline void Example_Exception(void)
{
	ExceptionTest et;

	try
	{
		et.throwExcpetion();
	}catch(std::exception & e)
	{
		std::cout<<e.what();
	}


	try
	{
		et.throwIOExpection();

	}catch(IOException & e)
	{
		std::cout<<e.what();
	}
}

}

#endif 