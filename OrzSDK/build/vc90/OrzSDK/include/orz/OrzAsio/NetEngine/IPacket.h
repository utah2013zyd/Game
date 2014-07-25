#ifndef __Orz_OrzAsio_IPacket_h__
#define __Orz_OrzAsio_IPacket_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/Toolkit_Plus/Toolkit/StringPtr.hpp>
namespace Orz
{
//! �������ӿ���
/*!
 *	\note �û�����ͨ���̳���ʵ���Լ����������࣬ͬʱҲҪʵ���Լ���IPacketParser��
 *	\note OrzAsio�ṩ��һ�׹�����������Ĭ��ʵ�֣���Packet��PacketParser��
 */
class IPacket
{
public:
	IPacket() {}
	virtual ~IPacket() {}

public:
	//! ������д��һ��8λ����
	virtual int32					writeByte				(int8 _data) = 0;

	//! ������д��һ��8λ�޷�������
	virtual int32					writeUbyte				(uint8 _data) = 0;

	//! ������д��һ��16λ����
	virtual int32					writeShort				(int16 _data) = 0;

	//! ������д��һ��16λ�޷�������
	virtual int32					writeUshort				(uint16 _data) = 0;

	//! ������д��һ��32λ����
	virtual int32					writeInt				(int32 _data) = 0;

	//! ������д��һ��32λ�޷�������
	virtual int32					writeUint				(uint32 _data) = 0;

	//! ������д��һ��64λ����
	virtual int32					writeLong				(int64 _data) = 0;

	//! ������д��һ��64λ�޷�������
	virtual int32					writeUlong				(uint64 _data) = 0;

	//! ������д��һ��32λ����
	virtual int32					writeFloat				(float32 _data) = 0;

	//! ������д��һ��64λ����
	virtual int32					writeDouble				(float64 _data) = 0;

	//! ������д��һ���ַ���������StringPtr����orz\Toolkit_Plus\Toolkit\StringPtr.hpp
	/*!
	 *	\note ������������ַ����ı����ʽ��������һ���ڴ���������ֽ�Ϊ��λ��
	 */
	virtual int32					writeStr				(const StringPtr& _str) = 0;

	//! ������д��һ��8λ����
	virtual int32					writeBytes				(const std::vector<int8>& _data) = 0;

	//! ������д��һ��8λ�޷�������
	virtual int32					writeUbytes				(const std::vector<uint8>& _data) = 0;

	//! ������д��һ��16λ����
	virtual int32					writeShorts				(const std::vector<int16>& _data) = 0;

	//! ������д��һ��16λ�޷�������
	virtual int32					writeUshorts			(const std::vector<uint16>& _data) = 0;

	//! ������д��һ��32λ����
	virtual int32					writeInts				(const std::vector<int32>& _data) = 0;

	//! ������д��һ��32λ�޷�������
	virtual int32					writeUints				(const std::vector<uint32>& _data) = 0;

	//! ������д��һ��64λ����
	virtual int32					writeLongs				(const std::vector<int64>& _data) = 0;

	//! ������д��һ��64λ�޷�������
	virtual int32					writeUlongs				(const std::vector<uint64>& _data) = 0;

	//! ������д��һ��32λ����
	virtual int32					writeFloats				(const std::vector<float32>& _data) = 0;

	//! ������д��һ��64λ����
	virtual int32					writeDoubles			(const std::vector<float64>& _data) = 0;

	//! ������д��һ���ַ���
	virtual int32					writeStrs				(const std::vector<StringPtr>& _data) = 0;

	//! ������д��һ��bool
	virtual int32					writeBool				(bool _data) = 0;

	//! ������д��һ���ڴ�ֵ
	/*!
	 *	\param[in] _data Ҫд����ڴ�ε�ͷָ��
	 *	\param[in] _size Ҫд����ڴ�εĳ��ȣ��ֽ�Ϊ��λ��Ҳ����1byte��
	 */
	virtual int32					put						(const void* _data, const uint32 _size) = 0;

	//! ������ָ��ƫ������λ��д��һ���ڴ�ֵ
	/*!
	 *	\param[in] _offset ��0��ʼ��ƫ�������ֽ�Ϊ��λ��
	 *	\param[in] _data Ҫд����ڴ�ε�ͷָ��
	 *	\param[in] _size Ҫд����ڴ�εĳ��ȣ��ֽ�Ϊ��λ��Ҳ����1byte��
	 *	\note ���������put����Ҫ�������put�ǰ���һ�����ƶ�fileʱ���ƫ����������˳��д�룻\n
	 *	��overWrite�����ֶ�ָ��ƫ������Ҳ����˵overWrite�ǿ��Ը��ǵġ�ͨ���̳�ʵ���Լ��ķ����ʱ����ע�������ߵ�����
	 */
	virtual int32					overWrite				(uint32 _offset, const void* _data, const uint32 _size) = 0;

