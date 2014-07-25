#include <QtGui/QApplication>

#include <QApplication>
#include <QPainterPath>
#include <QPainter>
#include <QTextEdit>
#include <QMap>
#include <qdebug.h>

#include <orz/Framework_Base/System.h>
#include <orz/Toolkit_Base/LogManager.h>
#include <orz/Framework_Base/Logic/GameFactories.h>
#include <orz/Framework_Base/Logic.h>
#include <orz/Toolkit_Base/TimerManager.h>
//#include "QtEditorEvents.h"

#include "controller_qt.h"
//#include "QtEditorTheater.h"
//#include "AvatarEditorDirector.h"

int main(int argc, char *argv[])
{
	using namespace Orz;
	
	//Orz::TheaterFactoryPtr theater(new QtEditorTheaterFactory());
	//Orz::DirectorFactoryPtr director(new AvatarEditorDirectorFactory());

//TimerManager, 
	SystemPtr system(new SystemList<boost::mpl::list<QtLogManager, SimpleTimerManager,OgreGraphicsManager, CEGUIManager, QtInputManager, PluginsManager> >());
	SystemInterface::getSingleton().setParame("w32_mouse",Orz::Variant(true));
	
	LogicConfiger::LogicFacadePtr  logic(new LogicConfiger::LogicFacade());



	LogicConfiger::ManualBuilder builder;
	
	//loader->addPlugin("");

	builder.addPlugin("Model_Base");
	builder.setTheater("TheaterBase", "main");
	//builder.addDirector("AvatarEditorDirector", "avatar_editor");
	//builder.setActiveDirector("avatar_editor");
	
	logic->building(builder);


	//LogicConfiger::ConfigerPtr logic(new LogicConfiger::Configer());

	//logic->readDate(LogicConfiger::LoaderPtr(new ControllerBaseXMLLoader("orzconfig.xml")));
	//logic->readDate(LogicConfiger::LoaderPtr(new ControllerBaseArgLoader(argc, argv)));

	int ret = 0;
//
	QApplication app(argc, argv);

	boost::shared_ptr<Controller_Qt> window(new Controller_Qt());
	Orz::ILogManager::getSingleton().unicode(L"²âÊÔÈÕÖ¾");
	Orz::WeakWindowPtr win = WindowPtr(window);
	system->setParame<Orz::WeakWindowPtr>("WINDOW", win);
	

	//logic->readDate(LogicConfiger::LoaderPtr(new LogicConfigerXMLLoader("orzconfig.xml")));
//	logic->readDate(LogicConfiger::LoaderPtr(new LogicConfigerArgLoader(argc, argv)));
	
	if(system->init())
	{
		

		if(logic->load())
		{
			window->show();
			ret = app.exec();

			logic->unload();
		}
		system->shutdown();
		
	}
	
	
//	GameFactories::getInstance().removeTheaterFactory(theater.get()); 
	return true;
}
