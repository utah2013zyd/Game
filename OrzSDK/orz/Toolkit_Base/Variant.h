#ifndef __Orz_ToolkitBase_Variant_h__
#define __Orz_ToolkitBase_Variant_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
namespace Orz
{

typedef boost::variant<
char, //0
unsigned char,//1
short, //2
unsigned short,//3
int,//4
unsigned int,//5 
long, //6
unsigned long,//7 
int64, //8
uint64, //9
float, //10
double,//11
void * , //12
bool, //13
intX2,//14 
floatX2,//15 

#if (!defined( _MSC_VER )) || (_MSC_VER >=1400) // vc 7.1 has fatal error C1067
	int_float,//16
	floatX3,//17 
	floatX4,// 18
#endif

Any//19
> Variant;

	
template<class T1, class T2 = boost::mpl::find< Variant::types, T1>::type, class T3 = boost::is_pointer<T1>::type>
class VariantData
{
public:
	static Variant set(const T1 & in)
	{
		return Variant(in);
	}
	

	static T1 get(const Variant & data)
	{
		return boost::get< T1 >(data);
	}
	
};

template<class T1>
class VariantData<T1, boost::mpl::end<Variant::types>::type, boost::false_type>
{

public:
	static Variant set(const T1 & in)
	{
		return Variant(Any(in));
	}
	static T1 get(const Variant & data)
	{
		return boost::any_cast<T1>(boost::get< Any >(data));
	}
};


template<class T1>
class VariantData<T1, boost::mpl::end<Variant::types>::type, boost::true_type>
{
public:
	static Variant set(const T1 & in)
	{
		return Variant(static_cast<void *>(in));
	}
	static T1 get(const Variant & data)
	{
			return static_cast<T1>(boost::get< void* >(data));
	}
};

template<class T1>
void VariantSet(Variant & var, const T1 & in)
{
	var = VariantData<T1>::set(in);
}

}

#endif