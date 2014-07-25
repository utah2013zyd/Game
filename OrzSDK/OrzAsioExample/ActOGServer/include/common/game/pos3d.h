//////////////////////////////////////////////////////////////
//
//      FileName: pos3d.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-10-22
//          Time: 18:37:00
//   Description: pos3d类，游戏中三维坐标计算的坐标类
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-10-22  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef POS3D_H_
#define POS3D_H_
#include <orz/OrzAsio/OrzAsioConfig.h>
using namespace Orz;

class pos3d
{
public:
	pos3d(float64 _x, float64 _y, float64 _z, float64 _min=0.000001f) 
		: mins(_min), x(_x), y(_y), z(_z) {}

	pos3d(float64 _min=0.000001f) 
		: mins(_min), x(0.0f), y(0.0f), z(0.0f) {}

public:
	//!
	pos3d normalize()
	{
		float64 abss = (float64)sqrt(x*x + y*y + z*z);
		if (abss >= mins)
		{
			x = x/abss;
			y = y/abss;
			z = z/abss;
		}
		return pos3d(x,y,z);
	}

	//!
	pos3d operator-(const pos3d& _p)
	{
		return pos3d(x-_p.x, y-_p.y, z-_p.z);
	}

	//!
	pos3d operator+(const pos3d& _p)
	{
		return pos3d(x+_p.x, y+_p.y, z+_p.z);
	}

	//!
	pos3d operator*(const pos3d& _p)
	{
		return pos3d(x*_p.x, y*_p.y, z*_p.z);
	}

	//!
	pos3d operator*(float64 _f)
	{
		return pos3d(x*_f, y*_f, z*_f);
	}

	//!
	pos3d &operator=(const pos3d& _p)
	{
		if (this == &_p)
			return *this;
		x = _p.x;
		y = _p.y;
		z = _p.z;

		return *this;
	}
	bool operator==(const pos3d& _p)
	{
		float64 x2 = _p.x - x;
		if (x2 < 0)
			x2 = -x2;

		float64 y2 = _p.y - y;
		if (y2 < 0)
			y2 = -y2;

		float64 z2 = _p.z - z;
		if (z2 < 0)
			z2 = -z2;

		if (x2<=mins && y2<=mins && z2<=mins)
			return true;
		else
			return false;
	}

	float64 length()
	{
		return (float64)sqrt(x*x + y*y + z*z);
	}
	
	float64				x;
	float64				y;
	float64				z;
	const float64		mins;
};

#endif
