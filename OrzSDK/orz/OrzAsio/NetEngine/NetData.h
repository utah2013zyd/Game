#ifndef __Orz_OrzAsio_NetData_h__
#define __Orz_OrzAsio_NetData_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
namespace Orz
{
//! NetData
/*!
 *	\note 不建议用户使用，这个结构供系统内部使用
 */
struct NetData
{
	ORZ_PLUS_INLINE
	NetData()
	{
		clear();
	}
	ORZ_PLUS_INLINE
	NetData(NetPort _port, IpString _ip, IConnection* _connection, void* _tag=0)
		: port(_port)
		, ip(_ip)
		, connection(_connection)
		, tag(_tag)
	{
	}

	ORZ_PLUS_INLINE void clear()
	{
		port = 0;
		ip.clear();
		connection = 0;
		tag = 0;
	}

	ORZ_PLUS_INLINE bool isValid() const
	{
		if (!connection)
			return false;
		else
			return true;
	}

	NetPort						port;
	IpString					ip;
	IConnection*				connection;
	void*						tag; 
};
}
#endif
