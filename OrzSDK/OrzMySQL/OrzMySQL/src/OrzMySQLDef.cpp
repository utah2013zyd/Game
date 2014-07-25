#include <OrzMySQL.h>

extern "C" Orz::IDatabase* dllStartDBPlugin(void)
{
	return new Orz::Mysql::OrzMySQL;
}

extern "C" void dllStopDBPlugin(Orz::IDatabase* _orzMySQL)
{
	if (_orzMySQL)
	{
		delete _orzMySQL;
	}
}
