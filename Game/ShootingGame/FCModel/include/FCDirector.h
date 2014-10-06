#ifndef __Orz_FC_FCDirector_h__
#define __Orz_FC_FCDirector_h__
#include "FCConfig.h"
#include "FCDirectorLogic.h"
#include <orz/Toolkit_Base/FSMLogic.h>

//#include "FCUI.h"
namespace Orz
{
	class CameraController;
	class PlayerControler;
	//这个类继承与框架提供的Director（导演），其作用是调控整个游戏
	class FCDirector : public Director, private FSMLogic<FCDirector, FCDirectorLogic>
	{
	public:
		FCDirector(const std::string & name = IDManager::BLANK);
		virtual ~FCDirector(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1在每一个frame调用
		void doExecute(Orz::Event *evt);
	private:
		boost::scoped_ptr<CameraController> _camCtrl;
		ScenePtr _scene;
		FCKnowledgePtr _knowledge;
		DataLoggerPtr _dataLogger;
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应导演。
	class FCDirectorFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 