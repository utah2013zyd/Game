#include <iostream>
#include <orz/Toolkit_Base/Variant.h>
#include <string>
using namespace Orz;


/**
�û��Զ������ͣ�
ע�⣬���б�Variant�����������Ҫ֧�ֿ������캯���������п��ܳ���
������Ϊ�ڴ���ֵ�Ĺ����п�����Ҫ���п�����
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


	//int���ͱ�Ĭ�ϵĴ�ջ���湹�������������ϸߵ�Ч��
	//��֧�ִ�ջ��������Ͱ�������
	//char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long, int64, uint64, float, double, void * , bool, std::pair<int, int>, std::pair<float, float>
	//����ʹ���Ͼ�������������������������������ܸߵ�Ч�ʣ���ͬ��boost::variant��
	Variant int_var = VariantData<int>::set(19811112);

	//std::string��û�б���Ϊ���ڽ����ͣ�������Ȼ�Ӷ��������ռ�
	//��ʵ����Щû���ڽ�֧�ֵ����ͱ�ת����boost::any������д���
	Variant str_var = VariantData<std::string>::set("test");
	
	//����֧���������͵Ĵ��棬��Ͱ����û��Զ������͡������ڴ���Ĺ����������п������̣����Ծ�������ϴ����Ĵ���
	//�������ֶ�����ʹ������ָ��������Ȼ����ͨ��Variant����
	Variant obj_var= VariantData<Foo>::set(Foo(12)); 

	//VariantҲ֧��ָ��Ĵ��棬������Ҫע����ǣ�Ϊ��Ч���������ڲ�����ת��Ϊvoid*���ͣ���Ҳ��Ψһû�����Ͱ�ȫ��״̬��
	//�����Ҫ���Ͱ�ȫ��ָ�봢�棬�����boost�ṩ������ָ�롣
	Variant pt_var= VariantData<Foo*>::set(new Foo(13)); 



	int int_out = VariantData<int>::get(int_var);
	std::string str_out = VariantData<std::string>::get(str_var);
	Foo obj_out(VariantData<Foo>::get(obj_var));
	Foo * pfoo= VariantData<Foo*>::get(pt_var);

	std::cout<<int_out<<std::endl;
	std::cout<<str_out<<std::endl;
	std::cout<<obj_out<<std::endl;
	std::cout<<*pfoo<<std::endl;
	
	//Variant��boost::variant��һ�����������԰���visitor��boost�ṩ�ķ���Ҳ������Variant����Ӧ�á�

	delete pfoo;

	return 0;
}