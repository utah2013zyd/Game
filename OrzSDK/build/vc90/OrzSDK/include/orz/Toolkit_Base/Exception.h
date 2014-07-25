#ifndef __Orz_ToolkitBase_Exception_h__
#define __Orz_ToolkitBase_Exception_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
namespace Orz
{
typedef boost::error_info<struct tag_errno,int> errno_info; 
typedef boost::error_info<struct tag_errno_text, std::string> errno_info_text;

typedef boost::error_info<struct tag_orz_function,std::string> orz_throw_function;
typedef boost::error_info<struct tag_orz_file,std::string> orz_throw_file;
typedef boost::error_info<struct tag_orz_line,int> orz_throw_line;
//  BOOST_VERSION % 100 is the patch level
//  BOOST_VERSION / 100 % 1000 is the minor version
//  BOOST_VERSION / 100000 is the major version

template<class T, int branch = static_cast<int>(BOOST_VERSION >= 103900) >
class EXCEPTION_RETURN{};

template<class T>
class EXCEPTION_RETURN<T, 1>
{
public: typedef T*  type;
};

template<class T>
class EXCEPTION_RETURN<T, 0>
{
public: typedef boost::shared_ptr<T>  type;
};



class _OrzToolkitBaseExport boost::exception;
class _OrzToolkitBaseExport Exception: public boost::exception, public std::exception 
{

public:
	  enum ExceptionCodes {
            ERR_CANNOT_WRITE_TO_FILE,//无法写入文件
            ERR_INVALID_STATE,//错误的状态
            ERR_INVALID_PARAMS,//错误的参数
            ERR_DUPLICATE_ITEM,//错误的副本
            ERR_ITEM_NOT_FOUND,//没有找到资源
            ERR_FILE_NOT_FOUND,//没有找到文件
            ERR_INTERNAL_ERROR,//内部错误
            ERR_RT_ASSERTION_FAILED, //运行时断言失败
			ERR_NOT_IMPLEMENTED//没有执行
        };
public:
	Exception(int tag)
	{
		(*this)<<errno_info(tag);
	}

