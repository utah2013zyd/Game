#ifndef __Orz_ToolkitPlus_IDatabase_h_
#define __Orz_ToolkitPlus_IDatabase_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
#include <orz/Toolkit_Plus/Toolkit/ClrId.hpp>
#include <orz/Toolkit_Plus/Database/IQueryResult.h>
#include <orz/Toolkit_Plus/Database/QRWrap.hpp>
#include <orz/Toolkit_Plus/Database/QueryQueue.hpp>
#include <orz/Toolkit_Plus/Plugin/IOrzPlugin.h>
namespace Orz
{
//! ���ݿ���ʽӿ���
/*!
 *	\note �û�����ͨ���̳����Զ����Լ������ݿ���ʽӿڣ������ǲ�ͬ�����ݿ⣨MySQL��MSSQL��PostgreSQL�ȣ�
 *	\note ʹ�÷�����OrzMySQL
 */
class IDatabase
	: public IOrzPlugin
{
public:
	IDatabase() {}
	virtual ~IDatabase() {}

public:
	virtual DBId			getId				() const = 0;
	virtual bool			initialize			(DBId _dbId, const Params<std::string>& _params) = 0;
	virtual bool			boost				() = 0;
	virtual void			shutdown			() = 0;
	virtual void			join				() = 0;
	virtual QRWrap			query				(const QueryArg& _qa, Query& _q) = 0;
	virtual AynQId			query				(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr) = 0;
	virtual bool			query				(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr) = 0;
};
}

typedef Orz::IDatabase* (*DLL_START_DB_PLUGIN)(void);
typedef void (*DLL_STOP_DB_PLUGIN)(Orz::IDatabase* _db);

#endif
