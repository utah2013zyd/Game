
#ifndef __Orz_TriggerEventHandlerPolling_h__
#define __Orz_TriggerEventHandlerPolling_h__
#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
#include <orz/Toolkit_Trigger/TriggerEventHandler.h>

namespace Orz
{

class _OrzToolkitTriggerExport TriggerEventHandlerPolling: public TriggerEventHandler
{
public:
	void doEnable();
	void doExecute(Event * evt);
};


}


#endif
