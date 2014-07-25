//  (C) Copyright Gennadiy Rozental 2005-2008.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test

#include "Config.h"
#include "DemoActor.h"
#include "DemoDirector.h"
#include <orz/Framework_Base/Logic.h>
int main(int argc, char **argv)
{
    using namespace Orz;
    //����һ�����ݹ���
    DemoDirectorFactory * director_factory = new Orz::DemoDirectorFactory();
    DemoActorFactory * actor_factory = new Orz::DemoActorFactory();
    //�ѵ��ݹ���ע�ᵽ��Ϸ������
    GameFactories::getInstance().addFactory(director_factory); 
    GameFactories::getInstance().addFactory(actor_factory); 

    //����������ϵͳ��������ֱ��ǣ���־����������ʱ��������������������Ogreͼ�ι�������OIS�����������Fmod��Ƶ�����������������
    SystemPtr system(new SystemList<boost::mpl::list<SimpleLogManager, SimpleTimerManager, OgreGraphicsManager, OISInputManager, PluginsManager> >());

    //����һ���߼�
    LogicConfiger::LogicFacadePtr logic(new LogicConfiger::LogicFacade());
 
    //ͨ���ֶ����Ƶط���������Ϸ�������߼�
    LogicConfiger::ManualBuilder  builder;
    //��������Model_base�����������ṩ�ˡ�TheaterBase�����ʹ����Ķ��塣
    builder.addPlugin("Model_Base");
    //����һ����TheaterBase�����ʹ���������Ϊ"main"��
    builder.setTheater("TheaterBase","main");
    //����һ���ղ����Ƕ���ĵ������͡�TutorialDirector��������ʵ������Ϊ"Tutorial"��
    builder.addDirector("DemoDirector","Demo");
    //�Ѹղŵĵ�������Ϊ�������ͬһʱ��ֻ����һ�����ݱ����
    builder.setActiveDirector("Demo");

    //ͨ��������Ϣ�������߼���
    logic->building(builder);

	try
	{
    if(system->init())//��ʼ��ϵͳ
    {
        if(logic->load())//�����߼�
        {
            system->run();//����ϵͳ
            logic->unload();//ж���߼�
        }
        system->shutdown();//�ر�ϵͳ
    }
	}
	catch(std::exception & e)
	{
		std::cout<<e.what()<<std::endl;
	}
    //���������˰�֮ǰ�ĵ��ݹ�����ע������Ȼ���ͷ��ˡ�
    GameFactories::getInstance().removeFactory(director_factory); 
    GameFactories::getInstance().removeFactory(actor_factory); 
    delete director_factory;
	delete actor_factory;
}
