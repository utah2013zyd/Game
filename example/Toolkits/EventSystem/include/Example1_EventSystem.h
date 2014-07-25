#ifndef __Orz_Example1_EventSystem__
#define __Orz_Example1_EventSystem__
#include <orz/Toolkit_Base/EventSystem.h>


class MyEventHandler:public Orz::EventHandler
{
public:
	/**
	�����յ���Ϣʱ����ã�����ֵ�����ṩ���������˽�ִ�н��
	**/
	virtual void doExecute(Orz::Event * evt)
	{
		std::cout<<"bool MyEventHander::doExecute(Orz::Event * evt)"<<std::endl;
		
	}
	
	/**���ͽ���EventWorld����֮����ã�����ֵ����ʱ��ɹ�**/
	virtual void doEnable(void)
	{
		std::cout<<"bool MyEventHander::doEnable(void)"<<std::endl;

	}

	/**���뿪EventWorld����֮�����**/
	virtual void doDisable(void)
	{
		std::cout<<"void MyEventHander::doDisable(void)"<<std::endl;
	}

	/**��ͨ��enableUpdate��������֮�󣬻���ÿһ��EventWorld����֮����ã�ͨ��disableUpdate�����ر�
		������falseʱ��ϵͳ�˳���
	**/
	virtual void doFrame(void)
	{
		std::cout<<"bool MyEventHander::doFrame(void)"<<std::endl;

	}
};

void Example1_EventSystem(void)
{
	using namespace Orz;
	


	
	std::cout<<"=== Example1 EventSystem Begin ==="<<std::endl;

	EventWorld world;//������Ϣ���磨����������
	EventHandlerPtr handler(new MyEventHandler());//��Ϣ��������ע��������Ҫ����ָ�롣
	world.init();//��ʼ��
	world.comeIn(handler);//��Ϣ����������
	handler->enableUpdate();//�򿪸��£����򽫲������doFrame
	Event * evt = EventFactory::getInstance().createEvent();//����һ����Ϣ
	evt->setMsg(1981);//������Ϣ��ʾ
	evt->setData<int>(1112);//�������ݣ��������ͣ�
	
	world.broadcasting(evt);//�㲥��Ϣ
	world.update();//��������
	world.update();
	world.update();
	world.goOut(handler);//��Ϣ�������˳�
	world.shutdown();//�ر�����


	
	std::cout<<"=== Example1 EventSystem End ==="<<std::endl<<std::endl<<std::endl;
}


#endif