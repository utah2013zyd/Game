#include <orz/Toolkit_Base/EventSystem.h>
#ifndef __Orz_MyEventSpace__
#define __Orz_MyEventSpace__
namespace Orz
{
/**
	�����ṩ����Ϣ�ռ��������Ϣ�Ķ��壬
	���Ǿ��������Ǳ��룩������Ϣ�����ڲ�ͬ�Ŀռ��ڣ��������ǾͿ����ٲ����˽����п�ܵĻ�������չ��Ϣ����������ܳ�ͻ��
	��Ϣ�����ڶ�̬�⣨Dll��֮�䴫�ݣ�
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