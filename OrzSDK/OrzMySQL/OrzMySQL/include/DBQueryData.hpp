#ifndef __Orz_OrzMySQL_DBQueryData_hpp__
#define __Orz_OrzMySQL_DBQueryData_hpp__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/IDatabase.h>
namespace Orz
{
namespace Mysql
{
typedef std::string DBQueryStr;
//! ����query���ݣ��û��������
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
