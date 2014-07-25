#ifndef __Orz_ToolkitPlus_Agent_h_
#define __Orz_ToolkitPlus_Agent_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! ӵ��˽���̵߳���
/*!
 *	\note �û�����ʹ��boost::bind����Agent�ڲ����߳�post����������ִ��
 *	\note �̰߳�ȫ
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

		// ֮���������߳��ڣ�
		int32 vParam = 2;
		agent.post(boost::bind(&SomeClass::fun, vParam));
		...
	}

	// xxx.cpp
	{
		// �û���ĳ���߳��ڣ�
		agent.stop();
		...
	}

	// ����̨����������
	// vParam: 1
	// vParam: 2
 \endcode
 */
class _OrzToolkitPlusExport Agent
	: private boost::noncopyable
{
public:
	//! Ĭ�Ϲ��캯��
	Agent();

	//! ��������
	~Agent();

public:
	//! ����Agent
	/*!
	 *	\return ����0�ɹ�����0ʧ��
	 */
	int32						start					();

	//! ����boost::asio::io_service
	boost::asio::io_service&	ioService				();

	//! ��Agent�ڲ����߳�post����
	/*!
	 *	\param[in] _hdr ͨ��boost::bind�󶨺�ĺ���
	 */
	template <typename Handler>
	void						post					(Handler _hdr)
	{
		m_workSvc.post(_hdr);
	}

	//! �������߳�
	void						join					();

	//! �ر�Agent
	void						stop					();

	//! ȡ���ڲ��߳�
	ThreadShrPtr				getThread				();

private:
	boost::asio::io_service								m_workSvc;
	boost::scoped_ptr<boost::asio::io_service::work>	m_work;
	ThreadShrPtr										m_workThread;
};
}
#endif

