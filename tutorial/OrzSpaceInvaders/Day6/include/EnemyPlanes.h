#ifndef __Orz_Tutorial_EnemyPlanes_h__
#define __Orz_Tutorial_EnemyPlanes_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{

	//(Day3)敌人,统一管理
	class EnemyPlanes : public Singleton<EnemyPlanes>//(Day5)这里我们使用了单件工具，这基本和Ogre里面相同（就是抄过来的）
	{

		//(Day3)boost提供的具有良好接口的数组对象
		typedef boost::array< Ogre::SceneNode *, 12> PlanesType;
	public:
		EnemyPlanes(Ogre::SceneManager * sm);

		~EnemyPlanes(void);
	
	
		
		void attackEnemy(uint32 id);
		void update(void);
	private:
		
		//(Day3)作为整个敌人的根节点
		Ogre::SceneNode * _all;
		//(Day3)敌人节点
		PlanesType _planes;
		//(Day3)用做时间记录
		float _time;


	};
	

		


	

}

#endif
