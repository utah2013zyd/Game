#ifndef __Orz_OrzAsio_PacketParser_h_
#define __Orz_OrzAsio_PacketParser_h_
#include <orz/OrzAsio/NetEngine/IPacketParser.h>
namespace Orz 
{
//! OrzAsio提供的网络封包解析类实现
/*!
 *	\note 接口说明见IPacketParser
 */
class PacketParser
	: public IPacketParser
{
public:
	PacketParser();
	virtual ~PacketParser();

public:
	int32							parse						(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets);
	void							clear						();
	IPacketParserShrPtr				create						();

private:
	bool							readHeadArg					(const char* _data, uint32 _lsts, uint32 _targetSize, 
																 uint32 _getSize1, uint32 _getSize2, uint32& _currSize);
	bool							readHeadArgEx				(const char* _data, uint32 _lsts, uint32& _rs, uint32 _os, 
																 uint32 _targetSize, uint32& _currSize);
private:
	std::string						m_data;
	uint32							m_size;
};
}
#endif
