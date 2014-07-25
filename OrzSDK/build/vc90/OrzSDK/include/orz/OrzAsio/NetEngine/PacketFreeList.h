#ifndef __Orz_OrzAsio_PacketFreeList_h_
#define __Orz_OrzAsio_PacketFreeList_h_
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
//! 配合boost::shared_ptr<IPacket>来自动回收网络封包对象的类
/*!
 *	\note 有关boost::shared_ptr的自定义deleter的信息，请见boost相关文档
 example:
	IPacketShrPtr pk(sPkFList.alloc(), PacketDeleter());
 */
class _OrzOrzAsioExport PacketDeleter
{
public:
	//! 默认构造函数
	PacketDeleter();

	//! 析构函数
	~PacketDeleter();

	//! 重载运算符()
	/*!
	 *	\param[in] _ptr 要被释放的对象指针
	 */
	void operator()(IPacket* _ptr);
};


//! 网络封包的FreeList类
/*!
 *	\note 针对网络封包的内存池
 *	\note set和clear不是线程安全的；alloc和free则是线程和共享安全的
 example:
 \code
	// 以下是单线程 
	NetPacketFLShrPtr pkFL(new FreeList<NetPacket>(10, 100));
	sPkFList.set(_packetFreeList);	
	// 以上是单线程

	// 以下是多线程
	...
	IPacketShrPtr pk = allocPacket();
	...
	// 以上是多线程

	// 以下是单线程 
	sPkFList.clear();	// 此步骤并非必须，PacketFreeList的析构函数会自动释放对象池；但某些情况下，例如必须在PacketFreeList的析构函数调用之前销毁对象池，则此方法就有用了
	// 以上是单线程
 \endcode
 */
class _OrzOrzAsioExport PacketFreeList
{
	//! 默认构造函数
	/*!
	 *	\note 单件模式，所以是私有的
	 */
	PacketFreeList();

	//! 拷贝构造函数
	/*!
	 *	\note 单件模式，所以是私有的
	 */
    PacketFreeList(const PacketFreeList& _other);

	//! 赋值运算符重载
	/*!
	 *	\note 单件模式，所以是私有的
	 */
	PacketFreeList& operator=(const PacketFreeList& _other);

public:
	//! 析构函数
	~PacketFreeList();

public:
	//! 单件方法
	/*!
	 *	\note 利用static来实现的单件模式，切记，保证第一次调用这个函数的时候是在单线程下
	 *	\note 请不要直接调用此函数，而是使用后面的宏#define sPkFList PacketFreeList::instance()
	 *	\return PacketFreeList对象的引用
	 */
	static PacketFreeList& instance();

public:
	//! 设置用户创建的FreeList<xxx>对象 
	/*!
	 *	\note 见orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\param[in] _freeList 设置用户创建的FreeList<xxx>对象
	 */
	void				set				(IFreeListShrPtr _freeList);

	//! 从对象池中分配一个网络封包对象
	/*!
	 *	\note 见orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\note 线程安全
	 *	\note 请尽量不要直接使用这个方法，而是allocPacket()来包装此方法
	 *	\note 如果对象池已经没有可分配的对象，则改用new来创建Packet对象
	 *	\return IPacket* 分配好的Packet对象
	 */
	IPacket*			alloc			();

	//! 从对象池中分配一个网络封包对象，使用boost::shared_ptr来处理生存周期
	/*!
	 *	\note 请使用此方法配合allocPacket宏来分配对象池的对象
	 *	\note 线程安全
	 example:
		{
			...
			IPacketShrPtr pk = allocPacket();
			...
		}
		// pk被销毁，同时由于没有其他地方对pk的引用，所以PacketDeleter被调用，这个pk指针指向的网络封包对象被PacketFreeList::free自动回收
	 */
	IPacketShrPtr		select			();

	//! 对象池回收一个网络封包对象
	/*!
	 *	\note 见orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 *	\note 线程安全
	 *	\note 请不要直接调用这个方法，而是配合boost::shared_ptr<IPacket>来让系统自动调用PacketFreeList::free，见PacketDeleter
	 *	\note 对象池会检测要被回收的指针对象是否是自己分配过的，如果不是，则改用delete来销毁对象，所以请不要将自己创建的Packet对象用free来销毁，会造成corruption
	 *	\param[in] _ptr 要被回收的Packet对象指针
	 */
	void				free			(IPacket* _ptr);

	//! 清除用户创建的FreeList<xxx>对象 
	/*!
	 *	\note 见orz\Toolkit_Plus\Toolkit\FreeList.hpp
	 */
	void				clear			();

private:
	IFreeListShrPtr		m_freeList;
};
}
#define sPkFList Orz::PacketFreeList::instance()
#define allocPacket Orz::PacketFreeList::instance().select
#endif
