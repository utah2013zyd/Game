#include "ExampleConfig.h"
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Framework_Base/System/SystemInterface.h>

namespace Orz
{
	class _OrzViewExport ExampleManager:public Singleton<ExampleManager>
	{
	public:
		ExampleManager(void);
		~ExampleManager(void);
		bool init(void);//��ѡ�ӿڣ��������Ļ��ᱻϵͳ��ʼ��ʱ����ã��������false��ϵͳ��ֹͣ��ʼ����ֱ���˳�
		void shutdown(void);//��ѡ�ӿڣ��������Ļ�����ϵͳ�ر�ʱ����á�
		bool update(TimeType interval);//��ѡ�ӿڣ�����ÿ�θ���ʱ����ã����������ε��õ�ʱ����������ֵ�����false��ϵͳ�������˳���

		void print(void);//������һ�������ϵͳ���еĽӿڣ�����ʵ��ϵͳ���ܡ�
	};

	//�����5��������ע���������ϵͳ����Ϣ��
	SYSTEMUNIT_BEGIN(ExampleManager) 
	SYSTEMUNIT_INIT 
	SYSTEMUNIT_UPDATE 
	SYSTEMUNIT_SHUTDOWN 
	SYSTEMUNIT_END(ExampleManager) 

}