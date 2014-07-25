#ifndef __Orz_FC_XMLFighterLoader_h__
#define __Orz_FC_XMLFighterLoader_h__
#include "FCConfig.h"


namespace Orz
{

	class XMLFighterLoader 
	{
	public:
		XMLFighterLoader();
		virtual ~XMLFighterLoader(void);
		bool load(const std::string & fileName);
		const std::string & getFighter1(void) const;
		const std::string & getFighter2(void) const;
	private:
		std::string _fighter1;
		std::string _fighter2;
		static const std::string FIGHTER;
		static const std::string FIGHTER1;
		static const std::string FIGHTER2;
		static const std::string NAME;
	
	};
	

		


}

#endif 