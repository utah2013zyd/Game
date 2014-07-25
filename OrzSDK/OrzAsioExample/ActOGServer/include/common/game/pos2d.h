//////////////////////////////////////////////////////////////
//
//      FileName: pos2d.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-10-14
//          Time: 18:41:00
//   Description: pos2d类，游戏中二维坐标计算的坐标类
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-10-14  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef POS2D_H_
#define POS2D_H_
#include <orz/OrzAsio/OrzAsioConfig.h>
using namespace Orz;

class pos2d
{
public:
	pos2d(float64 _x, float64 _y, float64 _min=0.000001f) 
		: mins(_min), x(_x), y(_y) {}

	pos2d(float64 _min=0.000001f) 
		: mins(_min), x(0.0f), y(0.0f) {}

public:
	//!
	pos2d normalize()
	{
		float64 abss = (float64)sqrt(x*x + y*y);
		if (abss >= mins)
		{
			x = x/abss;
			y = y/abss;
		}
		return pos2d(x,y);
	}

	//!
	pos2d operator-(const pos2d& _p)
	{
		return pos2d(x-_p.x, y-_p.y);
	}

	//!
	pos2d operator+(const pos2d& _p)
	{
		return pos2d(x+_p.x, y+_p.y);
	}

	//!
	pos2d operator*(const pos2d& _p)
	{
		return pos2d(x*_p.x, y*_p.y);
	}

	//!
	pos2d operator*(float64 _f)
	{
		return pos2d(x*_f, y*_f);
	}

	//!
	pos2d &operator=(const pos2d& _p)
	{
		if (this == &_p)
			return *this;
		x = _p.x;
		y = _p.y;

		return *this;
	}
	bool operator==(const pos2d& _p)
	{
		float64 x2 = _p.x - x;
		if (x2 < 0)
			x2 = -x2;

		float64 y2 = _p.y - y;
		if (y2 < 0)
			y2 = -y2;

		if (x2<=mins && y2<=mins)
			return true;
		else
			return false;
	}

	float64 length()
	{
		return (float64)sqrt(x*x + y*y);
	}
	
	float64				x;
	float64				y;
	const float64		mins;
};

#endif
