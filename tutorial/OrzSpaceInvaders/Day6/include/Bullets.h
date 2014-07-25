#ifndef __Orz_Tutorial_Bullet_h__
#define __Orz_Tutorial_Bullet_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Framework_Base/System/ISoundManager.h>
namespace Orz
{

	//(Day4)子弹类型
	class Bullet
	{
	public:
		
		Bullet(Ogre::SceneManager * sm);
		~Bullet(void);
		
		//(Day4)更新子弹
		void update(void);
		//(Day4)检查子弹是否被使用,返回true就是正在被使用中
		bool active(void);
		//(Day4)开火,第一个参数表示开火位置,第二个参数表明开火时的力量
		void fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force);
		
		//(Day4)检查子弹是否射中物体
		bool touch(void);
		//(Day4)执行爆炸动作
		void burst(void);

		//(Day4)检查是否飞行中(激活且没有爆炸)
		bool flying(void);

		inline uint32 getEnemyID(void){return _enemyID;}
	private:
		Ogre::Vector3 _force;
		Ogre::SceneNode * _node;
		Ogre::Entity * _ent;
		bool _active;
		Ogre::SceneManager * _sm;

		//(Day5)用于场景查询
		Ogre::SphereSceneQuery * _query;
		//(Day5)记录被击中敌人ID
		uint32 _enemyID;

		//(Day6)这里添加一个音频对象的智能指针，用于管理音效
		Orz::SoundPlayerPtr _sound;

	};

//(Day4)子弹管理器
	class BulletManager
	{
		typedef boost::shared_ptr<Bullet> BulletPtr;
		typedef std::vector<BulletPtr> BulletPtrList;
	public:
		BulletManager(Ogre::SceneManager * sm, int n);
		//(Day4)更新所有激活子弹
		void update(void);
		//(Day4)检查是否击中物体，如果集中，自动爆炸。
		Bullet * touch(void);
		~BulletManager(void);
		
		//(Day4)开火，从已有子弹列表中选择一个没有激活的子弹，调用其fire函数,第一个参数表示开火位置,第二个参数表明开火时的力量
		bool fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force);
		
	private:
		//子弹队列
		BulletPtrList _bullets;

	};
	

		


	

}

#endif
