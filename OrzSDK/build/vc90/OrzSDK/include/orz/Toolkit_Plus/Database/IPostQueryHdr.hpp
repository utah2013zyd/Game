#ifndef __Orz_ToolkitPlus_IPostQueryHdr_hpp_
#define __Orz_ToolkitPlus_IPostQueryHdr_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! ��postģʽ���õ��Ĳ����ӿ���
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
	//! ȡ��io_service
	ORZ_PLUS_INLINE
	boost::asio::io_service*	getIO			()
	{
		return m_io;
	}

	//! ����postSQL�ķ���
	/*!
	 *	\param[in] _cnid ����SQL������id
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
