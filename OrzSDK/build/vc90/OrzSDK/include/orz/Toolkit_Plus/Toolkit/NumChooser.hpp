#ifndef __Orz_ToolkitPlus_NumChooser_hpp__
#define __Orz_ToolkitPlus_NumChooser_hpp__
#include <orz/Toolkit_Plus/Toolkit/Rander.hpp>
namespace Orz
{
//! 数字选择器
/*!
 *	\note 线程安全
 *	\note 使用递增循环或者随机的方式选择指定范围内的一个数字
 example:
 \code
	NumChooser nc(0,9, 234));	// 创建一个从0~9种选择数字的数字选择器，随机种子数设为234
	Number num1 = nc.choose();	// 线程1
	Number num2 = nc.choose();	// 线程2
	// num1 和 num2 在长时间多次不同线程的调用下，大部分时间不会相同
 \endcode
 */
class NumChooser
{
public:
	ORZ_PLUS_INLINE
	NumChooser(Number _min, Number _max, uint32 _seed, bool _usingRandom = false) 
		: m_min(_min), m_max(_max), m_curr(_min), m_rdr(new Rander(_seed)), m_random(_usingRandom) 
	{
		m_curr = m_rdr->generate(m_min, m_max);
		if (m_curr < m_min || m_curr > m_max)
			m_curr = m_min;
	}

	ORZ_PLUS_INLINE
	~NumChooser() {}

	ORZ_PLUS_INLINE
	void upMax(Number _max)
	{
		m_max += _max;
	}

	ORZ_PLUS_INLINE
	Number choose()
	{
		Number curr;
		if (m_random)
			curr = m_rdr->generate(m_min, m_max);
		else
		{
			curr = m_curr;
			++curr;
			if (curr > m_max)
				curr = m_min;
			m_curr = curr;
		}
		return curr;
	}

private:
	Number				m_min;
	Number				m_max;
	Number				m_curr;
	RanderShrPtr		m_rdr;
	bool				m_random;
	uint32				m_seed;
};

}

#endif
