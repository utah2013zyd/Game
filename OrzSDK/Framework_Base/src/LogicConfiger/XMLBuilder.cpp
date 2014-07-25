#include <orz/Framework_Base/LogicConfiger/XMLBuilder.h>
#include "LogicConfiger/XMLBuilderImpl.h"
using namespace Orz;
using namespace LogicConfiger;

XMLBuilder::XMLBuilder(const std::string & filename)
:_impl(new XMLBuilderImpl(filename))
{

}
XMLBuilder::~XMLBuilder(void)
{

}

bool XMLBuilder::write(Data & data) const
{
	return _impl->write(data);
}