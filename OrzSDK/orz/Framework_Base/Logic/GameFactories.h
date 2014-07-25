#ifndef __Orz_FrameworkBase_GameFactories_h__
#define __Orz_FrameworkBase_GameFactories_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Toolkit_Base/IDManager/IDManager.h>

namespace Orz{

class GameFactoriesImpl;
template class _OrzFrameworkBaseExport boost::scoped_ptr<GameFactoriesImpl>;
class _OrzFrameworkBaseExport GameFactories: private boost::noncopyable
{
private:
	GameFactories(void);
public:

	~GameFactories(void);
	
	
	///添加DirectorFactory（导演工厂）
	void addDirectorFactory(DirectorFactory * factory);
	///添加SceneFactory（场景工厂）
	void addSceneFactory(SceneFactory * factory);
	///添加ActorFactory（角色工厂）
	void addActorFactory(ActorFactory * factory);
	///添加TheaterFactory（大厅工厂）
	void addTheaterFactory(TheaterFactory * factory);
	


	
	inline void addFactory(DirectorFactory * factory){addDirectorFactory(factory);}
	inline void addFactory(SceneFactory * factory){addSceneFactory(factory);}
	inline void addFactory(ActorFactory * factory){addActorFactory(factory);}
	inline void addFactory(TheaterFactory * factory){addTheaterFactory(factory);}


	
	///移除DirectorFactory（导演工厂）
	void removeDirectorFactory(DirectorFactory * factory);
	///移除SceneFactory（场景工厂）
	void removeSceneFactory(SceneFactory * factory);
	///移除ActorFactory（角色工厂）
	void removeActorFactory(ActorFactory * factory);
	///移除TheaterFactory（大厅工厂）
	void removeTheaterFactory(TheaterFactory * factory);



	inline void removeFactory(DirectorFactory * factory){removeDirectorFactory(factory);}
	inline void removeFactory(SceneFactory * factory){removeSceneFactory(factory);}
	inline void removeFactory(ActorFactory * factory){removeActorFactory(factory);}
	inline void removeFactory(TheaterFactory * factory){removeTheaterFactory(factory);}



	/**
	创建Director实体
	@param
          typeName 创建Director实体的类型
	@param
		  name 创建Director实体的实力名称，往往作为IDManager的管理名称
	@param
		  parameter 一个创建参数的列表 通过 NameValueList 类型实现
    @returns
          返回指向Director实体的智能指针
	*/
	DirectorPtr createDirector(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);

	/**
	创建Scene实体
	@param
          typeName 创建Scene实体的类型
	@param
		  name 创建Scene实体的实力名称，往往作为IDManager的管理名称
	@param
		  parameter 一个创建参数的列表 通过 NameValueList 类型实现
    @returns
          返回指向Scene实体的智能指针
	*/
	ScenePtr createScene(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);

	/**
	创建Actor实体
	@param
          typeName 创建Actor实体的类型
	@param
		  name 创建Actor实体的实力名称，往往作为IDManager的管理名称
	@param
		  parameter 一个创建参数的列表 通过 NameValueList 类型实现
    @returns
          返回指向Actor实体的智能指针
	*/
	ActorPtr createActor(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);

	/**
	创建Theater实体
	@param
          typeName 创建Theater实体的类型
	@param
		  name 创建Director实体的实力名称，往往作为IDManager的管理名称
	@param
		  parameter 一个创建参数的列表 通过 NameValueList 类型实现
    @returns
          返回指向Theater实体的智能指针
	*/
	TheaterPtr createTheater(const std::string & typeName, const std::string & name = IDManager::BLANK, NameValueList * parameter = NULL);//with world

public:
	///得到单件实例引用
	static GameFactories & getInstance();
	
	///得到单件实例指针
	static GameFactories * getInstancePtr();

private:
	boost::scoped_ptr<GameFactoriesImpl> _impl;
};




}


#include <orz/Framework_Base/logic/LogicTools.h>
#endif