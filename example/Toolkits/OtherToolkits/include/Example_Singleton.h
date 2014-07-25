#ifndef __Orz_Example_Singleton__
#define __Orz_Example_Singleton__
#include <orz/Toolkit_Base/Singleton.h>

namespace Orz
{

class Foo: public Singleton<Foo>
{
public:
	 virtual void print(void)
	 {
		 std::cout<<"class Foo"<<std::endl;
	 }
};

class FooDerived: public Foo
{
public:

	virtual void print(void)
	{
		std::cout<<"class FooDerived"<<std::endl;
	}
	static FooDerived& getSingleton(void)
	{
		return static_cast<FooDerived&>(Foo::getSingleton());
	}
	static FooDerived* getSingletonPtr(void)
	{
		return static_cast<FooDerived*>(Foo::getSingletonPtr());
	}
};

inline void Example_Singleton(void)
{
	Foo * foo = new Foo();
	{
		Foo::getSingleton().print();
	}
	delete(foo);

	foo = new FooDerived();
	{
		Foo::getSingletonPtr()->print();
		

	}
	delete(foo);
}

}

#endif