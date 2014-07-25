#ifndef __Orz_OrzAsio_IPacket_h__
#define __Orz_OrzAsio_IPacket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
//! 网络封包接口类
/*!
 *	\note 用户可以通过继承来实现自己的网络封包类，同时也要实现自己的IPacketParser类
 *	\note OrzAsio提供了一套关于网络封包的默认实现，见Packet，PacketParser类
 */
class IPacket
{
public:
	IPacket() {}
	virtual ~IPacket() {}

public:
	//! 向封包中写入一个8位整型
	virtual int32					writeByte				(int8 _data) = 0;

	//! 向封包中写入一个8位无符号整型
	virtual int32					writeUbyte				(uint8 _data) = 0;

	//! 向封包中写入一个16位整型
	virtual int32					writeShort				(int16 _data) = 0;

	//! 向封包中写入一个16位无符号整型
	virtual int32					writeUshort				(uint16 _data) = 0;

	//! 向封包中写入一个32位整型
	virtual int32					writeInt				(int32 _data) = 0;

	//! 向封包中写入一个32位无符号整型
	virtual int32					writeUint				(uint32 _data) = 0;

	//! 向封包中写入一个64位整型
	virtual int32					writeLong				(int64 _data) = 0;

	//! 向封包中写入一个64位无符号整型
	virtual int32					writeUlong				(uint64 _data) = 0;

	//! 向封包中写入一个32位浮点
	virtual int32					writeFloat				(float32 _data) = 0;

	//! 向封包中写入一个64位浮点
	virtual int32					writeDouble				(float64 _data) = 0;

	//! 向封包中写入一个字符串，对于StringPtr，见orz\Toolkit_Plus\Toolkit\StringPtr.hpp
	/*!
	 *	\note 封包不会管这个字符串的编码格式，它当作一段内存段来处理（字节为单位）
	 */
	virtual int32					writeStr				(const StringPtr& _str) = 0;

	//! 向封包中写入一组8位整型
	virtual int32					writeBytes				(const std::vector<int8>& _data) = 0;

	//! 向封包中写入一组8位无符号整型
	virtual int32					writeUbytes				(const std::vector<uint8>& _data) = 0;

	//! 向封包中写入一组16位整型
	virtual int32					writeShorts				(const std::vector<int16>& _data) = 0;

	//! 向封包中写入一组16位无符号整型
	virtual int32					writeUshorts			(const std::vector<uint16>& _data) = 0;

	//! 向封包中写入一组32位整型
	virtual int32					writeInts				(const std::vector<int32>& _data) = 0;

	//! 向封包中写入一组32位无符号整型
	virtual int32					writeUints				(const std::vector<uint32>& _data) = 0;

	//! 向封包中写入一组64位整型
	virtual int32					writeLongs				(const std::vector<int64>& _data) = 0;

	//! 向封包中写入一组64位无符号整型
	virtual int32					writeUlongs				(const std::vector<uint64>& _data) = 0;

	//! 向封包中写入一组32位浮点
	virtual int32					writeFloats				(const std::vector<float32>& _data) = 0;

	//! 向封包中写入一组64位浮点
	virtual int32					writeDoubles			(const std::vector<float64>& _data) = 0;

	//! 向封包中写入一组字符串
	virtual int32					writeStrs				(const std::vector<StringPtr>& _data) = 0;

	//! 向封包中写入一个bool
	virtual int32					writeBool				(bool _data) = 0;

	//! 向封包中写入一段内存值
	/*!
	 *	\param[in] _data 要写入的内存段的头指针
	 *	\param[in] _size 要写入的内存段的长度（字节为单位，也就是1byte）
	 */
	virtual int32					put						(const void* _data, const uint32 _size) = 0;

	//! 向封包中指定偏移量的位置写入一段内存值
	/*!
	 *	\param[in] _offset 从0开始的偏移量（字节为单位）
	 *	\param[in] _data 要写入的内存段的头指针
	 *	\param[in] _size 要写入的内存段的长度（字节为单位，也就是1byte）
	 *	\note 这个方法和put的主要区别就是put是按照一个类似读file时候的偏移量来按照顺序写入；\n
	 *	而overWrite则是手动指定偏移量，也就是说overWrite是可以覆盖的。通过继承实现自己的封包类时，请注意这两者的区别
	 */
	virtual int32					overWrite				(uint32 _offset, const void* _data, const uint32 _size) = 0;

	//! 从封包中读取一个8位整型
	virtual int8					readByte				(int32& _err) = 0;

	//! 从封包中读取一个8位无符号整型
	virtual uint8					readUbyte				(int32& _err) = 0;

	//! 从封包中读取一个16位整型
	virtual int16					readShort				(int32& _err) = 0;

	//! 从封包中读取一个16位无符号整型
	virtual uint16					readUshort				(int32& _err) = 0;

