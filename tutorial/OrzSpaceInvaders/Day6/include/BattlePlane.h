#ifndef __Orz_Tutorial_BattlePlane_h__
#define __Orz_Tutorial_BattlePlane_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/System/IInputManager.h>
#include "Bullets.h"
namespace Orz
{

	//(Day2)战斗机对象，实现键盘消息监听接口
	class BattlePlane : public KeyListener
	{
	public:
		BattlePlane(Ogre::SceneManager * sm);

		~BattlePlane(void);

	
		//(Day2)当键盘按键按下
		bool onKeyPressed(const KeyEvent & evt);
		
		//当键盘按键释放
		bool onKeyReleased(const KeyEvent & evt);
		
		//更新
		void update(void);
	private:
		
		Ogre::SceneNode * _node;
		bool _leftDown;
		bool _rightDown;
		Ogre::ParticleSystem * _thrusters;
		//(Day4)子弹管理器
		BulletManager _bulletManager;

	};
	

		


	

}

#endif