	const char* what() const throw()
	{

		std::stringstream err_stream;
		err_stream<<"Exception:";
		if(EXCEPTION_RETURN<std::string const>::type err = boost::get_error_info<errno_info_text>(*this) )
		{
			err_stream<<(*err)<<std::endl;
		}
		else
		{
			err_stream<<"no msg"<<std::endl;
		}

		if(   EXCEPTION_RETURN<std::string const>::type err = boost::get_error_info<orz_throw_file>(*this) )
		{
			err_stream<<"File:"<<(*err)<<std::endl;
		}
		
		if( EXCEPTION_RETURN<std::string const>::type err  = boost::get_error_info<orz_throw_function>(*this) )
		{
			err_stream<<"Function:"<<(*err)<<std::endl;
		}
		

		
		if( EXCEPTION_RETURN<const int>::type err  = boost::get_error_info<orz_throw_line>(*this) )
		{
			
			err_stream<<"Line:"<<(*err)<<std::endl;
		}
		


		static std::string s;
		s = err_stream.str();
		return s.c_str();
	}
	

};



class _OrzToolkitBaseExport UnimplementedException : public Exception 
{
public:
	UnimplementedException(int tag)
		: Exception(tag)
	{
		
	}
};


class _OrzToolkitBaseExport FileNotFoundException : public Exception
{
public:
	FileNotFoundException(int tag)
		: Exception(tag) 
	{
		//(*this)<<errno_info(ERR_FILE_NOT_FOUND);
	}
};


class _OrzToolkitBaseExport IOException : public Exception
{
public:
	IOException(int tag)
		: Exception(tag)
	{
		//(*this)<<errno_info(ERR_CANNOT_WRITE_TO_FILE);
	}
};


class _OrzToolkitBaseExport InvalidStateException : public Exception
{
public:
	InvalidStateException(int tag)
		: Exception(tag)
	{
		//(*this)<<errno_info(ERR_INVALID_STATE);
	}
};


	
class _OrzToolkitBaseExport InvalidParametersException : public Exception
{
public:
	InvalidParametersException(int tag)
		: Exception(tag)
	{
		//(*this)<<errno_info(ERR_INVALID_PARAMS);
	}
};


class _OrzToolkitBaseExport ItemIdentityException : public Exception
{
public:
	ItemIdentityException(int tag)
		: Exception(tag) 
	{
		//(*this)<<errno_info(ERR_ITEM_NOT_FOUND);
	}
};




class _OrzToolkitBaseExport InternalErrorException : public Exception
{
public:
	InternalErrorException(int tag)
		: Exception(tag) 
	{
		//(*this)<<errno_info(ERR_INTERNAL_ERROR);
	}
};

/*
class _OrzToolkitBaseExport RenderingAPIException : public Exception
{
public:
	RenderingAPIException(void)
		: Exception()
	{
		(*this)<<errno_info(ERR_INTERNAL_ERROR);
	}
};
*/

class _OrzToolkitBaseExport RuntimeAssertionException : public Exception
{
public:
	RuntimeAssertionException(int tag)
		: Exception(tag) 
	{
		//(*this)<<errno_info(ERR_RT_ASSERTION_FAILED);
	}
};
	

/** Template struct which creates a distinct type for each exception code.
@note
This is useful because it allows us to create an overloaded method
for returning different exception types by value without ambiguity. 
From 'Modern C++ Design' (Alexandrescu 2001).
*/
template <int num>
struct ExceptionCodeType
{
	enum { number = num };
};

class ExceptionFactory
{
private:
	/// Private constructor, no construction
	ExceptionFactory() {}
public:
	static UnimplementedException create(
		ExceptionCodeType<Exception::ERR_NOT_IMPLEMENTED> code 
		)
	{
		return UnimplementedException(code.number);
	}
	static FileNotFoundException create(
		ExceptionCodeType<Exception::ERR_FILE_NOT_FOUND> code
		)
	{
		return FileNotFoundException(code.number);
	}
	static IOException create(
		ExceptionCodeType<Exception::ERR_CANNOT_WRITE_TO_FILE> code
		)
	{
		return IOException(code.number);
	}
	static InvalidStateException create(
		ExceptionCodeType<Exception::ERR_INVALID_STATE> code
		) 
	{
		return InvalidStateException(code.number);
	}
	static InvalidParametersException create(
		ExceptionCodeType<Exception::ERR_INVALID_PARAMS> code
		)
	{
		return InvalidParametersException(code.number);
	}
	static ItemIdentityException create(
		ExceptionCodeType<Exception::ERR_ITEM_NOT_FOUND> code
		)
	{
		return ItemIdentityException(code.number);
	}
	static ItemIdentityException create(
		ExceptionCodeType<Exception::ERR_DUPLICATE_ITEM> code
		)
	{
		return ItemIdentityException(code.number);
	}
	static InternalErrorException create(
		ExceptionCodeType<Exception::ERR_INTERNAL_ERROR> code
		)
	{
		return InternalErrorException(code.number);
	}
	/*
	static RenderingAPIException create(
		ExceptionCodeType<Exception::ERR_RENDERINGAPI_ERROR> code
		)
	{
		return RenderingAPIException(code.number);
	} 
	*/
	static RuntimeAssertionException create(
		ExceptionCodeType<Exception::ERR_RT_ASSERTION_FAILED> code
		)
	{
		return RuntimeAssertionException(code.number);
	}

};

#ifndef ORZ_EXCEPTION
#define ORZ_EXCEPTION(num) (Orz::ExceptionFactory::create(Orz::ExceptionCodeType<num>())<<orz_throw_function(BOOST_CURRENT_FUNCTION)<<orz_throw_file(__FILE__)<<orz_throw_line((int)__LINE__))
#endif

}

#endif