	//! 从封包中读取一个32位整型
	virtual int32					readInt					(int32& _err) = 0;

	//! 从封包中读取一个32位无符号整型
	virtual uint32					readUint				(int32& _err) = 0;

	//! 从封包中读取一个64位整型
	virtual int64					readLong				(int32& _err) = 0;

	//! 从封包中读取一个64位无符号整型
	virtual uint64					readUlong				(int32& _err) = 0;

	//! 从封包中读取一个32位浮点
	virtual float32					readFloat				(int32& _err) = 0;

	//! 从封包中读取一个64位浮点
	virtual float64					readDouble				(int32& _err) = 0;

	//! 从封包中读取一个字符串
	virtual StringPtr				readStr					(int32& _err) = 0;

	//! 从封包中读取一组8位整型
	virtual std::vector<int8>		readBytes				(int32& _err) = 0;

	//! 从封包中读取一组8位无符号整型
	virtual std::vector<uint8>		readUbytes				(int32& _err) = 0;

	//! 从封包中读取一组16位整型
	virtual std::vector<int16>		readShorts				(int32& _err) = 0;

	//! 从封包中读取一组16位无符号整型
	virtual std::vector<uint16>		readUshorts				(int32& _err) = 0;

	//! 从封包中读取一组32位整型
	virtual std::vector<int32>		readInts				(int32& _err) = 0;

	//! 从封包中读取一组32位无符号整型
	virtual std::vector<uint32>		readUints				(int32& _err) = 0;

	//! 从封包中读取一组64位整型
	virtual std::vector<int64>		readLongs				(int32& _err) = 0;

	//! 从封包中读取一组64位无符号整型
	virtual std::vector<uint64>		readUlongs				(int32& _err) = 0;

	//! 从封包中读取一组32位浮点
	virtual std::vector<float32>	readFloats				(int32& _err) = 0;

	//! 从封包中读取一组64位浮点
	virtual std::vector<float64>	readDoubles				(int32& _err) = 0;

	//! 从封包中读取一组字符串
	virtual std::vector<StringPtr>	readStrs				(int32& _err) = 0;

	//! 从封包中读取一个bool
	virtual int32					readBool				(bool& _data) = 0;

	//! 从封包中读取一段指定长度的内存值
	/*!
	 *	\param[out] _data 保存读取的内存段头指针
	 *	\param[in] _size 要读取的内存段的长度（字节为单位）
	 */
	virtual int32					get						(void* _data, const uint32 _size) = 0;

	//! 从封包中读取一段指定长度的内存值
	/*!
	 *	\param[out] _rs 保存读取的内存段（头指针和长度）
	 *	\param[in] _size 要读取的内存段的长度（字节为单位）
	 */
	virtual int32					get						(StringPtr& _rs, const uint32 _size) = 0;

	//! 从封包中读取指定偏移量的位置的一段内存值
	/*!
	 *	\param[in] _offset 从0开始的偏移量（字节为单位）
	 *	\param[out] _data 保存读取的内存段头指针
	 *	\param[in] _size 要读取的内存段的长度（字节为单位）
	 */
	virtual int32					reRead					(uint32 _offset, void* _data, const uint32 _size) = 0;

	//! 返回这个封包的内存段的头指针
	virtual const char*				raw						() const = 0;

	//! 重置这个封包的内部的偏移量为0
	virtual void					reset					() = 0;

	//! 设置这个封包的不包含头部（见PacketEnv.h中PACKET_HEAD_SIZE）的长度
	virtual void					setSize					(uint32 _size) = 0;

	//! 取得这个封包的不包含头部的长度
	virtual uint32					getSize					() const = 0;

	//!	设置这个封包的消息类型
	virtual int32					setMsg					(const int32 _msg) = 0;

	//!	取得这个封包的消息类型
	virtual int32					getMsg					() const = 0;

	//! 设置这个封包的日期时间（见orz\Toolkit_Plus\Toolkit\Arithmetic.hpp中DateSys结构）
	virtual int32					setDateTime				(const DateSys& _dTm) = 0;

	//! 取得这个封包的日期时间（见orz\Toolkit_Plus\Toolkit\Arithmetic.hpp中DateSys结构）
	virtual int32					getDT					(DateSys& _dTm) const = 0;

	//! 检查这个封包是否为空
	virtual bool					empty					() const = 0;

	//! 清楚这个封包的内容，重置偏移量为0
	virtual void					clear					() = 0;

	//! 取得这个封包的包含头部在内的总长度
	virtual uint32					size					() const = 0;

	//! 是否是动态内存的封包（默认情况下，封包是定长的长度，只有超过最大长度后，系统才会将这个封包临时变更为动态的，好容纳更多的内容）
	virtual bool					isDyn					() const = 0;
};
}
#endif
