#ifndef __Orz_EchoServer_EchoServerPreDeclare_H_
#define __Orz_EchoServer_EchoServerPreDeclare_H_
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
	class EchoClient;
	typedef boost::shared_ptr<EchoClient> EchoClientShrPtr;

	template <typename T> class FreeList;
	typedef boost::shared_ptr<FreeList<EchoClient> > EchoClientFreeListShrPtr;
}
#endif
