#ifndef __Orz_OrzAsio_IConnection_h__
#define __Orz_OrzAsio_IConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class IConnectionImpl;

//! 网络连接基类
/*!
 *	用户通过继承此类可以构造自己的网络连接类，用于对网络连接的回调处理（callbacks）
 *	\note OnConnected、OnReceived、OnSended、OnDisconnected4个回调函数，只要是同一个Session，\n
 *	则这4个函数是不会同时调用的，这4者之间是线程安全的
 *
 example:
 \code
	// MyConnection.h
	class MyConnection
		: public IConnection
	{
	public:
		MyConnection();
		~MyConnection();

	public:
		void OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);
		void OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);
		void OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy);
		void OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy);
	};

	// MyConnection.cpp
	void MyConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
	{
		// 这里用户自定义自己的代码
		// 例如，如果是服务器端，可以将这个新连接进来的Session加入一个列表(例如Toolkit_Plus下的HashMap)，保存起来
		m_map.add(_snPxy, new Client(...));
		// 或者使用SessionDeputy，定义一个自己的Client类，加入SessionDeputy中，这个Client类可以是任何类型
		_snDy->setClient(ClientShrPtr(new Client));	// 记住在OnDisconnected中调用_snDy->clearClient();来释放内存
	}

	void MyConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
	{
		// 这里用户自定义自己的代码
		// 可以做一些针对Packet的操作
		int32 err = 0;
		int32 xxx = _pk->readInt(err);
		StringPtr str = _pk->readStr(err);
		if (err)
		{
			return;
		}
		// 然后如果之前在OnConnected中使用了_snDy->setClient(ClientShrPtr(new Client));则这里可以取得之前设置的Client对象来进行一定的操作
		IClientShrPtr client = _snDy->getClient();
	}

	void MyConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		// 这里用户自定义自己的代码
	}

	void MyConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		// 这里用户自定义自己的代码
		// 记住如果之前在OnConnected中使用了_snDy->setClient(ClientShrPtr(new Client));则这里必须调用_snDy->clearClient();来释放内存
		_snDy->clearClient();
		// 当然你也完全可以不使用SessionDeputy，不要被局限在这里；使用SessionDeputy的好处是无需查找每个Session的数据结构，省去了对Session的存储结构
	}
 \endcode
 */
class _OrzOrzAsioExport IConnection
{
public:
	//! 默认构造函数
	/*!
	 *	\note 通过此构造的IConnection对象默认将最大连接数目设为1
	 */
	IConnection();

	//! 构造函数
	/*!
	 *	\param[in] _maxInc 这个IConnection对象上最大的连接数目
	 *	\note 最大连接数，超过这个数值，则建立在这个Connetion对象上的连接会被拒绝
	 */
	IConnection(uint32 _maxInc);

	//! 拷贝构造函数
	/*!
	 *	\note 一般不会使用
	 */
	IConnection(const IConnection& _other);

	//! 析构函数
	virtual ~IConnection();

	//! 重载赋值运算符
	/*!
	 *	\note 一般不会使用
	 */
	IConnection& operator=(const IConnection& _other);

public:
	//! 取得实现类
	/*!
	 *	\note 这是系统内部需要用到的接口，用户无需使用，否则返回的IConnectionImpl对象无法编译通过（因为对应的.h文件是私有的）
	 */
	IConnectionImpl*				impl					();

	//! 当一个新的网络通信连接（OrzAsio统称为Session）建立后调用此回调函数（例如一个客户端建立了一个tcp/udp/ssl连接到服务器上）
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in, out] _snDy 这个Session的专用数据，用户可以用它保存一些自定义数据，详细请见SessionDeputy
	 *	\param[in] _isInc 是否是从外来的连接，真表示这个IConnection对象在当前进程中处理的是监听方面的网络连接的回调处理；\n
	 *	反之，表示这个IConnection对象在当前进程中处理的是连接到其他进程的网络连接的回调处理
	 *	\note 用户需要通过继承来自定义这个回调函数的内容
	 */
	virtual void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);

	//! 当Session收到一个网络封包（Packet）后调用
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in, out] _snDy 这个Session的专用数据，用户可以用它保存一些自定义数据，详细请见SessionDeputy
	 *	\param[in] _pk 收到的网络封包的智能指针
	 *	\note 用户需要通过继承来自定义这个回调函数的内容
	 *	\note 用户无需关心_pk的内存管理问题，系统自己会处理
	 */
	virtual void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);

	//! 当对这个Session成功发送了一个网络封包（Packet）后调用
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in, out] _snDy 这个Session的专用数据，用户可以用它保存一些自定义数据，详细请见SessionDeputy
	 *	\note 用户需要通过继承来自定义这个回调函数的内容
	 */
	virtual void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);

	//! 当Session的网络连接丢失后调用
	/*!
	 *	\param[in] _snPxy Session的Id
	 *	\param[in, out] _snDy 这个Session的专用数据，用户可以用它保存一些自定义数据，详细请见SessionDeputy
	 *	\note 用户需要通过继承来自定义这个回调函数的内容
	 *	\note 当这个函数调用的时候，这个Session的网络连接已经中断了，所以这时对这个Session发送封包是无效的
	 *	\note 当这个函数调用过后，_snPxy和_snDy参数虽然不会立刻被销毁，但已经无效化，所以请不要再继续使用它们
	 */
	virtual void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

private:
	//! IConnection实现类的对象，用户无需关心
	IConnectionImpl*				m_impl;
};
}
#endif
