#include <orz/Framework_Base/LogicConfiger/LogicFacade.h>
#include "LogicConfiger/LogicFacadeImpl.h"

using namespace Orz;
using namespace LogicConfiger;





LogicFacade::LogicFacade(void):_impl(new LogicFacadeImpl())
{

}
LogicFacade::~LogicFacade(void)
{

}

bool LogicFacade::unload(void)
{
	return _impl->unload();
}
bool LogicFacade::load(void)
{
	return _impl->load();
}

bool LogicFacade::building(const Builder & builder)
{
	return _impl->building(builder);
}

