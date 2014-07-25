
#include <orz/Toolkit_Trigger/SimpleTrigger.h>


//
//SimpleTrigger::~SimpleTrigger(void){}
//
//void SimpleTrigger::init(TriggerConditionsPtr conditions)
//{
//	_conditions = conditions;
//}
//
//
//void SimpleTrigger::doEnable(void)
//{
//	_conditions->enable();
//}
//void SimpleTrigger::doDisable(void)
//{
//	_conditions->disable();
//
//}
//
//bool SimpleTrigger::conditions(void)
//{
//	return (*_conditions)();
//}
//void SimpleTrigger::triggering(Event * evt)
//{
//	if(conditions())
//	{
//		_callBack->triggerAction(_id, _userData);
//		if(!_loop)
//			stop();
//	}
//}
//void SimpleTrigger::doExecute(Event * evt)
//{
//	if(isEnable() && isUpdateEvent(evt))
//	{
//		triggering(evt);
//	}
//}
