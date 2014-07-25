#include <orz/OrzAsio/NetEngine/PacketParser.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/Packet.hpp>
using namespace Orz;


PacketParser::PacketParser()
	: m_size(0) 
{
}

PacketParser::~PacketParser()
{
}

int32 PacketParser::parse(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets)
{
	if (!_data || _size == 0)
		return 1;

	try
	{
		size_t size = 0;
		uint32 rwPkSize = _size;
		while (size < _size)
		{
			uint32 lsts = rwPkSize - size;		// 每次循环是剩下的字节数
			// 如果封包完整则放入_packets中；如果不完整，保存起来等待剩下的部分以便合起来
			if (m_data.empty())
			{
				m_size = 0;

				// 判断封包头中的封包大小信息是否完整
				if (!readHeadArg(_data, lsts, PACKET_CAPACITY_SIZE, 0, PACKET_CAPACITY_SIZE, size))
					continue;

				memcpy(&m_size, m_data.c_str(), PACKET_CAPACITY_SIZE);

#ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
				swap32(&m_size);
#endif
				// 判断封包头中的封包类型信息是否完整
				if (!readHeadArg(_data, lsts, PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, PACKET_CAPACITY_SIZE, PACKET_TYPE_SIZE, size))
					continue;

				// 判断封包头中的时间信息是否完整
				if (!readHeadArg(_data, lsts, PACKET_HEAD_SIZE, PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, PACKET_DATETIME_SIZE, size))
					continue;

				// 继续读完这个封包中剩下的数据
				if (!readHeadArg(_data, lsts, m_size + PACKET_HEAD_SIZE, PACKET_HEAD_SIZE, m_size, size))
					continue;

				size += m_data.size();
			}
			else
			{
				uint32 getsize = 0;
				uint32 os = (uint32)m_data.size();
				uint32 rs = 0;	// 读取的字节数
				if (!readHeadArgEx(_data, lsts, rs, os, PACKET_CAPACITY_SIZE, size))
				{
					m_size = 0;
					continue;
				}
				memcpy(&m_size, m_data.c_str(), PACKET_CAPACITY_SIZE);
#ifdef ORZ_PLUS_CONFIG_BIG_ENDIAN
				swap32(&m_size);
#endif
				if (!readHeadArgEx(_data, lsts, rs, os, PACKET_CAPACITY_SIZE + PACKET_TYPE_SIZE, size))
					continue;

				if (!readHeadArgEx(_data, lsts, rs, os, PACKET_HEAD_SIZE, size))
					continue;

				if (!readHeadArgEx(_data, lsts, rs, os, PACKET_HEAD_SIZE + m_size, size))
					continue;

				size += m_data.size() - os;
			}

			IPacketShrPtr pk = allocPacket();
			pk->overWrite(PACKET_CAPACITY_SIZE, m_data.c_str()+PACKET_CAPACITY_SIZE, PACKET_TYPE_SIZE);
			pk->overWrite(PACKET_CAPACITY_SIZE+PACKET_TYPE_SIZE, m_data.c_str()+PACKET_CAPACITY_SIZE+PACKET_TYPE_SIZE, PACKET_DATETIME_SIZE);
			pk->put(m_data.c_str()+PACKET_HEAD_SIZE, m_size);

			_packets.push_back(pk);
			
			m_data.clear();
			m_size = 0;
		}

	}
	catch(...)
	{
		m_data.clear();
		m_size = 0;
		printf("Packet Catched !!\n");
		return 1;
	}

	return 0;
}

void PacketParser::clear()
{
	m_data.clear();
	m_size = 0;
}

IPacketParserShrPtr PacketParser::create()
{
	return IPacketParserShrPtr(new PacketParser);
}

bool PacketParser::readHeadArg(const char* _data, uint32 _lsts, uint32 _targetSize, 
							   uint32 _getSize1, uint32 _getSize2, uint32& _currSize)
{
	uint32 getSize = 0;
	if (_lsts < _targetSize)
		getSize = _lsts - _getSize1;
	else
		getSize = _getSize2;

	m_data.append(_data + _currSize + _getSize1, getSize);
	if (m_data.size() < _targetSize)
	{
		_currSize += m_data.size();
		return false;
	}
	return true;
}

bool PacketParser::readHeadArgEx(const char* _data, uint32 _lsts, uint32& _rs, uint32 _os, 
								 uint32 _targetSize, uint32& _currSize)
{
	if ((uint32)m_data.size() < _targetSize)
	{
		uint32 getSize = 0;
		if (_lsts - _rs < _targetSize - m_data.size())
			getSize = _lsts - _rs;
		else
			getSize = _targetSize - (uint32)m_data.size();

		m_data.append(_data + _currSize + _rs, getSize);
		_rs += getSize;
		if ((uint32)m_data.size() < _targetSize)
		{
			_currSize += m_data.size() - _os;
			return false;
		}
	}
	return true;
}
