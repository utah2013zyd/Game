#ifndef __Orz_ToolkitPlus_FastIdrCluster_hpp_
#define __Orz_ToolkitPlus_FastIdrCluster_hpp_
#include <orz/Toolkit_Plus/Toolkit/FastIdr.hpp>
#include <orz/Toolkit_Plus/Toolkit/NumChooser.hpp>
#include <orz/Toolkit_Plus/Toolkit/ClrId.hpp>
namespace Orz
{
//! 高效的整型Id组分配器
/*!
 *	\note 线程安全
 *	\note 利用C数组+链表指针方式来查找和分配Id，分配和回收时间是O(1)\n
 *	\note 多线程下只要保证同时分配和回收Id的线程数量小于等于内部的HashMap的hash数量则也是O(1)
 example:
 \code
	FastIdrCluster idr(10,100, 1);			// 创建一个最大1000（10x100）个可分配Id、10个Id分组每个分组最大100个可分配Id、每个分组最小Id为1的Id分配器
	ClrId clrId1 = idr.generate();			// 分配了一个(0,1)的Id组
	ClrId clrId2 = idr.generate(2);			// 在Id分组为2的分组中分配Id，结果是：clrId2 == (2, 1)
	idr.release(clrId);						// 回收了clrId1
 \endcode
 */
class FastIdrCluster
	: private boost::noncopyable
{
	FastIdrCluster();

	typedef boost::shared_ptr<FastIdr> FastIdrShrPtr;

	DynArray<FastIdrShrPtr>		m_idrs;
	NumChooser					m_idrCsr;
	const uint32				m_idrNum;
	const uint32				m_perIdNum;

public:
	ORZ_PLUS_INLINE
	FastIdrCluster(uint32 _idrNum, uint32 _perIdNum, Number _first=1)
		: m_idrs(_idrNum == 0 ? 1 : _idrNum)
		, m_idrCsr(0, _idrNum == 0 ? 0 : _idrNum - 1, 15, false)
		, m_idrNum(_idrNum == 0 ? 1 : _idrNum)
		, m_perIdNum(_perIdNum == 0 ? 1 : _perIdNum)
	{
		for (uint32 i=0; i<m_idrs.size(); ++i)
		{
			FastIdrShrPtr fipr(new FastIdr(m_perIdNum, _first));
			m_idrs[i] = fipr;
		}
	}

	ORZ_PLUS_INLINE
	virtual ~FastIdrCluster() {}

public:
	ORZ_PLUS_INLINE 
	ClrId					generate				()
	{
		uint32 clr = m_idrCsr.choose();
		Number id;

		id = m_idrs[clr]->generate();
		bool is_valid = id == m_idrs[clr]->errObj() ? false : true;
		return ClrId(id, clr, is_valid);
	}

	ORZ_PLUS_INLINE 
	ClrId					generate				(uint32 _clr)
	{
		Number id;
		if (_clr >= m_idrNum)
		{
			//return ClrId();
			//throw InvalidParamsExp(InvalidParamsExp::IPEXP_RANGE, ThrowAddress(__FILE__, __LINE__));
			return ClrId();
		}

		id = m_idrs[_clr]->generate();
		bool is_valid = id == m_idrs[_clr]->errObj() ? false : true;
		return ClrId(id, _clr, is_valid);
	}

	ORZ_PLUS_INLINE
	int32					release					(ClrId _clrId)
	{
		if (_clrId.clr >= m_idrs.size())
			return 1;

		return m_idrs[_clrId.clr]->release(_clrId.id);
	}

	ORZ_PLUS_INLINE
	uint32					idr_num					() const
	{
		return m_idrNum;
	}

	ORZ_PLUS_INLINE
	uint32					perIdNum				() const
	{
		return m_perIdNum;
	}

};

}

#endif
