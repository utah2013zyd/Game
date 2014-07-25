#ifndef __Orz_OrzMySQLTest_AppContext_h__
#define __Orz_OrzMySQLTest_AppContext_h__
#include <orz/Toolkit_Plus/Database/IDatabase.h>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
#include <orz/Toolkit_Plus/Plugin/IOrzPlugin.h>
#include <orz/Toolkit_Plus/Database/IPostQueryHdr.hpp>
#include <orz/Toolkit_Plus/Database/IQueryResultHdr.hpp>
#include <orz/Toolkit_Plus/Agent/Agent.h>
namespace Orz
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

/////////////////////////////////////////////////////////////////////////////////
class SelectQRHdr
	: public IQueryResultHdr
{
public:
	SelectQRHdr(boost::asio::io_service* _io, const char* _tableName);
	~SelectQRHdr();

public:
	void						handler			(AynQId _qid, std::vector<QRWrap> _qrws);

private:
	std::string					m_tableName;
};


class InsertQRHdr
	: public IQueryResultHdr
{
public:
	InsertQRHdr(boost::asio::io_service* _io);
	~InsertQRHdr();

public:
	void						handler			(AynQId _qid, std::vector<QRWrap> _qrws);

};
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
class SelectPQHdr
	: public IPostQueryHdr
	, public boost::enable_shared_from_this<SelectPQHdr>
{
public:
	SelectPQHdr(IDatabase* _db, boost::asio::io_service* _io, const char* _tableName);
	~SelectPQHdr();

public:
	void						handler			(DBCnId _cnid);

private:
	void						callback		(QRWrap _qrw);

private:
	IDatabase*					m_db;
	std::string					m_tableName;
};

class InsertPQHdr
	: public IPostQueryHdr
	, public boost::enable_shared_from_this<InsertPQHdr>
{
public:
	InsertPQHdr(IDatabase* _db, boost::asio::io_service* _io, 
		const char* _tableName, const std::vector<InsertSet>& _inserts);
	~InsertPQHdr();

public:
	void						handler			(DBCnId _cnid);

private:
	void						callback		(DBCnId _cnid, bool _isOk, uint32 _lastInsertId);
private:
	IDatabase*					m_db;
	std::string					m_tableName;
	std::vector<InsertSet>		m_inserts;
};
/////////////////////////////////////////////////////////////////////////////////


class AppContext
	: public IOrzPlugin
{
	struct PostInsertData
	{
		PostInsertData(const std::string& _tableName, const std::vector<InsertSet>& _insertSets)
			: tableName(_tableName)
			, insertSets(_insertSets)
		{
		}

		std::string				tableName;
		std::vector<InsertSet>	insertSets;
	};

	enum Mode
	{
		SYNC = 0,
		ASYNC,
		POST,
		TOTAL
	};
public:
	AppContext(IDatabase* _db);
	~AppContext();

public:
	bool		boost			();
	void		shutdown		();
	void		join			();

	bool		parseCmd		(const char* _cmd, uint32 _size);

private:
	void		parseCmdHdr		(Mode _m, std::string _cmd);

	void		insert			(const char* _tableName, const std::vector<InsertSet>& _inserts);
	void		asyncInsert		(const char* _tableName, const std::vector<InsertSet>& _inserts);
	void		postInsert		(const char* _tableName, const std::vector<InsertSet>& _inserts);

	void		select			(const char* _tableName);
	void		asyncSelect		(const char* _tableName);
	void		postSelect		(const char* _tableName);

private:
	IDatabase*					m_db;
	Mode						m_mode;
	Agent						m_SQLThread;
};
}
#endif
