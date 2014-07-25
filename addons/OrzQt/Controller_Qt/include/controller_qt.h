#ifndef CONTROLLER_QT_H
#define CONTROLLER_QT_H

#include <QtGui/QMainWindow>
#include "ui_controller_qt.h"
#include "ControllerQt.h"

#include <QtGui/QToolbar>
class Controller_Qt : public QMainWindow, public Orz::WindowInterface, public Orz::Singleton<Controller_Qt>
{
	Q_OBJECT

public:
	Controller_Qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Controller_Qt();
	size_t getHandle();
	size_t getWidth();


	//bool init(void);
	//void shutdown();
	
	//消息响应
	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	size_t getHeight();
	void orzWindowResizeEvent(void);

	void paintEvent(QPaintEvent *evt)
	{
		Q_UNUSED(evt);
		if(Orz::SystemInterface::getSingleton().isInit())
			Orz::SystemInterface::getSingleton().update(0.0f);
	}

	void timerEvent(QTimerEvent* evt)
	{
		Q_UNUSED(evt);
		if(Orz::SystemInterface::getSingleton().isInit())
			Orz::SystemInterface::getSingleton().update(0.004f);
	}
	QMenu * createQMenu(const std::wstring & text)
	{
		QMenu * menu = new QMenu(ui.menuBar);
		menu->setTitle(QString::fromUtf16(text.c_str()));
		ui.menuBar->addAction(menu->menuAction());
		return menu;
	}
	QAction * createQAction(const std::wstring & name, const std::wstring & text)
	{
		QAction * action = new QAction(this);
        action->setObjectName(QString::fromUtf16(name.c_str()));
        action->setText(QString::fromUtf16(text.c_str()));
	
		return action;
	}

	

	QToolBar * createQToolBar(const std::wstring & name, const std::wstring & text, Qt::ToolBarArea toolBarArea = Qt::TopToolBarArea)
	{
		QToolBar * toolBar = new QToolBar(this);

        toolBar->setObjectName(QString::fromUtf16(name.c_str()));
        toolBar->setWindowTitle(QString::fromUtf16(text.c_str()));
        addToolBar(toolBarArea, toolBar);

		return toolBar;
	}
private:
	Ui::Controller_QtClass ui;
	QWidget* _orzWindow; 
	
};

#endif // CONTROLLER_QT_H
