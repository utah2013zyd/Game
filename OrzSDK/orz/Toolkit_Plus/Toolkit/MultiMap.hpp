#ifndef __Orz_ToolkitPlus_MultiMap_hpp__
#define __Orz_ToolkitPlus_MultiMap_hpp__
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Toolkit/DynShrLock.hpp>
namespace Orz
{
//! 当id的模版参数只有一个的时候，就退化为一般的std::map
template 
<
	class Id1, 
	typename Value, 
	class Compare1 = std::less<Id1> 
>
class Map
{
public:
	Map()
	{
	}
	~Map()
	{
	}

public:
	//! 添加元素
	ORZ_PLUS_INLINE int32				add					(const Id1& _id1, const Value& _value, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (add2map(m_map, _id1, _value) == 0)
			return 1;

		return 0;
	}

	//! 通过Id1来取得元素
	ORZ_PLUS_INLINE Value*				getById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id1, Value, Compare1>::iterator itr(m_map.find(_id1));
		if (itr != m_map.end())
			return &itr->second;

		return 0;
	}

	//! 通过Id1来删除元素
	ORZ_PLUS_INLINE int32				rmvById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (rmv4mMap(m_map, _id1) == 0)
			return 1;

		return 0;
	}

	//! 取得容器大小
	ORZ_PLUS_INLINE uint32				size				(bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return (uint32)m_map.size();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									begin				()
	{
		return m_map.begin();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									begin				() const
	{
		return m_map.begin();
	}

	//!
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									end					()
	{
		return m_map.end();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									end					() const
	{
		return m_map.end();
	}

	ORZ_PLUS_INLINE void				beginRead			() const
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE void				endRead				() const
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE boost::shared_mutex&
									mutex				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE void				beginWrite			() const
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE void				endWrite			() const
	{
		m_shrMutex.unlock();
	}

	ORZ_PLUS_INLINE void				clear				(bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		m_map.clear();
	}

private:
	std::map<Id1, Value, Compare1>				m_map;
	typename std::map<Id1, Value, Compare1>::iterator
												m_itr;
	mutable boost::shared_mutex					m_shrMutex;
};


template 
<
	class Id1, 
	class Id2, 
	typename Value, 
	class Compare1 = std::less<Id1>, 
	class Compare2 = std::less<Id2> 
>
class DoubleMap
{
public:
	DoubleMap()
	{
	}

	~DoubleMap()
	{
	}

public:
	//! 添加元素
	ORZ_PLUS_INLINE int32				add					(const Id1& _id1, const Id2& _id2, const Value& _value, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (m_child.add(_id1, _value) != 0)
			return 1;

		if (add2map(m_id12, _id1, _id2) == 0)
			return 1;

		if (add2map(m_id21, _id2, _id1) == 0)
			return 1;

		return 0;
	}

	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE Value*				getById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_child.getById1(_id1);
	}

	//!
	ORZ_PLUS_INLINE Value*				getById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id2, Id1, Compare2>::iterator itr(m_id21.find(_id2));
		if (itr == m_id21.end())
			return 0;

		return m_child.getById1(itr->second);
	}
	//------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE Id1*				getId1ById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id2, Id1, Compare2>::iterator itr(m_id21.find(_id2));
		if (itr != m_id21.end())
			return &itr->second;

		return 0;
	}

	//!
	ORZ_PLUS_INLINE Id2*				getId2ById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id1, Id2, Compare1>::iterator itr(m_id12.find(_id1));
		if (itr != m_id12.end())
			return &itr->second;

		return 0;
	}
	//------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE int32				rmvById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		Id2* id2 = getId2ById1(_id1);
		if (!id2)
			return 1;

		if (rmv4mMap(m_id21, *id2) == 0)
			return 1;

		if (rmv4mMap(m_id12, _id1) == 0)
			return 1;

		return m_child.rmvById1(_id1);
	}

	//!
	ORZ_PLUS_INLINE int32				rmvById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		Id1* id1 = getId1ById2(_id2);
		if (!id1)
			return 1;

		if (m_child.rmvById1(*id1) != 0)
			return 1;

		if (rmv4mMap(m_id12, *id1) == 0)
			return 1;

		if (rmv4mMap(m_id21, _id2) == 0)
			return 1;

		return 0;
	}
	//------------------------------------------------------------------------------------

	//! 取得容器大小
	ORZ_PLUS_INLINE uint32				size				(bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_child.size();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									begin				()
	{
		return m_child.begin();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									begin				() const
	{
		return m_child.begin();
	}

	//!
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									end					()
	{
		return m_child.end();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									end					() const
	{
		return m_child.end();
	}

	ORZ_PLUS_INLINE void				beginRead			() const
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE void				endRead				() const
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE boost::shared_mutex&
									mutex				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE void				beginWrite			() const
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE void				endWrite			() const
	{
		m_shrMutex.unlock();
	}

	ORZ_PLUS_INLINE void				clear				(bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		m_child.clear(_usingLock);
		m_id12.clear();
		m_id21.clear();
	}
	
private:
	Map<Id1, Value, Compare1>					m_child;

	std::map<Id1, Id2, Compare1>				m_id12;
	typename std::map<Id1, Id2, Compare1>::iterator 
												m_id12Itr;

	std::map<Id2, Id1, Compare2>				m_id21;
	typename std::map<Id2, Id1, Compare2>::iterator
												m_id21Itr;
	mutable boost::shared_mutex					m_shrMutex;
};


template 
<
	class Id1, 
	class Id2, 
	class Id3, 
	typename Value, 
	class Compare1 = std::less<Id1>, 
	class Compare2 = std::less<Id2>,
	class Compare3 = std::less<Id3>
>
class TrebleMap
{
public:
	TrebleMap()
	{
	}

	~TrebleMap()
	{
	}

public:
	//! 添加元素
	ORZ_PLUS_INLINE int32				add					(const Id1& _id1, const Id2& _id2, const Id3& _id3, const Value& _value, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		if (m_child.add(_id1, _id2, _value) != 0)
			return 1;

		if (add2map(m_id12, _id1, _id2) == 0)
			return 1;
		
		if (add2map(m_id13, _id1, _id3) == 0)
			return 1;

		if (add2map(m_id21, _id2, _id1) == 0)
			return 1;
		
		if (add2map(m_id23, _id2, _id3) == 0)
			return 1;

		if (add2map(m_id31, _id3, _id1) == 0)
			return 1;

		if (add2map(m_id32, _id3, _id2) == 0)
			return 1;

		return 0;
	}

	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE Value*				getById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_child.getById1(_id1);
	}

	//!
	ORZ_PLUS_INLINE Value*				getById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_child.getById2(_id2);
	}

	//!
	ORZ_PLUS_INLINE Value*				getById3			(const Id3& _id3, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id3, Id1, Compare3>::iterator itr(m_id31.find(_id3));
		if (itr == m_id31.end())
			return 0;

		return m_child.getById1(itr->second);
	}
	//------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE Id1*				getId1ById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id2, Id1, Compare2>::iterator itr(m_id21.find(_id2));
		if (itr != m_id21.end())
			return &itr->second;

		return 0;
	}
	//!
	ORZ_PLUS_INLINE Id1*				getId1ById3			(const Id3& _id3, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id3, Id1, Compare3>::iterator itr(m_id31.find(_id3));
		if (itr != m_id31.end())
			return &itr->second;

		return 0;
	}

	//!
	ORZ_PLUS_INLINE Id2*				getId2ById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id1, Id2, Compare1>::iterator itr(m_id12.find(_id1));
		if (itr != m_id12.end())
			return &itr->second;

		return 0;
	}
	//!
	ORZ_PLUS_INLINE Id2*				getId2ById3			(const Id3& _id3, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id3, Id2, Compare3>::iterator itr(m_id32.find(_id3));
		if (itr != m_id32.end())
			return &itr->second;

		return 0;
	}

	//!
	ORZ_PLUS_INLINE Id3*				getId3ById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id2, Id3, Compare2>::iterator itr(m_id23.find(_id2));
		if (itr != m_id23.end())
			return &itr->second;

		return 0;
	}
	//!
	ORZ_PLUS_INLINE Id3*				getId3ById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		typename std::map<Id1, Id3, Compare1>::iterator itr(m_id13.find(_id1));
		if (itr != m_id13.end())
			return &itr->second;

		return 0;
	}
	//------------------------------------------------------------------------------------


	//------------------------------------------------------------------------------------
	//!
	ORZ_PLUS_INLINE int32				rmvById1			(const Id1& _id1, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		Id2* id2 = getId2ById1(_id1);
		if (!id2)
			return 1;

		Id3* id3 = getId3ById1(_id1);
		if (!id3)
			return 1;

		if (rmv4mMap(m_id21, *id2) == 0)
			return 1;
		if (rmv4mMap(m_id23, *id2) == 0)
			return 1;

		if (rmv4mMap(m_id31, *id3) == 0)
			return 1;
		if (rmv4mMap(m_id32, *id3) == 0)
			return 1;

		if (rmv4mMap(m_id12, _id1) == 0)
			return 1;
		if (rmv4mMap(m_id13, _id1) == 0)
			return 1;

		return m_child.rmvById1(_id1);
	}

	//!
	ORZ_PLUS_INLINE int32				rmvById2			(const Id2& _id2, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		Id1* id1 = getId1ById2(_id2);
		if (!id1)
			return 1;

		Id3* id3 = getId3ById2(_id2);
		if (!id3)
			return 1;

		if (rmv4mMap(m_id31, *id3) == 0)
			return 1;
		if (rmv4mMap(m_id32, *id3) == 0)
			return 1;

		if (rmv4mMap(m_id12, *id1) == 0)
			return 1;
		if (rmv4mMap(m_id13, *id1) == 0)
			return 1;

		if (rmv4mMap(m_id21, _id2) == 0)
			return 1;
		if (rmv4mMap(m_id23, _id2) == 0)
			return 1;

		return m_child.rmvById2(_id2);
	}

	//!
	ORZ_PLUS_INLINE int32				rmvById3			(const Id3& _id3, bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		Id1* id1 = getId1ById3(_id3);
		if (!id1)
			return 1;

		Id2* id2 = getId2ById3(_id3);
		if (!id2)
			return 1;

		if (m_child.rmvById1(*id1) != 0)
			return 1;

		if (rmv4mMap(m_id12, *id1) == 0)
			return 1;
		
		if (rmv4mMap(m_id13, *id1) == 0)
			return 1;

		if (rmv4mMap(m_id21, *id2) == 0)
			return 1;
		
		if (rmv4mMap(m_id23, *id2) == 0)
			return 1;

		if (rmv4mMap(m_id31, _id3) == 0)
			return 1;
		
		if (rmv4mMap(m_id32, _id3) == 0)
			return 1;

		return 0;
	}
	//------------------------------------------------------------------------------------

	//! 取得容器大小
	ORZ_PLUS_INLINE uint32				size				(bool _usingLock = false)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_child.size();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									begin				()
	{
		return m_child.begin();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									begin				() const
	{
		return m_child.begin();
	}

	//!
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::iterator
									end					()
	{
		return m_child.end();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE typename std::map<Id1, Value, Compare1>::const_iterator
									end					() const
	{
		return m_child.end();
	}

	ORZ_PLUS_INLINE void				beginRead			() const
	{
		m_shrMutex.lock_shared();
	}

	ORZ_PLUS_INLINE void				endRead				() const
	{
		m_shrMutex.unlock_shared();
	}

	ORZ_PLUS_INLINE boost::shared_mutex&
									mutex				(bool _usingLock = true) const
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK_SHR : DynShrLock::LT_DELAY);
		return m_shrMutex;
	}

	ORZ_PLUS_INLINE void				beginWrite			() const
	{
		m_shrMutex.lock();
	}

	ORZ_PLUS_INLINE void				endWrite			() const
	{
		m_shrMutex.unlock();
	}

	ORZ_PLUS_INLINE void				clear				(bool _usingLock = true)
	{
		DynShrLock lock(m_shrMutex, _usingLock ? DynShrLock::LT_LOCK : DynShrLock::LT_DELAY);
		m_child.clear(_usingLock);
		m_id12.clear();
		m_id13.clear();
		m_id21.clear();
		m_id23.clear();
		m_id31.clear();
		m_id32.clear();
	}
	
private:
	DoubleMap<Id1, Id2, Value, Compare1, Compare2>			
											m_child;

	//!
	std::map<Id1, Id2, Compare1>			m_id12;
	typename std::map<Id1, Id2, Compare1>::iterator 
											m_id12Itr;

	//!
	std::map<Id1, Id3, Compare1>			m_id13;
	typename std::map<Id1, Id3, Compare1>::iterator
											m_id13Itr;

	//!
	std::map<Id2, Id1, Compare2>			m_id21;
	typename std::map<Id2, Id1, Compare2>::iterator
											m_id21Itr;

	//!
	std::map<Id2, Id3, Compare2>			m_id23;
	typename std::map<Id2, Id3, Compare2>::iterator
											m_id23Itr;

	//!
	std::map<Id3, Id1, Compare3>			m_id31;
	typename std::map<Id3, Id1, Compare3>::iterator
											m_id31Itr;

	//!
	std::map<Id3, Id2, Compare3>			m_id32;
	typename std::map<Id3, Id2, Compare3>::iterator
											m_id32Itr;
	mutable boost::shared_mutex				m_shrMutex;
};

}

#endif