	//! �ӷ���ж�ȡһ��8λ����
	virtual int8					readByte				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��8λ�޷�������
	virtual uint8					readUbyte				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��16λ����
	virtual int16					readShort				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��16λ�޷�������
	virtual uint16					readUshort				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ����
	virtual int32					readInt					(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ�޷�������
	virtual uint32					readUint				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ����
	virtual int64					readLong				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ�޷�������
	virtual uint64					readUlong				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ����
	virtual float32					readFloat				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ����
	virtual float64					readDouble				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ���ַ���
	virtual StringPtr				readStr					(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��8λ����
	virtual std::vector<int8>		readBytes				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��8λ�޷�������
	virtual std::vector<uint8>		readUbytes				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��16λ����
	virtual std::vector<int16>		readShorts				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��16λ�޷�������
	virtual std::vector<uint16>		readUshorts				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ����
	virtual std::vector<int32>		readInts				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ�޷�������
	virtual std::vector<uint32>		readUints				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ����
	virtual std::vector<int64>		readLongs				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ�޷�������
	virtual std::vector<uint64>		readUlongs				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��32λ����
	virtual std::vector<float32>	readFloats				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��64λ����
	virtual std::vector<float64>	readDoubles				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ���ַ���
	virtual std::vector<StringPtr>	readStrs				(int32& _err) = 0;

	//! �ӷ���ж�ȡһ��bool
	virtual int32					readBool				(bool& _data) = 0;

	//! �ӷ���ж�ȡһ��ָ�����ȵ��ڴ�ֵ
	/*!
	 *	\param[out] _data �����ȡ���ڴ��ͷָ��
	 *	\param[in] _size Ҫ��ȡ���ڴ�εĳ��ȣ��ֽ�Ϊ��λ��
	 */
	virtual int32					get						(void* _data, const uint32 _size) = 0;

	//! �ӷ���ж�ȡһ��ָ�����ȵ��ڴ�ֵ
	/*!
	 *	\param[out] _rs �����ȡ���ڴ�Σ�ͷָ��ͳ��ȣ�
	 *	\param[in] _size Ҫ��ȡ���ڴ�εĳ��ȣ��ֽ�Ϊ��λ��
	 */
	virtual int32					get						(StringPtr& _rs, const uint32 _size) = 0;

	//! �ӷ���ж�ȡָ��ƫ������λ�õ�һ���ڴ�ֵ
	/*!
	 *	\param[in] _offset ��0��ʼ��ƫ�������ֽ�Ϊ��λ��
	 *	\param[out] _data �����ȡ���ڴ��ͷָ��
	 *	\param[in] _size Ҫ��ȡ���ڴ�εĳ��ȣ��ֽ�Ϊ��λ��
	 */
	virtual int32					reRead					(uint32 _offset, void* _data, const uint32 _size) = 0;

	//! �������������ڴ�ε�ͷָ��
	virtual const char*				raw						() const = 0;

	//! �������������ڲ���ƫ����Ϊ0
	virtual void					reset					() = 0;

	//! �����������Ĳ�����ͷ������PacketEnv.h��PACKET_HEAD_SIZE���ĳ���
	virtual void					setSize					(uint32 _size) = 0;

	//! ȡ���������Ĳ�����ͷ���ĳ���
	virtual uint32					getSize					() const = 0;

	//!	��������������Ϣ����
	virtual int32					setMsg					(const int32 _msg) = 0;

	//!	ȡ������������Ϣ����
	virtual int32					getMsg					() const = 0;

	//! ����������������ʱ�䣨��orz\Toolkit_Plus\Toolkit\Arithmetic.hpp��DateSys�ṹ��
	virtual int32					setDateTime				(const DateSys& _dTm) = 0;

	//! ȡ��������������ʱ�䣨��orz\Toolkit_Plus\Toolkit\Arithmetic.hpp��DateSys�ṹ��
	virtual int32					getDT					(DateSys& _dTm) const = 0;

	//! ����������Ƿ�Ϊ��
	virtual bool					empty					() const = 0;

	//! ��������������ݣ�����ƫ����Ϊ0
	virtual void					clear					() = 0;

	//! ȡ���������İ���ͷ�����ڵ��ܳ���
	virtual uint32					size					() const = 0;

	//! �Ƿ��Ƕ�̬�ڴ�ķ����Ĭ������£�����Ƕ����ĳ��ȣ�ֻ�г�����󳤶Ⱥ�ϵͳ�ŻὫ��������ʱ���Ϊ��̬�ģ������ɸ�������ݣ�
	virtual bool					isDyn					() const = 0;
};
}
#endif
