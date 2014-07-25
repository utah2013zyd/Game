#ifndef __Orz_ToolkitPlus_ObjectPtr_hpp_
#define __Orz_ToolkitPlus_ObjectPtr_hpp_
#include <orz/Toolkit_Plus/GlobalPlus.h>
namespace Orz
{
template <typename T>
class ObjectPtr
{
public:
	ORZ_PLUS_INLINE
	ObjectPtr()
		: m_ptr(0)
	{
	}

	ORZ_PLUS_INLINE
	ObjectPtr(T* _ptr)
		: m_ptr(_ptr)
	{
	}

	ORZ_PLUS_INLINE
	ObjectPtr(const T* _ptr)
		: m_ptr(const_cast<T*>(_ptr))
	{
	}

	ORZ_PLUS_INLINE
	~ObjectPtr() 
	{
	}

	template <typename Type>
	ORZ_PLUS_INLINE
	bool			operator==			(const ObjectPtr<Type>& _other) const
	{
		if (!m_ptr && !_other.m_ptr)
		{
			return true;
		}
		else if (m_ptr && _other.m_ptr)
		{
			return *m_ptr == *_other.m_ptr;
		}
		else 
		{
			return false;
		}
	}

public:
	ORZ_PLUS_INLINE
	const T*		ptr					() const
	{
		return m_ptr;
	}

	ORZ_PLUS_INLINE
	T*				ptr					()
	{
		return m_ptr;
	}

	ORZ_PLUS_INLINE 
	void			reset				(const T* _ptr)
	{
		m_ptr = const_cast<T*>(_ptr);
	}

	ORZ_PLUS_INLINE 
	void			reset				(T* _ptr)
	{
		m_ptr = _ptr;
	}

	ORZ_PLUS_INLINE 
	bool			isValid				() const
	{
		return m_ptr == 0 ? false : true;
	}

private:
	T*				m_ptr;
};
}
#endif
