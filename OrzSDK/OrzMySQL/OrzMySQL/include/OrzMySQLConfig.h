/* OrzMySQLConfig.h -- config of the 'OrzMySQL'

  Copyright (C) 2009 熊小磊 (Nous)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Please note that the OrzMySQL is based in part on the work of the
  boost and the mysql. This means that if you use the OrzMySQL in your product, 
  you must acknowledge somewhere in your documentation that 
  you've used those code. See the README files in the boost and the mysql 
  for further informations.
*/

#ifndef __Orz_OrzCORBA_OrzMySQLConfig_h__
#define __Orz_OrzCORBA_OrzMySQLConfig_h__
#ifndef ORZ_PLUS_CONFIG_BIG_ENDIAN
#	define ORZ_PLUS_CONFIG_BIG_ENDIAN
#endif
#include <orz/Toolkit_Plus/GlobalPlus.h>


#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#	if defined( ORZMYSQL_EXPORTS )
#		define _OrzMySQLExport __declspec( dllexport )
#	else
#		if defined( __MINGW32__ )
#			define _OrzMySQLExport
#		else
#			define _OrzMySQLExport __declspec( dllimport )
#		endif
#	endif
#define _OrzMySQLPrivate

#elif ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX 

// Enable GCC symbol visibility
#   if defined( ORZ_PLUS_GCC_VISIBILITY )
#       define _OrzMySQLExport  __attribute__ ((visibility("default")))
#       define _OrzMySQLPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _OrzMySQLExport
#       define _OrzMySQLPrivate
#   endif

#endif

