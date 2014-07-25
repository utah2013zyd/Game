#ifndef __Orz_ToolkitPlus_FreeList_hpp_
#define __Orz_ToolkitPlus_FreeList_hpp_
#include <orz/Toolkit_Plus/Toolkit/FastIdrCluster.hpp>
namespace Orz
{
//! ��Ч�Ķ���ؽӿ�
class IFreeList
{
public:
	IFreeList() {}
	virtual ~IFreeList() {}

public:
	virtual void*					alloc				() = 0;
	virtual void					free				(void* _ptr) = 0;
	virtual uint32					getClr				() const = 0;
	virtual uint32					getPer				() const = 0;
	virtual void*					get					(uint32 _index) = 0;
	virtual void*					get					(uint32 _clr, uint32 _id) = 0;
	virtual PtrIntType				clrId2uid			(ClrId _clrId) = 0;
	virtual ClrId					uid2ClrId			(PtrIntType _uid) = 0;
	virtual void					clear				() = 0;
};

//! ��Ч�Ķ����
/*!
 *	\note �̰߳�ȫ��ʹ��FastIdrCluster��ȷ����ȫ��Ч��
 *	\note ����C����+��ͬ�Ĵ�С�Ķ���ĵ�ַƫ����������ͻ��ն��󣬷���ͻ���ʱ����O(1)��������ȫȡ����C������ķ���ʱ�䣩
 example:
	IFreeListShrPtr objectPool(new FreeList<int32>(10,100));	// ����һ�����1000��10x100�����ɷ���int32���͵Ķ���10������ÿ���������100���ɷ������
	int32* obj1 = (int32*)objectPool->alloc();		// ������һ��int32 �Ķ���
	objectPool->free(obj1);							// ���������int32�Ķ���
 */
template <typename ObjectType>
class FreeList
	: public IFreeList
	, private boost::noncopyable
{
	FreeList();
public:
	ORZ_PLUS_INLINE
	FreeList(uint32 _clrNum, uint32 _perClrNum)
		: m_clrNum((PtrIntType)(_clrNum == 0 ? 1 : _clrNum))
		, m_perClrNum((PtrIntType)(_perClrNum == 0 ? 1 : _perClrNum))
		, m_max(m_clrNum*m_perClrNum)
		, m_objResClr(new ObjectType[m_clrNum*m_perClrNum])
		, m_resSlr(m_clrNum, m_perClrNum)
		, m_headAddress((PtrIntType)m_objResClr)
		, m_endAddress(m_headAddress + (PtrIntType)(m_clrNum*m_perClrNum*sizeof(ObjectType)))
	{
	}

	ORZ_PLUS_INLINE
	virtual ~FreeList()
	{
		if (m_objResClr)
			delete [] m_objResClr;
	}

public:
	ORZ_PLUS_INLINE
	void*					alloc				()
	{
		if (!m_objResClr)
			return 0;

		ClrId cid = m_resSlr.generate();
		if (cid.id >= m_perClrNum)
			return new ObjectType;
		else
			return (ObjectType*)(clrId2uid(cid)*sizeof(ObjectType) + m_headAddress);
	}

	ORZ_PLUS_INLINE
	void					free				(void* _ptr)
	{
		if (!_ptr)
			return;

		PtrIntType ptr = (PtrIntType)_ptr;
		if (ptr < m_headAddress || ptr >= m_endAddress)
		{
			delete (ObjectType*)_ptr;
			return;
		}
		else
			m_resSlr.release(uid2ClrId((ptr - m_headAddress)/sizeof(ObjectType)));
	}

	ORZ_PLUS_INLINE
	uint32					getClr				() const
	{
		return (uint32)m_clrNum;
	}

	ORZ_PLUS_INLINE
	uint32					getPer				() const
	{
		return (uint32)m_perClrNum;
	}

	ORZ_PLUS_INLINE
	uint32					getMax				() const
	{
		return (uint32)m_max;
	}

	ORZ_PLUS_INLINE
	void*					get					(uint32 _index)
	{
		if (!m_objResClr)
			return 0;

		if (_index >= m_max)
			return 0;

		return &m_objResClr[_index];
	}

	ORZ_PLUS_INLINE
	void*					get					(uint32 _clr, uint32 _id)
	{
		if (!m_objResClr)
			return 0;

		if (_clr >= m_clrNum || _id >= m_perClrNum)
			return 0;

		return &m_objResClr[_clr*m_perClrNum + _id];
	}
	
	ORZ_PLUS_INLINE
	PtrIntType				clrId2uid			(ClrId _clrId)
	{
		return (PtrIntType)_clrId.clr*m_perClrNum + (PtrIntType)_clrId.id;
	}

	ORZ_PLUS_INLINE
	ClrId					uid2ClrId			(PtrIntType _uid)
	{
		ClrId clrId;
		clrId.id = (Number)(_uid % m_perClrNum);
		clrId.clr = (uint32)(_uid/m_perClrNum);
		clrId.valid = true;
		return clrId;
	}

	ORZ_PLUS_INLINE
	void					clear				()
	{
		if (!m_objResClr)
			return;

		if (m_objResClr)
		{
			delete [] m_objResClr;
			m_objResClr = 0;
		}
	}

private:
	PtrIntType							m_clrNum;
	PtrIntType							m_perClrNum;
	PtrIntType							m_max;
	ObjectType*							m_objResClr;
	FastIdrCluster						m_resSlr;
	PtrIntType							m_headAddress;
	PtrIntType							m_endAddress;
};
}
#endif
