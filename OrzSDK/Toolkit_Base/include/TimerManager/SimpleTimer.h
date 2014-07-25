#ifndef __Orz_ToolkitBase_SimpleTimer_h__
#define __Orz_ToolkitBase_SimpleTimer_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/TimerManager/Timer.h>

namespace Orz{
class _OrzToolkitBasePrivate SimpleTimer:  public Timer
{
public:

	///��ʼ��ʱ�����У������ֱ�Ϊ���ü�������ô�������ʼʱ��
	virtual bool start(TimeType interval, uint32 times = 0, TimeType start = 0);

	

	///ǿ��ֹͣ��ʱ������
	virtual void stop(void);
	
	
	SimpleTimer(const TimerFunctorReturnBool & fun);
	virtual ~SimpleTimer(void);
	
	bool update(TimeType interval);
	inline void setEnable(bool enable){ _enable = enable; }

	///�����������ʣ����������Ϊһ���������������ٶ�
	void setSpeed(float speed){_speed = speed;}

	///�õ���������
	float getSpeed(void) const{return _speed;}



private:


	float _speed;
	TimerFunctorReturnBool _fun;
	TimeType _interval;
	uint32 _times;
	TimeType _other;


};

}
#endif
