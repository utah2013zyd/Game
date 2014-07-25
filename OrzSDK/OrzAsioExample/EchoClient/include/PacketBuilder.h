#ifndef __Orz_FpfSvr_PacketBuilder_h__
#define __Orz_FpfSvr_PacketBuilder_h__
#include <orz/OrzAsio/NetEngine/IPacketParser.h>
#include <orz/OrzAsio/NetEngine/IPacket.h>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <Packet.h>
namespace Orz
{
static MemString<32> flash_policy_str("<policy-file-request/>");
//static const size_t fps_size = flash_policy_str.size();
static const size_t fps_size = 512;
//static MemString<512> flash_policy_back = "<cross-domain-policy><site-control permitted-cross-domain-policies=\"all\"/><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";
class PacketBuilder
	: public IPacketParser
{
public:
	PacketBuilder() : m_size(0) { /*flash_policy_back.push_back('\0');*/ }
	virtual ~PacketBuilder() {}

public:
	int32					parse						(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets)
	{
		if (!_data ||/* _size > PACKET_MAX_SIZE ||*/ _size == 0)
			return 1;

		//std::string str(_data, _size);
		//std::cout << "packet_builder::recv - str: " << str << std::endl;

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

					// 判断收到的消息大小是否小于等于flash_policy_str
					//std::string data;
					uint32 getsize = 0;
					if (lsts < fps_size)
						getsize = lsts;
					else
						getsize = fps_size;

					m_data.append(_data + size, getsize);
					if (m_data.size() < fps_size)
					{
						//m_data = data;
						size += m_data.size();
						continue;
					}

					IPacketShrPtr pk = allocPacket();
					//IPacketShrPtr pk(new NetPk);
					pk->put(m_data.c_str(), getsize);
				
					_packets.push_back(pk);
					
					m_data.clear();
					m_size = 0;

					return 0;
				}
				else
				{
					uint32 getsize = 0;
					uint32 os = (uint32)m_data.size();
					uint32 rs = 0;	// 读取的字节数
					if (m_data.size() < fps_size)
					{
						if (lsts < fps_size - m_data.size())
							getsize = lsts;
						else
							getsize = fps_size - (uint32)m_data.size();

						m_data.append(_data+size, getsize);
						rs += getsize;
						if (m_data.size() < fps_size)		// 封包大小数据仍然不完整
						{
							size += m_data.size() - os;
							m_size = 0;
							continue;
						}
					}

					IPacketShrPtr pk = allocPacket();
					//IPacketShrPtr pk(new NetPk);
					pk->put(m_data.c_str(), fps_size);
				
					_packets.push_back(pk);
					
					m_data.clear();
					m_size = 0;
					return 0;
				}
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

	void					clear						()
	{
		m_data.clear();
		m_size = 0;
	}

	IPacketParserShrPtr		create						()
	{
		return IPacketParserShrPtr(new PacketBuilder);
	}

private:
	//std::string						m_data;
	MemString<513>					m_data;
	uint32							m_size;
};
}
#endif
