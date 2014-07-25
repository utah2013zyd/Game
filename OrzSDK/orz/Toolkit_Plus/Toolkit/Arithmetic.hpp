#ifndef __Orz_ToolkitPlus_Arithmetic_hpp__
#define __Orz_ToolkitPlus_Arithmetic_hpp__
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
#include <orz/Toolkit_Plus/Toolkit/MemString.hpp>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{

//------------------------------------------------------------------------------------
#define NUM_STR_MAX_SIZE 320
typedef MemString<NUM_STR_MAX_SIZE+1> NumStr;

#define TIME_STR_MAX_SIZE 320
typedef MemString<TIME_STR_MAX_SIZE+1> TimeStr;

#define DATA_STR_MAX_SIZE 320
typedef MemString<DATA_STR_MAX_SIZE> DataStr;

typedef uint64 ClockTime;
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
ORZ_PLUS_INLINE LONGLONG getSecondCount()
{
	static LARGE_INTEGER liCounter = {0};
	if (0 == liCounter.QuadPart)
		QueryPerformanceFrequency(&liCounter);
	
	return liCounter.QuadPart;
}

//! 取得当前的时间，单位：微妙（1秒 == 1000 000 微秒）
ORZ_PLUS_INLINE ClockTime now()
{
	LARGE_INTEGER liCurrent = {0};
	QueryPerformanceCounter(&liCurrent);

	return (ClockTime)(liCurrent.QuadPart * 1000000 / getSecondCount());
}
#else
//! 取得当前的时间，单位：微妙（1秒 == 1000 000 微秒）
ORZ_PLUS_INLINE ClockTime now()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);

	return (ClockTime)(1000000*tp.tv_sec + tp.tv_usec);
}
#endif
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
//! uint32 转 字符串
ORZ_PLUS_INLINE NumStr d2s(uint32 _d)
{
	char s[NUM_STR_MAX_SIZE];
	sprintf(s, "%u", _d);
	return NumStr(s, (uint32)strlen(s));
}

//! int32 转 字符串
ORZ_PLUS_INLINE NumStr i2s(int32 _d)
{
	char s[NUM_STR_MAX_SIZE];
	sprintf(s, "%d", _d);
	return NumStr(s, (uint32)strlen(s));
}

//! 字符串 转 uint32
ORZ_PLUS_INLINE uint32 s2d(const char* _s)
{
	if (!_s)
		throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
	uint32 d = 0;
	sscanf(_s,"%d",&d);
	return d;
}

//! 字符串 转 int32
ORZ_PLUS_INLINE int32 s2i(const char* _s)
{
	if (!_s)
		throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
	int32 d = 0;
	sscanf(_s, "%d", &d);
	return d;
}

//! 浮点 转 字符串
ORZ_PLUS_INLINE NumStr f2s(float64 _f)
{
	char f[NUM_STR_MAX_SIZE];
	sprintf(f,"%lf",_f);
	return NumStr(f, (uint32)strlen(f));
}

//! 字符串 转 浮点
ORZ_PLUS_INLINE float64 s2f(const char* _s)
{
	if (!_s)
		throw InvalidParamsExp(InvalidParamsExp::IPEXP_NULL_PTR, ThrowAddress(__FILE__, __LINE__));
	float64 f = 0;
	sscanf(_s, "%lf", &f);
	return f;
}

//! 指针 转 字符串
ORZ_PLUS_INLINE NumStr p2s(void* _ptr)
{
	char s[NUM_STR_MAX_SIZE];
	sprintf(s, "%p", _ptr);
	return NumStr(s, (uint32)strlen(s));
}
//------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------
//! 微秒 转 秒
ORZ_PLUS_INLINE float64 msec2sec(int32 _msec)
{
	return (float64)_msec / 1000000.0f;
}

//! 秒 转 微妙
ORZ_PLUS_INLINE int32 sec2msec(float64 _sec)
{
	return (int32)(_sec * 1000000.0f);
}
//------------------------------------------------------------------------------------


//! 取得一个字符串的.以后的后缀
/*!
 *	\param[in] 要处理的字符串
 *	\param[out] _ext 存储找到的后缀，如果为空则表示没找到
 example:
 \code
	char name[32] = "test.bmp";
	std::string r;
	findExtension(name, r);
	std::cout << r << std::endl;

	控制台输出：bmp
 \endcode
 */
ORZ_PLUS_INLINE void findExtension(const char* _name, std::string& _ext)
{
	int len = (int)strlen(_name);
	for (int i = len-1; i >= 0; i--)
	{
		if (_name[i] == '.')
		{
			_ext = (char*)(_name + i + 1);
			return;
		}
	}
	_ext = (char*)(_name + len);
	return;
}

//! 分解被指定字符分割的字符串
/*!
 *	\param[in] _str 要被分割的字符串
 *	\param[in] _size 要被分割的字符串的长度
 *	\param[in] _spt 用于分割的字符
 *	\param[out] _r 存储被分割后的字符串
 *	\return 0成功 非0失败
 example:
 \code
	char str[32] = "This is test string!";
	char spt = ' '; // 空格
	std::vector<StringPtr> r;
	if (separateStr(str, strlen(str), spt, r) == 0)
	{
		// r.size() == 4
		// r[0] == "This"
		// r[1] == "is"
		// r[2] == "test"
		// r[3] == "string!"
	}
 \endcode
	\note StringPtr 不会申请内存，它只是记录一个字符串起始指针和这个字符串的长度

 */
