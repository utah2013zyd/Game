#ifndef __Orz_ToolkitPlus_LogFreeList_h_
#define __Orz_ToolkitPlus_LogFreeList_h_
#include <orz/Toolkit_Plus/Log/LogEnv.h>
namespace Orz
{
class _OrzToolkitPlusExport LogDeleter
{
public:
	LogDeleter();
	~LogDeleter();

	void operator()(Log* _ptr);
};

class _OrzToolkitPlusExport LogFreeList
{
	LogFreeList();
	LogFreeList(const LogFreeList& _other);
	LogFreeList& operator=(const LogFreeList& _other);
public:
	~LogFreeList();

public:
	static LogFreeList& instance();

public:
	void				set				(IFreeListShrPtr _freeList);
	Log*				alloc			();
	LogShrPtr			select			(OutLevel _ol, bool _usingDoubleBuff);
	void				free			(Log* _ptr);
	void				clear			();

private:
	IFreeListShrPtr			m_freeList;
};
}

#define sLogFL Orz::LogFreeList::instance()
#define allocLog(ol, udb) Orz::LogFreeList::instance().select((ol), (udb))

#endif
