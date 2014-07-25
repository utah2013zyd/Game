#ifndef __Orz_OrzAsio_UdpSdrEndpoint_h__
#define __Orz_OrzAsio_UdpSdrEndpoint_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/NumDef.hpp>
namespace Orz
{
class _OrzOrzAsioExport UdpSdrEndpoint
{
public:
	UdpSdrEndpoint();
	~UdpSdrEndpoint();

public:
	bool operator==(const UdpSdrEndpoint& _other) const;
	Number operator%(uint32 _base) const;

	boost::asio::ip::udp::endpoint		m_senderEp;
};

class _OrzOrzAsioExport UdpSdrEndpointLess
{
public:
	bool operator() (const UdpSdrEndpoint& _usep1, const UdpSdrEndpoint& _usep2) const;
};
}
#endif
