#ifndef __Orz_ToolkitPlus_HashMap_hpp_
#define __Orz_ToolkitPlus_HashMap_hpp_
#include <orz/Toolkit_Plus/Toolkit/ThreadMap.hpp>
#include <orz/Toolkit_Plus/Toolkit/Counter.hpp>
namespace Orz
{
//! 哈希Map
/*!
 *	\note 线程安全，使用哈希原理和加锁的std::map来确保效率和安全兼顾
 *	\note HashMap用于直接存放元素，因为通过get方法返回的时存储在Map内的元素指针，而非元素的拷贝；\n
 *		而后面的HashShrMap用于存放元素的指针或者被boost::shared_ptr包装的元素，因为通过get方法返回的时map中value的拷贝
 example:
 \code
	HashMap<uint32, int32>	hmap(10);	// 创建了一个key为uint32、value为int32、hashArg为默认的key的类型，hash大小为10的HashMap
	hmap.add(1, 1, 1, true);	// 线程1
	hmap.add(2, 2, 2, true);	// 线程2
	hmap.add(3, 3, 3, true);	// 线程3
	hmap.add(11, 2, 11, true);	// 线程1
	...
	// 前3个被添加的元素分别被存放在hash分组的1、2和3组的各自的std::map中,true表示add的时候要加锁
	// 最后一个key为11，value为2的，被添加在和1同std::map中，因为默认的hash函数 为 hashArg 对 分组的最大数量求余： 1%10 == 11%10

	int32* i = hmap.get(1, 1, true);	// 线程4 *i == 1 true表示get的时候要加锁

	// 可以配合DynShrLock来使用：
	{
		DynShrLock lock(hmap.mutex(21));	// 对hmap中分组为21%10==1的std::map加锁，而其他0，2~9的分组均没有加锁，这样其它线程同时访问其他分组的时候，就可以wait-free
		hmap.add(21, 2, 21, false);			// false表示add不加锁
		int32* i = hmap.get(21,21, false);	// false表示add不加锁 *i == 2
	}
	// lock销毁，对21%10==1的分组的std::map解锁
 \endcode
 */
template 
<
	class Key, 
	typename Value, 
	typename HashArg = Key,
	class Compare = std::less<Key> 
>
class HashMap
{
public:
	ORZ_PLUS_INLINE
	HashMap() : m_base(1), m_maps(m_base), m_counter(m_base) {}

	ORZ_PLUS_INLINE explicit 
	HashMap(uint32 _bits) : m_base(_bits), m_maps(m_base), m_counter(m_base) {}

	ORZ_PLUS_INLINE
	~HashMap() {}

	ORZ_PLUS_INLINE
	void				setHash				()
	{
		m_hashHdr = boost::bind(&HashMap<Key, Value, HashArg, Compare>::defaultHash, this, _1);
		m_counter.setHash();
	}

	ORZ_PLUS_INLINE
	void				setHash				(const boost::function<Number (const HashArg&)>& _hash)
	{
		m_hashHdr = _hash;
		m_counter.setHash(_hash);
	}

public:
	ORZ_PLUS_INLINE
	int32				add					(const Key& _key, const Value& _value, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].add(_key, _value, _usingLock);
		if (!err)
			m_counter.inc(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	Value*				get					(const Key& _key, const HashArg& _hashArg, bool _usingLock = true)
	{
		return m_maps[m_hashHdr(_hashArg)].get(_key, _usingLock);
	}

	ORZ_PLUS_INLINE 
	int32				erase				(const Key& _key, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].erase(_key, _usingLock);
		if (!err)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	int32				erase				(const Key& _key, Value& _del, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].erase(_key, _del, _usingLock);
		if (!err)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	int32				eraseAdd			(const Key& _key, const Value& _value, Value& _del, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].eraseAdd(_key, _value, _del, _usingLock);
		if (err == 1)
			m_counter.inc(_hashArg);
		else if (err == 2)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE
	void				lockRead			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].beginRead();
	}

