#ifndef __Orz_ToolkitPlus_DynLibImpl_h__
#define __Orz_ToolkitPlus_DynLibImpl_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/DynLib/MTDynLib.h>
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

namespace Orz 
{   
class _OrzToolkitPlusPrivate MTDynLibImpl
{
	enum Status
	{
		ST_START = 0,
		ST_LOADED,
		ST_UNLOADED,
		ST_TOTAL
	};
public:
	MTDynLibImpl(const std::string& _name, MTDynLibId _dlid);
	MTDynLibImpl(const MTDynLibImpl& _dl);
	MTDynLibImpl& operator=(const MTDynLibImpl& _dl);
	~MTDynLibImpl();

public:
	bool				load			();
	bool				unload			();
	
	ORZ_PLUS_INLINE 
	const std::string&	getName			(void) const { return m_name; }
	ORZ_PLUS_INLINE
	MTDynLibId			getId			(void) const { return m_id; }
	void*				getSymbol		(const std::string& _strName) const throw();
	std::string			dynLibError		(void);

private:
	std::string			m_name;
	MTDynLibId			m_id;
	/// Handle to the loaded library.
	DYNLIB_HANDLE		m_hInst;
	Status				m_status;				
};
}
#endif
