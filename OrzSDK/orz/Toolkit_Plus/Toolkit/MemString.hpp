#ifndef __Orz_ToolkitPlus_MemString_hpp__
#define __Orz_ToolkitPlus_MemString_hpp__
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
namespace Orz
{
//! “纯净的内存串”，完全面向底层，不关心格式和内容，仅将数据作为内存中的字节处理；最大长度通过SIZE来设定；接口类似std::string
template <uint32 SIZE>
class MemString
{
public:
	ORZ_PLUS_INLINE
	MemString()
		: m_size(0)
		, m_bad_cr('\0')
	{
		m_data[0] = '\0';
	}

	ORZ_PLUS_INLINE
	MemString(const char* _data)
		: m_size(0)
		, m_bad_cr('\0')
	{
		if (!_data)
			return;

		uint32 size = 0;
		if (strlen(_data) > SIZE)
			size = SIZE;
		else
			size = (uint32)strlen(_data);

		m_size = size;
		memcpy(m_data, _data, m_size);
		if (m_size < SIZE)
			m_data[m_size] = '\0';
	}

	ORZ_PLUS_INLINE
	MemString(const char* _data, uint32 _size)
		: m_size(0)
		, m_bad_cr('\0')
	{
		if (!_data)
			return;

		uint32 size = 0;
		if (_size > SIZE)
			size = SIZE;
		else
			size = _size;

		m_size = size;
		memcpy(m_data, _data, m_size);
		if (m_size < SIZE)
			m_data[m_size] = '\0';
	}

	template <uint32 SZ>
	ORZ_PLUS_INLINE
	MemString(const MemString<SZ>& _other)
		: m_size(0)
		, m_bad_cr('\0')
	{
		if (_other.size() > SIZE)
			m_size = SIZE;
		else
			m_size = _other.size();
		memcpy(m_data, _other.c_str(), m_size);
		if (m_size < SIZE)
			m_data[m_size] = '\0';
	}

	ORZ_PLUS_INLINE
	~MemString()
	{
	}

public:
	template <uint32 SZ>
	ORZ_PLUS_INLINE
	MemString& operator=(const MemString<SZ>& _other)
	{
		if((const void*)this == (const void*)&_other)
            return *this;

		if (_other.size() > SIZE)
			m_size = SIZE;
		else
			m_size = _other.size();

		memcpy(m_data, _other.c_str(), m_size);

		if (m_size < SIZE)
			m_data[m_size] = '\0';

		return *this;
	}

	ORZ_PLUS_INLINE
	char&		operator[](uint32 _pos)
	{
		if (_pos >= m_size)
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
		//if (_pos >= m_size)
		//	return m_bad_cr;

			return m_data[_pos];
	}

	ORZ_PLUS_INLINE
	char		operator[](uint32 _pos) const
	{
		if (_pos >= m_size)
			throw RangeExp(RangeExp::REXP_ARRAY, ThrowAddress(__FILE__, __LINE__));
		else
			return m_data[_pos];
	}

	template <uint32 SZ>
	ORZ_PLUS_INLINE
	bool		operator==(const MemString<SZ>& _other) const
	{
		if (m_size != _other.size())
			return false;

		for (uint32 i=0; i<m_size; ++i)
		{
			if (m_data[i] != _other[i])
				return false;
		}
		return true;
	}
	
	template <uint32 SZ>
	ORZ_PLUS_INLINE
	MemString& operator+=(const MemString<SZ>& _other)
	{
		uint32 csize = 0;
		uint32 rmsz = m_size + _other.size();
		if (rmsz > SIZE)
			csize = SIZE - m_size;
		else
			csize = _other.size();

		memcpy(m_data+m_size, _other.c_str(), csize);
		m_size += csize;
		if (m_size < SIZE)
			m_data[m_size] = '\0';
		
		return *this;
	}

	ORZ_PLUS_INLINE
	MemString& append(const char* _data, uint32 _size)
	{
		if (!_data)
			return *this;

		uint32 csize = 0;
		uint32 rmsz = m_size + _size;
		if (rmsz > SIZE)
			csize = SIZE - m_size;
		else
			csize = _size;

		memcpy(m_data+m_size, _data, csize);
		m_size += (uint32)csize;
		if (m_size < SIZE)
			m_data[m_size] = '\0';
		
		return *this;
	}

public:
	//!
	ORZ_PLUS_INLINE uint32			size			() const
	{
		return m_size;
	}

	//!
	ORZ_PLUS_INLINE bool			empty			() const
	{
		if (m_size == 0)
			return true;
		else
			return false;
	}

	ORZ_PLUS_INLINE void			clear			()
	{
		m_size = 0;
		m_data[0] = '\0';
	}

	//! 1 失败，0成功
	ORZ_PLUS_INLINE int32			push_back		(const char _c)
	{
		if (m_size == SIZE)
			return 1;

		memcpy(m_data+m_size, &_c, 1);
		++m_size;

		if (m_size < SIZE)
			m_data[m_size] = '\0';

		return 0;
	}

	//!
	ORZ_PLUS_INLINE const char*		c_str			() const
	{
		return m_size == 0 ? 0 : m_data;
	}

	ORZ_PLUS_INLINE char*			raw				()
	{
		return m_data;
	}

private:
	char					m_data[SIZE];
	char					m_bad_cr;
	uint32					m_size;
};

}

#endif
