
#ifndef __Orz_TriggerAction_h__
#define __Orz_TriggerAction_h__
#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>

namespace Orz
{
class _OrzToolkitTriggerExport TriggerCallback
{
public:
	virtual void triggerAction(int id, Event * evt) = 0;
	virtual ~TriggerCallback(void){}
};


class _OrzToolkitTriggerExport TriggerAction
{
public:
	virtual void operator()(int id, Event * evt) = 0;
private:
	TriggerCallback * _callBack;
	void * _userData;
};

class _OrzToolkitTriggerExport TriggerActionCallback:public TriggerAction
{
public:
	TriggerActionCallback(TriggerCallback * callback):_callback(callback) 
	{
		assert(callback);
	}
	
	virtual void operator()(int id, Event * evt)
	{
		_callback->triggerAction(id, evt);
	}
private:
	TriggerCallback * _callback;
};

}


#endif

