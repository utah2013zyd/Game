#include <orz/Framework_Base/LogicConfiger/ArgBuilder.h>
#include "LogicConfiger/ArgBuilderImpl.h"
using namespace Orz;

using namespace LogicConfiger;

ArgBuilder::ArgBuilder(int argc, char **argv)
:_impl(new ArgBuilderImpl(argc, argv))
{

}
ArgBuilder::~ArgBuilder(void)
{

}

bool ArgBuilder::write(Data & data) const
{
	return  _impl->write(data);
}
