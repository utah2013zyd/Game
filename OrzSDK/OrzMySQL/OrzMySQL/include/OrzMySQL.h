#ifndef __Orz_OrzMySQL_OrzMySQL_h__
#define __Orz_OrzMySQL_OrzMySQL_h__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/IDatabase.h>
namespace Orz
{
namespace Mysql
{
class OrzMySQLImpl;
//! OrzMySQL的数据库访问接口类
/*!
 *	\note 一个OrzMySQL对象同一时间只能访问一个数据库，例如Mysql中有一个名称为OrzCORBA的数据库，\n
 *		则当这个OrzMySQL对象正常运行的时候，用户只能从这个OrzCORBA的数据库中执行SQL命令等相关操作，例如SELECT、INSERT、DELETE等
 *	\note 线呈安全，可以让用户自定义OrzMySQL到Mysql数据库服务器的网络连接数，每个连接会独立分配一个专有线程来运行，\n
		所以用户要根据自己服务器的负载来调整网络连接数，连接数越多，则OrzMySQL的同时处理的SQL请求就越多，吞吐量就越大。
 *	\note OrzMySQL的query和queryEx方法，是同步阻塞处理的，就是说一个SQL请求必须等待其返回后，query和queryEx方法才会返回\n
 *		，但在这种情况下一个SQL请求只会占用一个网络连接（也就是一个线程），其他的SQL请求只要还有额外的网络连接，就可以wait-free的来执行，请根据需求来配置连接数（线程数）
 *	\note OrzMySQL的asyncQuery和asyncQueryEx方法则是异步的，需要用户传递一个回调函数来异步回调返回SQL请求的结果
 */
class _OrzMySQLExport OrzMySQL
	: public IDatabase
	, private boost::noncopyable
{
public:
	//! 默认构造函数
	OrzMySQL();

	//! 析构函数
	~OrzMySQL();

public:
	//! 取得这个OrzMySQL对象的Id
	DBId			getId				() const;

	//! 对OrzMySQL进行初始化
	/*!
	 *	\param[in] _dbId 这个OrzMySQL对象的Id，用户可以自己根据需求进行设置（自定义分配）
	 *	\param[in] _params 需要传递的参数，Params在这里必须固定8个参数，每个参数一次如下：\n
	 *		#1 Mysql服务器的IP地址，例如192.168.1.133
	 *		#2 Mysql服务器的端口，例如3306
	 *		#3 登录Mysql服务器的用户名，例如root
	 *		#4 登录Mysql服务器的密码，例如******
	 *		#5 要使用的是数据名称，例如orz_corba
	 *		#6 连接Mysql的网络连接数，例如5
	 *		#7 OrzMySQL对象内部使用的SQL命令的执行结果对象的对象池参数一，例如10
	 *		#8 OrzMySQL对象内部使用的SQL命令的执行结果对象的对象池参数二，例如100
	 *	\return true则成功，false则失败
	 */
	bool			initialize			(DBId _dbId, const Params<std::string>& _params);

	//! 启动OrzMySQL
	bool			boost				();

	//! 关闭OrzMySQL
	void			shutdown			();

	//! 合并OrzMySQL内部线程到调用OrzMySQL::boost的线程
	void			join				();

	//! 在当前线程同步阻塞执行SQL命令
	/*!
	 *	\param[in] _qa query参数
	 *	\return 返回这次query的结果
	 *	\note 同步阻塞，线程安全
	 */
	QRWrap			query				(const QueryArg& _qa, Query& _q);

	//! 在OrzMySQL内部线程异步执行SQL命令
	/*!
	 *	\param[in] _qs 需要顺序执行的query队列
	 *	\param[in] _qrHdr 一个Functor，回调到用户线程执行query的结果
	 *	\return 返回这次异步query的id
	 *	\note 异步，线程安全
	 */
	AynQId			query				(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr);


	//! 向OrzMySQL内部的线程post用于执行SQL语句的用户自定义函数对象
	/*!
	 *	\param[in] _qa query参数
	 *	\param[in] _pqHdr 一个Functor 用户提供的用于执行SQL语句的函数对象；
	 *	\note 具体的query则是用户在_queryHdr函数中自己使用同步阻塞的query来执行
	 *	\note 用户要注意，这种方式会让_pqHdr运行在和用户的线程不同的线程上，所以_pqHdr中的线程安全需要用户自行处理
	 *	\note 用户可以在_pqHdr中使用post来post自己定义的回调函数到用户自己的线程上执行结果相关的逻辑，
	 *			这样结果相关的逻辑就和用户调用postQuery的线程是同一个线程了
	 */
	bool			query				(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr);

private:
	OrzMySQLImpl*						m_impl;
};
}
}
#endif
