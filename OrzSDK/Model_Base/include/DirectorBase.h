#ifndef __Orz_ModelBase_DirectorBase_h__
#define __Orz_ModelBase_DirectorBase_h__
#include "./ModelBaseConfig.h"
#include <orz/Framework_Base/Logic.h>
#include <orz/Toolkit_Base/EventSystem.h>
#include <orz/Toolkit_Base/FSMLogic.h>

#include <orz/View_Fmod/FmodSoundManager.h>
namespace Orz
{

	/*class TheaterBase;
	class GameLogic : public FSMLogic<GameLogic, TheaterBase>::Logic 
	{
	public:
		
	};*/
	class DirectorBase : public Director//, private FSMLogic<GameLogic, TheaterBase>
	{
	public:
		DirectorBase(const std::string & name = IDManager::BLANK);
		virtual ~DirectorBase(void);
		virtual void doEnable(void);
		virtual void doDisable(void);
	private:
		//5这里提供一个背景音乐的智能指针
		SoundPlayerPtr _bgm;

	};
	
	class DirectorBaseFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 

