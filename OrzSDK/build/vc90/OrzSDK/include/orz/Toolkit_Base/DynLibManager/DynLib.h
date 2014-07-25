
#ifndef __Orz_ToolkitBase_DynLib_h__
#define __Orz_ToolkitBase_DynLib_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>



namespace Orz {

class DynLibImpl;
template class _OrzToolkitBaseExport boost::scoped_ptr<DynLibImpl>;
/** Resource holding data about a dynamic library.
    @remarks
        This class holds the data required to get symbols from
        libraries loaded at run-time (i.e. from DLL's for so's)
    @author
        Adrian Cearnãu (cearny@cearny.ro)
    @since
        27 January 2002
    @see
        Resource
*/
class _OrzToolkitBaseExport DynLib:public boost::noncopyable
{
public:


	/** Load the library
    */
    void load(void);
    /** Unload the library
    */
    void unload(void);

    /** Default constructor - used by DynLibManager.
        @warning
            Do not call directly
    */
   DynLib( const std::string & name );

	//DynLib(const DynLib & dl);
	//DynLib& operator = (const DynLib& dl);
	//void swap(DynLib& dl) throw();

    /** Default destructor.
    */
    ~DynLib(void);


	/// Get the name of the library
	const std::string & getName(void) const;

    /**
        Returns the address of the given symbol from the loaded library.
        @param
            strName The name of the symbol to search for
        @returns
            If the function succeeds, the returned value is a handle to
            the symbol.
        @par
            If the function fails, the returned value is <b>NULL</b>.

    */
    void* getSymbol( const std::string & strName ) const throw();

private:

	
    
   boost::scoped_ptr<DynLibImpl> _impl;
};

}

#endif
