#ifndef __Orz_OrzMySQL_DBQueryData_hpp__
#define __Orz_OrzMySQL_DBQueryData_hpp__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/IDatabase.h>
namespace Orz
{
namespace Mysql
{
typedef std::string DBQueryStr;
//! 传递query数据，用户无需关心
struct DBQueryData
{
	DBQueryData() {}

	void clear()
	{
		dbQstr.clear();
	}

	DBQueryStr				dbQstr;
};
}
}
#endif
