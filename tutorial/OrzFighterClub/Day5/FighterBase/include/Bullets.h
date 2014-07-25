
#ifndef __Orz_FighterBase_Bullet_h__
#define __Orz_FighterBase_Bullet_h__
#include "FighterBaseConfig.h"
#include <orz/Framework_Base/System/ISoundManager.h>
namespace Orz
{

	//3�ӵ�����
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
		
		//3�����ӵ�
		void update(void);
		//3����ӵ��Ƿ�ʹ��,����true�������ڱ�ʹ����
		bool active(void);
		//3����,��һ��������ʾ����λ��,�ڶ���������������ʱ������
		void fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power);
		
		//3����ӵ��Ƿ���������
		bool touch(void);
		//3ִ�б�ը����
		void burst(void);

		//3����Ƿ������(������û�б�ը)
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

		//4���ڳ�����ѯ
		Ogre::SphereSceneQuery * _query;
		//4��¼�����е���ID
		//uint32 _fighterID;

		//5�������һ����Ƶ���������ָ�룬���ڹ�����Ч
		Orz::SoundPlayerPtr _sound;

		float _power;
		Ogre::MovableObject * _object;

	};

//3�ӵ�������
	class BulletManager
	{
		typedef boost::shared_ptr<Bullet> BulletPtr;
		typedef std::vector<BulletPtr> BulletPtrList;
	public:
		BulletManager(Ogre::SceneManager * sm, int n, int query = 0x2);
		//3�������м����ӵ�
		void update(void);
		//3����Ƿ�������壬������У��Զ���ը��
		Bullet * touch(void);
		~BulletManager(void);
		
		//3���𣬴������ӵ��б���ѡ��һ��û�м�����ӵ���������fire����,��һ��������ʾ����λ��,�ڶ���������������ʱ������
		bool fire(const Ogre::Vector3 & pos, const Ogre::Vector3 & force, float power);
		
	private:
		//�ӵ�����
		BulletPtrList _bullets;

	};
	

		


	

}

#endif
