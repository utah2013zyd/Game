/*
-----------------------------------------------------------------------------
代码改写自 ogre3d引擎
-----------------------------------------------------------------------------
*/
#ifndef __Orz_ToolkitBase_Platform_H_
#define __Orz_ToolkitBase_Platform_H_


namespace Orz {
/* 
	初始化平台/编译器相关设置
	Initial platform/compiler-related stuff to set.
*/
#define ORZ_PLATFORM_WIN 1
#define ORZ_PLATFORM_WIN32 ORZ_PLATFORM_WIN
#define ORZ_PLATFORM_LINUX 2

#define ORZ_COMPILER_MSVC 1
#define ORZ_COMPILER_GNUC 2

#define ORZ_ENDIAN_LITTLE 1
#define ORZ_ENDIAN_BIG 2

#define ORZ_ARCHITECTURE_32 1
#define ORZ_ARCHITECTURE_64 2

/* 
	设置编译器类型以及版本
	Finds the compiler type and version.
*/
#if defined( _MSC_VER )
#   define ORZ_COMPILER ORZ_COMPILER_MSVC
#   define ORZ_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
#   define ORZ_COMPILER ORZ_COMPILER_GNUC
#   define ORZ_COMP_VER (((__GNUC__)*100) + \
        (__GNUC_MINOR__*10) + \
        __GNUC_PATCHLEVEL__)

#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* 
在能使用__forceinline的情况下使用，否则使用__inline
See if we can use __forceinline or if we need to use __inline instead 
*/
#if ORZ_COMPILER == ORZ_COMPILER_MSVC
#   if ORZ_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* 设定当前平台
Finds the current platform */

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define ORZ_PLATFORM ORZ_PLATFORM_WIN
#else
#   define ORZ_PLATFORM ORZ_PLATFORM_LINUX
#endif

    /* Find the arch type 
	设置计算机环境(64位/32位)
	*/
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define ORZ_ARCH_TYPE ORZ_ARCHITECTURE_64
#else
#   define ORZ_ARCH_TYPE ORZ_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define ORZ_QUOTE_INPLACE(x) # x
#define ORZ_QUOTE(x) ORZ_QUOTE_INPLACE(x)
#define ORZ_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
// Windows 设置
#if ORZ_PLATFORM == ORZ_PLATFORM_WIN

// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
//
//

// Win32 compilers use _DEBUG for specifying debug builds.
#   ifdef _DEBUG
#       define ORZ_DEBUG_MODE 1
#   else
#       define ORZ_DEBUG_MODE 0
#   endif

// Disable unicode support on MingW at the moment, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLKIT_UNICODE__ in _mingw.h
#if defined( __MINGW32__ ) && !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__)
#	    define ORZ_UNICODE_SUPPORT 1
#   else
#       define ORZ_UNICODE_SUPPORT 0
#   endif
#else
#	define ORZ_UNICODE_SUPPORT 1
#endif

#endif
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Linux/Apple Settings
#if ORZ_PLATFORM == ORZ_PLATFORM_LINUX 


// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   ifdef DEBUG
#       define ORZ_DEBUG_MODE 1
#   else
#       define ORZ_DEBUG_MODE 0
#   endif


//ORZ_PLATFORM_LINUX
#define ORZ_PLATFORM_LIB "libOrzPlatform.so"

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define ORZ_UNICODE_SUPPORT 1

#endif



//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set ORZ_ENDIAN correctly
#ifdef ORZ_CONFIG_BIG_ENDIAN
#    define ORZ_ENDIAN ORZ_ENDIAN_BIG
#else
#    define ORZ_ENDIAN ORZ_ENDIAN_LITTLE
#endif

// Integer formats of fixed bit width
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
// define uint64 type
#if ORZ_COMPILER == ORZ_COMPILER_MSVC
	typedef unsigned __int64 uint64;
	typedef __int64 int64;
#else
	typedef unsigned long long uint64;
	typedef long long  int64;
#endif



}

#endif
