#include "OrzWindow.h"
#include "controller_qt.h"
#include "QtInputManager.h"

OrzWindow::OrzWindow(QWidget *parent, Qt::WFlags flags):QWidget(parent, flags)
{
	//setFocusPolicy(Qt::StrongFocus);//
	setMouseTracking(true);
}
OrzWindow::~OrzWindow()
{

}
	

void OrzWindow::mousePressEvent(QMouseEvent* evt)
{
	QtInputManager::getSingleton().mousePressEvent(evt);
}
void OrzWindow::mouseReleaseEvent(QMouseEvent* evt)
{
	QtInputManager::getSingleton().mouseReleaseEvent(evt);
}
void OrzWindow::mouseMoveEvent(QMouseEvent* evt)
{
	QtInputManager::getSingleton().mouseMoveEvent(evt);
}
void OrzWindow::wheelEvent(QWheelEvent* evt)
{
	QtInputManager::getSingleton().wheelEvent(evt);
}

void OrzWindow::resizeEvent(QResizeEvent* evt)
{
	Controller_Qt::getSingleton().orzWindowResizeEvent();	
}
