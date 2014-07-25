#ifndef __Orz_OrzMySQL_OrzMySQLImpl_h__
#define __Orz_OrzMySQL_OrzMySQLImpl_h__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Agency.hpp>
#include <orz/Toolkit_Plus/Toolkit/ThreadQueue.hpp>
#include <orz/Toolkit_Plus/Toolkit/Counter.hpp>
#include <orz/Toolkit_Plus/Toolkit/FastIdrCluster.hpp>
#include <orz/Toolkit_Plus/Database/QRWrap.hpp>
#include <OrzMySQLEnv.h>
#include <DBQueryData.hpp>
#include <MySQLCon.hpp>
namespace Orz
{
namespace Mysql
{
//! OrzMySQL实现类，用户无需关心
class _OrzMySQLPrivate OrzMySQLImpl
{
public:
	OrzMySQLImpl();
	~OrzMySQLImpl();

public:
	DBId				getId				() const;
	bool				initialize			(DBId _dbId, const Params<std::string>& _params);
	bool				boost				();
	void				shutdown			();
	void				join				();
	QRWrap				query				(const QueryArg& _qa, Query& _q);
	AynQId				query				(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr);
	bool				query				(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr);

public:
	void				initMySQLCon		(MySQLCon* _con);
	void				uninitMySQLCon		(MySQLCon* _con);

private:
	void				asyncQueryHdr		(DBCnId _cnid, AynQId _qid, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr);
	bool				sendQuery			(MySQLCon* _con, DBQueryDataShrPtr _dqd, bool _isSelf=false);
	IQueryResultShrPtr	returnQueryResult	(MySQLCon* _con, DBQueryDataShrPtr _dqd);
	int32				handleError			(MySQLCon* _con, uint32 _errorNum);
	int32				connect				(MySQLCon* _con);
	bool				disconnect			(MySQLCon* _con);

private:
	DBId								m_dbId;
	bool								m_hasInited;
	bool								m_isStopped;
	DBHostName							m_dbHn;
	uint16								m_dbPort;
	DBUserName							m_dbUn;
	DBPassword							m_dbPwd;
	DBName								m_dbN;
	uint32								m_conCount;

	NumChooser*							m_selector;
	Agency<MySQLCon>*					m_cons;
	ThreadQueue<int32>					m_initTimer;
	ThreadQueue<int32>					m_uninitTimer;
	Counter								m_mysqlCnOnlineCounter;
	Counter								m_mysqlErrCn;
	Counter								m_mysqlUninitedCn;
	boost::shared_mutex					m_mutex;
	FastIdrCluster*						m_queryIdr;
};
}
}
#endif
