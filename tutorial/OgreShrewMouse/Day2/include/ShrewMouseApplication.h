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
	//����ϵͳ�ص�����������
    void createScene(void);
	//�����ṩ�������Ҫ��FrameListener���͡�
	virtual void createFrameListener(void);
private:
	//�����ṩһ��������������Э�����еĵ���
	ShrewMouseManager * _miceManager;
};

#endif

