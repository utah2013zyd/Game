#ifndef __Orz_OrzAsio_Socket_h__
#define __Orz_OrzAsio_Socket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
//! RecvBuffer
/*!
 *	\note �������û�ʹ�ã�����ṹ��ϵͳ�ڲ�ʹ��
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
