#ifndef __Orz_ControllerQt_OrzWindow_h__
#define __Orz_ControllerQt_OrzWindow_h__
#include <QWidget>

class OrzWindow :public QWidget
{
	Q_OBJECT
public:
	OrzWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OrzWindow();

	
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);
 
	void resizeEvent(QResizeEvent* evt);
	/*void createLogic();
	LogicConfiger::ConfigerPtr getLogic()
	bool init(void);
	void shutdown(void);*/
private:
	//(new LogicConfiger::Configer());
};


#endif
