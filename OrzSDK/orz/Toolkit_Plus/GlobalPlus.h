#ifndef __Orz_ToolkitPlus_GlobalPlus_h__
#define __Orz_ToolkitPlus_GlobalPlus_h__
#include <orz/Toolkit_Plus/StdHeaderPlus.h>
#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC


// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

//#   pragma warning (disable : 4244)


#endif




#ifndef ORZ_PLUS_FIXED_POINT_ENABLE
#define ORZ_PLUS_FIXED_POINT_ENABLE 0
#endif


#define __plusUtf8(str) str
#define __plusUtf16(str) L##str


#define ORZ_PLUS_EXPORT_STL_VECTOR( dllmacro, vectype ) \
  template class dllmacro std::allocator< vectype >; \
  template class dllmacro std::vector<vectype, \
	std::allocator< vectype > >;



#define OrzPlusHashMap ::boost::unordered_map
#define OrzPlusHashSet ::boost::unordered_set 

#define ORZ_PLUS_VERSION_MAJOR 0
#define ORZ_PLUS_VERSION_MINOR 4
#define ORZ_PLUS_VERSION_PATCH 0
#define ORZ_PLUS_VERSION_SUFFIX ""
#define ORZ_PLUS_VERSION_NAME "Cow"

#define ORZ_PLUS_VERSION    ((ORZ_PLUS_VERSION_MAJOR << 16) | (ORZ_PLUS_VERSION_MINOR << 8) | ORZ_PLUS_VERSION_PATCH)

