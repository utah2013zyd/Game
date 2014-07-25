#ifndef __Orz_OrzMySQL_QueryResult_h_
#define __Orz_OrzMySQL_QueryResult_h_
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
#include <orz/Toolkit_Plus/Database/IQueryResult.h>
#include <SqlField.h>
#include <MySQL/mysql.h>
namespace Orz
{
namespace Mysql
{
struct MySQLCon;
//! SQL�����ִ�н����
/*!
 *	\note ������һ��SQL����������ִ�еĽ��������SELECT��䣬QueryResult�ᱣ���䷵�صĲ�ѯ���
 *	\note ����OrzMySQL��ִ����һ��SQL�����Զ�������������洢�����ݣ��û�������ʹ������Щ���ݺ��ֶ�����clear()����������ڴ�
 */
class _OrzMySQLExport QueryResult
	: public IQueryResult
	, public boost::enable_shared_from_this<QueryResult>
	, private boost::noncopyable
{
public:
	//! ���캯��
	/*!
	 *	\note �û�����ֱ�ӵ���
	 */
	QueryResult(MYSQL_RES* _res, uint32 _fieldCount, uint32 _rowCount, MySQLCon* _con);

	//! ��������
	~QueryResult();

public:
	//! ��QueryResult�ڲ�����ָ������ƶ�һ��
	/*!
	 *	\return true���ʾ�ƶ��ɹ���false��ʾ�Ѿ�û����һ����
	 */
	bool			nextRow				();

	//! ������post����OrzMySQL���߳��ж���ɾ��
	/*!
	 *	\note ��������Ĵ���Ŀ����Ϊ�˴���QueryResult�洢�����ݹ���ʱ�������ٶȽ����ģ�\n
	 *		Ϊ�˲�Ӱ���û��̣߳�ָ����OrzMySQL���У����������ٵĹ������ظ�OrzMySQL���߳̽���
	 */
	void			clear				();

	//! ȡ�õ�ǰ����ָ��ƫ����������
	/*!
	 *	\param[in] _offset ƫ��������0~�����ֵ-1
	 *	\return ����������ݣ���SqlField
	 */
	ISqlField*		fetch				(uint32 _offset);

	//! ȡ�ý�����е���Ŀ
	uint32			getFieldCount		() const;

	//! ȡ�ý�����е���Ŀ
	uint32			getRowCount			() const;

	//! ȡ�����SQL���ִ�н�����ĸ����Ӹ����͵�
	DBCnId			getConnection		() const;

private:
	MYSQL_RES*							m_result;
	uint32								m_fieldCount;
	uint32								m_rowCount;
	SqlField*							m_currentRow;
	uint32								m_type;
	MySQLCon*							m_con;
};
}
}
#endif
