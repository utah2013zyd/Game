#ifndef __Orz_FrameworkBase_LogicConfiger_XMLBuilder_h__
#define __Orz_FrameworkBase_LogicConfiger_XMLBuilder_h__

#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Framework_Base/LogicConfiger/Builder.h>
//class TiXmlElement;
namespace Orz
{


	namespace LogicConfiger
	{

		
		class XMLBuilderImpl;
		
		template class _OrzFrameworkBaseExport boost::scoped_ptr<XMLBuilderImpl>;
		class _OrzFrameworkBaseExport XMLBuilder: public Builder
		{

		public:
			XMLBuilder(const std::string & filename);
			virtual ~XMLBuilder(void);


			virtual bool write(Data & data) const;
		private:
			boost::scoped_ptr<XMLBuilderImpl> _impl;

		};

	}

}

#endif