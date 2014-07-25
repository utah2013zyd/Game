#ifndef __Orz_FrameworkBase_LogicConfiger_ManualBuilder_h__
#define __Orz_FrameworkBase_LogicConfiger_ManualBuilder_h__

#include <orz/Framework_Base/FrameworkBaseConfig.h>
#include <orz/Framework_Base/LogicConfiger/Builder.h>

namespace Orz
{

	namespace LogicConfiger
	{
		class _OrzFrameworkBaseExport ManualBuilder: public Builder
		{
		public:
			ManualBuilder(void);
			virtual ~ManualBuilder(void);
			virtual bool write(Data & data) const;
			void setActiveDirector(const std::string & active);

			void setTheater(const std::string & type, const std::string & name);

			void addDirector(const std::string & type, const std::string & name);
			void addPlugin(const std::string & name);
		private:
			std::vector<std::string> _plugins;
			std::pair<std::string, std::string> _theater;
			std::vector<std::pair<std::string, std::string> > _directors;
			std::string _activeDirector;

		};
	}

}

#endif