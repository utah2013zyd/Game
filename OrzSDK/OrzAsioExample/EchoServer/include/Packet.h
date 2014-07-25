#ifndef __Orz_FpfSvr_Packet_h__
#define __Orz_FpfSvr_Packet_h__
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <Common.h>
#include <NetMsg.h>
namespace Orz
{
typedef MemString<PKT_TYPE_SIZE+1> MsgStr;

template <uint32 SIZE>
class Packet
	: public IPacket
{
public:
	Packet()
		: m_size(0)
		, m_seeker(0)
	{
	}
	virtual ~Packet()
	{
	}

	void freeDyn() {}

public:
	int32					writeByte				(int8 _data) { return 0; }
	int32					writeUbyte				(uint8 _data) { return 0; }
	int32					writeShort				(int16 _data) { return 0; }
	int32					writeUshort				(uint16 _data) { return 0; }
	int32					writeInt				(int32 _data) { return 0; }
	int32					writeUint				(uint32 _data) { return 0; }
	int32					writeLong				(int64 _data) { return 0; }
	int32					writeUlong				(uint64 _data) { return 0; }
	int32					writeFloat				(float32 _data) { return 0; }
	int32					writeDouble				(float64 _data) { return 0; }
	int32					writeStr				(const StringPtr& _str) { return 0; }
	int32					writeBytes				(const std::vector<int8>& _data) { return 0; }
	int32					writeUbytes				(const std::vector<uint8>& _data) { return 0; }
	int32					writeShorts				(const std::vector<int16>& _data) { return 0; }
	int32					writeUshorts			(const std::vector<uint16>& _data) { return 0; }
	int32					writeInts				(const std::vector<int32>& _data) { return 0; }
	int32					writeUints				(const std::vector<uint32>& _data) { return 0; }
	int32					writeLongs				(const std::vector<int64>& _data) { return 0; }
	int32					writeUlongs				(const std::vector<uint64>& _data) { return 0; }
	int32					writeFloats				(const std::vector<float32>& _data) { return 0; }
	int32					writeDoubles			(const std::vector<float64>& _data) { return 0; }
	int32					writeStrs				(const std::vector<StringPtr>& _data) { return 0; }
	int32					writeBool				(bool _data) { return 0; }
	int32					put						(const void* _data, const uint32 _size) 
	{ 
		m_data.append((const char*)_data, _size);
		char* ptr = 0;
		ptr = m_data.raw();
		if (!ptr)
			return 1;

		m_size = m_data.size();
		return 0; 
	}
	int32					overWrite				(uint32 _offset, const void* _data, const uint32 _size) 
	{ 
		m_data.clear();
		m_data.append((const char*)_data, _size);
		m_data[_size+1] = '\0';
		m_size = _size;
		return 0;
	}

	int8					readByte				(int32& _err) { return 0; }
	uint8					readUbyte				(int32& _err) { return 0; }
	int16					readShort				(int32& _err) { return 0; }
	uint16					readUshort				(int32& _err) { return 0; }
	int32					readInt					(int32& _err) { return 0; }
	uint32					readUint				(int32& _err) { return 0; }
	int64					readLong				(int32& _err) { return 0; }
	uint64					readUlong				(int32& _err) { return 0; }
	float32					readFloat				(int32& _err) { return 0.0f; }
	float64					readDouble				(int32& _err) { return 0.0f; }
	StringPtr				readStr					(int32& _err) { return StringPtr(); }
	std::vector<int8>		readBytes				(int32& _err) { return std::vector<int8>(); }
	std::vector<uint8>		readUbytes				(int32& _err) { return std::vector<uint8>(); }
	std::vector<int16>		readShorts				(int32& _err) { return std::vector<int16>(); }
	std::vector<uint16>		readUshorts				(int32& _err) { return std::vector<uint16>(); }
	std::vector<int32>		readInts				(int32& _err) { return std::vector<int32>(); }
	std::vector<uint32>		readUints				(int32& _err) { return std::vector<uint32>(); }
	std::vector<int64>		readLongs				(int32& _err) { return std::vector<int64>(); }
	std::vector<uint64>		readUlongs				(int32& _err) { return std::vector<uint64>(); }
	std::vector<float32>	readFloats				(int32& _err) { return std::vector<float32>(); }
	std::vector<float64>	readDoubles				(int32& _err) { return std::vector<float64>(); }
	std::vector<StringPtr>	readStrs				(int32& _err) { return std::vector<StringPtr>(); }
	int32					readBool				(bool& _data) { return 0; }
	int32					get						(void* _data, const uint32 _size) { return 0; }
	int32					get						(StringPtr& _rs, const uint32 _size)
	{
		char* ptr = m_data.raw();
		if (!ptr)
			return 1;

		uint32 getsize = 0;
		uint32 lastsize = m_size - m_seeker;
		if (_size > lastsize)
			getsize = lastsize;
		else
			getsize = _size;

		StringPtr ms(ptr + m_seeker, getsize);
		m_seeker += getsize;

		if (m_seeker > m_size)
			m_seeker = m_size;

		_rs = ms;
		return 0;
	}
	int32					reRead					(uint32 _offset, void* _data, const uint32 _size) { return 0; }

	const char*				raw						() const 
	{
		return m_data.c_str();
	}
	void					reset					() 
	{ 
		m_seeker = 0;
	}
	void					setSize					(uint32 _size) { return; }
	uint32					getSize					() const 
	{
		return m_size;
	}
	int32					setMsg					(const int32 _msg) 
	{ 
		return 0; 
	}
	int32					getMsg					() const 
	{
		return 0;
	}
	int32					setDateTime				(const DateSys& _dTm) { return 0; }
	int32					getDT					(DateSys& _dTm) const { return 0; }

	bool					empty					() const 
	{
		if (m_data.size() == 0)
			return true;
		else
			return false;
	}
	void					clear					() 
	{ 
		m_data.clear();
		m_size = 0;
		m_seeker = 0;
	}
	uint32					size					() const 
	{
		return m_size;
	}
	bool					isDyn					() const 
	{
		return false;
	}

private:
	PacketData				m_data;
	uint32					m_size;
	uint32					m_seeker;		// 用于标识当前get的指针位置
};
typedef Packet<PKT_MAX_SIZE> NetPk;
}
#endif
