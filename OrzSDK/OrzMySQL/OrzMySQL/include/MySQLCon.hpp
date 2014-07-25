#ifndef __Orz_OrzMySQL_MySQLCon_hpp__
#define __Orz_OrzMySQL_MySQLCon_hpp__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Agent/Agent.h>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
#include <orz/Toolkit_Plus/Database/IQueryResult.h>
#include <MySQL/mysql.h>
namespace Orz
{
namespace Mysql
{
//! 用户无需关心
struct _OrzMySQLPrivate MySQLCon
{
	MySQLCon()
		: con(0)
		, busy(mtx, DynShrLock::LT_DELAY)
		, index(0)
	{
	}

	void clearQuery(IQueryResultShrPtr _qr)
	{
		// do nothing here, just wait _qr being deleted
	}

	MYSQL*						con;
	boost::shared_mutex			mtx;
	DynShrLock					busy;
	Agent						agt;
	DBCnId						index;
};
}
}
#endif
