#ifndef __Orz_OrzAsio_PacketFreeList_h_
#define __Orz_OrzAsio_PacketFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! ���boost::shared_ptr<IPacket>���Զ������������������
/*!
 *	\note �й�boost::shared_ptr���Զ���deleter����Ϣ�����boost����ĵ�
 example:
	IPacketShrPtr pk(sPkFList.alloc(), PacketDeleter());
 */
class _OrzOrzAsioExport PacketDeleter
{
public:
	//! Ĭ�Ϲ��캯��
	PacketDeleter();

	//! ��������
	~PacketDeleter();

	//! ���������()
	/*!
	 *	\param[in] _ptr Ҫ���ͷŵĶ���ָ��
	 */
	void operator()(IPacket* _ptr);
};


//! ��������FreeList��
/*!
 *	\note ������������ڴ��
 *	\note set��clear�����̰߳�ȫ�ģ�alloc��free�����̺߳͹���ȫ��
 example:
 \code
	// �����ǵ��߳� 
	NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
	sPkFList.set(_packetFreeList);	
	// �����ǵ��߳�

	// �����Ƕ��߳�
	...
	IPacketShrPtr pk = allocPacket();
	...
	// �����Ƕ��߳�

	// �����ǵ��߳� 
	sPkFList.clear();	// �˲��貢�Ǳ��룬PacketFreeList�������������Զ��ͷŶ���أ���ĳЩ����£����������PacketFreeList��������������֮ǰ���ٶ���أ���˷�����������
	// �����ǵ��߳�
 \endcode
 */
class _OrzOrzAsioExport PacketFreeList
{
	//! Ĭ�Ϲ��캯��
	/*!
	 *	\note ����ģʽ��������˽�е�
	 */
	PacketFreeList();

	//! �������캯��
	/*!
	 *	\note ����ģʽ��������˽�е�
	 */
    PacketFreeList(const PacketFreeList& _other);

	//! ��ֵ���������
	/*!
	 *	\note ����ģʽ��������˽�е�
	 */
	PacketFreeList& operator=(const PacketFreeList& _other);

public:
	//! ��������
	~PacketFreeList();

public:
	//! ��������
	/*!
	 *	\note ����static��ʵ�ֵĵ���ģʽ���мǣ���֤��һ�ε������������ʱ�����ڵ��߳���
	 *	\note �벻Ҫֱ�ӵ��ô˺���������ʹ�ú���ĺ�#define sPkFList PacketFreeList::instance()
	 *	\return PacketFreeList���������
	 */
	static PacketFreeList& instance();

public:
	//! �����û�������FreeList<xxx>���� 
	/*!
	 *	\note ��orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\param[in] _freeList �����û�������FreeList<xxx>����
	 */
	void				set				(IFreeListShrPtr _freeList);

	//! �Ӷ�����з���һ������������
	/*!
	 *	\note ��orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\note �̰߳�ȫ
	 *	\note �뾡����Ҫֱ��ʹ���������������allocPacket()����װ�˷���
	 *	\note ���������Ѿ�û�пɷ���Ķ��������new������Packet����
	 *	\return IPacket* ����õ�Packet����
	 */
	IPacket*			alloc			();

	//! �Ӷ�����з���һ������������ʹ��boost::shared_ptr��������������
	/*!
	 *	\note ��ʹ�ô˷������allocPacket�����������صĶ���
	 *	\note �̰߳�ȫ
	 example:
		{
			...
			IPacketShrPtr pk = allocPacket();
			...
		}
		// pk�����٣�ͬʱ����û�������ط���pk�����ã�����PacketDeleter�����ã����pkָ��ָ�������������PacketFreeList::free�Զ�����
	 */
	IPacketShrPtr		select			();

	//! ����ػ���һ������������
	/*!
	 *	\note ��orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\note �̰߳�ȫ
	 *	\note �벻Ҫֱ�ӵ�������������������boost::shared_ptr<IPacket>����ϵͳ�Զ�����PacketFreeList::free����PacketDeleter
	 *	\note ����ػ���Ҫ�����յ�ָ������Ƿ����Լ�������ģ�������ǣ������delete�����ٶ��������벻Ҫ���Լ�������Packet������free�����٣������corruption
	 *	\param[in] _ptr Ҫ�����յ�Packet����ָ��
	 */
	void				free			(IPacket* _ptr);

	//! ����û�������FreeList<xxx>���� 
	/*!
	 *	\note ��orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 */
	void				clear			();

private:
	IFreeListShrPtr		m_freeList;
};
}
#define sPkFList Orz::PacketFreeList::instance()
#define allocPacket Orz::PacketFreeList::instance().select
#endif
