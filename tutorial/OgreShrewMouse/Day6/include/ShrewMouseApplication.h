#ifndef __ShrewMouseApplication_H__
#define __ShrewMouseApplication_H__
#include "ExampleApplication.h"
class ShrewMouseManager;
class ShrewMouseApplication : public ExampleApplication
{
public:
    ShrewMouseApplication(void) ;
	~ShrewMouseApplication(void);

protected:

    // Just override the mandatory create scene method
    void createScene(void);
	virtual void createFrameListener(void);
	
private:
	ShrewMouseManager * _miceManager;
};

#endif

