#ifndef __Orz_ToolkitPlus_ThreadMap_hpp__
#define __Orz_ToolkitPlus_ThreadMap_hpp__
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
namespace Orz
{
template 
<
	class Id, 
	typename Value, 
	class Compare = std::less<Id> 
>
class ThreadMap
{

public:
	ORZ_PLUS_INLINE ThreadMap() {}
	ORZ_PLUS_INLINE ~ThreadMap() {}

public:
	//! ���Ԫ��
	ORZ_PLUS_INLINE
	int32				add					(const Id& _id, const Value& _value, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (add2map(m_map, _id, _value) == 0)
			return 1;

		return 0;
	}

	//! ͨ��Id��ȡ��Ԫ��
	ORZ_PLUS_INLINE 
	Value*				operator[]			(const Id& _id)
	{
		DynShrLock lock(m_shrMutex, DynShrLock::LT_LOCK);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr != m_map.end())
			return &itr->second;

		return 0;
	}

	//! ͨ��Id��ȡ��Ԫ��
	ORZ_PLUS_INLINE 
	Value*				get					(const Id& _id, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr != m_map.end())
			return &itr->second;

		return 0;
	}

	//! ͨ��Id��ɾ��Ԫ��
	ORZ_PLUS_INLINE 
	int32				erase				(const Id& _id, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (rmv4mMap(m_map, _id) == 0)
			return 1;

		return 0;
	}

	//! _del: ��ɾ����Ԫ�ؿ���
	ORZ_PLUS_INLINE 
	int32				erase				(const Id& _id, Value& _del, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr == m_map.end())
			return 1;

		_del = itr->second;
		m_map.erase(itr);

		return 0;
	}

	//! 0 ɾ���ɵ�����µ� 1 ûɾ���ɵģ�������µ� 2 ɾ���˾ɵ�����µ�ʧ�� 3 ��ûɾ���ɵģ�Ҳû����µ�
	ORZ_PLUS_INLINE 
	int32				eraseAdd			(const Id& _id, const Value& _value, Value& _del, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		int32 err = 0;
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr == m_map.end())
			err = 1;

		if (err == 0)
		{
			_del = itr->second;
			m_map.erase(itr);
		}

		if (add2map(m_map, _id, _value) == 0)
		{
			if (err == 0) err = 2;
			else if (err == 1) err = 3;
		}

		return err;
	}

	//! ȡ��������С
	ORZ_PLUS_INLINE 
	uint32				size				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return (uint32)m_map.size();
	}

	ORZ_PLUS_INLINE
	bool				empty				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return m_map.empty();
	}

	//!
	ORZ_PLUS_INLINE
	void				beginRead			() const
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE
	void				endRead				() const
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE
	boost::shared_mutex&
						mutex				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE
	void				beginWrite			() const
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE
	void				endWrite			() const
	{
		m_shrMutex.unlock();
	}

	//! ��Ԫ��ָ��ָ��������ͷ
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::iterator
						begin				()
	{
		return m_map.begin();
	}

	//! ��Ԫ��ָ��ָ��������ͷ
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::const_iterator
						begin				() const
	{
		return m_map.begin();
	}

	ORZ_PLUS_INLINE
	std::map<Id, Value, Compare>&
						map					()
	{
		return m_map;
	}

	//!
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::iterator
						end					()
	{
		return m_map.end();
	}

	//! 
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::const_iterator
						end					() const
	{
		return m_map.end();
	}

	ORZ_PLUS_INLINE
	void				clear				(bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		m_map.clear();
	}

private:
	std::map<Id, Value, Compare>			m_map;
	mutable boost::shared_mutex				m_shrMutex;

};


template 
<
	class Id, 
	typename Value, 
	class Compare = std::less<Id> 
>
class ThreadShrMap
{
public:
	ORZ_PLUS_INLINE ThreadShrMap() {}
	ORZ_PLUS_INLINE ~ThreadShrMap() {}

public:
	//! ���Ԫ��
	ORZ_PLUS_INLINE
	int32				add					(const Id& _id, const Value& _value, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (add2map(m_map, _id, _value) == 0)
			return 1;

		return 0;
	}

	//! ͨ��Id��ȡ��Ԫ��
	ORZ_PLUS_INLINE 
	Value				operator[]			(const Id& _id)
	{
		DynShrLock lock(m_shrMutex, DynShrLock::LT_LOCK);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr != m_map.end())
			return itr->second;

		return Value();
	}

	//! ͨ��Id��ȡ��Ԫ��
	ORZ_PLUS_INLINE 
	Value				get					(const Id& _id, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr != m_map.end())
			return itr->second;

		return Value();
	}

	//! ͨ��Id��ɾ��Ԫ��
	ORZ_PLUS_INLINE 
	int32				erase				(const Id& _id, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (rmv4mMap(m_map, _id) == 0)
			return 1;

		return 0;
	}

	//! _del: ��ɾ����Ԫ�ؿ���
	ORZ_PLUS_INLINE 
	int32				erase				(const Id& _id, Value& _del, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr == m_map.end())
			return 1;

		_del = itr->second;
		m_map.erase(itr);

		return 0;
	}

	//! 0 ɾ���ɵ�����µ� 1 ûɾ���ɵģ�������µ� 2 ɾ���˾ɵ�����µ�ʧ�� 3 ��ûɾ���ɵģ�Ҳû����µ�
	ORZ_PLUS_INLINE 
	int32				eraseAdd			(const Id& _id, const Value& _value, Value& _del, bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		int32 err = 0;
		typename std::map<Id, Value, Compare>::iterator itr(m_map.find(_id));
		if (itr == m_map.end())
			err = 1;

		if (err == 0)
		{
			_del = itr->second;
			m_map.erase(itr);
		}

		if (add2map(m_map, _id, _value) == 0)
		{
			if (err == 0) err = 2;
			else if (err == 1) err = 3;
		}

		return err;
	}

	//! ȡ��������С
	ORZ_PLUS_INLINE 
	uint32				size				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return (uint32)m_map.size();
	}

	ORZ_PLUS_INLINE
	bool				empty				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return m_map.empty();
	}

	//!
	ORZ_PLUS_INLINE
	void				beginRead			() const
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE
	void				endRead				() const
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE
	boost::shared_mutex&
						mutex				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE
	void				beginWrite			() const
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE
	void				endWrite			() const
	{
		m_shrMutex.unlock();
	}

	//! ��Ԫ��ָ��ָ��������ͷ
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::iterator
						begin				()
	{
		return m_map.begin();
	}

	//! ��Ԫ��ָ��ָ��������ͷ
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::const_iterator
						begin				() const
	{
		return m_map.begin();
	}

	ORZ_PLUS_INLINE
	std::map<Id, Value, Compare>&
						map					()
	{
		return m_map;
	}

	//!
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::iterator
						end					()
	{
		return m_map.end();
	}

	//! 
	ORZ_PLUS_INLINE 
	typename std::map<Id, Value, Compare>::const_iterator
						end					() const
	{
		return m_map.end();
	}

	ORZ_PLUS_INLINE
	void				clear				(bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		m_map.clear();
	}

private:
	std::map<Id, Value, Compare>			m_map;
	mutable boost::shared_mutex				m_shrMutex;

};

}

#endif
