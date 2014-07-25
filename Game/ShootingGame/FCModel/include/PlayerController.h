#include "FCConfig.h"

#include "FCFighter.h"
#include <orz/Toolkit_Base/FSMLogic.h>
#pragma once
namespace Orz
{

class PlayerController :
	public KeyListener, public MouseListener, public Actor
{
public:
	PlayerController(const std::string & name = IDManager::BLANK, const std::string & fighterName = IDManager::BLANK);
	~PlayerController(void);

	void doEnable(void){}
	void doDisable(void){}
	void doFrame(void){}
	void doExecute(Event *evt){}


	bool onMousePressed(const MouseEvent & evt);
	bool onMouseReleased(const MouseEvent & evt);
	bool onMouseMoved(const MouseEvent & evt);

	bool onKeyPressed(const Orz::KeyEvent &evt);
	bool onKeyReleased(const Orz::KeyEvent &evt);

private:
	std::string _fighterName;


};

		//ͬʱΪ����ϲ����ϵ�������ṩ����Ӧ�ĵ��ݹ����ࡣ����ϵͳ������������Ӧ��
	class PlayerControllerFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}