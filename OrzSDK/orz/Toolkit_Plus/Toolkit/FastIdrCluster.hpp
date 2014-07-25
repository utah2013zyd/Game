#ifndef __Orz_ToolkitPlus_FastIdrCluster_hpp_
#define __Orz_ToolkitPlus_FastIdrCluster_hpp_
#include <orz/Toolkit_Plus/Toolkit/FastIdr.hpp>
#include <orz/Toolkit_Plus/Toolkit/NumChooser.hpp>
#include <orz/Toolkit_Plus/Toolkit/ClrId.hpp>
namespace Orz
{
//! ��Ч������Id�������
/*!
 *	\note �̰߳�ȫ
 *	\note ����C����+����ָ�뷽ʽ�����Һͷ���Id������ͻ���ʱ����O(1)\n
 *	\note ���߳���ֻҪ��֤ͬʱ����ͻ���Id���߳�����С�ڵ����ڲ���HashMap��hash������Ҳ��O(1)
 example:
 \code
	FastIdrCluster idr(10,100, 1);			// ����һ�����1000��10x100�����ɷ���Id��10��Id����ÿ���������100���ɷ���Id��ÿ��������СIdΪ1��Id������
	ClrId clrId1 = idr.generate();			// ������һ��(0,1)��Id��
	ClrId clrId2 = idr.generate(2);			// ��Id����Ϊ2�ķ����з���Id������ǣ�clrId2 == (2, 1)
	idr.release(clrId);						// ������clrId1
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
