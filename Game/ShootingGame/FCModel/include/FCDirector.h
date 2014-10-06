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
	//�����̳������ṩ��Director�����ݣ����������ǵ���������Ϸ
	class FCDirector : public Director, private FSMLogic<FCDirector, FCDirectorLogic>
	{
	public:
		FCDirector(const std::string & name = IDManager::BLANK);
		virtual ~FCDirector(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1��ÿһ��frame����
		void doExecute(Orz::Event *evt);
	private:
		boost::scoped_ptr<CameraController> _camCtrl;
		ScenePtr _scene;
		FCKnowledgePtr _knowledge;
		DataLoggerPtr _dataLogger;
	};
	

		

	//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ���ݡ�
	class FCDirectorFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 