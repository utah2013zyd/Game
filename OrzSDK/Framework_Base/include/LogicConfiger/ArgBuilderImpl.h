#ifndef __Orz_FrameworkBase_LogicConfiger_ArgBuilderImpl_h__
#define __Orz_FrameworkBase_LogicConfiger_ArgBuilderImpl_h__


#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include "LogicConfiger/Data.h"
namespace Orz
{
namespace LogicConfiger
{
class _OrzToolkitBasePrivate ArgBuilderImpl
{
	

public:
	ArgBuilderImpl(int argc, char **argv);
	~ArgBuilderImpl(void);

	bool write(Data & data) const;
private:
	int _argc;
	char ** _argv;
};

}

}


#endif
