#ifndef __Orz_OrzMySQLTest_OrzMySQLTest_h__
#define __Orz_OrzMySQLTest_OrzMySQLTest_h__
#include <orz/Toolkit_Plus/Database/IDatabase.h>

typedef Orz::IDatabase* (*DLL_START_DB_PLUGIN)(void);
typedef void (*DLL_STOP_DB_PLUGIN)(Orz::IDatabase* _db);

#endif
