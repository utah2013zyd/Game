#include "QtInputManager.h"
#include <qevent.h>
#include <orz/Toolkit_Base/LogManager.h>
QtInputManager& QtInputManager::getSingleton(void)
{
	return static_cast<QtInputManager&> (IInputManager::getSingleton());
}
QtInputManager* QtInputManager::getSingletonPtr(void)
{
	return static_cast<QtInputManager*> (IInputManager::getSingletonPtr());
}

QtInputManager::QtInputManager(void):_lastX(-1),_lastY(-1),_relX(0),_relY(0)
{
	
}
QtInputManager::~QtInputManager(void)
{
	
}

void QtInputManager::keyPressEvent(QKeyEvent* evt)
{

	Orz::ILogManager::getSingleton().unicode(L"¼üÅÌÊÂ¼þ");
	unsigned int text = 0;
	if(evt->text().size() == 1)
	{
		text = evt->text().at(0).unicode();
	}
	Orz::KeyEvent ke(QtKC2OrzKC(evt->key()), text);
	_keyPressed(ke);
}


	
void QtInputManager::keyReleaseEvent(QKeyEvent* evt)
{
	
	unsigned int text = 0;
	if(evt->text().size() == 1)
	{
		text = evt->text().at(0).unicode();
	}
	Orz::KeyEvent ke(QtKC2OrzKC(evt->key()), text);
	_keyReleased(ke);
}



void QtInputManager::mousePressEvent(QMouseEvent* evt)
{
	
	Orz::MouseEvent me(_relX, _relY, evt->x(), evt->y(), QtMB2OrzMB(evt->button()));
	_mousePressed(me);
	
}



void QtInputManager::mouseReleaseEvent(QMouseEvent* evt)
{
		Orz::MouseEvent me(_relX, _relY, evt->x(), evt->y(), QtMB2OrzMB(evt->button()));
		_mouseMoved(me);
}

void QtInputManager::mouseMoveEvent(QMouseEvent* evt)
{
	if(_lastX != -1)
	{
		_relX = evt->x() - _lastX;
	}
	if(_lastY != -1)
	{
		_relY = evt->y() - _lastY;
	}
	
	
	Orz::MouseEvent me(_relX, _relY, evt->x(), evt->y(), 0);
	_mouseMoved(me);
	_lastX = evt->x();
	_lastY = evt->y();
}
void QtInputManager::wheelEvent(QWheelEvent* evt)
{
	Orz::MouseEvent me(_relX, _relY, evt->x(), evt->y(), evt->delta());
	_mouseMoved(me);
}
