#ifndef __Orz_ToolkitPlus_OrzAsioException_hpp_
#define __Orz_ToolkitPlus_OrzAsioException_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
struct ThrowAddress
{
	ThrowAddress(const char* _fileName, int _line)
		: fileName(_fileName)
		, line(_line)
	{
	}

	std::string fileName;
	int line;
};

typedef boost::error_info<struct asio_file_name, std::string> asio_file_name_errno_info_text;
typedef boost::error_info<struct asio_line, int> asio_line_errno_info;
typedef boost::error_info<struct asio_tag_errno, int> asio_errno_info;

class OrzAsioException 
	: public boost::exception
{
public:
	enum ExceptionCodes // 0~99
	{
		EXP_FILE_IO = 0,			// 文件IO错误
		EXP_INVALID_STATE,			// 错误的状态
		EXP_INVALID_PARAMS,			// 错误的参数
		EXP_INTERNAL_ERROR,			// 内部错误
		EXP_NOT_IMPLEMENTED,		// 没有执行
		EXP_TOTAL
	};
public:
	OrzAsioException(int _tag, const ThrowAddress& _throwAdrs) throw()
		: m_err("no msg")
		, m_tag(_tag)
		, m_ta(_throwAdrs)
	{
		(*this) << asio_file_name_errno_info_text(_throwAdrs.fileName) 
				<< asio_line_errno_info(_throwAdrs.line) 
				<< asio_errno_info(_tag);
	}
	OrzAsioException(int _tag, const ThrowAddress& _throwAdrs, OrzAsioException& _last) throw()
		: m_err("no msg")
		, m_tag(_tag)
		, m_ta(_throwAdrs)
		, m_last(_last.clone())
	{
		(*this) << asio_file_name_errno_info_text(_throwAdrs.fileName) 
				<< asio_line_errno_info(_throwAdrs.line) 
				<< asio_errno_info(_tag);
	}
	virtual ~OrzAsioException() throw() {}

	virtual OrzAsioExceptionShrPtr clone()
	{
		if (!m_last)
			return OrzAsioExceptionShrPtr(new OrzAsioException(m_tag, m_ta));
		else
			return OrzAsioExceptionShrPtr(new OrzAsioException(m_tag, m_ta, *m_last));
	}

	std::vector<const char*> what() const throw()
	{
		m_err = boost::diagnostic_information(*this);
		std::vector<const char*> errs;
		if (m_last)
			errs = m_last->what();
		errs.push_back(m_err.c_str());
		return errs;
	}

protected:
	OrzAsioExceptionShrPtr last()
	{
		return m_last;
	}

	const ThrowAddress& throwAdr() const
	{
		return m_ta;
	}

private:
	mutable std::string				m_err;
	int								m_tag;
	ThrowAddress					m_ta;
	mutable OrzAsioExceptionShrPtr	m_last;
};

// file io error exp
typedef boost::error_info<struct asio_file_io_tag_errno, int> asio_file_io_errno_info;
class FileIOExp : public OrzAsioException 
{
public:
	enum FileIOExpCodes // 100~199
	{
		FIEXP_CANNOT_WRITE_TO_FILE = 100,	// 无法写入文件
		FIEXP_FILE_PTR_ERR,					// 文件指针错误
		FIEXP_FILE_NOT_FOUND,				// 找不到文件
	};
public:
	FileIOExp(int _childTag, const ThrowAddress& _throwAdrs) throw()
		: OrzAsioException(EXP_FILE_IO, _throwAdrs)
		, m_tag(_childTag)
	{
		(*this) << asio_file_io_errno_info(_childTag);
	}
	FileIOExp(int _childTag, const ThrowAddress& _throwAdrs, OrzAsioException& _last) throw()
		: OrzAsioException(EXP_FILE_IO, _throwAdrs, _last)
		, m_tag(_childTag)
	{
		(*this) << asio_file_io_errno_info(_childTag);
	}
	virtual ~FileIOExp() throw() {}

	virtual OrzAsioExceptionShrPtr clone()
	{
		OrzAsioExceptionShrPtr oae = OrzAsioException::last();
		if (!oae)
			return OrzAsioExceptionShrPtr(new FileIOExp(m_tag, OrzAsioException::throwAdr()));
		else
			return OrzAsioExceptionShrPtr(new FileIOExp(m_tag, OrzAsioException::throwAdr(), *oae));
	}

private:
	int					m_tag;
};

// invalid params error exp
typedef boost::error_info<struct asio_invalid_params_tag_errno, int> asio_invalid_params_errno_info;
class InvalidParamsExp : public OrzAsioException 
{
public:
	enum InvalidParamsExpCodes // 200~299
	{
		IPEXP_RANGE = 200,	// 越界
		IPEXP_NULL_PTR,		// 空指针
	};
public:
	InvalidParamsExp(int _childTag, const ThrowAddress& _throwAdrs) throw()
		: OrzAsioException(EXP_INVALID_PARAMS, _throwAdrs)
		, m_tag(_childTag)
	{
		(*this) << asio_invalid_params_errno_info(_childTag);
	}
	InvalidParamsExp(int _childTag, const ThrowAddress& _throwAdrs, OrzAsioException& _last) throw()
		: OrzAsioException(EXP_INVALID_PARAMS, _throwAdrs, _last)
		, m_tag(_childTag)
	{
		(*this) << asio_invalid_params_errno_info(_childTag);
	}
	virtual ~InvalidParamsExp() throw() {}

	virtual OrzAsioExceptionShrPtr clone()
	{
		OrzAsioExceptionShrPtr oae = OrzAsioException::last();
		if (!oae)
			return OrzAsioExceptionShrPtr(new InvalidParamsExp(m_tag, OrzAsioException::throwAdr()));
		else
			return OrzAsioExceptionShrPtr(new InvalidParamsExp(m_tag, OrzAsioException::throwAdr(), *oae));
	}

private:
	int					m_tag;
};

// range error exp
typedef boost::error_info<struct asio_range_tag_errno, int> asio_range_errno_info;
class RangeExp : public InvalidParamsExp 
{
public:
	enum RangeExpCodes // 300~499
	{
		REXP_ARRAY = 300,	// 数组越界
	};
public:
	RangeExp(int _childTag, const ThrowAddress& _throwAdrs) throw()
		: InvalidParamsExp(IPEXP_RANGE, _throwAdrs)
		, m_tag(_childTag)
	{
		(*this) << asio_range_errno_info(_childTag);
	}
	RangeExp(int _childTag, const ThrowAddress& _throwAdrs, OrzAsioException& _last) throw()
		: InvalidParamsExp(IPEXP_RANGE, _throwAdrs, _last)
		, m_tag(_childTag)
	{
		(*this) << asio_range_errno_info(_childTag);
	}
	virtual ~RangeExp() throw() {}

	virtual OrzAsioExceptionShrPtr clone()
	{
		OrzAsioExceptionShrPtr oae = OrzAsioException::last();
		if (!oae)
			return OrzAsioExceptionShrPtr(new RangeExp(m_tag, OrzAsioException::throwAdr()));
		else
			return OrzAsioExceptionShrPtr(new RangeExp(m_tag, OrzAsioException::throwAdr(), *oae));
	}

private:
	int					m_tag;
};
}
#endif
