#ifndef __Orz_ToolkitPlus_IQueryResultHdr_hpp_
#define __Orz_ToolkitPlus_IQueryResultHdr_hpp_
#include <orz/Toolkit_Plus/Database/QRWrap.hpp>
namespace Orz
{
//! query��Ҫ�õ��Ĳ����ӿ���
class IQueryResultHdr
{
public:
	ORZ_PLUS_INLINE
	IQueryResultHdr(boost::asio::io_service* _io)
		: m_io(_io)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~IQueryResultHdr()
	{
	}

public:
	//! ȡ��io_service���˲�������Databaseϵͳ���ڲ�����Ϊһ��functor�Ĵ˶�������û��߳�ִ��
	ORZ_PLUS_INLINE
	boost::asio::io_service*	getIO			()
	{
		return m_io;
	}

	//! ����SQl����ķ���
	/*!
	 *	\param[in] _qid ���query��id
	 *	\param[in] _qrws SQLִ�еĽ��
	 */
	virtual void				handler			(AynQId _qid, std::vector<QRWrap> _qrws)
	{
	}

private:
	boost::asio::io_service*	m_io;
};
}
#endif
