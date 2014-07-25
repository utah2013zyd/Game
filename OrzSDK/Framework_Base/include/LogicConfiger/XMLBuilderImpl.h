#ifndef __Orz_FrameworkBase_LogicConfiger_XMLBuilderImpl_h__
#define __Orz_FrameworkBase_LogicConfiger_XMLBuilderImpl_h__


#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include "LogicConfiger/Data.h"
class TiXmlElement;
namespace Orz
{
	namespace LogicConfiger
	{
		class _OrzToolkitBasePrivate XMLBuilderImpl
		{



		public:
			XMLBuilderImpl(const std::string & filename);
			~XMLBuilderImpl(void);


			bool write(Data & data) const;

		private:
			bool readPlugin(TiXmlElement*  element, Data & data) const;
			bool readTheater(TiXmlElement*  element, Data & data) const;
		private:
			std::string _filename;

		};

	}


}

#endif