#ifndef __Orz_OrzAsio_IConnection_h__
#define __Orz_OrzAsio_IConnection_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class IConnectionImpl;

//! �������ӻ���
/*!
 *	�û�ͨ���̳д�����Թ����Լ������������࣬���ڶ��������ӵĻص�����callbacks��
 *	\note OnConnected��OnReceived��OnSended��OnDisconnected4���ص�������ֻҪ��ͬһ��Session��\n
 *	����4�������ǲ���ͬʱ���õģ���4��֮�����̰߳�ȫ��
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
		// �����û��Զ����Լ��Ĵ���
		// ���磬����Ƿ������ˣ����Խ���������ӽ�����Session����һ���б�(����Toolkit_Plus�µ�HashMap)����������
		m_map.add(_snPxy, new Client(...));
		// ����ʹ��SessionDeputy������һ���Լ���Client�࣬����SessionDeputy�У����Client��������κ�����
		_snDy->setClient(ClientShrPtr(new Client));	// ��ס��OnDisconnected�е���_snDy->clearClient();���ͷ��ڴ�
	}

	void MyConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
	{
		// �����û��Զ����Լ��Ĵ���
		// ������һЩ���Packet�Ĳ���
		int32 err = 0;
		int32 xxx = _pk->readInt(err);
		StringPtr str = _pk->readStr(err);
		if (err)
		{
			return;
		}
		// Ȼ�����֮ǰ��OnConnected��ʹ����_snDy->setClient(ClientShrPtr(new Client));���������ȡ��֮ǰ���õ�Client����������һ���Ĳ���
		IClientShrPtr client = _snDy->getClient();
	}

	void MyConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		// �����û��Զ����Լ��Ĵ���
	}

	void MyConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
	{
		// �����û��Զ����Լ��Ĵ���
		// ��ס���֮ǰ��OnConnected��ʹ����_snDy->setClient(ClientShrPtr(new Client));������������_snDy->clearClient();���ͷ��ڴ�
		_snDy->clearClient();
		// ��Ȼ��Ҳ��ȫ���Բ�ʹ��SessionDeputy����Ҫ�����������ʹ��SessionDeputy�ĺô����������ÿ��Session�����ݽṹ��ʡȥ�˶�Session�Ĵ洢�ṹ
	}
 \endcode
 */
class _OrzOrzAsioExport IConnection
{
public:
	//! Ĭ�Ϲ��캯��
	/*!
	 *	\note ͨ���˹����IConnection����Ĭ�Ͻ����������Ŀ��Ϊ1
	 */
	IConnection();

	//! ���캯��
	/*!
	 *	\param[in] _maxInc ���IConnection����������������Ŀ
	 *	\note ��������������������ֵ�����������Connetion�����ϵ����ӻᱻ�ܾ�
	 */
	IConnection(uint32 _maxInc);

	//! �������캯��
	/*!
	 *	\note һ�㲻��ʹ��
	 */
	IConnection(const IConnection& _other);

	//! ��������
	virtual ~IConnection();

	//! ���ظ�ֵ�����
	/*!
	 *	\note һ�㲻��ʹ��
	 */
	IConnection& operator=(const IConnection& _other);

public:
	//! ȡ��ʵ����
	/*!
	 *	\note ����ϵͳ�ڲ���Ҫ�õ��Ľӿڣ��û�����ʹ�ã����򷵻ص�IConnectionImpl�����޷�����ͨ������Ϊ��Ӧ��.h�ļ���˽�еģ�
	 */
	IConnectionImpl*				impl					();

	//! ��һ���µ�����ͨ�����ӣ�OrzAsioͳ��ΪSession����������ô˻ص�����������һ���ͻ��˽�����һ��tcp/udp/ssl���ӵ��������ϣ�
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in, out] _snDy ���Session��ר�����ݣ��û�������������һЩ�Զ������ݣ���ϸ���SessionDeputy
	 *	\param[in] _isInc �Ƿ��Ǵ����������ӣ����ʾ���IConnection�����ڵ�ǰ�����д�����Ǽ���������������ӵĻص�����\n
	 *	��֮����ʾ���IConnection�����ڵ�ǰ�����д���������ӵ��������̵��������ӵĻص�����
	 *	\note �û���Ҫͨ���̳����Զ�������ص�����������
	 */
	virtual void					OnConnected				(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc);

	//! ��Session�յ�һ����������Packet�������
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in, out] _snDy ���Session��ר�����ݣ��û�������������һЩ�Զ������ݣ���ϸ���SessionDeputy
	 *	\param[in] _pk �յ���������������ָ��
	 *	\note �û���Ҫͨ���̳����Զ�������ص�����������
	 *	\note �û��������_pk���ڴ�������⣬ϵͳ�Լ��ᴦ��
	 */
	virtual void					OnReceived				(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk);

	//! �������Session�ɹ�������һ����������Packet�������
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in, out] _snDy ���Session��ר�����ݣ��û�������������һЩ�Զ������ݣ���ϸ���SessionDeputy
	 *	\note �û���Ҫͨ���̳����Զ�������ص�����������
	 */
	virtual void					OnSended				(const SessionPxy& _snPxy, SessionDeputy& _snDy);

	//! ��Session���������Ӷ�ʧ�����
	/*!
	 *	\param[in] _snPxy Session��Id
	 *	\param[in, out] _snDy ���Session��ר�����ݣ��û�������������һЩ�Զ������ݣ���ϸ���SessionDeputy
	 *	\note �û���Ҫͨ���̳����Զ�������ص�����������
	 *	\note ������������õ�ʱ�����Session�����������Ѿ��ж��ˣ�������ʱ�����Session���ͷ������Ч��
	 *	\note ������������ù���_snPxy��_snDy������Ȼ�������̱����٣����Ѿ���Ч���������벻Ҫ�ټ���ʹ������
	 */
	virtual void					OnDisconnected			(const SessionPxy& _snPxy, SessionDeputy& _snDy);

private:
	//! IConnectionʵ����Ķ����û��������
	IConnectionImpl*				m_impl;
};
}
#endif
