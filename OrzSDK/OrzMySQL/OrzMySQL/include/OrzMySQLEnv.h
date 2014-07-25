#ifndef __Orz_OrzMySQL_OrzMySQLEnv_h__
#define __Orz_OrzMySQL_OrzMySQLEnv_h__
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
namespace Orz
{
namespace Mysql
{
#define MYSQL_HOSTNAME_MAX_LEN		128
#define MYSQL_USERNAME_MAX_LEN		128
#define MYSQL_PASSWORD_MAX_LEN		128
#define MYSQL_NAME_MAX_LEN			128

typedef MemString<MYSQL_HOSTNAME_MAX_LEN+1>	DBHostName;
typedef MemString<MYSQL_USERNAME_MAX_LEN+1>	DBUserName;
typedef MemString<MYSQL_PASSWORD_MAX_LEN+1>	DBPassword;
typedef MemString<MYSQL_NAME_MAX_LEN+1>		DBName;
}
}
#endif
