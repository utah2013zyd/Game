#include <OrzMySQLTest.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/Toolkit_Plus/DynLib/MTDynLibMgr.h>
#include <orz/Toolkit_Plus/DynLib/MTDynLib.h>
#include <AppContext.h>
using namespace Orz;


#define INPUT_MAX_LEN 1024

void printMsg(const char* _msg)
{
	//printf("%s\n", _msg);
	sLogger->out(OL_INFO, _msg, 0);
}

IDatabase* loadDB(MTDynLibShrPtr _myDB)
{
	try
	{
		if (!_myDB)
		{
			printMsg("loadDB - !myDB");
			return 0;
		}

		DLL_START_DB_PLUGIN dllStartDBFunc = (DLL_START_DB_PLUGIN)_myDB->getSymbol("dllStartDBPlugin");
		if (!dllStartDBFunc)
		{
			printMsg("loadDB - !dllStartDBFunc");
			return 0;
		}

		IDatabase* orzMysql = dllStartDBFunc();
		if (!orzMysql)
		{
			printMsg("loadDB - !orzMysql");
			return 0;
		}

		printMsg("loadDB - Load OrzMySQL success!");
		return orzMysql;
	}
	catch(OrzAsioException& _e)
	{
		std::vector<const char*> errs = _e.what();
		for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
		{
			sLogger->outFormat(OL_ERROR, false, *itr);
		}
	}
	return 0;
}

void unloadDB(IDatabase* _db, MTDynLibShrPtr _myDB)
{
	try
	{
		MTDynLibMgr* dynLibMgr = sAsioMgr->getDynLibMgr();
		if (!dynLibMgr)
		{
			printMsg("unloadDB - !dynLibMgr");
			return;
		}

		DLL_STOP_DB_PLUGIN dllStopDBFunc = (DLL_STOP_DB_PLUGIN)_myDB->getSymbol("dllStopDBPlugin");
		if (!dllStopDBFunc)
		{
			printMsg("unloadDB - !dllStopDBFunc");
			return;
		}

		_db->shutdown();
		_db->join();

		dllStopDBFunc(_db);
		dynLibMgr->unload(_myDB);

		printMsg("unloadDB - Unload OrzMySQL success!");
		sLogger->shutdown();
		sLogger->join();
	}
	catch(OrzAsioException& _e)
	{
		std::vector<const char*> errs = _e.what();
		for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
		{
			sLogger->outFormat(OL_ERROR, false, *itr);
		}
	}
}

int main(int argc, char* argv[])
{
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (argc < 8)
	{
		std::cerr << "Usage: exe  <DB lib name, i.e. OrzMySQL> <mysql ip> <port>" 
			<< " <username> <passwd> <db name> <log4cxx config file, i.e. log4cxx.properties>\n";
		return 1;
	}

	try
	{
		sLogIns.set(new OrzAsioLogger(argv[7]));
		sLogger->boost();
		sAsioIns.set(new AsioManager(10,10));
		printMsg("main - will start");

		MTDynLibMgr* dynLibMgr = sAsioMgr->getDynLibMgr();
		if (!dynLibMgr)
		{
			printMsg("main - !dynLibMgr");
			return 1;
		}

		MTDynLibShrPtr myDB = dynLibMgr->load(argv[1]);

		IDatabase* orzMysql = loadDB(myDB);
		if (!orzMysql)
		{
			printMsg("main - !orzMysql");
			dynLibMgr->unload(myDB);
			return 1;
		}

		if (!orzMysql->initialize((DBId)1, Params<std::string>(&std::string(argv[2]), &std::string(argv[3]), &std::string(argv[4]), 
															   &std::string(argv[5]), &std::string(argv[6]), &std::string("5"), 
															   &std::string("10"), &std::string("100") )))
		{
			printMsg("main - orzMysql->initialize failed");
			unloadDB(orzMysql, myDB);
			return 1;
		}

		if (!orzMysql->boost())
		{
			printMsg("main - orzMysql->boost failed");
			unloadDB(orzMysql, myDB);
			return 1;
		}

		AppContext app(orzMysql);
		app.boost();
		
		char line[INPUT_MAX_LEN + 1];
		while (std::cin.getline(line, INPUT_MAX_LEN + 1))
		{
			if (!app.parseCmd(line, (uint32)std::cin.gcount()-1))
			{
				break;
			}
		}

		unloadDB(orzMysql, myDB);
		app.shutdown();
		app.join();
	}
	catch(OrzAsioException& _e)
	{
		std::vector<const char*> errs = _e.what();
		for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
		{
			printMsg(*itr);
		}
	}

	return 0;
}

