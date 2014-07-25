#include <OrzMySQL.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogEnv.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <DBQydFreeList.h>
#include <OrzMySQLImpl.h>
using namespace Orz;
using namespace Orz::Mysql;


OrzMySQL::OrzMySQL()
	: m_impl(new OrzMySQLImpl)
{
}

OrzMySQL::~OrzMySQL()
{
	if (m_impl)
	{
		delete m_impl;
	}
}

DBId OrzMySQL::getId() const
{
	return m_impl->getId();
}

bool OrzMySQL::initialize(DBId _dbId, const Params<std::string>& _params)
{
	return m_impl->initialize(_dbId, _params);
}

bool OrzMySQL::boost()
{
	return m_impl->boost();
}

void OrzMySQL::shutdown()
{
	m_impl->shutdown();
}

void OrzMySQL::join()
{
	m_impl->join();
}

QRWrap OrzMySQL::query(const QueryArg& _qa, Query& _q)
{
	return m_impl->query(_qa, _q);
}

AynQId OrzMySQL::query(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr)
{
	return m_impl->query(_qa, _qs, _qrHdr);
}

bool OrzMySQL::query(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr)
{
	return m_impl->query(_qa, _pqHdr);
}
