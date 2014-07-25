#ifndef GAME_COMMON_H_
#define GAME_COMMON_H_

#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>

namespace Orz
{

typedef unsigned int team_id;
typedef unsigned int game_id;				// 游戏id

static const float64 g_min = 0.000001f;		// 极小浮点数
static const float64	PI = 3.14159265f;
static const float64	TPI = PI*2.0f;

//! game的类型
enum game_type
{
	GT_DEMO1 = 0,
	GT_TOTAL
};

//! 将弧度归为2pi以内
ORZ_PLUS_INLINE float64 normalize_angle(float64 _angle)
{
	// 取绝对值
	float64 angle = fabs(_angle);

	if (angle >= 0.0f || angle <= TPI)
		return angle;

	// 求余 也就求出2pi内值
	float64 surplus = fmod(angle, TPI);

	// 根据正负判断不同的值
	if (_angle >= 0.0f)
		return surplus;
	else
		return TPI - surplus;
}

//! 将角度转换为弧度
ORZ_PLUS_INLINE float64 ator(int _angle)
{
	return (float64)_angle * (TPI / 360.0f);
}

//! 将弧度转换为角度
ORZ_PLUS_INLINE int32 rtoa(float64 _radian)
{
	return (int)(_radian * (360.0f / TPI));
}

}	// end of namespace Orz

#endif
