#ifndef __Orz_Trigger_h__
#define __Orz_Trigger_h__
#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
#include <orz/Toolkit_Base/EventSystem.h>
#include "TriggerConditions.h"
#include "TriggerEventHandler.h"
#include "TriggerAction.h"
namespace Orz
{
typedef boost::shared_ptr<TriggerConditions>  TriggerConditionsPtr;
typedef boost::shared_ptr<TriggerAction>  TriggerActionPtr;
class _OrzToolkitTriggerExport Trigger:public TriggerEventHandlerCallback
{
protected:
	typedef std::pair<TriggerEventHandlerPtr, EventWorld *> EventHandlerPair;
	typedef std::vector<EventHandlerPair> EventHandlerList;
	typedef std::vector<TriggerConditionsPtr> ConditionsList;
	typedef std::vector<TriggerActionPtr> ActionList;
public:
	Trigger(int id);
	virtual ~Trigger(void);

	
	void addEventHandler(TriggerEventHandlerPtr eh, EventWorld * world);
	void addAction(TriggerActionPtr action);

	
	void addConditions(TriggerConditionsPtr conditions);
	void clearEventHandler(void);
	void enable(void);
	void disable(void);

private:

	void execute(Event * evt);
	void _action(Event * evt);
	void _start(void);
	void _stop(void);
	const int _id;
	EventHandlerList _eventHandlers;
	ConditionsList _conditionses;
	ActionList _actions;
	bool _isEnable;
};
typedef boost::shared_ptr<Trigger>  TriggerPtr;
}


#endif