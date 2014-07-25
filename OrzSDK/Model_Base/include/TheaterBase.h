#ifndef __Orz_ModelBase_TheaterBase_h__
#define __Orz_ModelBase_TheaterBase_h__
#include "./ModelBaseConfig.h"
#include <orz/Framework_Base/Logic.h>
#include <orz/Toolkit_Base/EventSystem.h>
#include <orz/Toolkit_Base/FSMLogic.h>

namespace Orz
{

	/*class TheaterBase;
	class GameLogic : public FSMLogic<GameLogic, TheaterBase>::Logic 
	{
	public:
		
	};*/
	class TheaterBase : public Theater//, private FSMLogic<GameLogic, TheaterBase>
	{
	public:
		TheaterBase(const std::string & name = IDManager::BLANK);
		virtual ~TheaterBase(void);
		virtual void doActiveDirector(DirectorPtr director);
		virtual void doEnable(void);
		virtual void doDisable(void);
	private:
		DirectorPtr _activeDirector;
	};
	
	class TheaterBaseFactory: public IFactory<Theater>
	{
	public:

		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 

