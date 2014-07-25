#ifndef __Orz_ControllerQt_QtInputManager_h__
#define __Orz_ControllerQt_QtInputManager_h__

#include <orz/Framework_Base/System/IInputManager.h>

#include <QWidget>

#include "QtKeyMap.h"

#include <orz/Framework_Base/System/SystemInterface.h>
class QtInputManager:public Orz::IInputManager
{
	
public:
	static QtInputManager& getSingleton(void);
	static QtInputManager* getSingletonPtr(void);

	QtInputManager(void);
	virtual ~QtInputManager(void);


	//消息响应
	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);
	
	
private:
	int _lastX;
	int _lastY;
	int _relX;
	int _relY;
	
};
namespace Orz
{
SYSTEMUNIT_BEGIN(QtInputManager) 
SYSTEMUNIT_END(QtInputManager) 
}

#endif