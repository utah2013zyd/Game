//#ifndef __Orz_ListTrigger_h__
//#define __Orz_ListTrigger_h__
//#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
//#include <orz/Toolkit_Trigger/Trigger.h>
//namespace Orz
//{
//
//class _OrzToolkitTriggerExport ListTrigger:public Trigger
//{
//private:
//	typedef std::vector<std::pair<int, TriggerConditionsPtr> > ConditionsList;
//public:
//	ListTrigger(void);
//	virtual ~ListTrigger(void);
//	inline void push_back(int id, const TriggerFunctor & functor)
//	{
//		push_back(id, TriggerConditionsPtr(new TriggerConditionsProxy(functor)));
//	}
//	void push_back(int id, TriggerConditionsPtr conditions);
//	void reset(void);
////	bool trigger(void);
//	
//	void doEnable(void);
//	void doDisable(void);
//
//	
//	void doExecute(Event * evt);
//	bool conditions(void);
//	void triggering(Event * evt);
//private:
//	 ConditionsList _conditions;
//	 ConditionsList::iterator _currTrigger;
//};
//
//}
//
//
//#endif