	ORZ_PLUS_INLINE
	void				unlockRead			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].endRead();
	}

	ORZ_PLUS_INLINE
	boost::shared_mutex&
						mutex				(const Key& _key, const HashArg& _hashArg, bool _usingLock = true) const
	{
		return m_maps[m_hashHdr(_hashArg)].mutex(_usingLock);
	}

	ORZ_PLUS_INLINE
	void				lockWrite			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].beginWrite();
	}

	ORZ_PLUS_INLINE
	void				unlockWrite			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].endWrite();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::iterator
						mapBegin			(const Key& _key, const HashArg& _hashArg)
	{
		return m_maps[m_hashHdr(_hashArg)].begin();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::const_iterator
						mapBegin			(const Key& _key, const HashArg& _hashArg) const
	{
		return m_maps[m_hashHdr(_hashArg)].begin();
	}

	//!
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::iterator
						mapEnd				(const Key& _key, const HashArg& _hashArg)
	{
		return m_maps[m_hashHdr(_hashArg)].end();
	}

	//! 
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::const_iterator
						mapEnd				(const Key& _key, const HashArg& _hashArg) const
	{
		return m_maps[m_hashHdr(_hashArg)].end();
	}

	ORZ_PLUS_INLINE 
	uint32				size				() const
	{
		int32 r = m_counter.get();
		if (r < 0)
		{
			r = 0;
		}
		return (uint32)r;
	}

	ORZ_PLUS_INLINE 
	uint32				arraySize			() const
	{
		return m_maps.size();
	}

	ORZ_PLUS_INLINE
	bool				empty				() const
	{
		return m_counter.get() == 0 ? true : false;
	}

	ORZ_PLUS_INLINE
	std::map<Key, Value, Compare>&
						map					(uint32 _index)
	{
		if (_index >= m_maps.size())
			return m_maps[0].map();
		else
			return m_maps[_index].map();
	}

	ORZ_PLUS_INLINE
	ThreadMap<Key, Value, Compare>&
						threadMap			(uint32 _index)
	{
		if (_index >= m_maps.size())
			return m_maps[0];
		else
			return m_maps[_index];
	}

	ORZ_PLUS_INLINE
	uint32				base				() const
	{
		return m_base;
	}

	ORZ_PLUS_INLINE
	void				clear				(bool _usingLock = true)
	{
		for (uint32 i=0; i<m_maps.size(); ++i)
		{
			m_maps[i].clear(_usingLock);
		}
	}

private:
	ORZ_PLUS_INLINE
	Number				defaultHash			(const HashArg& _hashArg)
	{
		return _hashArg % m_base;
	}

private:
	uint32											m_base;
	DynArray<ThreadMap<Key, Value, Compare> >		m_maps;
	HashCounter<HashArg>							m_counter;
	boost::function<Number (const HashArg&)>		m_hashHdr;
};


//! 哈希Map
/*!
 *	\note 线程安全，使用哈希原理和加锁的std::map来确保效率和安全兼顾
 *	\note HashShrMap用于存放元素的指针或者被boost::shared_ptr包装的元素，因为通过get方法返回的时map中value的拷贝\n
 *		而之前的HashMap用于直接存放元素，因为通过get方法返回的时存储在Map内的元素指针，而非元素的拷贝；
 example:
 \code
	HashShrMap<uint32, int32>	hmap(10);	// 创建了一个key为uint32、value为int32、hashArg为默认的key的类型，hash大小为10的HashMap
	hmap.add(1, 1, 1, true);	// 线程1
	hmap.add(2, 2, 2, true);	// 线程2
	hmap.add(3, 3, 3, true);	// 线程3
	hmap.add(11, 2, 11, true);	// 线程1
	...
	// 前3个被添加的元素分别被存放在hash分组的1、2和3组的各自的std::map中,true表示add的时候要加锁
	// 最后一个key为11，value为2的，被添加在和1同std::map中，因为默认的hash函数 为 hashArg 对 分组的最大数量求余： 1%10 == 11%10

	int32 i = hmap.get(1, 1, true);	// 线程4 i == 1 true表示get的时候要加锁

	// 可以配合DynShrLock来使用：
	{
		DynShrLock lock(hmap.mutex(21));	// 对hmap中分组为21%10==1的std::map加锁，而其他0，2~9的分组均没有加锁，这样其它线程同时访问其他分组的时候，就可以wait-free
		hmap.add(21, 2, 21, false);			// false表示add不加锁
		int32 i = hmap.get(21,21, false);	// false表示get不加锁 i == 2
	}
	// lock销毁，对21%10==1的分组的std::map解锁
 \endcode
 */
template 
<
	class Key, 
	typename Value, 
	typename HashArg = Key,
	class Compare = std::less<Key> 
>
class HashShrMap
{
public:
	ORZ_PLUS_INLINE
	HashShrMap() : m_base(1), m_maps(m_base), m_counter(m_base) {}

	ORZ_PLUS_INLINE explicit
	HashShrMap(uint32 _bits) : m_base(_bits), m_maps(m_base), m_counter(m_base) {}

	ORZ_PLUS_INLINE
	~HashShrMap() {}

