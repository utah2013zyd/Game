#ifndef __Orz_ActOGServer_common_h__
#define __Orz_ActOGServer_common_h__
#include <orz/Tookit_Plus/Tookit/Arithmetic.hpp>
namespace Orz
{
#define PKT_MAX_SIZE 512
#define PKT_HEAD_SIZE 12		// PKT_CAPACITY_SIZE + PKT_TYPE_SIZE 
#define PKT_CAPACITY_SIZE 4
#define PKT_TYPE_SIZE 8

typedef MemString<PKT_MAX_SIZE> PacketData;

//! 将数字前面补给定数量的0
FORCEINLINE NumStr convert_num(uint32 _count, const NumStr& _num)
{
	if (_num.empty())
		return NumStr();

	if (_count == 0)
		return NumStr(_num);

	typedef MemString<2> temp;

	int32 n = _count - _num.size();
	NumStr r;
	if (n > 0)
	{
		NumStr zeros;
		temp t("0");
		for (uint32 i=0; i<(uint32)n; ++i)
		{
			zeros += t;
		}
		r += zeros;
	}
	r += _num;
	return r;
}
// 分解被指定字符分割的packet
FORCEINLINE int32 separateStr(const char* _str, uint32 _size, const char& _spt, std::vector<PacketData>& _r)
{
	if (!_str || _size == 0)
		return 1;

	// 找出所有的_spt
	std::vector<uint32> dposs;		// 分割符_spt的位置 
	for (uint32 i=0; i<_size; ++i)
	{
		if (_str[i] == _spt)
			dposs.push_back(i);
	}

	if (dposs.empty())
	{
		PacketData str(_str, _size);
		_r.push_back(str);
	}
	else
	{
		if (dposs.front() == 0)
			_r.push_back(PacketData());
		else
			_r.push_back(PacketData(_str, dposs.front()));

		for (uint32 i=0; i<dposs.size(); ++i)
		{
			if (i < dposs.size() - 1)
				_r.push_back(PacketData(_str + dposs[i] + 1, dposs[i+1] - dposs[i] - 1));
			else
				_r.push_back(PacketData(_str + dposs[i] + 1, _size - dposs[i] - 1));	
		}
	}

	return 0;
}
}
#endif
