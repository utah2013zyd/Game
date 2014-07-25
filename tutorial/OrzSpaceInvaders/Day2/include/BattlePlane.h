#ifndef __Orz_Tutorial_BattlePlane_h__
#define __Orz_Tutorial_BattlePlane_h__
#include <orz/Framework_Base/System/IInputManager.h>

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
namespace Orz
{

	//(Day2)ս��������ʵ�ּ�����Ϣ�����ӿ�
	class BattlePlane : public KeyListener
	{
	public:
		BattlePlane(Ogre::SceneManager * sm);

		~BattlePlane(void);

	
		//(Day2)�����̰�������
		bool onKeyPressed(const KeyEvent & evt);
		
		//(Day2)�����̰����ͷ�
		bool onKeyReleased(const KeyEvent & evt);
		
		Ogre::Vector3 getPosition(){return _node->getPosition();} 
		//(Day2)����
		void update(void);
	private:


		Ogre::SceneNode * _node;
		bool _leftDown;
		bool _rightDown;
	};
	

		


	

}

#endif
