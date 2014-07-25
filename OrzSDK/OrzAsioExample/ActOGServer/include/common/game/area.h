#ifndef AREA_H_
#define AREA_H_

#include <common/game/game_common.h>
#include <common/game/pos2d.h>

using namespace Orz;

class area
{
public:
	area(float64 _angle, float64 _radius)
		: m_angle(normalize_angle(_angle))
		, m_radius(_radius)
	{
	}
	~area()
	{
	}

public:
	//! 判断一个给定的点是否在区域以内. _face_angle: 面朝的角度，整个扇形（或者圆形）中心线的角度(<2pi >=0)
	ORZ_PLUS_INLINE bool					check_is_in					(const pos2d& _center, const pos2d& _check_point, float64 _face_angle)
	{
		// 根据圆心坐标，将给定点变换到以圆心为原点的坐标系中
		pos2d check_point(_check_point.x - _center.x, _check_point.y - _center.y);

		// 求给定点到圆心的距离 如果大于半径，则说明肯定不在区域内
		float64 distance = sqrt(pow(check_point.x,2) + pow(check_point.y,2));
		if (distance > m_radius)
			return false;

		// 根据给定的面朝的角度，以及先前的圆心角度，计算扇形（或者圆形）两条边的角度
		float64 face_angle = normalize_angle(_face_angle);
		// 中心线右边的角度
		float64 right_angle = normalize_angle(face_angle - m_angle/2.0f);
		// 中心线左边的角度
		float64 left_angle = normalize_angle(face_angle + m_angle/2.0f);

		// 根据给定点变换后的坐标，求出其与圆心点连线与x轴的角度
		float64 degrees = atan(check_point.y / check_point.x);
		
		float64 rs = 0.0f;
		// 根据给定点的坐标正负,来判定其位于那个象限,并根据象限来得出最终的角度
		if (check_point.x >= 0.0f && check_point.y >= 0.0f)		// 第一象限
			rs = degrees;
		else if (check_point.x <= 0.0f && check_point.y >= 0.0f)	// 第二象限
			rs = PI + degrees;
		else if (check_point.x <= 0.0f && check_point.y <= 0.0f)	// 第三象限
			rs = PI + degrees;
		else	// 第四象限
			rs = TPI + degrees;

		if (rs <= left_angle && rs >= right_angle)
			return true;
		else
			return false;
	}

private:
	//! 区域扇形的圆心角
	float64						m_angle;

	//! 区域扇形的半径
	float64						m_radius;
};

#endif
