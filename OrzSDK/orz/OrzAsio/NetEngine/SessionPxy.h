#ifndef __Orz_OrzAsio_SessionPxy_h__
#define __Orz_OrzAsio_SessionPxy_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class Session;
class SessionPxyLess;
//! Session��Id��
/*!
 *	\note ����Session��Id��ʶ��Ϣ
 */
class _OrzOrzAsioExport SessionPxy
{
	friend class Session;
	friend class SessionPxyLess;
public:
	//! Ĭ�Ϲ��캯��
	SessionPxy();

	//! ���캯��
	/*!
	 *	\note �û������Ϻ��ٻ�ʹ�õ�������캯��
	 */
	SessionPxy(SessionId _snid, ISession* _sn, ValidCode _vcode, int32 _socketType);

	//! ��������
	~SessionPxy();

public:
	//! ȡ��������ʽ��Session��Id
	SessionId				getSnid						() const;

	//! ȡ��Sessionָ��
	ISession*				getSession					() const;

	//! ���������==
	bool					operator==					(const SessionPxy& _other) const;

	//! ���������<
	bool					operator<					(const SessionPxy& _other) const;

	//! �û������Ϻ��ٻ�ʹ��
	ValidCode				getVcode					() const;

	//! ȡ�����Idָ���Session��socket����
	int32					getSocketType				() const;

private:
	void					setVcode					(ValidCode _vcode);
	void					setSession					(ISession* _sn);
	
private:
	SessionId				m_snid;
	mutable ISession*		m_sn;
	ValidCode				m_vcode;
	int32					m_socketType;
};

//! ����std::map��key����
class _OrzOrzAsioExport SessionPxyLess
{
public:
	bool operator() (const SessionPxy& _s1, const SessionPxy& _s2) const;
};

}
#endif
