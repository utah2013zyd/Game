#include <DynLib/MTDynLibImpl.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
using namespace Orz;
//-----------------------------------------------------------------------
MTDynLibImpl::MTDynLibImpl(const std::string& _name, MTDynLibId _dlid) 
	: m_name(_name)
	, m_id(_dlid)
	, m_hInst(NULL)
	, m_status(MTDynLibImpl::ST_START)
{
}

MTDynLibImpl::MTDynLibImpl(const MTDynLibImpl& _dl) 
	: m_name(_dl.m_name)
	, m_id(_dl.m_id)
	, m_hInst(_dl.m_hInst)
	, m_status(MTDynLibImpl::ST_START)
{
}

MTDynLibImpl& MTDynLibImpl::operator=(const MTDynLibImpl& _dl)
{
	m_name = _dl.m_name;
	m_id = _dl.m_id;
	m_hInst = _dl.m_hInst;
	m_status = _dl.m_status;
	return (*this);
}

MTDynLibImpl::~MTDynLibImpl()
{
}

bool MTDynLibImpl::load()
{
    // Log library load
    //    LogManager::getSingleton().out("Loading library ");
	if (m_status == MTDynLibImpl::ST_START)
	{
		std::string _name = m_name;
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX
		// dlopen() does not add .so to the filename, like windows does for .dll
	 //   if (_name.substr(_name.length() - 3, 3) != ".so")
	 //      _name += ".so";
		//if (_name.substr(0, 3) != "lib")
	 //      //_name = Orz::UTF8String("lib") + _name;
		//   _name = std::string("lib") + _name;	// Nous: change to std::string
		 m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( _name.c_str() );
#else
#	if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#		ifdef UNICODE
				std::wstring utf16;
				::utf8::utf8to16(_name.begin(), _name.end(), std::back_inserter(utf16));
				m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( utf16.c_str() );
#		else
				m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( _name.c_str() );
#		endif
#	else
			m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( _name.c_str() );
#	endif
#endif
		if (!m_hInst)
		{
			//throw ORZ_EXCEPTION(Exception::ERR_INTERNAL_ERROR)<<errno_info_text(dynlibError());
			//throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__)) << asio_file_name_errno_info_text(dynLibError());
			return false;
		}
		m_status = MTDynLibImpl::ST_LOADED;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------
bool MTDynLibImpl::unload()
{
	if (m_status == MTDynLibImpl::ST_LOADED)
	{
		if (DYNLIB_UNLOAD(m_hInst))
		{
			//throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__)) << asio_file_name_errno_info_text(dynLibError());
			return false;
		}
		m_status = MTDynLibImpl::ST_START;
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------
void* MTDynLibImpl::getSymbol(const std::string& _strName) const throw()
{
    return (void*)DYNLIB_GETSYM(m_hInst, _strName.c_str());
}

//-----------------------------------------------------------------------
std::string MTDynLibImpl::dynLibError(void)
{
	
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
        );
	std::string ret = (char*)lpMsgBuf;
    // Free the buffer.
    LocalFree(lpMsgBuf);
    return ret;
#elif ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX
    return std::string(dlerror());
#else
    return std::string("");
#endif
	return "";
}

