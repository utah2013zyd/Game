#ifndef __Orz_ToolkitPlus_IPostQueryHdr_hpp_
#define __Orz_ToolkitPlus_IPostQueryHdr_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 在post模式下用到的参数接口类
class IPostQueryHdr
{
public:
	ORZ_PLUS_INLINE
	IPostQueryHdr(boost::asio::io_service* _io)
		: m_io(_io)
	{
	}

	ORZ_PLUS_INLINE
	virtual ~IPostQueryHdr() 
	{
	}

public:
	//! 取得io_service
	ORZ_PLUS_INLINE
	boost::asio::io_service*	getIO			()
	{
		return m_io;
	}

	//! 处理postSQL的方法
	/*!
	 *	\param[in] _cnid 运行SQL的连接id
	 */
	ORZ_PLUS_INLINE
	virtual void				handler			(DBCnId _cnid)
	{
	}

private:
	boost::asio::io_service*	m_io;
};
}
#endif