	ORZ_PLUS_INLINE
	void				setHash				()
	{
		m_hashHdr = boost::bind(&HashShrMap<Key, Value, HashArg, Compare>::defaultHash, this, _1);
		m_counter.setHash();
	}

	ORZ_PLUS_INLINE
	void				setHash				(const boost::function<Number (const HashArg&)>& _hash)
	{
		m_hashHdr = _hash;
		m_counter.setHash(_hash);
	}

public:
	ORZ_PLUS_INLINE
	int32				add					(const Key& _key, const Value& _value, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].add(_key, _value, _usingLock);
		if (!err)
			m_counter.inc(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	Value				get					(const Key& _key, const HashArg& _hashArg, bool _usingLock = true)
	{
		return m_maps[m_hashHdr(_hashArg)].get(_key, _usingLock);
	}

	ORZ_PLUS_INLINE 
	int32				erase				(const Key& _key, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].erase(_key, _usingLock);
		if (!err)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	int32				erase				(const Key& _key, Value& _del, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].erase(_key, _del, _usingLock);
		if (!err)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE 
	int32				eraseAdd			(const Key& _key, const Value& _value, Value& _del, const HashArg& _hashArg, bool _usingLock = true)
	{
		int32 err = m_maps[m_hashHdr(_hashArg)].eraseAdd(_key, _value, _del, _usingLock);
		if (err == 1)
			m_counter.inc(_hashArg);
		else if (err == 2)
			m_counter.dec(_hashArg);
		return err;
	}

	ORZ_PLUS_INLINE
	void				lockRead			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].beginRead();
	}

	ORZ_PLUS_INLINE
	void				unlockRead			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].endRead();
	}

	ORZ_PLUS_INLINE
	boost::shared_mutex&
						mutex				(const Key& _key, const HashArg& _hashArg, bool _usingLock = true) const
	{
		return m_maps[m_hashHdr(_hashArg)].mutex(_usingLock);
	}

	ORZ_PLUS_INLINE
	void				lockWrite			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].beginWrite();
	}

	ORZ_PLUS_INLINE
	void				unlockWrite			(const Key& _key, const HashArg& _hashArg)
	{
		m_maps[m_hashHdr(_hashArg)].endWrite();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::iterator
						mapBegin			(const Key& _key, const HashArg& _hashArg)
	{
		return m_maps[m_hashHdr(_hashArg)].begin();
	}

	//! 将元素指针指向容器开头
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::const_iterator
						mapBegin			(const Key& _key, const HashArg& _hashArg) const
	{
		return m_maps[m_hashHdr(_hashArg)].begin();
	}

	//!
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::iterator
						mapEnd				(const Key& _key, const HashArg& _hashArg)
	{
		return m_maps[m_hashHdr(_hashArg)].end();
	}

	//! 
	ORZ_PLUS_INLINE 
	typename std::map<Key, Value, Compare>::const_iterator
						mapEnd				(const Key& _key, const HashArg& _hashArg) const
	{
		return m_maps[m_hashHdr(_hashArg)].end();
	}

	ORZ_PLUS_INLINE 
	uint32				size				() const
	{
		int32 r = m_counter.get();
		if (r < 0)
		{
			r = 0;
		}
		return (uint32)r;
	}

	ORZ_PLUS_INLINE 
	uint32				arraySize			() const
	{
		return m_maps.size();
	}

	ORZ_PLUS_INLINE
	bool				empty				() const
	{
		return size() == 0 ? true : false;
	}

	ORZ_PLUS_INLINE
	std::map<Key, Value, Compare>&
						map					(uint32 _index)
	{
		if (_index >= m_maps.size())
			return m_maps[0].map();
		else
			return m_maps[_index].map();
	}

	ORZ_PLUS_INLINE
	ThreadShrMap<Key, Value, Compare>&
						threadMap			(uint32 _index)
	{
		if (_index >= m_maps.size())
			return m_maps[0];
		else
			return m_maps[_index];
	}

	ORZ_PLUS_INLINE
	uint32				base				() const
	{
		return m_base;
	}

	ORZ_PLUS_INLINE
	void				clear				(bool _usingLock = true)
	{
		for (uint32 i=0; i<m_maps.size(); ++i)
		{
			m_maps[i].clear(_usingLock);
		}
	}

private:
	ORZ_PLUS_INLINE
	Number				defaultHash			(const HashArg& _hashArg)
	{
		return _hashArg % m_base;
	}

private:
	uint32											m_base;
	DynArray<ThreadShrMap<Key, Value, Compare> >	m_maps;
	HashCounter<HashArg>							m_counter;
	boost::function<Number (const HashArg&)>		m_hashHdr;
};
}
#endif
