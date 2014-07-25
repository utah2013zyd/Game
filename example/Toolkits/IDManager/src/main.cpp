#include <iostream>
#include <orz/Toolkit_Base/IDManager.h>
#include <string>
using namespace Orz;


/**
����������ͨ��ö�ٵķ�ʽ���������ֲ�ͬ�Ķ������ͣ�
��ͬ��������֮�����ʹ����ͬ�����ƣ�
�����һ�������й�����������ͬ���Ƶ�ʵ�������׳�һ���쳣
**/
enum EType
{
	egroup1 = 1,
	egroup2 = 2
};


/**
Foo1���ͼ̳���IDManager���̶���ʵ�����ᱻIDManager����
������Ҫע��������ǲ����˿�����IDManager::BLANK��Ϊʵ�����ַ������ơ�
������ϵͳ�����һ���Զ����ɵ����ƣ���ʵ��������Ͼ��ǡ�@+��ʵ��id����

��������ԭ������û���ͼ��������õ���������@������Ϊ���ַ���
�����ϵͳ�Զ��������Ƴ�ͻ�������������׳�һ���쳣��

����ʹ����etype1���ͣ����ںͺ������Ϊ����
**/
class Foo1: public IDManager
{
public:
	Foo1():IDManager(IDManager::BLANK, egroup1){}

};


/**
Foo2�������Foo1Ψһ����������ṩ���û��Զ����Ƶķ�����
������Ҫָ������������Ҫ�ڹ���ʱ�ƶ����������������ڼ䲻�ܸı䡣
����ʹ����etype2���͡�
**/
class Foo2: public IDManager
{
public:
	Foo2(const std::string & name):IDManager(name, egroup2)
	{}
};


/**
Foo2_Other��Foo2��һ�������࣬ͬʱҲ�̳��˸��������etype2��
**/
class Foo2_Other: public Foo2
{
public:
	Foo2_Other(const std::string & name = IDManager::BLANK):Foo2(name)
	{}
};

int main()
{


	Foo1 object1;//���ﱻ������id:1; name:@1;
	Foo2 object2("object2");
	Foo2_Other object3("object3");
	Foo2 object4(IDManager::BLANK);


	assert(object2.getGroup() == object3.getGroup());

	std::cout<<"object1 ID:"<<object1.getID()<<"; Group:"<<object1.getGroup()<<"; Name:"<<object1.getName()<<std::endl;
	std::cout<<"object2 ID:"<<object2.getID()<<"; Group:"<<object2.getGroup()<<"; Name:"<<object2.getName()<<std::endl;
	std::cout<<"object3 ID:"<<object3.getID()<<"; Group:"<<object3.getGroup()<<"; Name:"<<object3.getName()<<std::endl;
	std::cout<<"object4 ID:"<<object4.getID()<<"; Group:"<<object4.getGroup()<<"; Name:"<<object4.getName()<<std::endl;
	/**
		���Ϊ:

		object1 ID:1; Group:1; Name:@1
		object2 ID:2; Group:2; Name:object2
		object3 ID:3; Group:2; Name:object3
		object4 ID:4; Group:2; Name:@4
	**/





	/**
	������ʾ����δ�һ��ID����������������Ϣ�ط�����
	**/
	int object1_id = object1.getID();

	
	IDManager * p1 = IDManager::getPointer(object1_id);
	assert(p1 == &object1);
	std::cout<<"ID:"<<object1_id<<"; Pointer:" <<p1<<"; Name:"  <<p1->getName()<<"; Group:"<<p1->getGroup()<<std::endl;
	/**
		���Ϊ:
		ID:1; Pointer:00D9FF4C; Name:@1; Group:1
	**/





	/**
	������ʾ����δ����ƺ���������������������Ϣ�ط�����
	**/
	int object3_group = egroup2;
	std::string object3_name("object3");

	int object3_id = IDManager::getTargetID(object3_name, object3_group);
	IDManager * p3 = IDManager::getPointer(object3_id);
	assert(p3 == &object3);
	std::cout<<"Group:"<<object3_group<<"; Name:" <<object3_name<<"; Pointer:"  <<p3<<"; ID:"<<object3_id<<std::endl;
	/**
		���Ϊ:
		Group:2; Name:object3; Pointer:00D9FF2C; ID:3
	**/
	return 0;
}