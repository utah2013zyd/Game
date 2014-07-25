#include "ShrewMouseManager.h"


ShrewMouseManager::ShrewMouseManager(Ogre::SceneManager * sm):_sm(sm)
{
	using namespace Ogre;



	for(int i=0; i<3; ++i)
	{

		for(int j =0; j<3; ++j)
		{
			//����ģ�͡�ogrehead.mesh����ͨ����������������һ��ʵ�塣
			Entity *ent = _sm->createEntity("head"+Ogre::StringConverter::toString(i+j*3), "ogrehead.mesh");
			//�ڳ����������У��ҵ�����������ڵ��ָ�롣
			SceneNode * rsn = _sm->getRootSceneNode();
			//������ڵ��´���һ���ӽڵ㣬λ����Ogre::Vector3(100 *i, 0, 100 *j)��
			SceneNode * node = rsn->createChildSceneNode(Ogre::Vector3(100 *i, 0, 100 *j));
			//��֮ǰ������ʵ��ҽӵ�����ڵ��ϡ�
			node->attachObject(ent);
		}
	}
}
ShrewMouseManager::~ShrewMouseManager(void)
{
	
}