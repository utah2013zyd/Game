//
//#include <orz/Toolkit_Trigger/ListTrigger.h>
//using namespace Orz;
//
//
//ListTrigger::ListTrigger(void)
//{
//}
//ListTrigger::~ListTrigger(void)
//{
//}
//
//void ListTrigger::push_back(int id, TriggerConditionsPtr conditions)
//{
//	_conditions.push_back(std::make_pair(id, conditions));
//}
//
//void ListTrigger::doEnable(void)
//{
//	reset();
//	_currTrigger->second->enable();
//}
//void ListTrigger::doDisable(void)
//{
//	if(_currTrigger != _conditions.end())
//		_currTrigger->second->disable();
//}
//
//
//void ListTrigger::reset(void)
//{
//	_currTrigger = _conditions.begin();
//}
////bool ListTrigger::trigger(void)
////{
////	assert(_currTrigger != _conditions.end());
////	return (*(*_currTrigger).second)();
////}
////	
//
//bool ListTrigger::conditions(void)
//{
//	assert(_currTrigger != _conditions.end());
//	return (*(*_currTrigger).second)();
//}
//void ListTrigger::triggering(Event * evt)
//{
//	if(conditions())
//	{
//		_callBack->triggerAction((*_currTrigger).first, _userData);
//		_currTrigger->second->disable();
//		++_currTrigger;
//
//		if(_currTrigger != _conditions.end())
//		{
//			_currTrigger->second->enable();
//		}else
//		{
//			if(!_loop)
//			{	
//				stop();
//			}else
//			{
//				restart();
//			}
//
//		}
//	}
//
//}
//void ListTrigger::doExecute(Event * evt)
//{
//	if(isEnable() && isUpdateEvent(evt))
//	{
//		triggering(evt);
//	}
//}