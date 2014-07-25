
#include <orz/Toolkit_Trigger/TriggerEventHandlerPolling.h>

using namespace Orz;


void TriggerEventHandlerPolling::doEnable()
{
	enableUpdate();
}
void TriggerEventHandlerPolling::doExecute(Event * evt)
{
	if(isUpdateEvent(evt))
	{
		TriggerEventHandler::doExecute(evt);
	}
}
