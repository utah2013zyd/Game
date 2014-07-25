#ifndef __Orz_OrzAsio_Packet_h__
#define __Orz_OrzAsio_Packet_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/OrzAsio/NetEngine/PacketEnv.h>
namespace Orz 
{
//! OrzAsio提供的网络封包类实现
/*!
 *	\note 接口说明见IPacket
 *	\note big endian 问题已经在内部自动处理，用户可以根据ORZ_PLUS_CONFIG_BIG_ENDIAN宏来控制是否为big endian
 *	\note 默认是big endian
 example:
 \code
	#define PACKET_MAX_LEN 1400
	typedef Packet<PACKET_MAX_LEN>	NetPacket;
 \endcode
 */
template <uint32 SIZE>
class Packet
	: public IPacket
{
	typedef MemString<SIZE> PacketData;
public:
	ORZ_PLUS_INLINE
	Packet()
		: m_dynData(0)
		, m_bIsDyn(false)
		, m_size(PACKET_HEAD_SIZE)
		, m_seeker(0)
#ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
		, m_be(true)
#else
		, m_be(false)
#endif
	{
		int32 size = 0;
		int32 type = 0;
		char head[PACKET_HEAD_SIZE];
		m_data.append(head, PACKET_HEAD_SIZE);
		setSize(size);
		setMsg(type);
		setDateTime(getDateTime());
	}
	ORZ_PLUS_INLINE
	virtual ~Packet()
	{
		freeDyn();
	}

private:
	ORZ_PLUS_INLINE
	void freeDyn()
	{
		if (m_dynData)
		{
			delete m_dynData;
			m_dynData = 0;
			clear();
		}
	}

public:
	//---------------------------------------------------------------------
	//! 返回 0成功 1失败 拥有write_前缀的方法能够自动进行字节序的转换
	ORZ_PLUS_INLINE int32			writeByte			(int8 _data)			{ return put(_data); }
	ORZ_PLUS_INLINE int32			writeUbyte			(uint8 _data)			{ return put(_data); }
	ORZ_PLUS_INLINE int32			writeShort			(int16 _data)			{ if (m_be) swap16(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeUshort			(uint16 _data)			{ if (m_be) swap16(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeInt			(int32 _data)			{ if (m_be) swap32(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeUint			(uint32 _data)			{ if (m_be) swap32(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeLong			(int64 _data)			{ if (m_be) swap64(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeUlong			(uint64 _data)			{ if (m_be) swap64(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeFloat			(float32 _data)			{ if (m_be) swapfloat(&_data); return put(_data); }
	ORZ_PLUS_INLINE int32			writeDouble			(float64 _data)			{ if (m_be) swapdouble(&_data); return put(_data); }

	ORZ_PLUS_INLINE int32			writeStr			(const StringPtr& _str)
	{
		int16 size = (int16)_str.size();
		if (!_str.ptr())
		{
			size = 0;
		}
		if (m_be)
			swap16(&size);
		put(size);
		put(_str.ptr(), _str.size());
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeBytes			(const std::vector<int8>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<int8>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			put(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeUbytes			(const std::vector<uint8>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<uint8>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			put(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeShorts			(const std::vector<int16>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<int16>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeShort(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeUshorts		(const std::vector<uint16>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<uint16>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeUshort(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeInts			(const std::vector<int32>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<int32>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeInt(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeUints			(const std::vector<uint32>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<uint32>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeUint(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeLongs			(const std::vector<int64>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<int64>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeLong(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeUlongs			(const std::vector<uint64>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<uint64>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeUlong(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeFloats			(const std::vector<float32>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<float32>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeFloat(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeDoubles		(const std::vector<float64>& _data)
	{
		if (_data.empty())
			return 1;

		int16 size = (int16)_data.size();
		if (m_be)
			swap16(&size);
		put(size);
		for (std::vector<float64>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeDouble(*itr);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			writeStrs			(const std::vector<StringPtr>& _data)
	{
		if (_data.empty())
			return 1;

		int16 tsize = (int16)_data.size();
		if (m_be)
			swap16(&tsize);
		put(tsize);
		for (std::vector<StringPtr>::const_iterator itr = _data.begin(); itr!=_data.end(); ++itr)
		{
			writeStr(*itr);
		}
		return 0;
	}

	ORZ_PLUS_INLINE int32			writeBool			(bool _data)
	{
		int8 b = 0;
		if (_data) b = 1;
		return put(&b, sizeof(int8));
	}

private:
	ORZ_PLUS_INLINE int32			put					(int8 _data)			{ return put(&_data, sizeof(int8)); }
	ORZ_PLUS_INLINE int32			put					(uint8 _data)			{ return put(&_data, sizeof(uint8)); }
	ORZ_PLUS_INLINE int32			put					(int16 _data)			{ return put(&_data, sizeof(int16)); }
	ORZ_PLUS_INLINE int32			put					(uint16 _data)			{ return put(&_data, sizeof(uint16)); }
	ORZ_PLUS_INLINE int32			put					(int32 _data)			{ return put(&_data, sizeof(int32)); }
	ORZ_PLUS_INLINE int32			put					(uint32 _data)			{ return put(&_data, sizeof(uint32)); }
	ORZ_PLUS_INLINE int32			put					(int64 _data)			{ return put(&_data, sizeof(int64)); }
	ORZ_PLUS_INLINE int32			put					(uint64 _data)			{ return put(&_data, sizeof(uint64)); }
	ORZ_PLUS_INLINE int32			put					(float32 _data)			{ return put(&_data, sizeof(float32)); }
	ORZ_PLUS_INLINE int32			put					(float64 _data)			{ return put(&_data, sizeof(float64)); }

public:
	ORZ_PLUS_INLINE int32			put					(const void* _data, const uint32 _size)
	{
		if (_data == 0 || _size == 0)
			return 0;

		if (m_bIsDyn)
		{
			m_dynData->append((const char*)_data, _size);
			uint32 size = m_size - PACKET_HEAD_SIZE + _size;
			m_size += (uint32)_size;
			setSize(size);
			return 0;
		}

		if (m_size + _size > SIZE)
		{
			// malloc mem and cp mem to dy
			uint32 size = m_size - PACKET_HEAD_SIZE + _size;
			m_dynData = new std::string;
			m_dynData->append(m_data.c_str(), m_size);
			m_dynData->append((const char*)_data, _size);
			m_size += (uint32)_size;
			m_bIsDyn = true;
			setSize(size);
			return 0;
		}

		m_data.append((const char*)_data, (uint32)_size);
		char* ptr = m_data.raw();
		if (!ptr)
			return 1;

		int32 size = m_size - (uint32)PACKET_HEAD_SIZE + _size;
		setSize(size);
		m_size = m_data.size();
		return 0;
	}
	ORZ_PLUS_INLINE int32			overWrite			(uint32 _offset, const void* _data, const uint32 _size)
	{
		if (_offset + _size >= m_size)
			return 1;

		if (m_bIsDyn)
			m_dynData->replace(_offset, _size, (const char*)_data, _size);
		else
			memcpy(m_data.raw()+_offset, _data, _size);
		return 0;
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	ORZ_PLUS_INLINE int8			readByte			(int32& _err)
	{
		int8 i; 
		int32 err = get(i);
		if (err)
			_err = err;
		return i; 
	}
	ORZ_PLUS_INLINE uint8			readUbyte			(int32& _err)
	{
		uint8 i; 
		int32 err = get(i);
		if (err)
			_err = err;
		return i; 
	}
	ORZ_PLUS_INLINE int16			readShort			(int32& _err)
	{
		int16 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap16(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE uint16			readUshort			(int32& _err)
	{
		uint16 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap16(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE int32			readInt				(int32& _err)
	{
		int32 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap32(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE uint32			readUint			(int32& _err)
	{
		uint32 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap32(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE int64			readLong			(int32& _err)
	{
		int64 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap64(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE uint64			readUlong			(int32& _err)
	{
		uint64 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swap64(&i); 
		return i; 
	}
	ORZ_PLUS_INLINE float32			readFloat			(int32& _err)
	{
		float32 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swapfloat(&i); 
		return i;
	}
	ORZ_PLUS_INLINE float64			readDouble			(int32& _err)
	{
		float64 i; 
		int32 err = get(i); 
		if (err)
			_err = err;
		if (m_be) swapdouble(&i); 
		return i;
	}

	ORZ_PLUS_INLINE StringPtr		readStr				(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		StringPtr rs;
		if (m_be)
			swap16(&size);

		if (err != 0)
		{
			_err = 1;
			return rs;
		}

		err = get(rs, size);
		if (err)
			_err = err;
		return rs;
	}
	ORZ_PLUS_INLINE 
	std::vector<int8>			readBytes			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<int8> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readByte(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<uint8>			readUbytes			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<uint8> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readUbyte(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<int16>			readShorts			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<int16> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readShort(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<uint16>			readUshorts			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<uint16> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readUshort(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<int32>			readInts			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<int32> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readInt(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<uint32>			readUints			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<uint32> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readUint(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<int64>			readLongs			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<int64> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readLong(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<uint64>			readUlongs			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<uint64> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readUlong(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<float32>		readFloats			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<float32> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readFloat(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<float64>		readDoubles			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<float64> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readDouble(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}
	ORZ_PLUS_INLINE 
	std::vector<StringPtr>		readStrs			(int32& _err)
	{
		int16 size = 0;
		int32 err = get(size);
		if (m_be)
			swap16(&size);

		std::vector<StringPtr> vec;
		for (int16 i=0; i<size; ++i)
		{
			vec.push_back(readStr(err));
		}
		if (vec.empty() || err)
			_err = 1;
		return vec;
	}

	ORZ_PLUS_INLINE int32			readBool			(bool& _data)
	{
		int8 b;
		int32 err = get(&b, sizeof(int8));
		if (b == 0) _data = false;
		else 
		if (b == 1) _data = true;
		return err;
	}
private:
	ORZ_PLUS_INLINE int32			get					(int8& _data)			{ return get(&_data, sizeof(int8)); }
	ORZ_PLUS_INLINE int32			get					(uint8& _data)			{ return get(&_data, sizeof(uint8)); }
	ORZ_PLUS_INLINE int32			get					(int16& _data)			{ return get(&_data, sizeof(int16)); }
	ORZ_PLUS_INLINE int32			get					(uint16& _data)			{ return get(&_data, sizeof(uint16)); }
	ORZ_PLUS_INLINE int32			get					(int32& _data)			{ return get(&_data, sizeof(int32)); }
	ORZ_PLUS_INLINE int32			get					(uint32& _data)			{ return get(&_data, sizeof(uint32)); }
	ORZ_PLUS_INLINE int32			get					(int64& _data)			{ return get(&_data, sizeof(int64)); }
	ORZ_PLUS_INLINE int32			get					(uint64& _data)			{ return get(&_data, sizeof(uint64)); }
	ORZ_PLUS_INLINE int32			get					(float32& _data)		{ return get(&_data, sizeof(float32)); }
	ORZ_PLUS_INLINE int32			get					(float64& _data)		{ return get(&_data, sizeof(float64)); }

public:
	ORZ_PLUS_INLINE int32			get					(void* _data, const uint32 _size)
	{
		if (_size == 0)
			return 0;

		if (m_size < m_seeker + PACKET_HEAD_SIZE)
			return 1;

		uint32 getsize = 0;
		uint32 lastsize = m_size - m_seeker - PACKET_HEAD_SIZE;
		if (_size > lastsize)
			return 1;

		if (m_bIsDyn)
		{
			if (!m_dynData || m_dynData->empty())
				return 1;

			memcpy(_data, m_dynData->c_str() + PACKET_HEAD_SIZE + m_seeker, _size);
			m_seeker += _size;
			return 0;
		}

		char* ptr = m_data.raw();
		if (!ptr)
			return 1;
		
		memcpy(_data, ptr + PACKET_HEAD_SIZE + m_seeker, _size);
		m_seeker += _size;
		return 0;
	}
	ORZ_PLUS_INLINE int32			get					(StringPtr& _rs, const uint32 _size)
	{
		if (_size == 0)
			return 0;

		if (m_size < m_seeker + PACKET_HEAD_SIZE)
			return 1;

		uint32 getsize = 0;
		uint32 lastsize = m_size - m_seeker - PACKET_HEAD_SIZE;
		if (_size > lastsize)
			return 1;

		if (m_bIsDyn)
		{
			if (!m_dynData || m_dynData->empty())
				return 1;

			_rs.reset(&(*m_dynData)[m_seeker+PACKET_HEAD_SIZE], _size);
			m_seeker += _size;
			return 0;
		}

		char* ptr = m_data.raw();
		if (!ptr)
			return 1;
		
		_rs.reset(ptr+m_seeker+PACKET_HEAD_SIZE, _size);
		m_seeker += _size;
		return 0;
	}
	ORZ_PLUS_INLINE int32			reRead				(uint32 _offset, void* _data, const uint32 _size)
	{
		if (_offset + _size > m_size - PACKET_HEAD_SIZE)
			return 1;

		if (m_bIsDyn)
			memcpy(_data, m_dynData->substr(_offset, _size).c_str(), _size);
		else
			memcpy(_data, m_data.raw()+_offset, _size);
		return 0;
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	ORZ_PLUS_INLINE const char*		raw					() const
	{
		return m_bIsDyn ? m_dynData->c_str() : m_data.c_str();
	}
	ORZ_PLUS_INLINE void			reset				()
	{
		m_seeker = 0;
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	ORZ_PLUS_INLINE void			setSize				(uint32 _size)
	{
		if (m_be) 
			swap32(&_size);

		if (m_bIsDyn)
		{
			m_dynData->replace(0, PACKET_CAPACITY_SIZE, (const char*)&_size, PACKET_CAPACITY_SIZE);
			m_size = (uint32)m_dynData->size();
		}
		else
		{
			memcpy(m_data.raw(), &_size, PACKET_CAPACITY_SIZE);
			m_size = m_data.size();
		}
	}
	ORZ_PLUS_INLINE uint32			getSize				() const
	{
		return m_size - PACKET_HEAD_SIZE;
	}
	ORZ_PLUS_INLINE int32			setMsg				(const int32 _msg)
	{
		int32 msg = _msg;

		if (m_be)
			swap32(&msg);

		if (m_bIsDyn)
			m_dynData->replace(PACKET_CAPACITY_SIZE, PACKET_TYPE_SIZE, (const char*)&msg, PACKET_TYPE_SIZE);
		else
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE, &msg, PACKET_TYPE_SIZE);

		return 0;
	}
	ORZ_PLUS_INLINE int32			getMsg				() const
	{
		int32 type;
		if (m_bIsDyn)
			memcpy(&type, m_dynData->c_str()+PACKET_CAPACITY_SIZE, PACKET_TYPE_SIZE);
		else
			memcpy(&type, m_data.c_str()+PACKET_CAPACITY_SIZE, PACKET_TYPE_SIZE);

		if (m_be)
			swap32(&type);
		return type;
	}
	ORZ_PLUS_INLINE int32			setDateTime			(const DateSys& _dTm)
	{
		int16 year	= (int16)_dTm.year;
		int8 month	= (int8)_dTm.month;
		int8 day	= (int8)_dTm.day;
		int8 hour	= (int8)_dTm.hour;
		int8 min	= (int8)_dTm.min;
		int8 sec	= (int8)_dTm.sec;
		int16 msec	= (int16)_dTm.msec;
		if (m_be)
		{
			swap16(&year); 
			swap16(&msec);
		}
		if (m_bIsDyn)
		{
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, 
																		PKT_DT_YEAR_SIZE, (const char*)&year, PKT_DT_YEAR_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE,						
																		PKT_DT_MONTH_SIZE, (const char*)&month, PKT_DT_MONTH_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE, 
																		PKT_DT_DAY_SIZE, (const char*)&day, PKT_DT_DAY_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE, 
																		PKT_DT_HOUR_SIZE, (const char*)&hour, PKT_DT_HOUR_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE, 
																		PKT_DT_MIN_SIZE, (const char*)&min, PKT_DT_MIN_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE, 
																		PKT_DT_SEC_SIZE, (const char*)&sec, PKT_DT_SEC_SIZE);
			m_dynData->replace(PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE + PKT_DT_SEC_SIZE, 
																		PKT_DT_MSEC_SIZE, (const char*)&msec, PKT_DT_MSEC_SIZE);
		}
		else
		{
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, 
																		(const char*)&year, PKT_DT_YEAR_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE,						
																		(const char*)&month, PKT_DT_MONTH_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE, 
																		(const char*)&day, PKT_DT_DAY_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE, 
																		(const char*)&hour, PKT_DT_HOUR_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE, 
																		(const char*)&min, PKT_DT_MIN_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE, 
																		(const char*)&sec, PKT_DT_SEC_SIZE);
			memcpy(m_data.raw() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE + PKT_DT_SEC_SIZE, 
																		(const char*)&msec, PKT_DT_MSEC_SIZE);
		}
		return 0;
	}
	ORZ_PLUS_INLINE int32			getDT				(DateSys& _dTm) const
	{
		int16 year; int8 month; int8 day; int8 hour;
		int8 min;	int8 sec;	int16 msec;
		
		if (m_bIsDyn)
		{
			memcpy(&year, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, 
																							PKT_DT_YEAR_SIZE);
			memcpy(&month, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE,						
																							PKT_DT_MONTH_SIZE);
			memcpy(&day, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE, 
																							PKT_DT_DAY_SIZE);
			memcpy(&hour, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE, 
																							PKT_DT_HOUR_SIZE);
			memcpy(&min, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE, 
																							PKT_DT_MIN_SIZE);
			memcpy(&sec, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE, 
																							PKT_DT_SEC_SIZE);
			memcpy(&msec, m_dynData->c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE + PKT_DT_SEC_SIZE, 
																							PKT_DT_MSEC_SIZE);
		}
		else
		{
			memcpy(&year, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, 
																							PKT_DT_YEAR_SIZE);
			memcpy(&month, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE,						
																							PKT_DT_MONTH_SIZE);
			memcpy(&day, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE, 
																							PKT_DT_DAY_SIZE);
			memcpy(&hour, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE, 
																							PKT_DT_HOUR_SIZE);
			memcpy(&min, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE, 
																							PKT_DT_MIN_SIZE);
			memcpy(&sec, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE, 
																							PKT_DT_SEC_SIZE);
			memcpy(&msec, m_data.c_str() + PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE 
								+ PKT_DT_YEAR_SIZE + PKT_DT_MONTH_SIZE + PKT_DT_DAY_SIZE + PKT_DT_HOUR_SIZE + PKT_DT_MIN_SIZE + PKT_DT_SEC_SIZE, 
																							PKT_DT_MSEC_SIZE);
		}

		if (m_be)
		{
			swap16(&year); 
			swap16(&msec);
		}
		_dTm.year = year;
		_dTm.month = month;
		_dTm.day = day;
		_dTm.hour = hour;
		_dTm.min = min;
		_dTm.sec = sec;
		_dTm.msec = msec;
		return 0;
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	ORZ_PLUS_INLINE bool			empty				() const
	{
		if (m_size <= PACKET_HEAD_SIZE)
			return true;
		else
			return false;
	}
	ORZ_PLUS_INLINE void			clear				()
	{
		m_data.clear();
		m_size = PACKET_HEAD_SIZE;

		int32 size = 0;
		int32 type = 0;
		char head[PACKET_HEAD_SIZE];
		m_data.append(head, PACKET_HEAD_SIZE);

		m_bIsDyn = false;
		setSize(size);
		setMsg(type);
		setDateTime(getDateTime());

		reset();
		freeDyn();
	}
	ORZ_PLUS_INLINE uint32			size				() const
	{
		return m_size;
	}
	ORZ_PLUS_INLINE bool			isDyn				() const
	{
		return m_bIsDyn;
	}
	//---------------------------------------------------------------------

private:
	PacketData					m_data;
	std::string*				m_dynData;
	bool						m_bIsDyn;
	uint32						m_size;
	uint32						m_seeker;
	bool						m_be;
};
}
#endif
