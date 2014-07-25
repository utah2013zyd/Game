#ifndef __Orz_ControllerQt_LogWindow_h__
#define __Orz_ControllerQt_LogWindow_h__
#include <QTextEdit>

class LogWindow :public QTextEdit
{
	Q_OBJECT
public:
	LogWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LogWindow();


private:

};


#endif
