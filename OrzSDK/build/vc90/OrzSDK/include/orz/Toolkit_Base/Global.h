#ifndef __Orz_ToolkitBase_Global_h__
#define __Orz_ToolkitBase_Global_h__
#include <orz/Toolkit_Base/StdHeader.h>
#if ORZ_COMPILER == ORZ_COMPILER_MSVC


// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#pragma warning( disable: 4661)
#pragma warning( disable: 4251)

//for boost::variant
#pragma warning( disable: 4503)
//for boost's pdb
//#   pragma warning (disable : 4244)


#endif


#ifndef ORZ_FIXED_POINT_ENABLE
#define ORZ_FIXED_POINT_ENABLE 0
#endif


#define __utf8(str) str
#define __utf16(str) L##str


#define EXPORT_STL_VECTOR( dllmacro, vectype ) \
  template class dllmacro std::allocator< vectype >; \
  template class dllmacro std::vector<vectype, \
	std::allocator< vectype > >;



#define OrzHashMap ::boost::unordered_map
#define OrzHashSet ::boost::unordered_set 

#define ORZ_VERSION_MAJOR 0
#define ORZ_VERSION_MINOR 4
#define ORZ_VERSION_PATCH 0
#define ORZ_VERSION_SUFFIX ""
#define ORZ_VERSION_NAME "Rabbit"

#define ORZ_VERSION    ((ORZ_VERSION_MAJOR << 16) | (ORZ_VERSION_MINOR << 8) | ORZ_VERSION_PATCH)





//using boost hashmap
namespace Orz
{
	

	inline float QuakeRsqrt(float p)
	{
		long i;
		float x2, rt;
		const float threehalfs = 1.5F;

		x2 = p *0.5f;
		rt = p;
		i = *(static_cast<long *>( static_cast<void*>(&rt)));
		i = 0x5f3759df -(i >>1);

		rt = *(static_cast<float *>( static_cast<void*>(&i)));
		//rt = * (float*)&i;
		rt = rt *(threehalfs -(x2 *rt *rt));
		return rt;
	}



	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;


		
	typedef boost::tuple<int, int> intX2;
	typedef boost::tuple<float, float> floatX2;
	typedef boost::tuple<int, float> int_float;
	typedef boost::tuple<float, float, float> floatX3; 
	typedef boost::tuple<float, float, float, float> floatX4;

	typedef boost::any Any;
	typedef boost::shared_ptr<boost::any> AnyPtr;
	//template <class T> AnyPtr make_any_ptr(const T & object){return AnyPtr(new Any(object));}
	
	



	//template <class T> T get_any_ptr(Variant var)
	//{
	//	AnyPtr data;
	//	data = boost::get< AnyPtr >(var);

	//	T object;
	//	object = boost::any_cast< T >(*data);
	//	return object;
	//}
	//
typedef float TimeType;
typedef boost::function< bool ( void ) > TimerFunctorReturnBool;
//typedef boost::function< void ( void ) > TimerFunNoReturn;
//typedef boost::variant<TimerFunctorReturnBool, TimerFunNoReturn> TimerFun;
const std::string ORZ_PLATFORM_WIN32_STRING("Win32");
const std::string ORZ_PLATFORM_LINUX_STRING("Linux");

#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
	const std::string ORZ_PLATFORM_STRING(ORZ_PLATFORM_WIN32_STRING);
#elif ORZ_PLATFORM == ORZ_PLATFORM_LINUX
	const std::string ORZ_PLATFORM_STRING(ORZ_PLATFORM_LINUX_STRING);
#endif

}

#endif
