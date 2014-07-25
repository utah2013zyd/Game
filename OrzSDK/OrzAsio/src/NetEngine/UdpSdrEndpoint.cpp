#include <orz/OrzAsio/NetEngine/UdpSdrEndpoint.h>
using namespace Orz;

UdpSdrEndpoint::UdpSdrEndpoint()
{
}

UdpSdrEndpoint::~UdpSdrEndpoint()
{
}

bool UdpSdrEndpoint::operator==(const UdpSdrEndpoint& _other) const
{
	return m_senderEp == _other.m_senderEp;
}

Number UdpSdrEndpoint::operator%(uint32 _base) const
{
	return (Number)(m_senderEp.port() % _base);
}

bool UdpSdrEndpointLess::operator()(const UdpSdrEndpoint& _usep1, const UdpSdrEndpoint& _usep2) const
{
	return _usep1.m_senderEp < _usep2.m_senderEp;
}
