#include <iostream>
#include <orz/Toolkit_Base/Variant.h>
#include <string>
using namespace Orz;


/**
用户自定义类型，
注意，所有被Variant管理的类型需要支持拷贝构造函数，否则有可能出错，
这是因为在传递值的过程中可能需要进行拷贝。
*/
class Foo
{
public:
	Foo(int n):_n(n)
	{}
	Foo(const Foo & foo)
	{
		_n= foo._n;
	}
	friend std::ostream & operator << (std::ostream &os, const Foo& f)
	{
		os<<"I am Foo:"<<f._n;
		return os;
	}
private:
	int _n;
};

int main(void)
{


	//int类型被默认的从栈上面构建，这样带来较高的效率
	//被支持从栈上面的类型包括以下
	//char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, int64, uint64, float, double, void * , bool, std::pair<int, int>, std::pair<float, float>
	//具体使用上尽量采用以上类型来处理，这样会带来很高的效率（等同于boost::variant）
	Variant int_var = VariantData<int>::set(19811112);

	//std::string并没有被认为是内建类型，所以仍然从堆上面分配空间
	//事实上这些没有内建支持的类型被转发到boost::any上面进行处理
	Variant str_var = VariantData<std::string>::set("test");
	
	//我们支持任意类型的储存，这就包括用户自定义类型。但是在处理的过程中难免有拷贝过程，所以尽量避免较大对象的传递
	//对于这种对象尽量使用智能指针来处理，然后再通过Variant储存
	Variant obj_var= VariantData<Foo>::set(Foo(12)); 

	//Variant也支持指针的储存，但是需要注意的是，为了效率我们在内部把它转换为void*类型，这也是唯一没有类型安全的状态，
	//如果需要类型安全的指针储存，请采用boost提供的智能指针。
	Variant pt_var= VariantData<Foo*>::set(new Foo(13)); 



	int int_out = VariantData<int>::get(int_var);
	std::string str_out = VariantData<std::string>::get(str_var);
	Foo obj_out(VariantData<Foo>::get(obj_var));
	Foo * pfoo= VariantData<Foo*>::get(pt_var);

	std::cout<<int_out<<std::endl;
	std::cout<<str_out<<std::endl;
	std::cout<<obj_out<<std::endl;
	std::cout<<*pfoo<<std::endl;
	
	//Variant是boost::variant的一种特例，所以包括visitor等boost提供的方法也可以在Variant上面应用。

	delete pfoo;

	return 0;
}