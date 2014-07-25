#ifndef __Orz_ToolkitPlus_Agency_hpp_
#define __Orz_ToolkitPlus_Agency_hpp_
#include <orz/Toolkit_Plus/Toolkit/NumChooser.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynArray.hpp>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
namespace Orz
{
//! �����������
/*!
 *	\note �̰߳�ȫ
 *	\note ����C�������ڶ��߳���ѡ����ʵĶ���ֻҪͬʱ���ʵ��߳�����С�ڵ����ڲ��������������������ۼ����߳�ͬʱ����ʱ�䶼��O(1)
 *	\note �����ڶ��߳��¶���߳�wait-free�ģ������߳�ʱ��һ�������ʶ���
 example:
 \code
	Agency<int32> agy(10,NumChooser(0,9, 234));	// ����һ��10��Ԫ�ص����飬ͬʱʹ��NumChooser��Ϊѡ��������NumChooser��
	int32& v1 = agy.select();	// �߳�1
	int32& v2 = agy.select();	// �߳�2
 \endcode
 */
template <typename Object, typename Chooser = NumChooser>
class Agency
{
public:
	ORZ_PLUS_INLINE
	Agency(uint32 _obj_num, const Chooser& _csr)
		: m_obj_num(_obj_num)
		, m_objects(_obj_num)
		, m_csr(_csr)
	{
	}
	ORZ_PLUS_INLINE
	~Agency()
	{
	}

public:
	ORZ_PLUS_INLINE
	Object&				select				()
	{
		uint32 i = m_csr.choose();
		if (i >= m_objects.size())
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
			return m_objects[i];
	}

	ORZ_PLUS_INLINE
	uint32				selectId			()
	{
		return m_csr.choose();
	}

	ORZ_PLUS_INLINE
	Object&				operator[]			(uint32 _pos)
	{
		if (_pos >= m_objects.size())
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
			return m_objects[_pos];
	}

	ORZ_PLUS_INLINE
	uint32				size				() const
	{
		return m_objects.size();
	}

	ORZ_PLUS_INLINE
	DynArray<Object>&	all					()
	{
		return m_objects;
	}

private:
	uint32					m_obj_num;
	DynArray<Object>		m_objects;
	Chooser					m_csr;

};

}

#endif