ORZ_PLUS_INLINE int32 separateStr(const char* _str, uint32 _size, const char& _spt, std::vector<StringPtr>& _r)
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
		StringPtr str(_str, _size);
		_r.push_back(str);
	}
	else
	{
		if (dposs.front() == 0)
			_r.push_back(StringPtr());
		else
			_r.push_back(StringPtr(_str, dposs.front()));

		std::vector<uint32>::iterator itr(dposs.begin());
		for (uint32 i=0; itr!=dposs.end(); ++i, ++itr)
		{
			if (i < dposs.size() - 1)
				_r.push_back(StringPtr(_str + *itr + 1, dposs[i+1] - *itr - 1));
			else
				_r.push_back(StringPtr(_str + *itr + 1, _size - *itr - 1));	
		}
	}

	return 0;
}


struct DateSys
{
	DateSys() : year(-1), month(-1), day(-1), hour(-1), min(-1), sec(-1), msec(-1) {}

	DateSys(int32 _year, int32 _month, int32 _day, int32 _hour, int32 _min, int32 _sec, int32 _msec) 
		: year(_year)
		, month(_month)
		, day(_day)
		, hour(_hour)
		, min(_min)
		, sec(_sec)
		, msec(_msec) 
	{
	}

	ORZ_PLUS_INLINE 
	DataStr formatString() const
	{
		NumStr year_(i2s(year).c_str());
		NumStr month_(i2s(month).c_str());
		NumStr day_(i2s(day).c_str());
		NumStr hour_(i2s(hour).c_str());
		NumStr min_(i2s(min).c_str());
		NumStr sec_(i2s(sec).c_str());
		NumStr msec_(i2s(msec).c_str());
		NumStr r1("-");
		NumStr r2(":");
		NumStr r3(" ");

		DataStr dstr;
		dstr += year_;
		dstr += r1;
		dstr += month_;
		dstr += r1;
		dstr += day_;
		dstr += r3;
		dstr += hour_;
		dstr += r2;
		dstr += min_;
		dstr += r2;
		dstr += sec_;
		dstr += r2;
		dstr += msec_;

		return dstr;
	}

	int32 year;
	int32 month;
	int32 day;
	int32 hour;
	int32 min;
	int32 sec;
	int32 msec;
};




//------------------------------------------------------------------------------------
#if ORZ_PLUS_PLATFORM == ORZ_PLUS_PLATFORM_WIN
//! 取得UTC/Local时间 返回时间，例如：2008-08-01 8:34:13:152
ORZ_PLUS_INLINE DateSys getDateTime(bool _isUtc=false)
{
	SYSTEMTIME systime;
	// 日期时间
	if (_isUtc)
		GetSystemTime(&systime);
	else
		GetLocalTime(&systime);

	return DateSys(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
}
#else
//! 取得UTC/Local时间 返回时间，例如：2008-08-01 8:34:13:152 (年-月-日 小时:分钟:秒:毫秒)
ORZ_PLUS_INLINE DateSys getDateTime(bool _isUtc=false)
{
	struct timeval tp;
	time_t timep;
	struct tm *p = 0;
	time(&timep);
	// 日期时间
	if (_isUtc)
		p = gmtime(&timep);
	else
		p = localtime(&timep);
	gettimeofday(&tp, NULL);

	if (!p)
		return DateSys();

	return DateSys(1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, (int32)(tp.tv_usec/1000));
}
#endif
//------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------
//! 添加元素进入map 返回值：0 失败 非 0 成功
template <typename Key, typename Value, typename Compare>
ORZ_PLUS_INLINE int32 add2map(std::map<Key, Value, Compare>& _map, const Key& _key, const Value& _value)
{
	typename std::map<Key, Value, Compare>::iterator itr = _map.find(_key);
	if (itr != _map.end())
		return 0;

	typename std::pair<typename std::map<Key, Value, Compare>::iterator, bool> pr;
	pr = _map.insert(std::pair<Key, Value>(_key, _value));
	return 1;
}

//! 添加元素进入map 返回值：0 失败 非 0 成功
template <typename Key, typename Value, typename Compare>
ORZ_PLUS_INLINE int32 add2map(std::map<Key, Value*, Compare>& _map, const Key& _key, Value* _value)
{
	typename std::map<Key, Value*, Compare>::iterator itr = _map.find(_key);
	if (itr != _map.end())
		return 0;

	typename std::pair<typename std::map<Key, Value*, Compare>::iterator, bool> pr;
	pr = _map.insert(std::pair<Key, Value*>(_key, _value));
	return 1;
}

//! 删除map元素 返回值：0 失败 非0 成功
template <typename Key, typename Value, typename Compare>
ORZ_PLUS_INLINE int32 rmv4mMap(std::map<Key, Value, Compare>& _map, const Key& _key)
{
	typename std::map<Key, Value, Compare>::iterator itr = _map.find(_key);
	if (itr == _map.end())
		return 0;

	_map.erase(itr);
	return 1;
}
//! 删除map元素 返回值：0 失败 非0 成功 - 指针版本
template <typename Key, typename Value, typename Compare>
ORZ_PLUS_INLINE Value* rmv4mMap(std::map<Key, Value*, Compare>& _map, const Key& _key)
{
	typename std::map<Key, Value*, Compare>::iterator itr = _map.find(_key);
	if (itr == _map.end())
		return 0;

	Value* v = itr->second;

	_map.erase(itr);
	return v;
}
//------------------------------------------------------------------------------------



}

#endif
