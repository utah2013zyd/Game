#ifndef __Orz_ToolkitPlus_ObjectList_hpp_
#define __Orz_ToolkitPlus_ObjectList_hpp_
#include <orz/Toolkit_Plus/Toolkit/ObjectSelector.hpp>
namespace Orz
{
// not thread satfy! just using in object inside, and user self deal with thread safty
template <typename ObjectType>
class ObjectList
{
	ObjectList();

public:
	ORZ_PLUS_INLINE
	ObjectList(PtrIntType _arraySize)
		: m_objSlr(_arraySize == 0 ? 1 : _arraySize)
		, m_objectSize(sizeof(ObjectType))
		, m_objsNum(_arraySize == 0 ? 1 : _arraySize)
		, m_objs(new ObjectType[_arraySize == 0 ? 1 : _arraySize])
	{
		m_head = (PtrIntType)m_objs;
		m_end = (PtrIntType)&m_objs[m_objsNum-1]+sizeof(ObjectType);
	}

	ORZ_PLUS_INLINE
	~ObjectList()
	{
		if (m_objs)
			delete [] m_objs;
	}

public:
	ORZ_PLUS_INLINE
	ObjectType*				select				()
	{
		if (m_objSlr.isFull())
			return new ObjectType;

		try
		{
			return &(m_objs[m_objSlr.select()]);
		}
		catch(OrzAsioException& _e)
		{
			throw OrzAsioException(OrzAsioException::EXP_INVALID_STATE, ThrowAddress(__FILE__, __LINE__), _e);
		}
		catch(...)
		{
			throw OrzAsioException(OrzAsioException::EXP_INTERNAL_ERROR, ThrowAddress(__FILE__, __LINE__));
		}
	}

	ORZ_PLUS_INLINE
	uint32					size				()
	{
		return (uint32)m_objsNum;
	}

	ORZ_PLUS_INLINE
	ObjectType*				operator[]			(uint32 _index)
	{
		if (_index >= (uint32)m_objsNum)
			return 0;

		return &(m_objs[_index]);
	}

	ORZ_PLUS_INLINE
	int32					release				(ObjectType* _ptr)
	{
		if (_ptr == 0)
			return 1;

		if ((PtrIntType)_ptr < m_head || (PtrIntType)_ptr >= m_end)
		{
			delete _ptr;
			return 0;
		}

		return m_objSlr.release((PtrIntType)(((PtrIntType)_ptr - m_head) / m_objectSize));
	}

private:
	ObjectSelector<ObjectType>		m_objSlr;
	ObjectType*						m_objs;
	PtrIntType						m_objsNum;
	PtrIntType						m_objectSize;
	PtrIntType						m_head;
	PtrIntType						m_end;
	mutable boost::shared_mutex		m_shrMutex;
};

}

#endif
