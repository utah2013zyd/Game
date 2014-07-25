#ifndef __Orz_OrzAsio_AsioManager_h__
#define __Orz_OrzAsio_AsioManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class AsioManagerImpl;
//! OrzAsio�Ĺ�������
class _OrzOrzAsioExport AsioManager
{
public:
	//! Ĭ�Ϲ��캯��
	AsioManager(uint32 _dynLibMgrHash, uint32 _pluginsMgrHash);

	//! ��������
	~AsioManager();

	//! ��ʼ������
	/*!
	 *	\param[in] _svcMgr Ҫʹ�õ�ServiceManager����
	 *	\return true�ɹ� falseʧ��
	 */
	bool				initialize			(IServiceManager* _svcMgr);

	//! ����OrzAsio
	/*!
	 *	\param[in] _isBlock true���������У�false�������
	 *	\return true�ɹ� falseʧ��
	 */
	bool				boost				(bool _isBlock=false);

	//! ȡ��IServiceManager
	/*!
	 *	\note ��ʹ�ú�#define sSvcMgr(Impl) reinterpret_cast<Impl*>(AsioSingleton::instance().get()->getSvcMgr())
	 */
	IServiceManager*	getSvcMgr			();

	//! ȡ��MTDynLibMgr
	/*!
	 *	\note ��ʹ�ú�#define sDynLibMgr AsioSingleton::instance().get()->getDynLibMgr()
	 */
	MTDynLibMgr*		getDynLibMgr		();

	//! ȡ��MTPluginMgr
	/*!
	 *	\note ��ʹ�ú�#define sPluginsMgr AsioSingleton::instance().get()->getPluginsMgr()
	 */
	MTPluginMgr*		getPluginsMgr		();

	//! �������߳� ���OrzAsio��Examples
	void				join				();

	//! �ر�OrzAsio
	void				shutdown			();

private:
	AsioManagerImpl*						m_impl;
};
}
#endif

