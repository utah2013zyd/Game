#ifndef __Orz_OrzAsio_IServiceManager_h__
#define __Orz_OrzAsio_IServiceManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! IServiceManager
/*!
 *	\note �������û�ʹ�ã�����๩ϵͳ�ڲ�ʹ��
 */
class IServiceManager
{
public:
	IServiceManager() {}
	virtual ~IServiceManager() {}

public:
	virtual void			run					() = 0;
	virtual void			shutdown			() = 0;
};
}
#endif