/*! \mainpage OrzMySQL 0.4.0 API documentation
 *
 *	\image html OrzAsioLogo.PNG
 *
 *	Copyright (C) 2008-2009 熊小磊 (Nous)
 *
 *	\section intro 简介
 *
 *	欢迎来到OrzAsio的OrzMySQL辅助库API文档。
 *	在这里，你可以找到关于OrzAsio的OrzMySQL辅助库的一些使用和介绍信息，它可以帮助你更好的使用OrzAsio来开发你的网络程序。
 *	
 *	OrzAsio的目的是为了成为一个高效、可扩展、易用的异步网络库，给用户提供相对高效、灵活和稳定的网络引擎；而它的辅助库OrzMySQL
 *	则是作为数据库访问接口来让用户方便灵活的访问Mysql数据库，OrzMySQL继承自Toolkit_Plus中的Database模块。
 *
 *	OrzMySQL辅助库本身是线程安全的，并且可以自由设定连接到Mysql数据库服务器的网络连接数量以及内部的线程数量，每个连接都是由一个独立
 *	的线程来运行，所以连接越多，OrzMySQL的访问Mysql数据库服务器的吞吐量就越大，请用户自己根据自己服务器的负载来配置。
 *	
 *	更多信息请访问网站：<A HREF="http://ogre3d.cn/" >ogre3d.cn</A>
 *
 *	\section orzmysql 入手点
 *
 *	用户使用OrzMySQL一般只需要接触以下几个类：OrzMySQL、QueryResult、SqlField
 *
 *	\section orzmysqlexample 示例代码
 *
 *	\code
	// main.cpp
	#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
	#include <orz/Toolkit_Plus/Log/LogSingleton.h>
	#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
	#include <orz/OrzAsio/AsioSingleton.h>
	#include <orz/Toolkit_Plus/DynLib/MTDynLibMgr.h>
	#include <orz/Toolkit_Plus/DynLib/MTDynLib.h>
	#include <orz/Toolkit_Plus/Database/IDatabase.h>
	#include <AppContext.h>

	typedef Orz::IDatabase* (*DLL_START_DB_PLUGIN)(void);
	typedef void (*DLL_STOP_DB_PLUGIN)(Orz::IDatabase* _db);

	using namespace Orz::Mysql;

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

			dllStopDBFunc(_db);
			dynLibMgr->unload(_myDB);

			printMsg("unloadDB - Unload OrzMySQL success!");
		}
		catch(OrzAsioException& _e)
		{
		}
	}

	int main(int argc, char* argv[])
	{
	#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

		if (argc < 4 || !argv[1] || !argv[2])
		{
			std::cerr << "Usage: exe  <DB lib name, i.e. OrzMySQL> <log type, i.e. 1> <log4cxx config file, i.e. log4cxx.properties>\n";
			return 1;
		}

		sLogIns.set(new OrzAsioLogger(argv[3]));

		try
		{
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
				dynLibMgr->unload(myDB);
				printMsg("main - !orzMysql");
				return 1;
			}

			if (!orzMysql->initialize((DBId)1, Params<std::string>(&std::string("127.0.0.1"), &std::string("3306"), &std::string("username"), 
																   &std::string("passwd"), &std::string("orz_corba"), &std::string("5"), 
																   &std::string("10"), &std::string("100") )))
			{
				unloadDB(orzMysql, myDB);
				printMsg("main - orzMysql->initialize failed");
				return 1;
			}

			if (!orzMysql->boost())
			{
				unloadDB(orzMysql, myDB);
				printMsg("main - orzMysql->boost failed");
				return 1;
			}

			AppContext app(orzMysql);

			char line[INPUT_MAX_LEN + 1];
			while (std::cin.getline(line, INPUT_MAX_LEN + 1))
			{
				if (!app.parseCmd(line, (uint32)std::cin.gcount()))
				{
					orzMysql->shutdown();
					break;
				}
			}

			unloadDB(orzMysql, myDB);
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
	// end of file main.cpp


	// AppContext.h
	#ifndef __Orz_OrzMySQLTest_AppContext_h__
	#define __Orz_OrzMySQLTest_AppContext_h__
	#include <orz/Toolkit_Plus/Database/IDatabase.h>
	#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
	#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
	namespace Orz
	{
	class AppContext
	{
		struct InsertSet
		{
			InsertSet(StringPtr _fieldName, StringPtr _insertValue)
				: fieldName(_fieldName.ptr(), _fieldName.size())
				, insertValue(_insertValue.ptr(), _insertValue.size())
			{
			}

			std::string		fieldName;
			std::string		insertValue;
		};
	public:
		AppContext(IDatabase* _db);
		~AppContext();

	public:
		bool		parseCmd		(const char* _cmd, uint32 _size);

	private:
		void		insert			(const char* _tableName, const std::vector<InsertSet>& _inserts);
		void		select			(const char* _tableName);

	private:
		IDatabase*					m_db;
	};
	}
	#endif
	// end of file AppContext.h


	// AppContext.cpp
	#include <AppContext.h>
	#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
	#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
	#include <orz/Toolkit_Plus/Database/ISqlField.h>
	#include <orz/Toolkit_Plus/Log/LogSingleton.h>
	using namespace Orz;

	AppContext::AppContext(IDatabase* _db)
		: m_db(_db)
	{
	}

	AppContext::~AppContext()
	{
	}

	bool AppContext::parseCmd(const char* _cmd, uint32 _size)
	{
		if (!_cmd)
			return true;

		if (strcmp(_cmd, "quit") == 0 || 
			strcmp(_cmd, "bye") == 0 || 
			strcmp(_cmd, "esc") == 0)
		{
			return false;
		}

		std::vector<StringPtr> args;
		separateStr(_cmd, _size, ' ', args);
		if (args.empty())
		{
			return true;
		}

		if (args[0] == "insert")
		{
			if (args.size() < 3)
			{
				return true;
			}

			std::string tableName(args[1].ptr(), args[1].size());
			std::vector<InsertSet> is;
			for (uint32 i=2; i<args.size(); i+=2)
			{
				is.push_back(InsertSet(args[i], args[i+1]));
			}
			insert(tableName.c_str(), is);
			return true;
		}

		if (args[0] == "select")
		{
			if (args.size() < 2)
			{
				return true;
			}

			std::string tableName(args[1].ptr(), args[1].size());
			select(tableName.c_str());
			return true;
		}

		return true;
	}

	void AppContext::insert(const char* _tableName, const std::vector<InsertSet>& _inserts)
	{
		if (!_tableName || _inserts.empty())
		{
			return;
		}

		std::string insertStr;
		uint32 i=0;
		for (std::vector<InsertSet>::const_iterator itr=_inserts.begin(); itr!=_inserts.end(); ++itr, ++i)
		{
			const InsertSet& is = *itr;
			insertStr += is.fieldName;
			insertStr += "='";
			insertStr += is.insertValue;
			insertStr += "'";
			if (i+1 < _inserts.size())
			{
				insertStr += ",";
			}
		}

		QRWrap qrw = m_db->query("INSERT INTO ", 0, _tableName, 0, " SET ", 0, insertStr.c_str(), 0, 0);
		if (!qrw.isOk)
		{
			return;
		}

		qrw = m_db->queryEx(qrw.cnid, "SELECT LAST_INSERT_ID()", 0, 0);
		if (qrw.qr)
		{
			uint32 lastInsertId = qrw.qr->fetch(0)->getUint32();
			sLogger->out(OL_INFO, "Last insert id: ", d2s(lastInsertId).c_str(), 0);
		}
	}

	void AppContext::select(const char* _tableName)
	{
		bool isOk = false;
		QRWrap qrw = m_db->query("SELECT * FROM ", 0, _tableName, 0, 0);

		if (!qrw.isOk || !qrw.qr)
		{
			return;
		}
		else
		{
			uint32 rowCount = 0;
			rowCount = qrw.qr->getRowCount();
			
			sLogger->out(OL_INFO, "Table[", _tableName, "]:", 0);
			for (uint32 i=0; i<rowCount; qrw.qr->nextRow(), ++i)
			{
				std::string print("  Row[");
				print += d2s(i).c_str();
				print += "]: ";

				uint32 fieldCount = qrw.qr->getFieldCount();
				for (uint32 j=0; j<fieldCount; ++j)
				{
					print += qrw.qr->fetch(j)->getString();
					if (j+1 < fieldCount)
					{
						print += " - ";
					}
				}
				sLogger->out(OL_INFO, print.c_str(), 0);
			}
		}
	}
	// end of file AppContext.cpp
 *	\endcode
 *
 *	使用方法：Usage: exe <DB lib name, i.e. OrzMySQL> <log type, i.e. 1> <log4cxx config file, i.e. log4cxx.properties>
 */


class _OrzMySQLExport boost::noncopyable_::noncopyable;
class _OrzMySQLExport boost::detail::shared_count;
class _OrzMySQLExport boost::asio::io_service;
#include <OrzMySQLPreDeclare.h>
#endif
