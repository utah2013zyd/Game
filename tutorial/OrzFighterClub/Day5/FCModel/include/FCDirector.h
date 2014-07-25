#ifndef __Orz_FC_FCDirector_h__
#define __Orz_FC_FCDirector_h__
#include "FCConfig.h"
namespace Orz
{
	class CameraControler;
	//�����̳������ṩ��Director�����ݣ����������ǵ���������Ϸ
	class FCDirector : public Director
	{
	public:
		FCDirector(const std::string & name = IDManager::BLANK);
		virtual ~FCDirector(void);
		
		void doEnable(void);
		void doDisable(void);
		void doFrame(void);//1��ÿһ��frame����
	private:
		//FCUIPTR _ui;
		boost::scoped_ptr<CameraControler> _camCtrl;
		ScenePtr _scene;
		FCKnowledgePtr _knowledge;
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