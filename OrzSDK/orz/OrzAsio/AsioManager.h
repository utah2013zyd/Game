#ifndef __Orz_OrzAsio_AsioManager_h__
#define __Orz_OrzAsio_AsioManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class AsioManagerImpl;
//! OrzAsio的管理器类
class _OrzOrzAsioExport AsioManager
{
public:
	//! 默认构造函数
	AsioManager(uint32 _dynLibMgrHash, uint32 _pluginsMgrHash);

	//! 析构函数
	~AsioManager();

	//! 初始化方法
	/*!
	 *	\param[in] _svcMgr 要使用的ServiceManager对象
	 *	\return true成功 false失败
	 */
	bool				initialize			(IServiceManager* _svcMgr);

	//! 启动OrzAsio
	/*!
	 *	\param[in] _isBlock true则阻塞进行；false则非阻塞
	 *	\return true成功 false失败
	 */
	bool				boost				(bool _isBlock=false);

	//! 取得IServiceManager
	/*!
	 *	\note 请使用宏#define sSvcMgr(Impl) reinterpret_cast<Impl*>(AsioSingleton::instance().get()->getSvcMgr())
	 */
	IServiceManager*	getSvcMgr			();

	//! 取得MTDynLibMgr
	/*!
	 *	\note 请使用宏#define sDynLibMgr AsioSingleton::instance().get()->getDynLibMgr()
	 */
	MTDynLibMgr*		getDynLibMgr		();

	//! 取得MTPluginMgr
	/*!
	 *	\note 请使用宏#define sPluginsMgr AsioSingleton::instance().get()->getPluginsMgr()
	 */
	MTPluginMgr*		getPluginsMgr		();

	//! 加入主线程 详见OrzAsio的Examples
	void				join				();

	//! 关闭OrzAsio
	void				shutdown			();

private:
	AsioManagerImpl*						m_impl;
};
}
#endif

