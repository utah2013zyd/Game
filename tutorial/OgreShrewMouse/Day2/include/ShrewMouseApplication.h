#ifndef __ShrewMouseApplication_H__
#define __ShrewMouseApplication_H__
#include "ExampleApplication.h"
class ShrewMouseManager;
class ShrewMouseApplication : public ExampleApplication
{
public:
    ShrewMouseApplication(void);
	~ShrewMouseApplication(void);
protected:
	//用于系统回调来创建场景
    void createScene(void);
	//用于提供框架所需要的FrameListener类型。
	virtual void createFrameListener(void);
private:
	//这里提供一个管理器，用于协调所有的地鼠。
	ShrewMouseManager * _miceManager;
};

#endif

