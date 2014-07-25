#ifndef __Orz_ToolkitPlus_IQueryResultHdr_hpp_
#define __Orz_ToolkitPlus_IQueryResultHdr_hpp_
#include <orz/Toolkit_Plus/Database/QRWrap.hpp>
namespace Orz
{
//! query需要用到的参数接口类
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
	//! 取得io_service，此参数用于Database系统在内部将作为一个functor的此对象放入用户线程执行
	ORZ_PLUS_INLINE
	boost::asio::io_service*	getIO			()
	{
		return m_io;
	}

	//! 处理SQl结果的方法
	/*!
	 *	\param[in] _qid 这次query的id
	 *	\param[in] _qrws SQL执行的结果
	 */
	virtual void				handler			(AynQId _qid, std::vector<QRWrap> _qrws)
	{
	}

private:
	boost::asio::io_service*	m_io;
};
}
#endif
