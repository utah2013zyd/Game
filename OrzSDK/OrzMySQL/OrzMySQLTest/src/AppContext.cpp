#include <AppContext.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
#include <orz/Toolkit_Plus/Database/ISqlField.h>
#include <orz/Toolkit_Plus/Database/QueryArg.hpp>
#include <orz/Toolkit_Plus/Database/Query.hpp>
#include <orz/Toolkit_Plus/Database/QueryFreeList.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
using namespace Orz;


/////////////////////////////////////////////////////////////////////////////////
SelectQRHdr::SelectQRHdr(boost::asio::io_service* _io, const char* _tableName)
	: IQueryResultHdr(_io)
	, m_tableName(_tableName)
{
}

SelectQRHdr::~SelectQRHdr()
{
}

void SelectQRHdr::handler(AynQId _qid, std::vector<QRWrap> _qrws)
{
	for (std::vector<QRWrap>::iterator itr=_qrws.begin(); itr!=_qrws.end(); ++itr)
	{
		QRWrap& qrw = *itr;
		if (qrw.isOk)
		{
			if (qrw.qr)
			{
				uint32 rowCount = 0;
				rowCount = qrw.qr->getRowCount();
				
				sLogger->out(OL_INFO, "Table[", m_tableName.c_str(), "]:", 0);
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
	}
}


InsertQRHdr::InsertQRHdr(boost::asio::io_service* _io)
	: IQueryResultHdr(_io)
{
}

InsertQRHdr::~InsertQRHdr()
{
}

void InsertQRHdr::handler(AynQId _qid, std::vector<QRWrap> _qrws)
{
	if (_qrws.size() == 2)
	{
		if (_qrws[0].isOk)
		{
			if (_qrws[1].qr)
			{
				uint32 lastInsertId = _qrws[1].qr->fetch(0)->getUint32();
				sLogger->out(OL_INFO, "Last insert id: ", d2s(lastInsertId).c_str(), 0);
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
SelectPQHdr::SelectPQHdr(IDatabase* _db, boost::asio::io_service* _io, const char* _tableName)
	: IPostQueryHdr(_io)
	, m_db(_db)
	, m_tableName(_tableName)
{
}

SelectPQHdr::~SelectPQHdr()
{
}

void SelectPQHdr::handler(DBCnId _cnid)
{
	Query q;
	//q << "SELECT * FROM " << m_tableName;
	q.put("SELECT * FROM %s", m_tableName.c_str());

	QRWrap qrw = m_db->query(QueryArg(_cnid), q);
	IPostQueryHdr::getIO()->post(boost::bind(&SelectPQHdr::callback, shared_from_this(), qrw));
}

void SelectPQHdr::callback(QRWrap _qrw)
{
	if (_qrw.isOk)
	{
		if (_qrw.qr)
		{
			uint32 rowCount = 0;
			rowCount = _qrw.qr->getRowCount();
			
			sLogger->out(OL_INFO, "Table[", m_tableName.c_str(), "]:", 0);
			for (uint32 i=0; i<rowCount; _qrw.qr->nextRow(), ++i)
			{
				std::string print("  Row[");
				print += d2s(i).c_str();
				print += "]: ";

				uint32 fieldCount = _qrw.qr->getFieldCount();
				for (uint32 j=0; j<fieldCount; ++j)
				{
					print += _qrw.qr->fetch(j)->getString();
					if (j+1 < fieldCount)
					{
						print += " - ";
					}
				}
				sLogger->out(OL_INFO, print.c_str(), 0);
			}
		}
	}
}


InsertPQHdr::InsertPQHdr(IDatabase* _db, boost::asio::io_service* _io, 
						 const char* _tableName, const std::vector<InsertSet>& _inserts)
	: IPostQueryHdr(_io)
	, m_db(_db)
	, m_tableName(_tableName)
	, m_inserts(_inserts)
{
}

InsertPQHdr::~InsertPQHdr()
{
}

void InsertPQHdr::handler(DBCnId _cnid)
{
	if (m_tableName.empty() || m_inserts.empty())
	{
		return;
	}

	std::string insertStr;
	uint32 i=0;
	for (std::vector<InsertSet>::const_iterator itr=m_inserts.begin(); itr!=m_inserts.end(); ++itr, ++i)
	{
		const InsertSet& is = *itr;
		insertStr += is.fieldName;
		insertStr += "='";
		insertStr += is.insertValue;
		insertStr += "'";
		if (i+1 < m_inserts.size())
		{
			insertStr += ",";
		}
	}

	Query q1;
	//q1 << "INSERT INTO " << m_tableName << " SET " << insertStr;
	q1.put("INSERT INTO %s SET %s", m_tableName.c_str(), insertStr.c_str());
	QRWrap qrw = m_db->query(QueryArg(), q1);

	if (qrw.isOk)
	{
		Query q2;
		//q2 << "SELECT LAST_INSERT_ID()";
		q2.put("SELECT LAST_INSERT_ID()");
		qrw = m_db->query(QueryArg(qrw.cnid), q2);
		if (qrw.isOk)
		{
			uint32 lastInsertId = qrw.qr->fetch(0)->getUint32();
			IPostQueryHdr::getIO()->post(boost::bind(&InsertPQHdr::callback, shared_from_this(), _cnid, true, lastInsertId));
			return;
		}
	}
	IPostQueryHdr::getIO()->post(boost::bind(&InsertPQHdr::callback, shared_from_this(), _cnid, false, 0));
}

void InsertPQHdr::callback(DBCnId _cnid, bool _isOk, uint32 _lastInsertId)
{
	if (_isOk)
	{
		//sLogger->out(OL_INFO, "Last insert id: ", d2s(_lastInsertId).c_str(), 0);
		sLogger->outFormat(OL_INFO, false, "Last insert id: %u", _lastInsertId);
	}
	else
	{
		//sLogger->out(OL_INFO, "Insert failed!", 0);
		sLogger->outFormat(OL_INFO, false, "Insert failed!");
	}
}
/////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////
AppContext::AppContext(IDatabase* _db)
	: m_db(_db)
	, m_mode(SYNC)
{
	sQueryFL.set(IFreeListShrPtr(new FreeList<Query>(5, 5)));
}

AppContext::~AppContext()
{
}

bool AppContext::boost()
{
	if (m_SQLThread.start() != 0)
	{
		return false;
	}
	return true;
}

void AppContext::shutdown()
{
	m_SQLThread.stop();
}
	
void AppContext::join()
{
	m_SQLThread.join();
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

	if (strcmp(_cmd, "sync") == 0)
	{
		m_mode = SYNC;
	}
	
	if (strcmp(_cmd, "async") == 0)
	{
		m_mode = ASYNC;
	}

	if (strcmp(_cmd, "post") == 0)
	{
		m_mode = POST;
	}

	m_SQLThread.post(boost::bind(&AppContext::parseCmdHdr, this, m_mode, std::string(_cmd, _size)));
	return true;
}

void AppContext::parseCmdHdr(Mode _m, std::string _cmd)
{
	if (_cmd.empty())
	{
		return;
	}

	std::vector<StringPtr> args;
	separateStr(_cmd.c_str(), _cmd.size(), ' ', args);
	if (args.empty())
	{
		return;
	}

	if (args[0] == "insert")
	{
		if (args.size() < 3)
		{
			return;
		}

		std::string tableName(args[1].ptr(), args[1].size());
		std::vector<InsertSet> is;
		for (uint32 i=2; i<args.size(); i+=2)
		{
			is.push_back(InsertSet(args[i], args[i+1]));
		}
		switch (_m)
		{
		case SYNC:
			insert(tableName.c_str(), is);
			break;
		case ASYNC:
			asyncInsert(tableName.c_str(), is);
			break;
		case POST:
			postInsert(tableName.c_str(), is);
			break;
		}
		return;
	}

	if (args[0] == "select")
	{
		if (args.size() < 2)
		{
			return;
		}

		std::string tableName(args[1].ptr(), args[1].size());
		switch (_m)
		{
		case SYNC:
			select(tableName.c_str());
			break;
		case ASYNC:
			asyncSelect(tableName.c_str());
			break;
		case POST:
			postSelect(tableName.c_str());
			break;
		}
		return;
	}
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

	Query q1;
	//q1 << "INSERT INTO " << _tableName << " SET " << insertStr;
	q1.put("INSERT INTO %s SET %s", _tableName, insertStr.c_str());
	QRWrap qrw = m_db->query(QueryArg(), q1);

	if (qrw.isOk)
	{
		Query q2;
		//q2 << "SELECT LAST_INSERT_ID()";
		q2.put("SELECT LAST_INSERT_ID()");
		qrw = m_db->query(QueryArg(qrw.cnid), q2);
		if (qrw.isOk)
		{
			uint32 lastInsertId = qrw.qr->fetch(0)->getUint32();
			//sLogger->out(OL_INFO, "Last insert id: ", d2s(lastInsertId).c_str(), 0);
			sLogger->outFormat(OL_INFO, false, "Last insert id: %u", lastInsertId);
		}
	}
}

void AppContext::asyncInsert(const char* _tableName, const std::vector<InsertSet>& _inserts)
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

	QueryQueueShrPtr qq(new QueryQueue);
	//QueryShrPtr q1(new Query);
	QueryShrPtr q1 = allocQuery();
	//*q1 << "INSERT INTO " << _tableName << " SET " << insertStr;
	q1->put("INSERT INTO %s SET %s", _tableName, insertStr.c_str());
	qq->push(q1);

	//QueryShrPtr q2(new Query);
	QueryShrPtr q2 = allocQuery();
	//*q2 << "SELECT LAST_INSERT_ID()";
	q2->put("SELECT LAST_INSERT_ID()");
	qq->push(q2);

	AynQId qid = m_db->query(QueryArg(), qq, IQueryResultHdrShrPtr(new InsertQRHdr(&m_SQLThread.ioService())));
}

void AppContext::postInsert(const char* _tableName, const std::vector<InsertSet>& _inserts)
{
	m_db->query(QueryArg(), IPostQueryHdrShrPtr(new InsertPQHdr(m_db, &m_SQLThread.ioService(), _tableName, _inserts)));
}

void AppContext::select(const char* _tableName)
{
	if (!_tableName)
	{
		return;
	}

	Query q;
	//q << "SELECT * FROM " << _tableName;
	q.put("SELECT * FROM %s", _tableName);

	QRWrap qrw = m_db->query(QueryArg(), q);
	if (qrw.isOk)
	{
		if (qrw.qr)
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
}

void AppContext::asyncSelect(const char* _tableName)
{
	if (!_tableName)
	{
		return;
	}

	QueryQueueShrPtr qq(new QueryQueue);
	//QueryShrPtr q1(new Query);
	QueryShrPtr q1 = allocQuery();
	//*q1 << "SELECT * FROM " << _tableName;
	q1->put("SELECT * FROM %s", _tableName);
	qq->push(q1);

	AynQId qid = m_db->query(QueryArg(), qq, IQueryResultHdrShrPtr(new SelectQRHdr(&m_SQLThread.ioService(), _tableName)));
}

void AppContext::postSelect(const char* _tableName)
{
	m_db->query(QueryArg(), IPostQueryHdrShrPtr(new SelectPQHdr(m_db, &m_SQLThread.ioService(), _tableName)));
}

