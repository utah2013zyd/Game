#ifndef __Orz_ActOGServer_packet_builder_h__
#define __Orz_ActOGServer_packet_builder_h__
#include <orz/View_asio/NetEngine/Packet/IPacketParser.h>
#include <orz/Tookit_Plus/Tookit/FreeList.hpp>
#include <orz/View_asio/NetEngine/Packet/PacketFreeList.h>
#include <packet/packet.h>
#include <orz/Tookit_Plus/Log/LogWrap.h>
#include <orz/Tookit_Plus/Log/OrzAsioLogger.h>
#include <orz/Tookit_Plus/Tookit/OrzAsioException.hpp>
namespace Orz
{
class packet_builder
	: public IPacketParser
{
public:
	packet_builder() : m_size(0) {}
	virtual ~packet_builder() {}

public:
	int32					parse						(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets)
	{
		if (!_data ||/* _size > PACKET_MAX_SIZE ||*/ _size == 0)
			return 1;

		try
		{
			size_t size = 0;
			uint32 rwPkSize = _size;
			//printf("\n######################################\nrecv new data! \n_size: %d\n", _size);
			while (size < _size)
			{
				//printf("size: %d\n", size);
				uint32 lsts = rwPkSize - size;		// 每次循环是剩下的字节数
				// 如果封包完整则放入_packets中；如果不完整，保存起来等待剩下的部分以便合起来
				if (m_data.empty())
				{
					//printf("\n$$$$$$$$$$$$$$$$$$$$$$\n\n");
					m_size = 0;

					// 判断封包头中的封包大小信息是否完整
					//std::string data;
					uint32 getsize = 0;
					if (lsts < PKT_CAPACITY_SIZE)
						getsize = lsts;
					else
						getsize = PKT_CAPACITY_SIZE;

					m_data.append(_data + size, getsize);
					if (m_data.size() < PKT_CAPACITY_SIZE)
					{
						//m_data = data;
						size += m_data.size();
						continue;
					}

					m_size = s2d(m_data.c_str());
					//if (m_size == 0)
					//{
					//	printf("packet empty!drow it.########## 2 - data: %s\n", m_data.c_str());
					//	m_size = 0;
					//	size += m_data.size();
					//	m_data.clear();
					//	continue;
					//}

					//printf("packet body size: %d\n", m_size);

					// 判断封包头中的封包类型信息是否完整
					if (lsts < PKT_HEAD_SIZE)
						getsize = lsts - PKT_CAPACITY_SIZE;
					else
						getsize = PKT_TYPE_SIZE;

					m_data.append(_data + size + PKT_CAPACITY_SIZE, getsize);
					if (m_data.size() < PKT_HEAD_SIZE)
					{
						//m_data = data;
						size += m_data.size();
						continue;
					}

					// 继续读完这个封包中剩下的数据
					if (lsts < m_size + PKT_HEAD_SIZE)
						getsize = lsts - PKT_HEAD_SIZE;
					else
						getsize = m_size;

					m_data.append(_data + size + PKT_HEAD_SIZE, getsize);
					if (m_data.size() < m_size + PKT_HEAD_SIZE)
					{
						//m_data = data;
						size += m_data.size();
						continue;
					}

					IPacketShrPtr pk = allocPacket();
					// get and set msg
					//MsgStr mt(_data+PKT_CAPACITY_SIZE, PKT_TYPE_SIZE);
					//pk->setMsg(get_msg(mt));
					pk->put(_data+PKT_HEAD_SIZE, m_size);
				
					_packets.push_back(pk);
					
					size += m_data.size();
					m_data.clear();
					m_size = 0;
				}
				else
				{
					//printf("\n@@@@@@@@@@@@@@@@@@@@@@@\n\n");
					uint32 getsize = 0;
					uint32 os = (uint32)m_data.size();
					uint32 rs = 0;	// 读取的字节数
					if (m_data.size() < PKT_CAPACITY_SIZE)
					{
						if (lsts < PKT_CAPACITY_SIZE - m_data.size())
							getsize = lsts;
						else
							getsize = PKT_CAPACITY_SIZE - (uint32)m_data.size();

						m_data.append(_data+size, getsize);
						rs += getsize;
						if (m_data.size() < PKT_CAPACITY_SIZE)		// 封包大小数据仍然不完整
						{
							size += m_data.size() - os;
							m_size = 0;
							continue;
						}
						m_size = s2d(m_data.c_str());
						//if (m_size == 0)
						//{
						//	printf("packet empty!drow it.\n");
						//	m_data.clear();
						//	m_size = 0;
						//	size += m_data.size() - os;
						//	continue;
						//}
					}

					if (m_data.size() < PKT_HEAD_SIZE)
					{
						if (lsts - rs < PKT_HEAD_SIZE - m_data.size())
							getsize = lsts - rs;
						else
							getsize = PKT_HEAD_SIZE - (uint32)m_data.size();

						m_data.append(_data+size+rs, getsize);
						rs += getsize;
						if (m_data.size() < PKT_HEAD_SIZE)			// 封包类型数据不完整
						{
							size += m_data.size() - os;
							continue;
						}
					}

					if (m_data.size() < PKT_HEAD_SIZE + m_size)
					{
						if (lsts - rs < PKT_HEAD_SIZE + m_size - m_data.size())
							getsize = lsts - rs;
						else
							getsize = PKT_HEAD_SIZE + m_size - m_data.size();

						m_data.append(_data+size+rs, getsize);
						rs += getsize;
						if (m_data.size() < PKT_HEAD_SIZE + m_size)
						{
							size += m_data.size() - os;
							continue;
						}
					}

					// set msg
					IPacketShrPtr pk = allocPacket();
					// get and set msg
					//MsgStr mt(m_data.c_str()+PKT_CAPACITY_SIZE, PKT_TYPE_SIZE);
					//pk->setMsg(get_msg(mt));
					pk->put(m_data.c_str()+PKT_HEAD_SIZE, m_size);
				
					_packets.push_back(pk);
					
					size += m_data.size() - os;
					m_data.clear();
					m_size = 0;
				}
			}

		}
		catch (OrzAsioException& _e)
		{
			std::vector<const char*> errs = _e.what();
			for (std::vector<const char*>::iterator itr=errs.begin(); itr!=errs.end(); ++itr)
			{
				sLogger->out(sLogWrap.getCurrentAppender(), OL_DEBUG, *itr, 0);
				printf("%s: \n", *itr);
			}
			m_data.clear();
			m_size = 0;
			return 1;
		}
		catch(...)
		{
			m_data.clear();
			m_size = 0;
			printf("packet Catched !!\n");
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
		return IPacketParserShrPtr(new packet_builder);
	}

private:
	std::string						m_data;
	uint32							m_size;
};
}
#endif
