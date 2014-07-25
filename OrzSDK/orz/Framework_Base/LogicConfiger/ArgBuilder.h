#ifndef __Orz_FrameworkBase_LogicConfiger_ArgBuilder_h__
#define __Orz_FrameworkBase_LogicConfiger_ArgBuilder_h__


#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Framework_Base/LogicConfiger/Builder.h>

namespace Orz
{

	namespace LogicConfiger
	{

		class ArgBuilderImpl;
		template class _OrzFrameworkBaseExport boost::scoped_ptr<ArgBuilderImpl>;
		class _OrzFrameworkBaseExport ArgBuilder: public Builder
		{
		public:
			ArgBuilder(int argc, char **argv);
			virtual ~ArgBuilder(void);
			virtual bool write(Data & data) const;


		private:
			boost::scoped_ptr<ArgBuilderImpl> _impl;
		};

	}

}


#endif
