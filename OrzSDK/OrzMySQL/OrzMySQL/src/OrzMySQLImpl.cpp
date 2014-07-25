#include <OrzMySQLImpl.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
#include <orz/Toolkit_Plus/Database/QueryQueue.hpp>
#include <orz/Toolkit_Plus/Database/Query.hpp>
#include <orz/Toolkit_Plus/Database/IPostQueryHdr.hpp>
#include <orz/Toolkit_Plus/Database/QueryArg.hpp>
#include <orz/Toolkit_Plus/Database/IQueryResultHdr.hpp>
#include <DBQydFreeList.h>
#include <QueryResult.h>
using namespace Orz;
using namespace Orz::Mysql;


OrzMySQLImpl::OrzMySQLImpl()
	: m_dbId(0)
	, m_hasInited(false)
	, m_isStopped(false)
	, m_dbPort(3306)
	, m_conCount(1)
	, m_selector(0)
	, m_cons(0)
	, m_initTimer(1000, 1000)
	, m_uninitTimer(1000, 1000)
	, m_queryIdr(0)
{
}

OrzMySQLImpl::~OrzMySQLImpl()
{
	if (m_cons)
	{
		delete m_cons;
	}

	if (m_selector)
	{
		delete m_selector;
	}

	if (m_queryIdr)
	{
		delete m_queryIdr;
	}

	sDBQydFList.clear();
}

DBId OrzMySQLImpl::getId() const
{
	return m_dbId;
}

bool OrzMySQLImpl::initialize(DBId _dbId, const Params<std::string>& _params)
{
	if (m_hasInited)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " OrzMySQLImpl::initialize - has initialized!", 0);
		return false;
	}

	const std::vector<std::string*>& params = _params.getParamList();

	if (params.size() != 8)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " OrzMySQLImpl::initialize - params count error!", 0);
		return false;
	}

	for (std::vector<std::string*>::const_iterator itr=params.begin(); itr!=params.end(); ++itr)
	{
		if (!(*itr))
		{
			sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " OrzMySQLImpl::initialize - param error!", 0);
			return false;
		}
	}

	m_dbId = _dbId;
	m_dbHn.append(params[0]->c_str(), params[0]->size());
	m_dbPort = (uint16)s2d(params[1]->c_str());
	m_dbUn.append(params[2]->c_str(), params[2]->size());
	m_dbPwd.append(params[3]->c_str(), params[3]->size());
	m_dbN.append(params[4]->c_str(), params[4]->size());
	m_conCount = s2d(params[5]->c_str());
	uint32 clr = s2d(params[6]->c_str());
	uint32 per = s2d(params[7]->c_str());
	if (m_conCount == 0)
		m_conCount = 1;
	
	m_selector = new NumChooser(0, m_conCount - 1, 21087);
	m_cons = new Agency<MySQLCon>(m_conCount, *m_selector);
	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		(*m_cons)[i].index = i;
	}

	sDBQydFList.set(IFreeListShrPtr(new FreeList<DBQueryData>(clr, per)));

	m_queryIdr = new FastIdrCluster(m_conCount, per, 0);

	m_hasInited = true;
	m_isStopped = false;
	return true;
}

bool OrzMySQLImpl::boost()
{
	sLogger->out(OL_INFO, "Connecting to MySQL on ", m_dbHn.c_str(), " with (",  m_dbUn.c_str(), " : *********)", 0);

	// boost workers
	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		if ((*m_cons)[i].agt.start() != 0)
		{
			sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " one worker thread boost error!", 0);
			continue;
		}
		(*m_cons)[i].agt.post(boost::bind(&OrzMySQLImpl::initMySQLCon, this, &((*m_cons)[i])));
	}

	// wait untill all connections ok
	uint32 cTryCount = m_cons->size()*3;
	int32 t;
	for (uint32 i=0; i<cTryCount; ++i)
	{
		m_initTimer.pop(t);
		if (m_mysqlCnOnlineCounter.get() == m_cons->size())
		{
			return true;
		}
		if (m_mysqlErrCn.get() == m_cons->size())
		{
			break;
		}
	}

	return false;
}

