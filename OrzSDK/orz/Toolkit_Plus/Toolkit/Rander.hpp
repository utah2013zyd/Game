#ifndef __Orz_ToolkitPlus_Rander_hpp__
#define __Orz_ToolkitPlus_Rander_hpp__
#include <orz/Toolkit_Plus/Toolkit/NumDef.hpp>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
namespace Orz
{
class Rander
{
	typedef boost::unique_lock<boost::shared_mutex> BoostLock;
public:
	Rander() : m_seed(0), m_maxSeed(65534) {}
	Rander(uint32 _seed) : m_seed(_seed), m_maxSeed(65534) {}
	~Rander() {}

public:
	//! 在指定的范围内产生随机整数。注意如果_min > _max有可能产生的结果不正确
	ORZ_PLUS_INLINE 
	Number						generate				(Number _min, Number _max)
	{
		if (_min > _max)
			//return 0;
			throw InvalidParamsExp(InvalidParamsExp::IPEXP_RANGE, ThrowAddress(__FILE__, __LINE__));

		BoostLock lock(m_mutex);

		++m_seed;
		if (m_seed >= m_maxSeed)
			m_seed = 0;
		srand(m_seed);

		Number sub = _max - _min + 1;
		
		return rand()%sub + _min;
	}

private:
	boost::shared_mutex			m_mutex;
	uint32						m_seed;
	const uint32				m_maxSeed;
};
}
#endif
