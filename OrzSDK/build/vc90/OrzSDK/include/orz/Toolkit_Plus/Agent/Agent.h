#ifndef __Orz_ToolkitPlus_Agent_h_
#define __Orz_ToolkitPlus_Agent_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 拥有私有线程的类
/*!
 *	\note 用户可以使用boost::bind来向Agent内部的线程post函数来让其执行
 *	\note 线程安全
 example:
 \code
	// SomeClass.h
	class SomeClass
	{
		...
		void fun(int32 _vParam)
		{
			printf("vParam: %d\n", _vParam);
		}
		...
	};

	// main.cpp
	void main()
	{
		...
		Agent agent;
		if (agent.start() == 0)
		{
			int32 vParam = 1;
			agent.post(boost::bind(&SomeClass::fun, vParam));
		}
		...

		// 之后，在其它线程内：
		int32 vParam = 2;
		agent.post(boost::bind(&SomeClass::fun, vParam));
		...
	}

	// xxx.cpp
	{
		// 用户在某个线程内：
		agent.stop();
		...
	}

	// 控制台上输出结果：
	// vParam: 1
	// vParam: 2
 \endcode
 */
class _OrzToolkitPlusExport Agent
	: private boost::noncopyable
{
public:
	//! 默认构造函数
	Agent();

	//! 析构函数
	~Agent();

public:
	//! 启动Agent
	/*!
	 *	\return 返回0成功，非0失败
	 */
	int32						start					();

	//! 返回boost::asio::io_service
	boost::asio::io_service&	ioService				();

	//! 向Agent内部的线程post函数
	/*!
	 *	\param[in] _hdr 通过boost::bind绑定后的函数
	 */
	template <typename Handler>
	void						post					(Handler _hdr)
	{
		m_workSvc.post(_hdr);
	}

	//! 加入主线程
	void						join					();

	//! 关闭Agent
	void						stop					();

	//! 取得内部线程
	ThreadShrPtr				getThread				();

private:
	boost::asio::io_service								m_workSvc;
	boost::scoped_ptr<boost::asio::io_service::work>	m_work;
	ThreadShrPtr										m_workThread;
};
}
#endif

