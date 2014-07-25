
#include <orz/Toolkit_Trigger/TriggerEventHandler.h>

using namespace Orz;

TriggerEventHandler::TriggerEventHandler(void)
:_callback(NULL)
{}
void TriggerEventHandler::stop(void)
{
	EventWorld * world = getWorld();
	if(world)
	{
		world->goOut(shared_from_this());
	}
}

void TriggerEventHandler::doExecute(Event * evt)
{
	if(isEnable())
	{
		assert(evt);
		_callback->execute(evt);
	}
}
void TriggerEventHandler::setCallback(TriggerEventHandlerCallback * callback)
{
	_callback = callback;
}