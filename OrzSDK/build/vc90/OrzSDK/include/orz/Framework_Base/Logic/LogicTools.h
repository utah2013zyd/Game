
#ifndef __Orz_FrameworkBase_LogicTools_h__
#define __Orz_FrameworkBase_LogicTools_h__
namespace Orz
{


template<class T>
inline void AddGameFactory(T * factory)
{
	GameFactories::getInstance().addFactory(factory);
}


template<class T>
inline void RemoveGameFactory(T * factory)
{
	GameFactories::getInstance().removeFactory(factory);
}


class GameRegister
{
public:
	GameRegister(void)
	{}
	virtual ~GameRegister(void)
	{}
};
template<class Base, class T>
class GameModelFactory: public Base
{
public:
	GameModelFactory(void)
	{
		AddGameFactory(&_factory);
	}
	virtual ~GameModelFactory(void)
	{
		RemoveGameFactory(&_factory);
	}
private:
	T _factory;
};

template<class TList>
class GameRegisterList :public boost::mpl::inherit_linearly< TList, GameModelFactory<boost::mpl::placeholders::_, boost::mpl::placeholders::_>, GameRegister>::type {};



typedef boost::shared_ptr<Orz::GameRegister> GameRegisterPtr;
}


#endif