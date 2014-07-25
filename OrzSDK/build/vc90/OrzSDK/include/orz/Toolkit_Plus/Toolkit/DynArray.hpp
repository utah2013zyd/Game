#ifndef __Orz_ToolkitPlus_DynArray_hpp_
#define __Orz_ToolkitPlus_DynArray_hpp_
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
namespace Orz
{
//! 使用动态内存的数组
/*!
 *	\note 线程安全
 example:
 \code
	DynArray<int32> ints(5);	// 分配了一个有5个int32元素的数组
	ints[1] = 2;	// 将第二个元素值设为2

	ints.reset(2);	// 删除了旧的数组，重新分配了一个有2个int32元素的数组
 \endcode
 */
template <typename T>
class DynArray
{
public:
	ORZ_PLUS_INLINE
	DynArray()
		: m_size(0)
		, m_array(0)
	{
	}

	ORZ_PLUS_INLINE
	DynArray(uint32 _size)
		: m_size(0)
		, m_array(0)
	{
		if (_size == 0)
			return;
		m_size = _size;
		m_array = new T[m_size];
	}

	ORZ_PLUS_INLINE
	~DynArray()
	{
		if (m_array)
			delete [] m_array;
	}

	ORZ_PLUS_INLINE 
	void					reset					(uint32 _size)
	{
		if (m_array)
			delete [] m_array;

		m_size = _size;

		if (_size == 0)
			return;
		m_array = new T[m_size];
	}

public:
	ORZ_PLUS_INLINE
	T& operator[](uint32 _pos)
	{
		if (_pos >= m_size)
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
			return m_array[_pos];
	}

	ORZ_PLUS_INLINE
	const T& operator[](uint32 _pos) const
	{
		if (_pos >= m_size)
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
			return m_array[_pos];
	}

public:
	ORZ_PLUS_INLINE 
	uint32					size					() const
	{
		return m_size;
	}

	ORZ_PLUS_INLINE 
	bool					empty					() const
	{
		return m_size == 0 ? true : false;
	}

	ORZ_PLUS_INLINE 
	T&						front					()
	{
		return m_array[0];
	}

	ORZ_PLUS_INLINE 
	const T&				front					() const
	{
		return m_array[0];
	}

	ORZ_PLUS_INLINE 
	T&						back					()
	{
		return m_array[m_size - 1];
	}

	ORZ_PLUS_INLINE 
	const T&				back					() const
	{
		return m_array[m_size - 1];
	}

	ORZ_PLUS_INLINE 
	const T*				raw						() const
	{
		return m_array;
	}

private:
	T*							m_array;
	uint32						m_size;
};

}

#endif
