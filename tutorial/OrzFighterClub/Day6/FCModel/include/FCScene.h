#ifndef __Orz_FC_FCScene_h__
#define __Orz_FC_FCScene_h__
#include "FCConfig.h"
#include "FCMap.h"

namespace Orz
{

	class FCScene : public Scene
	{
	private:
		typedef std::vector<ActorPtr> EnemiesList;
		typedef std::vector<ActorPtr> WallList;
	public:
		FCScene(const std::string & name = IDManager::BLANK);
		virtual ~FCScene(void);
		
		void doEnable(void);
		void doDisable(void);
	private:
		EnemiesList _enemies;
		boost::scoped_ptr<FCMap> _map;
		WallList _walls;
	};
	

		

	//同时为了配合插件体系，我们提供了相应的导演工厂类。用于系统来“生产”相应导演。
	class FCSceneFactory: public IFactory<Scene>
	{
	public:

		virtual const std::string & getTypeName() const;

		virtual pointer_type createInstance(const std::string& instanceName = IDManager::BLANK, parameter_type parameter = NULL);
	};

}

#endif 