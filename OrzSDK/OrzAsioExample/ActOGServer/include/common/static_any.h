#ifndef STATIC_ANY_H_
#define STATIC_ANY_H_

#include <typeinfo>
#include <orz/OrzAsio/OrzAsioConfig.h>


template <uint32 SIZE>
class static_any
{
public:
	class Nil {};

	static_any()
		: m_size(0)
		, m_type(&typeid(Nil))
	{
	}

	template <typename ValueType>
	static_any(const ValueType& _value, uint32 _size=sizeof(ValueType))
		: m_size(_size)
		, m_type(&typeid(ValueType))
	{
		memcpy(m_data, &_value, _size);
	}

	static_any(const static_any<SIZE>& _other)
		: m_size(0)
		, m_type(&typeid(Nil))
	{
		memcpy(m_data, _other.m_data, _other.m_size);
		m_size = _other.m_size;
		m_type = _other.m_type;
	}

	~static_any()
	{
	}

public:
	static_any<SIZE>&				operator=				(const static_any<SIZE>& _other)
	{
		if (&_other == this)
			return *this;

		memcpy(m_data, _other.m_data, _other.m_size);
		m_size = _other.m_size;
		m_type = _other.m_type;

		return *this;
	}

public:
	//!
	ORZ_PLUS_INLINE const std::type_info&	type					() const
	{
		return *m_type;
	}

	//!
	ORZ_PLUS_INLINE uint32					size					() const
	{
		return m_size;
	}

	//!
	ORZ_PLUS_INLINE bool						is_empty				() const
	{
		if (*m_type == typeid(Nil))
			return true;
		else
			return false;
	}

	//!
	ORZ_PLUS_INLINE void						clear					()
	{
		m_type = &typeid(Nil);
		m_size = 0;
	}

	template <typename ValueType>
	ORZ_PLUS_INLINE void						set_data				(const ValueType& _value, uint32 _size=sizeof(ValueType))
	{
		m_size = _size;
		m_type = &typeid(ValueType);
		memcpy(m_data, &_value, _size);
	}

	template <typename ValueType>
	ORZ_PLUS_INLINE ValueType*				get_data				() const
	{
		if (typeid(ValueType) != *m_type)
			return 0;
		else
		{
			char* temp = const_cast<char*>(m_data);
			return reinterpret_cast<ValueType*>(temp);
		}
	}

private:
	char									m_data[SIZE];
	uint32									m_size;
	const std::type_info*					m_type;
};

#endif
