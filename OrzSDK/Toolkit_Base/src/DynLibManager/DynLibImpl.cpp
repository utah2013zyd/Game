
#include "DynLibManager/DynLibImpl.h"
#include <orz/Toolkit_Base/Exception.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

using namespace Orz;



//-----------------------------------------------------------------------
DynLibImpl::DynLibImpl( const std::string & name ):_name(name),_hInst(NULL)
{
}



DynLibImpl::DynLibImpl(const DynLibImpl & dl):_name(dl._name),_hInst(dl._hInst)
{

}
DynLibImpl& DynLibImpl::operator = (const DynLibImpl& dl)
{
	_name = dl._name;
	_hInst = dl._hInst;
	return (*this);
}



//-----------------------------------------------------------------------
DynLibImpl::~DynLibImpl()
{
}

//-----------------------------------------------------------------------
void DynLibImpl::load()
{
    // Log library load
    //    ILogManager::getSingleton().out("Loading library ");

	std::string name = _name;
#if ORZ_PLATFORM == ORZ_PLATFORM_LINUX
    // dlopen() does not add .so to the filename, like windows does for .dll
    if (name.substr(name.length() - 3, 3) != ".so")
       name += ".so";
	if (name.substr(0, 3) != "lib")
       //name = Orz::UTF8String("lib") + name;
	   name = std::string("lib") + name;	// Nous: change to std::string
	 _hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );
#else
	#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
		#ifdef UNICODE
			std::wstring utf16;
			::utf8::utf8to16(name.begin(), name.end(), std::back_inserter(utf16));
			_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( utf16.c_str() );
		#else
			_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );
		#endif
	#else
		_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD( name.c_str() );
	#endif
#endif
    if( !_hInst )
    {
		throw ORZ_EXCEPTION(Exception::ERR_INTERNAL_ERROR)<<errno_info_text(dynlibError());
		
    }
	
}

//-----------------------------------------------------------------------
void DynLibImpl::unload()
{
   
    if( DYNLIB_UNLOAD( _hInst ) )
	{
		throw ORZ_EXCEPTION(Exception::ERR_INTERNAL_ERROR)<<errno_info_text(dynlibError());
	}

}

//-----------------------------------------------------------------------
void* DynLibImpl::getSymbol( const std::string & strName ) const throw()
{
    return (void*)DYNLIB_GETSYM( _hInst, strName.c_str() );
}
//-----------------------------------------------------------------------
std::string DynLibImpl::dynlibError( void )
{
#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
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
    LocalFree( lpMsgBuf );
    return ret;
#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX
    return std::string(dlerror());
#else
    return std::string("");
#endif
}

