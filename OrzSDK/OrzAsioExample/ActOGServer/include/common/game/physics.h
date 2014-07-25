#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <common/game/pos2d.h>
using namespace Orz;

//! 直线运动 
struct beeline_move
{
	beeline_move(float64 _v0, float64 _max_dis=-1.0f, float64 _a=0.0f)
		: v0(_v0)
		, a(_a)
		, last(0.0f)
		, start_time(now())
		, ct(0)
		, max_dis(fabs(_max_dis))
		, stopped(false)
	{
	}

	//!
	ORZ_PLUS_INLINE void	set_a(float64 _a)
	{
		a = _a;
	}

	//! 返回偏移量，确保每0.04秒，也就是40毫秒调用一次，
	//	这里无需计算dt，因为计算了已经过去的时间
	ORZ_PLUS_INLINE float64 run()
	{
		if (stopped)
			return 0.0f;

		// 取得当前时间
		ct = now();

		// 当前时间减去运动开始时刻的时间，就是运动已经持续了多少时间
		float64 pt = (float64)(ct - start_time)/1000000.0f;
		
		// s = v0*t + 0.5*a*t*t 根据速度公式
		// （v0初始速度，a为加速度，t为当前时间相对于开始的时间差值）
		// 计算当前的位移（相对于开始时刻）
		float64 s = v0*pt + 0.5f*a*pt*pt;

		// 计算与上一针位移的偏移量
		float64 ds = s - last;
		last = s;
		// 如果当前的位移已经大于给定的最大位移，则停止
		if (fabs(s) >= max_dis)
		{
			stopped = true;
			return ds;
		}
		
		// 返回当前位移偏移量，这样，人物可以用这个值 “加到”人物坐标上，
		// 来设置人物根据这个运动的轨迹的改变量
		return ds;
	}

	//!
	ORZ_PLUS_INLINE bool	is_stopped() const
	{
		return stopped;
	}

private:
	//! 初速度（从下往上阶段的初始速度）
	const float64			v0;
	//! 加速度
	float64					a;
	//! 上一次run的结果
	float64					last;
	//!
	ClockTime				start_time;
	ClockTime				ct;
	float64					max_dis;
	bool					stopped;
};

//! 上抛+自由落体
struct falling
{
	falling(float64 _v0, float64 _g) 
		: v0(_v0)
		, g(fabs(_g))
		, last(0.0f)
		, start_time(now())
		, ct(0)
	{
	}

	//! 计算上抛+自由落体的改变值
	ORZ_PLUS_INLINE float64 run()
	{
		ct = now();
		float64 pt = (float64)(ct - start_time)/1000000.0f;
		
		// y = -(v0*t - 0.5*g*t*t)
		float64 y = -(v0*pt - 0.5f*g*pt*pt);
		float64 dy = y - last;
		last = y;
		
		return dy;
	}

	ORZ_PLUS_INLINE float64 get_v0()
	{
		return v0;
	}

private:
	//! 初速度（从下往上阶段的初始速度）
	const float64			v0;
	//!
	const float64			g;
	//! 上一次run的结果
	float64					last;
	//!
	ClockTime				start_time;
	ClockTime				ct;

};

//! 自由落体的抛物线（方向均向右，从(0,0)开始抛物）
struct parabola
{
	parabola(float64 _max_h, float64 _speed_x, float64 _speed_y, float64 _g, unsigned int _tfps)
		: max_h(_max_h)
		, ff(sqrt(2*fabs(_max_h)/fabs(_g))*fabs(_g), _g)
		, speed_x(_speed_x)
		, speed_y(_speed_y)
		, st(now())
		, ct(now())
		, tfps(_tfps)
	{
	}

	ORZ_PLUS_INLINE void	set_speed(float64 _speed_x, float64 _speed_y)
	{
		speed_x = _speed_x;
		speed_y = _speed_y;
	}

	ORZ_PLUS_INLINE float64 get_v0()
	{
		return ff.get_v0();
	}

	//! 返回dx和dy
	ORZ_PLUS_INLINE pos2d run()
	{
		ClockTime curr_t = now();
		float64 dt = (float64)(curr_t - ct)/1000000.0f;
		ct = curr_t;

		float64 fix = dt * (float64)tfps;
		//printf("parabola::run - fix: " << fix << std::endl;
		float64 dy = ff.run();
		dy += speed_y * fix;
		float64 dx = speed_x * fix;
		//printf("parabola::run - speed_x: " << speed_x << std::endl;

		return pos2d(dx, dy);
	}

private:
	float64					max_h;
	falling					ff;
	float64					speed_x;
	float64					speed_y;
	ClockTime				st;
	ClockTime				ct;
	unsigned int			tfps;
};


#endif
