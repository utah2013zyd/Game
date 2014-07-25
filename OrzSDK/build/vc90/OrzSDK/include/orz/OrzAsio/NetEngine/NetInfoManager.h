#ifndef __Orz_OrzAsio_NetInfoManager_h__
#define __Orz_OrzAsio_NetInfoManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class NetInfoManagerImpl;
//! NetInfoManager
/*!
 *	\note �������û�ʹ�ã�����๩ϵͳ�ڲ�ʹ��
 */
class _OrzOrzAsioExport NetInfoManager
{
public:
	NetInfoManager(uint32 _cBase);
	~NetInfoManager();

public:
	uint32				getSocketNum				() const;
	uint32				getInConnectionNum			() const;
	uint32				getOutConnectionNum			() const;
	uint32				getInConnectionNum			(NetPort _port) const;
	uint32				getOutConnectionNum			(NetPort _port) const;

	boost::shared_ptr<NetInfoManagerImpl>
						impl						();

private:
	boost::shared_ptr<NetInfoManagerImpl>			m_impl;
};
}
#endif
