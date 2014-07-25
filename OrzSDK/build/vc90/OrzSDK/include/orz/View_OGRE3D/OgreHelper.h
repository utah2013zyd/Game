#ifndef __Orz_ViewOgre_OgreHelper_h__
#define __Orz_ViewOgre_OgreHelper_h__
#include <orz/View_OGRE3D/ViewOgreConfig.h>
#pragma warning(push)
#pragma warning(disable:4819)
#include <Ogre/Ogre.h>
#pragma warning(pop)


namespace Orz{



class OgreHelper
{
public:
	inline OgreHelper(Ogre::SceneManager * sceneMgr);
	inline ~OgreHelper(void);

	inline Ogre::SceneNode * createSceneNode(void);
	inline Ogre::SceneNode * createSceneNode(const Ogre::String& name);
	inline Ogre::Entity* createEntity(const Ogre::String& entityName, const Ogre::String& meshName);

	inline void destroyAllSceneNode(void);
	inline void destroyAllEntityNode(void);
	inline void destroyAll(void);


	inline Ogre::SceneNode* createChildSceneNode(Ogre::SceneNode * sn, const Ogre::Vector3& translate = Ogre::Vector3::ZERO, const Ogre::Quaternion& rotate = Ogre::Quaternion::IDENTITY );
	inline Ogre::SceneNode* createChildSceneNode(Ogre::SceneNode * sn, const Ogre::String& name, const Ogre::Vector3& translate = Ogre::Vector3::ZERO, const Ogre::Quaternion& rotate = Ogre::Quaternion::IDENTITY);



private:
    Ogre::SceneManager * _sceneMgr;
	std::vector<Ogre::SceneNode *> _nodes;
	std::vector<Ogre::Entity *> _entities;
};



OgreHelper::OgreHelper(Ogre::SceneManager * sceneMgr):_sceneMgr(sceneMgr)
{

}
OgreHelper::~OgreHelper(void)
{
	 destroyAll();
}

Ogre::SceneNode * OgreHelper::createSceneNode(void)
{
	Ogre::SceneNode * sn = _sceneMgr->createSceneNode();
	_nodes.push_back(sn);
	return sn;
}
Ogre::SceneNode * OgreHelper::createSceneNode(const std::string& name)
{
	Ogre::SceneNode * sn = _sceneMgr->createSceneNode(name);
	_nodes.push_back(sn);
	return sn;
}
Ogre::Entity* OgreHelper::createEntity(const std::string& entityName, const std::string& meshName)
{
	Ogre::Entity * e = _sceneMgr->createEntity(entityName, meshName);
	_entities.push_back(e);
	return e;
}


void OgreHelper::destroyAllSceneNode(void)
{
    BOOST_FOREACH(Ogre::Entity * e, _entities)
	{
		_sceneMgr->destroyEntity(e);
	}
	_entities.clear();
}
void OgreHelper::destroyAllEntityNode(void)
{
	BOOST_FOREACH(Ogre::SceneNode * sn, _nodes)
	{
		_sceneMgr->destroySceneNode(sn);
	}
	_nodes.clear();
}
void OgreHelper::destroyAll(void)
{
	destroyAllSceneNode();
	destroyAllEntityNode();
}



Ogre::SceneNode* OgreHelper::createChildSceneNode(Ogre::SceneNode * sn, const Ogre::Vector3& translate, const Ogre::Quaternion& rotate)
{
	Ogre::SceneNode * node = sn->createChildSceneNode(translate, rotate);
	_nodes.push_back(node);
	return node;

}
Ogre::SceneNode* OgreHelper::createChildSceneNode(Ogre::SceneNode * sn, const Ogre::String& name, const Ogre::Vector3& translate, const Ogre::Quaternion& rotate)
{
	Ogre::SceneNode * node = sn->createChildSceneNode(name, translate, rotate);
	_nodes.push_back(node);
	return node;
}



}



//\
// \
//SYSTEMUNIT_FACTORY(Interface, OgreGraphicsManager) 

#endif