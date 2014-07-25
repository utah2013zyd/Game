#ifndef __Orz_ToolkitPlus_PluginsManager_h__
#define __Orz_ToolkitPlus_PluginsManager_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Plugin/PluginWrap.hpp>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
namespace Orz
{
class MTPluginMgrImpl;
//! Orz插件管理器
class _OrzToolkitPlusExport MTPluginMgr
{
public:
	//! 构造函数
	/*!
	 *	\param[in] _plgsHash 存储插件对象的HashMap的hash大小，一般1~10（1就是1个桶，10就是10个桶，视系统负载而定）
	 *	\param[in] _dynLibMgr 插件管理器，用于管理动态库/对象形式的Orz插件
	 */
	MTPluginMgr(uint32 _plgsHash, MTDynLibMgr* _dynLibMgr);

	//! 析构函数
	~MTPluginMgr();

public:
	//! 加载动态库形式的Orz插件
	/*!
	 *	\param[in] _pluginName 插件名字，也就是动态库名称，见MTDynLibMgr
	 *	\param[in] _params 要传递的参数，使用方法见orz/Toolkit_Plus/Toolkit/Params.hpp
	 *	\return 返回创建好的插件对象的包装对象，内包含Orz插件对象，见PluginWrap
	 *	\note 返回的插件对象的包装对象主要用于让用户使用addPlugin来添加新加载的插件
	 */
	PluginWrap		loadPlugin			(const char* _pluginName, const Params<std::string>& _params);

	//! 卸载动态库形式的Orz插件
	/*!
	 *	\param[in] _plgWrap 要卸载的插件对象的包装对象
	 *	\return 返回被卸载的Orz插件Id
	 *	\note 返回的Id用于让用户使用removePlugin来移除这个被卸载的插件
	 */
	bool			unloadPlugin		(PluginWrap _plgWrap);

	//! 添加Orz插件
	/*!
	 *	\param[in] _plgWrap 插件对象的包装对象
	 *	\return 返回添加的Orz插件的Id
	 *	\note 使用loadPlugin加载成功后，还需要用addPlugin来加入系统
	 *	\note 用户也可以自行创建插件（不使用loadPlugin），然后用addPlugin来加入系统，这样这个插件系统并不是只能管理动态库形式的Orz插件
	 *	\note 如果用户自己创建PluginWrap，则请使用PluginWrap(IOrzPlugin* _plg)这个构造函数，必须使另一个参数为空，见PluginWrap
	 */
	OrzPluginId		addPlugin			(PluginWrap _plgWrap);

	//! 移除Orz插件
	/*!
	 *	\param[in] _id Orz插件的Id
	 *	\return 返回被卸载的插件对象的包装对象
	 *	\note 返回的插件对象的包装对象主要用于让用户使用unloadPlugin来卸载这个插件，当然前提是这个插件是动态库形式的
	 */
	PluginWrap		removePlugin		(OrzPluginId _id);

	//! 启动插件管理器
	bool			boost				();

	//! 启动指定Id的插件
	bool			boostPlugin			(OrzPluginId _plgId);

	//! 关闭指定Id的插件
	void			shutdownPlugin		(OrzPluginId _plgId);

	//! 合并指定Id的插件内部线程
	void			joinPlugin			(OrzPluginId _plgId);

	//! 关闭插件管理器
	void			shutdown			();

	//! 合并所有插件内部线程
	void			join				();

private:
	MTPluginMgrImpl*						m_impl;
};
}
#endif
