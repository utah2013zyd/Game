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
	//! �ж�һ�������ĵ��Ƿ�����������. _face_angle: �泯�ĽǶȣ��������Σ�����Բ�Σ������ߵĽǶ�(<2pi >=0)
	ORZ_PLUS_INLINE bool					check_is_in					(const pos2d& _center, const pos2d& _check_point, float64 _face_angle)
	{
		// ����Բ�����꣬��������任����Բ��Ϊԭ�������ϵ��
		pos2d check_point(_check_point.x - _center.x, _check_point.y - _center.y);

		// ������㵽Բ�ĵľ��� ������ڰ뾶����˵���϶�����������
		float64 distance = sqrt(pow(check_point.x,2) + pow(check_point.y,2));
		if (distance > m_radius)
			return false;

		// ���ݸ������泯�ĽǶȣ��Լ���ǰ��Բ�ĽǶȣ��������Σ�����Բ�Σ������ߵĽǶ�
		float64 face_angle = normalize_angle(_face_angle);
		// �������ұߵĽǶ�
		float64 right_angle = normalize_angle(face_angle - m_angle/2.0f);
		// ��������ߵĽǶ�
		float64 left_angle = normalize_angle(face_angle + m_angle/2.0f);

		// ���ݸ�����任������꣬�������Բ�ĵ�������x��ĽǶ�
		float64 degrees = atan(check_point.y / check_point.x);
		
		float64 rs = 0.0f;
		// ���ݸ��������������,���ж���λ���Ǹ�����,�������������ó����յĽǶ�
		if (check_point.x >= 0.0f && check_point.y >= 0.0f)		// ��һ����
			rs = degrees;
		else if (check_point.x <= 0.0f && check_point.y >= 0.0f)	// �ڶ�����
			rs = PI + degrees;
		else if (check_point.x <= 0.0f && check_point.y <= 0.0f)	// ��������
			rs = PI + degrees;
		else	// ��������
			rs = TPI + degrees;

		if (rs <= left_angle && rs >= right_angle)
			return true;
		else
			return false;
	}

private:
	//! �������ε�Բ�Ľ�
	float64						m_angle;

	//! �������εİ뾶
	float64						m_radius;
};

#endif
