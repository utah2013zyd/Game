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
//! SQL命令的执行结果类
/*!
 *	\note 保存了一条SQL命令或者语句执行的结果，例如SELECT语句，QueryResult会保存其返回的查询结果
 *	\note 会由OrzMySQL在执行完一条SQL语句后自动创建，但如果存储有数据，用户必须在使用完这些数据后，手动调用clear()方法来清除内存
 */
class _OrzMySQLExport QueryResult
	: public IQueryResult
	, public boost::enable_shared_from_this<QueryResult>
	, private boost::noncopyable
{
public:
	//! 构造函数
	/*!
	 *	\note 用户无需直接调用
	 */
	QueryResult(MYSQL_RES* _res, uint32 _fieldCount, uint32 _rowCount, MySQLCon* _con);

	//! 析构函数
	~QueryResult();

public:
	//! 将QueryResult内部的行指针向后移动一个
	/*!
	 *	\return true则表示移动成功，false表示已经没有下一行了
	 */
	bool			nextRow				();

	//! 将自身post进入OrzMySQL的线程中独立删除
	/*!
	 *	\note 这个方法的存在目的是为了处理当QueryResult存储的数据过大时，销毁速度较慢的，\n
	 *		为了不影响用户线程（指调用OrzMySQL进行），而将销毁的工作返回给OrzMySQL的线程进行
	 */
	void			clear				();

	//! 取得当前行中指定偏移量的数据
	/*!
	 *	\param[in] _offset 偏移量，从0~列最大值-1
	 *	\return 返回这个数据，见SqlField
	 */
	ISqlField*		fetch				(uint32 _offset);

	//! 取得结果中列的数目
	uint32			getFieldCount		() const;

	//! 取得结果中行的数目
	uint32			getRowCount			() const;

	//! 取得这个SQL语句执行结果是哪个连接负责传送的
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