void OrzMySQLImpl::shutdown()
{
	if (m_isStopped)
		return;

	sLogger->out(OL_INFO, " sql: Closing all connections...", 0);
	uint32 closed = 0;
	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		(*m_cons)[i].agt.post(boost::bind(&OrzMySQLImpl::disconnect, this, &((*m_cons)[i])));
	}

	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		(*m_cons)[i].agt.post(boost::bind(&OrzMySQLImpl::uninitMySQLCon, this, &((*m_cons)[i])));
	}

	// wait untill all connections uninit
	uint32 cTryCount = m_cons->size()*3;
	int32 t;
	for (uint32 i=0; i<cTryCount; ++i)
	{
		m_uninitTimer.pop(t);
		if (m_mysqlUninitedCn.get() == m_cons->size())
		{
			break;
		}
	}

	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		(*m_cons)[i].agt.stop();
	}

	Counter cr;
	m_mysqlUninitedCn = cr;
	m_mysqlErrCn = cr;

	m_hasInited = false;
	m_isStopped = true;
}

void OrzMySQLImpl::join()
{
	for(uint32 i=0; i<m_cons->size(); ++i)
	{
		(*m_cons)[i].agt.join();
	}
}

QRWrap OrzMySQLImpl::query(const QueryArg& _qa, Query& _q)
{
	MySQLCon* con = 0;
	if (!_qa.hasCn())
	{
		con = &m_cons->select();
	}
	else
	{
		if (_qa.getCnId() >= m_cons->size())
		{
			return QRWrap();
		}
		con = &((*m_cons)[_qa.getCnId()]);
	}

	con->busy.lock();
	QRWrap qrw;
	DBQueryDataShrPtr dbq = allocDBQyd();
	dbq->dbQstr = _q.str();
	if (sendQuery(con, dbq))
	{
		qrw.isOk = true;
		qrw.cnid = con->index;
		qrw.qr = returnQueryResult(con, dbq);
	}
	con->busy.unlock();
	return qrw;
}

AynQId OrzMySQLImpl::query(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr)
{
	AynQId qid = m_queryIdr->generate();
	if (!qid.isValid())
	{
		return AynQId();
	}

	if (!_qs || !_qrHdr || !_qrHdr->getIO())
	{
		return AynQId();
	}
	
	MySQLCon* con = 0;
	if (!_qa.hasCn())
	{
		con = &m_cons->select();
	}
	else
	{
		if (_qa.getCnId() >= m_cons->size())
		{
			return AynQId();
		}
		con = &((*m_cons)[_qa.getCnId()]);
	}
	con->agt.post(boost::bind(&OrzMySQLImpl::asyncQueryHdr, this, con->index, qid, _qs, _qrHdr));
	return qid;
}

bool OrzMySQLImpl::query(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr)
{
	if (!_pqHdr || !_pqHdr->getIO())
	{
		return false;
	}

	DBCnId cnid = _qa.getCnId();
	MySQLCon* con = 0;
	if (!_qa.hasCn())
	{
		con = &m_cons->select();
		cnid = con->index;
	}
	else
	{
		if (cnid >= m_cons->size())
		{
			return false;
		}
		con = &((*m_cons)[cnid]);
	}

	con->agt.post(boost::bind(&IPostQueryHdr::handler, _pqHdr, cnid));
	return true;
}

void OrzMySQLImpl::initMySQLCon(MySQLCon* _con)
{
	if (!_con)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " _con == 0!", 0);
		return;
	}

	if (connect(_con) != 0)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " mysql_thread_init() != 0!", 0);
		return;
	}
	sLogger->out(OL_INFO, " One MySQL connections established!", 0);
}

void OrzMySQLImpl::uninitMySQLCon(MySQLCon* _con)
{
	if (!_con)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", " _con == 0!", 0);
		return;
	}

	mysql_thread_end();
	m_mysqlUninitedCn.inc();
	m_uninitTimer.push((int32)1);
}

void OrzMySQLImpl::asyncQueryHdr(DBCnId _cnid, AynQId _qid, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr)
{
	MySQLCon& con = (*m_cons)[_cnid];

	std::vector<QRWrap> qrws;
	QueryShrPtr q;
	con.busy.lock();
	while (_qs->pop(q))
	{
		QRWrap qrw;
		DBQueryDataShrPtr dbq = allocDBQyd();
		dbq->dbQstr = q->str();
		if (sendQuery(&con, dbq))
		{
			qrw.isOk = true;
			qrw.cnid = con.index;
			qrw.qr = returnQueryResult(&con, dbq);
		}
		qrws.push_back(qrw);
	}
	con.busy.unlock();
	
	_qrHdr->getIO()->post(boost::bind(&IQueryResultHdr::handler, _qrHdr, _qid, qrws));
}

