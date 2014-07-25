#ifndef __Orz_FCBase_FCKnowledge_h__
#define __Orz_FCBase_FCKnowledge_h__


#include "FCBaseConfig.h"
#include "FCMapInfo.h"
#include <Ogre/Ogre.h>
#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{
class _FCBaseExport FCKnowledge: public Singleton<FCKnowledge>
{
public:
	FCKnowledge(void);
	virtual ~FCKnowledge(void);
	FCMapInfo & mapInfo(void);
	void setSceneName(const std::string sceneName);
	void setDirectorName(const std::string directorName);
	void setPlayerPosition(const Ogre::Vector3 pos);
	std::string getSceneName();
	std::string getDirectorName();
	Ogre::Vector3 getPlayerPosition();
private:
	FCMapInfo _map;
	std::string _sceneName;
	std::string _directorName;
	Ogre::Vector3 _playerPos;
};


typedef  boost::shared_ptr<FCKnowledge> FCKnowledgePtr;
}

#endif