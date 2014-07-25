
#ifndef __Orz_TriggerEventHandler_h__
#define __Orz_TriggerEventHandler_h__
#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
namespace Orz
{
class _OrzToolkitTriggerExport TriggerEventHandlerCallback
{
public:
	virtual void execute(Event * evt) = 0;
	virtual ~TriggerEventHandlerCallback(void){}
};


class _OrzToolkitTriggerExport TriggerEventHandler: public EventHandler
{
public:
	TriggerEventHandler(void);
	void stop(void);

	inline bool isEnable(void) const {return isInWorld() && _callback;}
	void doExecute(Event * evt);
	void setCallback(TriggerEventHandlerCallback * callback);
private:
	TriggerEventHandlerCallback * _callback;

};


typedef boost::shared_ptr<TriggerEventHandler>  TriggerEventHandlerPtr;

}


#endif
