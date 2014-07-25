//#ifndef __Orz_SimpleTrigger_h__
//#define __Orz_SimpleTrigger_h__
//#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
//
//#include "Trigger.h"
//namespace Orz
//{
//
//class _OrzToolkitTriggerExport SimpleTrigger: public Trigger
//{
//public:
//	SimpleTrigger(int id = 0, TriggerConditionsPtr conditions = TriggerConditionsPtr() ):_id(id)
//	{
//		init(conditions);
//	}
//	SimpleTrigger(int id, TriggerFunctor functor)
//	{
//		init(TriggerConditionsPtr(new TriggerConditionsProxy(functor)));
//	}
//	virtual ~SimpleTrigger(void);
//	void doEnable(void);
//	void doDisable(void);
//	void doExecute(Event * evt);
//	bool conditions(void);
//	void triggering(Event * evt);
//private:
//	void init(TriggerConditionsPtr conditions);
//	TriggerConditionsPtr _conditions;
//	int _id;
//};
//
//}
//
//
//#endif