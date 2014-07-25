#ifndef __Orz_ToolkitPlus_ClrId_hpp_
#define __Orz_ToolkitPlus_ClrId_hpp_
#include <orz/Toolkit_Plus/Toolkit/NumDef.hpp>
namespace Orz
{
struct ClrId
{
	ORZ_PLUS_INLINE
	ClrId() : id(0), clr(0), valid(false) {}

	ORZ_PLUS_INLINE
	ClrId(Number _id, uint32 _clr, bool _is_valid) : id(_id), clr(_clr), valid(_is_valid) {}

	ORZ_PLUS_INLINE
	bool isValid() const
	{
		return valid;
	}

	ORZ_PLUS_INLINE
	void clear()
	{
		id = 0;
		clr = 0;
		valid = false;
	}

	ORZ_PLUS_INLINE
	Number getUniqueId(uint32 _base)
	{
		return _base*clr + id;
	}

	ORZ_PLUS_INLINE
	bool operator==(const ClrId& _other) const
	{
		return (id == _other.id && clr == _other.clr);
	}

	ORZ_PLUS_INLINE
	Number operator%(uint32 _base) const
	{
		return (id+clr)%_base;
	}

	ORZ_PLUS_INLINE
	bool operator<(const ClrId& _other) const
	{
		if (clr < _other.clr)
			return true;
		else if (clr > _other.clr)
			return false;

		return id < _other.id;
	}

	Number id;
	uint32 clr;
	bool valid;
};

class ClrIdLess
{
public:
	ORZ_PLUS_INLINE
	bool operator() (const ClrId& _clrId1, const ClrId& _clrId2) const
	{
		if (_clrId1.clr < _clrId2.clr)
			return true;
		else if (_clrId1.clr > _clrId2.clr)
			return false;

		return _clrId1.id < _clrId2.id;
	}
};

}

#endif
