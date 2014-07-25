#include "controller_qt.h"
#include "ControllerQt.h"


template<> Controller_Qt* Orz::Singleton<Controller_Qt>::_singleton = NULL;

Controller_Qt::Controller_Qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	using namespace Orz;
	ui.setupUi(this);

	startTimer(2);

	
	
	//OgreWidget *swatch = new OgreWidget(tr("White"), this, 0 );
//	addDockWidget(Qt::LeftDockWidgetArea, swatch);

	//QMenu * help = addMenu(tr("&Dock Widgets"));
	//menuBar()->insertItem( "&Help", help );

	//help->insertItem( "&About", this, SLOT(about()), Key_F1 );
    //params["externalWindowHandle"] = Ogre::StringConverter::toString((size_t)(HWND)winId());
}

//ÏûÏ¢ÏìÓ¦
void Controller_Qt::keyPressEvent(QKeyEvent* evt)
{
	QtInputManager::getSingleton().keyPressEvent(evt);
}
void Controller_Qt::keyReleaseEvent(QKeyEvent* evt)
{
	QtInputManager::getSingleton().keyReleaseEvent(evt);
}


//bool Controller_Qt::init(void)
//{
//	return _system->init();
//}
//void Controller_Qt::shutdown(void)
//{
//	return _system->shutdown();
//}
size_t Controller_Qt::getHandle()
{
	return (size_t)(HWND)ui._orzWindow->winId();
}

size_t Controller_Qt::getWidth() 
{
	return static_cast<size_t>(ui._orzWindow->width());
}
size_t Controller_Qt::getHeight()
{
	return static_cast<size_t>(ui._orzWindow->height());
}


void Controller_Qt::orzWindowResizeEvent(void)
{
	_sizeChange(getWidth(), getHeight());
}


	


Controller_Qt::~Controller_Qt()
{
	
//	_system.reset();
}
