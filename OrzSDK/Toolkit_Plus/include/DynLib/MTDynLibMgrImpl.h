#ifndef __Orz_ToolkitPlus_DynLibManagerImpl_h__
#define __Orz_ToolkitPlus_DynLibManagerImpl_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/HashMap.hpp>
#include <orz/Toolkit_Plus/Toolkit/FastIdr.hpp>
namespace Orz 
{
/** Manager for Dynamic-loading Libraries.
    @remarks
        This manager keeps a track of all the open dynamic-loading
        libraries, opens them and returns references to already-open
        libraries.
*/
class _OrzToolkitPlusPrivate MTDynLibMgrImpl
{
public:
    /** Default constructor.
        @note
            <br>Should never be called as the singleton is automatically
            created during the creation of the Root object.
        @see
            Root::Root
    */
    explicit MTDynLibMgrImpl(uint32 _dynLibsHash);

    /** Default destructor.
        @see
            Root::~Root
    */
    virtual ~MTDynLibMgrImpl();

public:
    /** Loads the passed library.
        @param
            filename The name of the library. The extension can be omitted
    */
	MTDynLibShrPtr		load			(const std::string& _filename);

	/** Unloads the passed library.
	@param
	filename The name of the library. The extension can be omitted
	*/
	void				unload			(MTDynLibShrPtr _lib);

private:
	HashShrMap<MTDynLibId, MTDynLibShrPtr>	m_libList;
	FastIdr								m_dlIdr;
};
}

#endif
