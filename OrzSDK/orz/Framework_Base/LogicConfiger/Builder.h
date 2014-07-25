#ifndef __Orz_FrameworkBase_LogicConfig_Builder_h__
#define __Orz_FrameworkBase_LogicConfig_Builder_h__

#include <orz/Framework_Base/FrameworkBaseConfig.h>

namespace Orz
{
	namespace LogicConfiger
	{

		class Data;
		typedef boost::shared_ptr<Data> DataPtr;

		class _OrzFrameworkBaseExport Builder
		{
		public:
			virtual bool write(Data & data) const = 0;
			virtual ~Builder(){}
		};


	}

}


#endif 