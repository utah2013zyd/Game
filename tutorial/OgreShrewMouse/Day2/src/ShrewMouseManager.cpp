#include "ShrewMouseManager.h"


ShrewMouseManager::ShrewMouseManager(Ogre::SceneManager * sm):_sm(sm)
{
	using namespace Ogre;



	for(int i=0; i<3; ++i)
	{

		for(int j =0; j<3; ++j)
		{
			//采用模型“ogrehead.mesh”，通过场景管理器创建一个实体。
			Entity *ent = _sm->createEntity("head"+Ogre::StringConverter::toString(i+j*3), "ogrehead.mesh");
			//在场景管理器中，找到场景最根部节点的指针。
			SceneNode * rsn = _sm->getRootSceneNode();
			//在这个节点下创建一个子节点，位置是Ogre::Vector3(100 *i, 0, 100 *j)。
			SceneNode * node = rsn->createChildSceneNode(Ogre::Vector3(100 *i, 0, 100 *j));
			//把之前建立的实体挂接到这个节点上。
			node->attachObject(ent);
		}
	}
}
ShrewMouseManager::~ShrewMouseManager(void)
{
	
}