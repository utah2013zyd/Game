#ifndef __Orz_OrzAsio_IPacketParser_h__
#define __Orz_OrzAsio_IPacketParser_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! 网络封包解析接口类
/*!
 *	\note 用户可以通过继承来实现自己的网络封包解析类
 *	\note OrzAsio提供了一套关于网络封包的默认实现，见Packet，PacketParser类
 */
class IPacketParser
{
public:
	IPacketParser() {}
	virtual ~IPacketParser() {}

public:
	//! 解析封包
	/*!
	 *	\param[in] _data 要解析的封包内存段头指针
	 *	\param[in] _size 要解析的封包内存段长度
	 *	\param[out] _packets 解析后的结果
	 */
	virtual int32					parse						(const char* _data, uint32 _size, std::vector<IPacketShrPtr>& _packets) = 0;

	//! 清除自己
	virtual void					clear						() = 0;

	//! 创建一个新的IPacketParser对象
	/*!
	 *	\note 用户实现此方法的时候需要注意必须重新分配一块新的内存，不能返回自身的指针、也不要复制自身给这个新的对象
	 */
	virtual IPacketParserShrPtr		create						() = 0;
};
}
#endif
