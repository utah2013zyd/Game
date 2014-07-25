#ifndef __Orz_ToolkitBase_StdHeaders_h__
#define __Orz_ToolkitBase_StdHeaders_h__
#include <orz/Toolkit_Base/Platform.h>



#if ORZ_PLATFORM == ORZ_PLATFORM_WIN32
#define NOMINMAX

#  if defined( __MINGW32__ )
#    include <unistd.h>
#  endif
#endif

#if ORZ_PLATFORM == ORZ_PLATFORM_LINUX
extern "C" {

#   include <unistd.h>
#   include <dlfcn.h>

}
#endif






#if defined ( ORZ_GCC_VISIBILITY )
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
#pragma warning(disable:4996)
#pragma warning(disable:4099)
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
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/size.hpp>


#include <boost/type_traits.hpp>

#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/shallow_history.hpp>
#include <boost/statechart/deferral.hpp>

#include <boost/foreach.hpp>

# include <boost/exception/exception.hpp>
# include <boost/current_function.hpp>



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

#if defined ( ORZ_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

#endif
