#include "LogWindow.h"
#include "QtLogManager.h"
LogWindow::LogWindow(QWidget *parent, Qt::WFlags flags):QTextEdit(parent)
{
	QtLogManager::getSingleton().addWindow(this);
	
}
LogWindow::~LogWindow()
{
	QtLogManager::getSingleton().removeWindow(this);
}
