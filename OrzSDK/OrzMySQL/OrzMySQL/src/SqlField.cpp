#include <SqlField.h>
using namespace Orz;
using namespace Orz::Mysql;


SqlField::SqlField()
{
}

SqlField::~SqlField()
{
}

void SqlField::setValue(char* _value, uint32 _length)
{
	m_value = _value;
	m_length = _length;
}

uint32 SqlField::getLength()
{
	return m_length;
}

const char* SqlField::getString()
{
	return m_value;
}

float32 SqlField::getFloat32()
{
	return m_value ? static_cast<float32>(atof(m_value)) : 0; 
}

float64 SqlField::getFloat64()
{
	return m_value ? static_cast<float64>(atof(m_value)) : 0; 
}

bool SqlField::getBool()
{
	return m_value ? atoi(m_value) > 0 : false; 
}

uint8 SqlField::getUint8()
{
	return m_value ? static_cast<uint8>(atol(m_value)) : 0;
}

int8 SqlField::getInt8()
{
	return m_value ? static_cast<int8>(atol(m_value)) : 0;
}
	
uint16 SqlField::getUint16()
{
	return m_value ? static_cast<uint16>(atol(m_value)) : 0; 
}

int16 SqlField::getInt16()
{
	return m_value ? static_cast<int16>(atol(m_value)) : 0; 
}

uint32 SqlField::getUint32()
{
	return m_value ? static_cast<uint32>(atol(m_value)) : 0;
}

int32 SqlField::getInt32()
{
	return m_value ? static_cast<int32>(atol(m_value)) : 0;
}

uint64 SqlField::getUint64()
{
	if (m_value)
	{
		uint64 value;
		sscanf(m_value, I64FMTD, &value);
		return value;
	}
	else
		return 0;
}

int64 SqlField::getInt64()
{
	if (m_value)
	{
		int64 value;
		sscanf(m_value, I64FMTD, &value);
		return value;
	}
	else
		return 0;
}