bool OrzMySQLImpl::sendQuery(MySQLCon* _con, DBQueryDataShrPtr _dqd, bool _isSelf)
{
	int32 result = mysql_real_query(_con->con, _dqd->dbQstr.c_str(), _dqd->dbQstr.size());
	if (result > 0)
	{
		sLogger->out(OL_INFO, "Sql query failed due to [", mysql_error(_con->con), "], Query: [", _dqd->dbQstr.c_str(), "]", 0);
		if (!_isSelf && handleError(_con, mysql_errno(_con->con)) == 0)
		{
			// Re-send the query, the connection was successful.
			// The true on the end will prevent an endless loop here, as it will
			// stop after sending the query twice.
			result = sendQuery(_con, _dqd, true);
		}
	}

	return (result == 0 ? true : false);
}

IQueryResultShrPtr OrzMySQLImpl::returnQueryResult(MySQLCon* _con, DBQueryDataShrPtr _dqd)
{
	// We got a valid query. :)
	MYSQL_RES* result = mysql_store_result(_con->con);

	IQueryResultShrPtr qr;

	// Don't think we're gonna have more than 4 billion rows......
	uint32 rowCount = (uint32)mysql_affected_rows(_con->con);
	uint32 fieldCount = mysql_field_count(_con->con);

	// Check if we have no rows.
	if(!rowCount || !fieldCount) 
	{
		mysql_free_result(result);
		return qr;
	}
	else 
	{
		qr = IQueryResultShrPtr(new QueryResult(result, fieldCount, rowCount, _con));
		qr->nextRow();
		return qr;
	}
	//return 0;
}

int32 OrzMySQLImpl::handleError(MySQLCon* _con, uint32 _errorNum)
{
	// Handle errors that should cause a reconnect to the Database.
	switch(_errorNum)
	{
	case 2006:  // Mysql server has gone away
	case 2008:  // Client ran out of memory
	case 2013:  // Lost connection to sql server during query
	case 2055:  // Lost connection to sql server - system error
		{
			// Let's instruct a reconnect to the db when we encounter these errors.
			disconnect(_con);
			return connect(_con);
		}break;
	}

	return 1;
}

int32 OrzMySQLImpl::connect(MySQLCon* _con)
{
	DynShrLock lock(m_mutex);
	MYSQL* Descriptor = mysql_init(NULL);
	memset(Descriptor, 0, sizeof(MYSQL));
	if (mysql_options(Descriptor, MYSQL_SET_CHARSET_NAME, "utf8"))
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", "sql: Could not set ", "utf8 character set!", 0);
	}

	// set reconnect
	my_bool my_true = true;
	if (mysql_options(Descriptor, MYSQL_OPT_RECONNECT, &my_true))
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", 
					 "sql: MYSQL_OPT_RECONNECT could not be set", ", connection drops may occur ", "but will be counteracted.\n", 0);
	}

	_con->con = mysql_real_connect(Descriptor, m_dbHn.c_str(), m_dbUn.c_str(), m_dbPwd.c_str(), "", m_dbPort, NULL, 0);
	if (_con->con == NULL)
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", 
					 "sql: Connection failed. ", "Reason was `", mysql_error(Descriptor), "`\n", 0);
		m_mysqlErrCn.inc();
		m_initTimer.push((int32)0);
		return 1;
	}
	
	if (mysql_select_db(_con->con, m_dbN.c_str()))
	{
		sLogger->out(OL_ERROR, __FILE__, " - [", d2s(__LINE__).c_str(), "]: ", 
					 "sql: Select of Database ", m_dbN.c_str(), " failed due to `", mysql_error(_con->con), "`\n", 0);
		m_mysqlErrCn.inc();
		m_initTimer.push((int32)0);
		return 1;
	}

	m_mysqlCnOnlineCounter.inc();
	if (m_mysqlCnOnlineCounter.get() == m_cons->size())
	{
		m_initTimer.push((int32)1);
	}

	return 0;
}

bool OrzMySQLImpl::disconnect(MySQLCon* _con)
{
	if(_con->con)
	{
		mysql_close(_con->con);
		_con->con = NULL;
		sLogger->out(OL_INFO, "sql: one connection closed.", 0);
		m_mysqlCnOnlineCounter.dec();
		return true;
	}
	return false;
}
