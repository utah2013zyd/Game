#ifndef __Orz_OrzAsio_AsioSingleton_h__
#define __Orz_OrzAsio_AsioSingleton_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/AsioManager.h>
namespace Orz
{
//! OrzAsio的管理器单件包装类
/*!
 *	\note 单件
 example:
 \code
	AsioManager* orzAsioMgr = new AsioManager;
	sAsioIns.set(orzAsioMgr);
	...
	sAsioMgr->start(true);
	...
	sAsioMgr->shutdown();
 \endcode
 */
class _OrzOrzAsioExport AsioSingleton
{
	AsioSingleton();
	AsioSingleton(const AsioSingleton& _other);
	AsioSingleton& operator=(const AsioSingleton& _other);
public:
	~AsioSingleton();

public:
	static AsioSingleton& instance();

public:
	void				set					(AsioManager* _asioMgr);
	AsioManager*		get					();
	void				clear				();

private:
	AsioManager*		m_asioMgr;
};
}
#define sAsioIns Orz::AsioSingleton::instance()
#define sAsioMgr Orz::AsioSingleton::instance().get()
#define sSvcMgr(Impl) reinterpret_cast<Impl*>(Orz::AsioSingleton::instance().get()->getSvcMgr())
#define sDynLibMgr Orz::AsioSingleton::instance().get()->getDynLibMgr()
#define sPluginsMgr Orz::AsioSingleton::instance().get()->getPluginsMgr()
#endif
