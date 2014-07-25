#ifndef GAME_COMMON_H_
#define GAME_COMMON_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>

namespace Orz
{

typedef unsigned int team_id;
typedef unsigned int game_id;				// ��Ϸid

static const float64 g_min = 0.000001f;		// ��С������
static const float64	PI = 3.14159265f;
static const float64	TPI = PI*2.0f;

//! game������
enum game_type
{
	GT_DEMO1 = 0,
	GT_TOTAL
};

//! �����ȹ�Ϊ2pi����
ORZ_PLUS_INLINE float64 normalize_angle(float64 _angle)
{
	// ȡ����ֵ
	float64 angle = fabs(_angle);

	if (angle >= 0.0f || angle <= TPI)
		return angle;

	// ���� Ҳ�����2pi��ֵ
	float64 surplus = fmod(angle, TPI);

	// ���������жϲ�ͬ��ֵ
	if (_angle >= 0.0f)
		return surplus;
	else
		return TPI - surplus;
}

//! ���Ƕ�ת��Ϊ����
ORZ_PLUS_INLINE float64 ator(int _angle)
{
	return (float64)_angle * (TPI / 360.0f);
}

//! ������ת��Ϊ�Ƕ�
ORZ_PLUS_INLINE int32 rtoa(float64 _radian)
{
	return (int)(_radian * (360.0f / TPI));
}

}	// end of namespace Orz

#endif
