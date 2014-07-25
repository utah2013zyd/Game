#ifndef __Orz_OrzAsio_Socket_h__
#define __Orz_OrzAsio_Socket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
//! RecvBuffer
/*!
 *	\note 不建议用户使用，这个结构供系统内部使用
 */
struct RecvBuffer
{
	RecvBuffer() : max(RECV_ONCE_SIZE), recvedSize(0) {}

	char buffer[RECV_ONCE_SIZE];
	const uint32 max;
	uint32 recvedSize;
};
}
#endif
