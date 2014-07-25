#ifndef __Orz_ToolkitPlus_StringPtr_hpp__
#define __Orz_ToolkitPlus_StringPtr_hpp__
#include <orz/Toolkit_Plus/GlobalPlus.h>
namespace Orz
{
//! just a ptr to string. no malloc no free no new no delete!
class StringPtr
{
public:
	ORZ_PLUS_INLINE
	StringPtr() : p(0), s(0) {}

	ORZ_PLUS_INLINE
	StringPtr(char* _ptr, uint32 _size) : p(_ptr), s(_size) {}

	ORZ_PLUS_INLINE
	StringPtr(char* _ptr) : p(_ptr), s((uint32)strlen(_ptr)) {}

	ORZ_PLUS_INLINE
	StringPtr(const std::string& _str) : p(const_cast<char*>(_str.c_str())), s((uint32)_str.size()) {}

	ORZ_PLUS_INLINE
	StringPtr(const char* _ptr, uint32 _size) : p(const_cast<char*>(_ptr)), s(_size) {}

	ORZ_PLUS_INLINE
	StringPtr(const char* _ptr) : p(const_cast<char*>(_ptr)), s((uint32)strlen(_ptr)) {}

	ORZ_PLUS_INLINE
	~StringPtr() {}

public:
	ORZ_PLUS_INLINE
	bool						operator==			(const char* _mem) const
	{
		return memcmp(p, _mem, s) == 0;
	}

	ORZ_PLUS_INLINE
	bool						operator==			(const StringPtr& _other) const
	{
		return memcmp(p, _other.p, s) == 0;
	}

	ORZ_PLUS_INLINE 
	const char*					ptr					() const
	{
		static char* NULLPTR = "";
		if (!p)
		{
			return NULLPTR;
		}
		return p;
	}

	ORZ_PLUS_INLINE 
	char*						ptr					()
	{
		static char* NULLPTR = "";
		if (!p)
		{
			return NULLPTR;
		}
		return p;
	}

	ORZ_PLUS_INLINE 
	void						reset				(char* _ptr, uint32 _size)
	{
		p = _ptr;
		s = _size;
	}

	ORZ_PLUS_INLINE 
	void						reset				(const char* _ptr, uint32 _size)
	{
		p = const_cast<char*>(_ptr);
		s = _size;
	}

	ORZ_PLUS_INLINE 
	bool						isValid				() const
	{
		return p == 0 ? false : true;
	}

	ORZ_PLUS_INLINE 
	const uint32				size				() const
	{
		return s;
	}

	ORZ_PLUS_INLINE 
	bool						empty				() const
	{
		return s == 0 ? true : false;
	}

private:
	char*			p;
	uint32			s;
};

}

#endif
