#ifndef __Orz_ToolkitPlus_StdHeaderPlus_h__
#define __Orz_ToolkitPlus_StdHeaderPlus_h__
#include <orz/Toolkit_Plus/PlatformPlus.h>


#if defined ( ORZ_PLUS_GCC_VISIBILITY )
/* Until libstdc++ for gcc 4.2 is released, we have to declare all
 * symbols in libstdc++.so externally visible, otherwise we end up
 * with them marked as hidden by -fvisible=hidden.
 *
 * See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=20218
 */
#   pragma GCC visibility push(default)
#endif

#include <time.h>
#include <stdarg.h>
#include <typeinfo>

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#	if ORZ_PLUS_ARCH_TYPE == ORZ_PLUS_ARCHITECTURE_32
#		ifndef WINVER						// Allow use of features specific to Windows XP or later.
#			define WINVER 0x0500			// Change this to the appropriate value to target other versions of Windows.
#		endif
#		ifndef _WIN32_WINNT					// Allow use of features specific to Windows XP or later.                   
#			define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target other versions of Windows.
#		endif						
#		ifndef _WIN32_WINDOWS				// Allow use of features specific to Windows 98 or later.
#			define _WIN32_WINDOWS 0x0410	// Change this to the appropriate value to target Windows Me or later.
#		endif
#		ifndef _WIN32_IE					// Allow use of features specific to IE 6.0 or later.
#			define _WIN32_IE 0x0600			// Change this to the appropriate value to target other versions of IE.
#		endif
#		ifndef WIN32_LEAN_AND_MEAN
#			define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#		endif
#		include <tchar.h>
#		pragma warning(disable:4996)
#		pragma warning(disable:4311)
#		pragma warning(disable:4267)
#		pragma warning(disable:4251)
#		pragma warning(disable:4503)
#		pragma warning(disable:4275)
#	else
#		ifndef WINVER						// Allow use of features specific to Windows XP or later.
#			define WINVER 0x0500			// Change this to the appropriate value to target other versions of Windows.
#		endif
#		ifndef _WIN64_WINNT					// Allow use of features specific to Windows XP or later.                   
#			define _WIN64_WINNT 0x0500		// Change this to the appropriate value to target other versions of Windows.
#		endif						
#		ifndef _WIN64_WINDOWS				// Allow use of features specific to Windows 98 or later.
#			define _WIN64_WINDOWS 0x0410	// Change this to the appropriate value to target Windows Me or later.
#		endif
#		ifndef _WIN64_IE					// Allow use of features specific to IE 6.0 or later.
#			define _WIN64_IE 0x0600			// Change this to the appropriate value to target other versions of IE.
#		endif
#		ifndef WIN64_LEAN_AND_MEAN
#			define WIN64_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#		endif
#		include <tchar.h>
#		pragma warning(disable:4996)
#		pragma warning(disable:4311)
#		pragma warning(disable:4267)
#		pragma warning(disable:4251)
#		pragma warning(disable:4503)
#	endif
#endif

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX
#	include <sys/time.h>
#	include <sys/types.h>
#	include <sys/ioctl.h>
#	include <sys/socket.h>
#	include <netinet/in.h>
#	include <arpa/inet.h>
#	include <unistd.h>
#	include <signal.h>
#	include <netdb.h>
#	if ORZ_PLUS_COMP_VER >= 40200 
#		include <ext/atomicity.h>
#	else
#		include <bits/atomicity.h>
#	endif
#endif


#pragma warning(push)
#	pragma warning(disable:4244)
#	pragma warning(disable:4800)
#	pragma warning(disable:4396)
#	pragma warning(disable:4675)
#	include <boost/asio.hpp>
#	include <boost/enable_shared_from_this.hpp>
#	include <boost/thread.hpp>
#	include <boost/thread/tss.hpp>
#	include <boost/thread/recursive_mutex.hpp>
#	include <boost/thread/condition.hpp>
#	include <boost/thread/condition_variable.hpp>
#	include <boost/thread/locks.hpp>
#pragma warning(pop)


#if defined ( ORZ_PLUS_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif



