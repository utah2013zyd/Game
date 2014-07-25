#ifndef __Orz_ToolkitPlus_DynLibManager_h__
#define __Orz_ToolkitPlus_DynLibManager_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class MTDynLibMgrImpl;
//! 动态库管理器
/*!
 *	\note 封装了windows和linux平台的动态库（dll/so）
 */
class _OrzToolkitPlusExport MTDynLibMgr
{
public:
	//! 构造函数
	/*!
	 *	\param[in] _dynLibsHash 存储动态库的HashMap的hash大小，一般1~10（1就是1个桶，10就是10个桶，视系统负载而定）
	 */
	explicit MTDynLibMgr(uint32 _dynLibsHash);

	//! 析构函数
    ~MTDynLibMgr();

public:
	//! 加载指定名称的动态库
	/*!
	 *	\param[in] _filename 动态库的名字
	 *	\return 构建好的动态库对象，如果加载失败，则返回的指针为空
	 *	\note _filename在windows平台下要去掉.dll后缀，而在Linux平台下要包含全称，例如windows下“MyDLL.dll”要变成MyDLL，而Linux下要变成MyDLL.so
	 */
	MTDynLibShrPtr		load			(const std::string& _filename);

	//! 卸载动态库
	/*!
	 *	\param[in] _lib 动态库对象
	 */
	void				unload			(MTDynLibShrPtr _lib);

private:
	MTDynLibMgrImpl*	m_impl;
};
}
#endif