namespace Orz
{
/* Define this if you're using a big-endian machine */
#ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
#	if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC
ORZ_PLUS_INLINE uint16 bswap16(uint16 x)
{
    return (x & 0x00ff) << 8 |
     (x & 0xff00) >> 8 
    ;
}
ORZ_PLUS_INLINE uint32 bswap32(uint32 x)
{
    return     ( x & 0xff000000 ) >>24 | 
        ( x & 0x00ff0000 ) >>8 | 
        ( x & 0x0000ff00 ) <<8 | 
        ( x & 0x000000ff ) << 24 
    ; 
}
ORZ_PLUS_INLINE int64 bswap64(int64 host)   
{   
	int64 ret;   
	int32 high,low;   
	low = (int32)(host & 0xFFFFFFFF);
	host >>= 32;   
	high = (int32)(host & 0xFFFFFFFF);
	low = htonl(low);
	high = htonl(high);
	ret = 0;
	ret = low;
	ret <<= 32;
	ret |= high;
	return ret;
}
#		define __bswap16(x) bswap16(x)
#		define __bswap32(x) bswap32(x)
#		define __bswap64(x) bswap64(x)
#	else
#		include <byteswap.h>
#		define __bswap16(x) bswap_16(x)
#		define __bswap32(x) bswap_32(x)
#		define __bswap64(x) bswap_64(x)
#	endif

ORZ_PLUS_INLINE static void swap16(uint16* p) { *p = __bswap16((uint16)*p); }
ORZ_PLUS_INLINE static void swap32(uint32* p) { *p = __bswap32((uint32)*p); }
ORZ_PLUS_INLINE static void swap64(uint64* p) { *p = __bswap64((uint64)*p); }

ORZ_PLUS_INLINE static float32 swapfloat(float32 p)
{
	union { float32 asfloat; uint8 asbytes[4]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
    
	return u2.asfloat;
}

ORZ_PLUS_INLINE static float64 swapdouble(float64 p)
{
	union { float64 asfloat; uint8 asbytes[8]; } u1, u2;
	u1.asfloat = p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];

	return u2.asfloat;
}

ORZ_PLUS_INLINE static void swapfloat(float32 * p)
{
	union { float32 asfloat; uint8 asbytes[4]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[3];
	u2.asbytes[1] = u1.asbytes[2];
	u2.asbytes[2] = u1.asbytes[1];
	u2.asbytes[3] = u1.asbytes[0];
	*p = u2.asfloat;
}

ORZ_PLUS_INLINE static void swapdouble(float64 * p)
{
	union { float64 asfloat; uint8 asbytes[8]; } u1, u2;
	u1.asfloat = *p;
	/* swap! */
	u2.asbytes[0] = u1.asbytes[7];
	u2.asbytes[1] = u1.asbytes[6];
	u2.asbytes[2] = u1.asbytes[5];
	u2.asbytes[3] = u1.asbytes[4];
	u2.asbytes[4] = u1.asbytes[3];
	u2.asbytes[5] = u1.asbytes[2];
	u2.asbytes[6] = u1.asbytes[1];
	u2.asbytes[7] = u1.asbytes[0];
	*p = u2.asfloat;
}

ORZ_PLUS_INLINE static uint16 swap16(uint16 p) { return __bswap16((uint16)p); }
ORZ_PLUS_INLINE static uint32 swap32(uint32 p) { return __bswap32((uint32)p); }
ORZ_PLUS_INLINE static uint64 swap64(uint64 p) { return __bswap64((uint64)p); }

ORZ_PLUS_INLINE static void swap16(int16* p) { *p = __bswap16((uint16)*p); }
ORZ_PLUS_INLINE static void swap32(int32* p) { *p = __bswap32((uint32)*p); }
ORZ_PLUS_INLINE static void swap64(int64* p) { *p = __bswap64((uint64)*p); }

ORZ_PLUS_INLINE static int16 swap16(int16 p) { return __bswap16((uint16)p); }
ORZ_PLUS_INLINE static int32 swap32(int32 p) { return __bswap32((uint32)p); }
ORZ_PLUS_INLINE static int64 swap64(int64 p) { return __bswap64((uint64)p); }
#endif

// fast int abs
static ORZ_PLUS_INLINE int32 int32abs( const int32 value )
{
	return (value ^ (value >> 31)) - (value >> 31);
}

// fast int abs and recast to unsigned
static ORZ_PLUS_INLINE uint32 int32abs2uint32( const int32 value )
{
	return (uint32)(value ^ (value >> 31)) - (value >> 31);
}

/// Fastest Method of float2int32
static ORZ_PLUS_INLINE int32 float2int32(const float32 value)
{
#if ORZ_PLUS_ARCH_TYPE != ORZ_PLUS_ARCHITECTURE_64 && ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC && !defined ( ORZ_PLUS_CONFIG_BIG_ENDIAN )
	int i;
	__asm {
		fld value
		frndint
		fistp i
	}
	return i;
#else
	union { int32 asInt[2]; float64 asDouble; } n;
	n.asDouble = value + 6755399441055744.0;

#	ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
	return n.asInt [1];
#	else
	return n.asInt [0];
#	endif
#endif
}


// Fastest Method of double2int32
static ORZ_PLUS_INLINE int32 double2int32(const float64 value)
{
#if ORZ_PLUS_ARCH_TYPE != ORZ_PLUS_ARCHITECTURE_64 && ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC && !defined ( ORZ_PLUS_CONFIG_BIG_ENDIAN )
	int i;
	__asm {
		fld value
		frndint
		fistp i
	}
	return i;
#else
  union { int32 asInt[2]; float64 asDouble; } n;
  n.asDouble = value + 6755399441055744.0;

#	ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
	return n.asInt [1];
#	else
	return n.asInt [0];
#	endif
#endif
}

template <typename T>
bool CAS_assembly(T volatile * _ptr, T oldVal, T newVal)
{
    register bool f;

#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_GNUC
    __asm__ __volatile__(
        "lock\n"
		"cmpxchgl %2,%1\n"
        "sete %0\n"
            : "=q" (f), "=m" (*_ptr)
            : "r" (newVal), "m" (*_ptr), "a" (oldVal)
			: "memory");
#else
    _asm
    {
        mov ecx,_ptr
        mov eax,oldVal
        mov ebx,newVal
        lock cmpxchg [ecx],ebx
        setz f
    }
#endif // __GNUC__

    return f;
}

#if ORZ_PLUS_PLATFORM != ORZ_PLUS_PLATFORM_WIN

ORZ_PLUS_INLINE int32 msleep(unsigned long _milisec)
{
    struct timespec req = {0};

    time_t sec = (int32)(_milisec/1000);
    _milisec =_milisec - sec*1000;

    req.tv_sec = sec;
    req.tv_nsec = _milisec*1000000L;

    while (nanosleep(&req, &req) == -1)
         continue;
    return 1;
}

#	define Sleep(ms) msleep(ms)
#endif


}

#endif
