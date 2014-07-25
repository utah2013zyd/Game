#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <common/game/pos2d.h>
using namespace Orz;

//! ֱ���˶� 
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

	//! ����ƫ������ȷ��ÿ0.04�룬Ҳ����40�������һ�Σ�
	//	�����������dt����Ϊ�������Ѿ���ȥ��ʱ��
	ORZ_PLUS_INLINE float64 run()
	{
		if (stopped)
			return 0.0f;

		// ȡ�õ�ǰʱ��
		ct = now();

		// ��ǰʱ���ȥ�˶���ʼʱ�̵�ʱ�䣬�����˶��Ѿ������˶���ʱ��
		float64 pt = (float64)(ct - start_time)/1000000.0f;
		
		// s = v0*t + 0.5*a*t*t �����ٶȹ�ʽ
		// ��v0��ʼ�ٶȣ�aΪ���ٶȣ�tΪ��ǰʱ������ڿ�ʼ��ʱ���ֵ��
		// ���㵱ǰ��λ�ƣ�����ڿ�ʼʱ�̣�
		float64 s = v0*pt + 0.5f*a*pt*pt;

		// ��������һ��λ�Ƶ�ƫ����
		float64 ds = s - last;
		last = s;
		// �����ǰ��λ���Ѿ����ڸ��������λ�ƣ���ֹͣ
		if (fabs(s) >= max_dis)
		{
			stopped = true;
			return ds;
		}
		
		// ���ص�ǰλ��ƫ������������������������ֵ ���ӵ������������ϣ�
		// �����������������˶��Ĺ켣�ĸı���
		return ds;
	}

	//!
	ORZ_PLUS_INLINE bool	is_stopped() const
	{
		return stopped;
	}

private:
	//! ���ٶȣ��������Ͻ׶εĳ�ʼ�ٶȣ�
	const float64			v0;
	//! ���ٶ�
	float64					a;
	//! ��һ��run�Ľ��
	float64					last;
	//!
	ClockTime				start_time;
	ClockTime				ct;
	float64					max_dis;
	bool					stopped;
};

//! ����+��������
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

	//! ��������+��������ĸı�ֵ
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
	//! ���ٶȣ��������Ͻ׶εĳ�ʼ�ٶȣ�
	const float64			v0;
	//!
	const float64			g;
	//! ��һ��run�Ľ��
	float64					last;
	//!
	ClockTime				start_time;
	ClockTime				ct;

};

//! ��������������ߣ���������ң���(0,0)��ʼ���
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

	//! ����dx��dy
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
