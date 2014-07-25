#ifndef __Orz_OrzAsio_IPacketParser_h__
#define __Orz_OrzAsio_IPacketParser_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! �����������ӿ���
/*!
 *	\note �û�����ͨ���̳���ʵ���Լ���������������
 *	\note OrzAsio�ṩ��һ�׹�����������Ĭ��ʵ�֣���Packet��PacketParser��
 */
class IPacketParser
{
public:
	IPacketParser() {}
	virtual ~IPacketParser() {}

public:
	//! �������
	/*!
	 *	\param[in] _data Ҫ�����ķ���ڴ��ͷָ��
	 *	\param[in] _size Ҫ�����ķ���ڴ�γ���
	 *	\param[out] _packets ������Ľ��
	 */
	virtual int32					parse						(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets) = 0;

	//! ����Լ�
	virtual void					clear						() = 0;

	//! ����һ���µ�IPacketParser����
	/*!
	 *	\note �û�ʵ�ִ˷�����ʱ����Ҫע��������·���һ���µ��ڴ棬���ܷ��������ָ�롢Ҳ��Ҫ�������������µĶ���
	 */
	virtual IPacketParserShrPtr		create						() = 0;
};
}
#endif
