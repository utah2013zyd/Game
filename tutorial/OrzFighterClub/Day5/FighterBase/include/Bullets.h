
#ifndef __Orz_FighterBase_Bullet_h__
#define __Orz_FighterBase_Bullet_h__
#include "FighterBaseConfig.h"
#include <orz/Framework_Base/System/ISoundManager.h>
namespace Orz
{

	//3子弹类型
class _FighterBaseExport Bullet
	{
	public:
		class BulletInfo
		{
		public:
			BulletInfo(float power, Ogre::MovableObject * object, int fighterID);
			float getPower(void) const;
			int getFighterID(void) const;
			const Ogre::MovableObject * getObject(void) const
			{
				return _object;
			}

		private:
			const float _power;
			const Ogre::MovableObject * _object;
			const int _fighterID;
		};
		Bullet(Ogre::SceneManager * sm, int query = 0x2);
		~Bullet(void);
		
		//3更新子弹
		void update(void);
		//3检查子弹是否被使用,返回true就是正在被使用中
		bool active(void);
		//3开火,第一个参数表示开火位置,第二个参数表明开火时的力量
		void fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power);
		
		//3检查子弹是否射中物体
		bool touch(void);
		//3执行爆炸动作
		void burst(void);

		//3检查是否飞行中(激活且没有爆炸)
		bool flying(void);
		BulletInfo getInfo(int id){return BulletInfo(_power, _object, id);}
		inline Ogre::MovableObject * getObject(void){return _object;}
		inline float getPower(void) const{return _power;}
	private:
		
		Ogre::Vector3 _force;
		Ogre::SceneNode * _node;
		Ogre::Entity * _ent;
		bool _active;
		Ogre::SceneManager * _sm;

		//4用于场景查询
		Ogre::SphereSceneQuery * _query;
		//4记录被击中敌人ID
		//uint32 _fighterID;

		//5这里添加一个音频对象的智能指针，用于管理音效
		Orz::SoundPlayerPtr _sound;

		float _power;
		Ogre::MovableObject * _object;

	};

//3子弹管理器
	class BulletManager
	{
		typedef boost::shared_ptr<Bullet> BulletPtr;
		typedef std::vector<BulletPtr> BulletPtrList;
	public:
		BulletManager(Ogre::SceneManager * sm, int n, int query = 0x2);
		//3更新所有激活子弹
		void update(void);
		//3检查是否击中物体，如果集中，自动爆炸。
		Bullet * touch(void);
		~BulletManager(void);
		
		//3开火，从已有子弹列表中选择一个没有激活的子弹，调用其fire函数,第一个参数表示开火位置,第二个参数表明开火时的力量
		bool fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power);
		
	private:
		//子弹队列
		BulletPtrList _bullets;

	};
	

		


	

}

#endif