#if ORZ_PLUS_PLATFORM != ORZ_PLUS_PLATFORM_WIN 
#	if ORZ_PLUS_ARCH_TYPE != ORZ_PLUS_ARCHITECTURE_64
#		if defined (__GNUC__)
#			if ORZ_PLUS_COMP_VER >= 30400
#				ifdef HAVE_DARWIN
#					define __fastcall
#				else
#    				define __fastcall __attribute__((__fastcall__))
#				endif
#			else
#				define __fastcall __attribute__((__regparm__(3)))
#			endif
#		else
#			define __fastcall __attribute__((__fastcall__))
#		endif
#	else
#		define __fastcall  
#	endif
#endif

namespace Orz
{
// Integer formats of fixed bit width
#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC
	typedef signed __int32 int32;
	typedef signed __int16 int16;
	typedef signed __int8 int8;
#else
	typedef int32_t int32;
	typedef int16_t int16;
	typedef int8_t int8;
#endif
}


#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC
#	define I64FMT "%016I64X"
#	define I64FMTD "%I64u"
#	define SI64FMTD "%I64d"
#	define snprintf _snprintf
#	define atoll __atoi64
#else
#	define stricmp strcasecmp
#	define strnicmp strncasecmp
#	define I64FMT "%016llX"
#	define I64FMTD "%llu"
#	define SI64FMTD "%lld"
#endif


#if ORZ_PLUS_PLATFORM != ORZ_PLUS_PLATFORM_WIN 
#	ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
#		define GUID_LOPART(x) ( ( x >> 32 ) )
#		define GUID_HIPART(x) ( ( x & 0x00000000ffffffff ) )
#	else
#		define GUID_HIPART(x) ( ( x >> 32 ) )
#		define GUID_LOPART(x) ( ( x & 0x00000000ffffffff ) )
#	endif
#else
#	define GUID_HIPART(x) (*(((uint32*)&(x))+1))
#	define GUID_LOPART(x) (*((uint32*)&(x)))
#endif

#define atol(a) strtoul( a, NULL, 10)

#define STRINGIZE(a) #a

// fix buggy MSVC's for variable scoping to be reliable =S
#define for if(true) for


#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC && ORZ_PLUS_COMP_VER >= 1400
#	pragma float_control(push)
#	pragma float_control(precise, on)
#endif


#if ORZ_PLUS_COMPILER == ORZ_PLUS_COMPILER_MSVC && ORZ_PLUS_COMP_VER >= 1400
#	pragma float_control(pop)
#endif


#if ORZ_PLUS_PLATFORM != ORZ_PLUS_PLATFORM_WIN
#	include <sys/timeb.h>
#endif


#if ORZ_PLUS_PLATFORM != ORZ_PLUS_PLATFORM_WIN
#	define FALSE   0
#	define TRUE	1
#endif

#if defined ( ORZ_PLUS_GCC_VISIBILITY )
/* Until libstdc++ for gcc 4.2 is released, we have to declare all
 * symbols in libstdc++.so externally visible, otherwise we end up
 * with them marked as hidden by -fvisible=hidden.
 *
 * See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=20218
 */
#   pragma GCC visibility push(default)
#endif

#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4396)
#pragma warning(disable:4675)
#pragma warning(disable:4267)
//#include <boost/thread/tss.hpp>
//#include <boost/thread/recursive_mutex.hpp>
//#include <boost/thread/condition.hpp>
//#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/checked_delete.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility.hpp>
#include <boost/array.hpp>
#include <boost/cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/exception.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/static_assert.hpp>
#include <boost/exception.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp> 
#include <boost/mpl/empty.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/find.hpp>


#include <boost/type_traits.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/shallow_history.hpp>
#include <boost/statechart/deferral.hpp>



//#include <loki/HierarchyGenerators.h>
//#include <loki/Typelist.h>


#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <iostream>   
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <errno.h>

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


#include <utf8/utf8.h>
#pragma warning(pop)








extern "C" {

#   include <sys/types.h>
#   include <sys/stat.h>

}

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
#  undef min
#  undef max
#  if defined( __MINGW32__ )
#    include <unistd.h>
#  endif
#endif

#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_LINUX
extern "C" {

#   include <unistd.h>
#   include <dlfcn.h>

}
#endif




#if defined ( ORZ_PLUS_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

#endif
