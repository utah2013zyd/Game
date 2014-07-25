#ifndef __Orz_OrzAsio_SSLContext_h__
#define __Orz_OrzAsio_SSLContext_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! ssl��ȫ������Ҫʹ�õ�ssl��Ϣ��
/*!
 *	\note ���boost::asio�ĵ�
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
