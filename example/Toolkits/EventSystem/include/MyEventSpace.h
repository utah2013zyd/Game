#include <orz/Toolkit_Base/EventSystem.h>
#ifndef __Orz_MyEventSpace__
#define __Orz_MyEventSpace__
namespace Orz
{
/**
	这里提供了消息空间和其中消息的定义，
	我们尽量（不是必须），把消息放置在不同的空间内，这样我们就可以再不用了解已有框架的基础上扩展消息。（否则可能冲突）
	消息可以在动态库（Dll）之间传递，
**/
DEF_EVENT_BEGIN(MyEventSpace1)

DEF_EVENT(MyEventA)
DEF_EVENT(MyEventB)
DEF_EVENT(MyEventC)
DEF_EVENT(MyEventD)

DEF_EVENT_END(MyEventSpace1)




DEF_EVENT_BEGIN(MyEventSpace2)

DEF_EVENT(MyEvent0)
DEF_EVENT(MyEvent1)
DEF_EVENT(MyEvent2)
DEF_EVENT(MyEvent3)

DEF_EVENT_END(MyEventSpace2)



}
#endif