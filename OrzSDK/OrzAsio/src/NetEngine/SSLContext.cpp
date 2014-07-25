#include <orz/OrzAsio/NetEngine/SSLContext.h>
#include <orz/OrzAsio/NetEngine/SSLService.h>
using namespace Orz;

SSLContext::SSLContext(boost::asio::io_service& _io, boost::asio::ssl::context_base::method _m, const char* _passwd)
	: m_context(_io, _m)
	, m_passwd(_passwd)
{
}

//SSLContext(boost::asio::io_service& _io, boost::asio::ssl::context_base::method _m, const std::string& _passwd)
//	: m_context(_io, _m)
//	, m_passwd(_passwd)
//{
//
//}

SSLContext::~SSLContext()
{
}

boost::asio::ssl::context& SSLContext::getContext()
{
	return m_context;
}

std::string SSLContext::getPassword() const
{
	return m_passwd;
}
