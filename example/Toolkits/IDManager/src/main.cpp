#include <iostream>
#include <orz/Toolkit_Base/IDManager.h>
#include <string>
using namespace Orz;


/**
我们在这里通过枚举的方式定义了两种不同的对象类型，
不同对象类型之间可以使用相同的名称，
如果在一种类型中构建了两个相同名称的实例，会抛出一个异常
**/
enum EType
{
	egroup1 = 1,
	egroup2 = 2
};


/**
Foo1类型继承于IDManager，继而其实例将会被IDManager管理。
这里需要注意的是我们采用了空名称IDManager::BLANK作为实例的字符串名称。
这样做系统会分配一个自动生成的名称，事实这个名称上就是“@+本实例id”。

基于以上原因，如果用户企图给对象采用的名称中用@符号作为首字符，
将会和系统自动生成名称冲突，这样做将会抛出一个异常。

这里使用了etype1类型，用于和后面的作为区别。
**/
class Foo1: public IDManager
{
public:
	Foo1():IDManager(IDManager::BLANK, egroup1){}

};


/**
Foo2和上面的Foo1唯一的区别就是提供了用户自定名称的方法，
这里需要指出对象名称需要在构造时制定，而且在其生命期间不能改变。
这里使用了etype2类型。
**/
class Foo2: public IDManager
{
public:
	Foo2(const std::string & name):IDManager(name, egroup2)
	{}
};


/**
Foo2_Other是Foo2的一个派生类，同时也继承了父类的类型etype2。
**/
class Foo2_Other: public Foo2
{
public:
	Foo2_Other(const std::string & name = IDManager::BLANK):Foo2(name)
	{}
};

int main()
{


	Foo1 object1;//这里被分配了id:1; name:@1;
	Foo2 object2("object2");
	Foo2_Other object3("object3");
	Foo2 object4(IDManager::BLANK);


	assert(object2.getGroup() == object3.getGroup());

	std::cout<<"object1 ID:"<<object1.getID()<<"; Group:"<<object1.getGroup()<<"; Name:"<<object1.getName()<<std::endl;
	std::cout<<"object2 ID:"<<object2.getID()<<"; Group:"<<object2.getGroup()<<"; Name:"<<object2.getName()<<std::endl;
	std::cout<<"object3 ID:"<<object3.getID()<<"; Group:"<<object3.getGroup()<<"; Name:"<<object3.getName()<<std::endl;
	std::cout<<"object4 ID:"<<object4.getID()<<"; Group:"<<object4.getGroup()<<"; Name:"<<object4.getName()<<std::endl;
	/**
		输出为:

		object1 ID:1; Group:1; Name:@1
		object2 ID:2; Group:2; Name:object2
		object3 ID:3; Group:2; Name:object3
		object4 ID:4; Group:2; Name:@4
	**/





	/**
	下面演示了如何从一个ID来索引对象其他信息地方法。
	**/
	int object1_id = object1.getID();

	
	IDManager * p1 = IDManager::getPointer(object1_id);
	assert(p1 == &object1);
	std::cout<<"ID:"<<object1_id<<"; Pointer:" <<p1<<"; Name:"  <<p1->getName()<<"; Group:"<<p1->getGroup()<<std::endl;
	/**
		输出为:
		ID:1; Pointer:00D9FF4C; Name:@1; Group:1
	**/





	/**
	下面演示了如何从名称和类行来索引对象其他信息地方法。
	**/
	int object3_group = egroup2;
	std::string object3_name("object3");

	int object3_id = IDManager::getTargetID(object3_name, object3_group);
	IDManager * p3 = IDManager::getPointer(object3_id);
	assert(p3 == &object3);
	std::cout<<"Group:"<<object3_group<<"; Name:" <<object3_name<<"; Pointer:"  <<p3<<"; ID:"<<object3_id<<std::endl;
	/**
		输出为:
		Group:2; Name:object3; Pointer:00D9FF2C; ID:3
	**/
	return 0;
}