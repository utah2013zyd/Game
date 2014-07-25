#ifndef __Orz_OrzAsio_SSLContext_h__
#define __Orz_OrzAsio_SSLContext_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! ssl安全连接需要使用的ssl信息类
/*!
 *	\note 详见boost::asio文档
 */
class _OrzOrzAsioExport SSLContext
{
public:
	SSLContext(boost::asio::io_service& _io, boost::asio::ssl::context_base::method _m, const char* _passwd=0);
	~SSLContext();

public:
	boost::asio::ssl::context&	getContext				();
	std::string					getPassword				() const;

private:
	boost::asio::ssl::context	m_context;
	std::string					m_passwd;
};
}
#endif
