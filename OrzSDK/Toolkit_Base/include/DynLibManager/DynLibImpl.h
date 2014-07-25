#ifndef __Orz_ToolkitBase_DynLibImpl_h__
#define __Orz_ToolkitBase_DynLibImpl_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/DynLibManager/DynLib.h>

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#    define DYNLIB_HANDLE hInstance
#    define DYNLIB_LOAD( a ) LoadLibraryEx( a, NULL, LOAD_WITH_ALTERED_SEARCH_PATH )
#    define DYNLIB_GETSYM( a, b ) GetProcAddress( a, b )
#    define DYNLIB_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX
#    define DYNLIB_HANDLE void*
#    define DYNLIB_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#    define DYNLIB_GETSYM( a, b ) dlsym( a, b )
#    define DYNLIB_UNLOAD( a ) dlclose( a )
#endif

namespace Orz {

   
	class _OrzToolkitBasePrivate DynLibImpl
    {

    public:
      
		DynLibImpl( const std::string & name );
		DynLibImpl(const DynLibImpl & dl);
		DynLibImpl& operator = (const DynLibImpl& dl);

        ~DynLibImpl();

        void load();
        
        void unload();
		
		inline const std::string & getName(void) const { return _name; }

    
        void* getSymbol( const std::string& strName ) const throw();

	private:

		 std::string dynlibError(void);


		std::string _name;
        /// Handle to the loaded library.
        DYNLIB_HANDLE _hInst;
    };

}

#endif
