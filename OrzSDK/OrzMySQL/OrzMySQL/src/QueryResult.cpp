#include <QueryResult.h>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <SqlField.h>
#include <OrzMySQLImpl.h>
using namespace Orz;
using namespace Orz::Mysql;


QueryResult::QueryResult(MYSQL_RES* _res, uint32 _fieldCount, uint32 _rowCount, MySQLCon* _con)
	: m_result(_res)
	, m_fieldCount(_fieldCount == 0 ? 1 : _fieldCount)
	, m_rowCount(_rowCount == 0 ? 1 : _rowCount)
	, m_con(_con)
{
	m_currentRow = new SqlField[m_fieldCount];
}

QueryResult::~QueryResult()
{
	mysql_free_result(m_result);
	delete [] m_currentRow;
}

bool QueryResult::nextRow()
{
	MYSQL_ROW row = mysql_fetch_row(m_result);
	if(row == NULL)
		return false;

	unsigned long* lengths = mysql_fetch_lengths(m_result);
	for(uint32 i = 0; i < m_fieldCount; ++i)
	{
		m_currentRow[i].setValue(row[i], (uint32)(lengths[i]));
	}

	return true;
}

void QueryResult::clear()
{
	m_con->agt.post(boost::bind(&MySQLCon::clearQuery, m_con, shared_from_this()));
}

ISqlField* QueryResult::fetch(uint32 _offset)
{
	if (_offset >= m_fieldCount)
	{
		return 0;
	}
	else
	{
		return &m_currentRow[_offset]; 
	}
}

uint32 QueryResult::getFieldCount() const 
{ 
	return m_fieldCount; 
}

uint32 QueryResult::getRowCount() const 
{ 
	return m_rowCount; 
}

uint32 QueryResult::getConnection() const
{
	return m_con->index;
}
