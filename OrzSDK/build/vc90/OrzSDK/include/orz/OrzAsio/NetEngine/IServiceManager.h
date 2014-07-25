#ifndef __Orz_OrzAsio_IServiceManager_h__
#define __Orz_OrzAsio_IServiceManager_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! IServiceManager
/*!
 *	\note 不建议用户使用，这个类供系统内部使用
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
