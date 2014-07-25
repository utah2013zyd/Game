#ifndef __Orz_OrzAsio_SessionPxy_h__
#define __Orz_OrzAsio_SessionPxy_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
class Session;
class SessionPxyLess;
//! Session的Id类
/*!
 *	\note 保存Session的Id标识信息
 */
class _OrzOrzAsioExport SessionPxy
{
	friend class Session;
	friend class SessionPxyLess;
public:
	//! 默认构造函数
	SessionPxy();

	//! 构造函数
	/*!
	 *	\note 用户基本上很少会使用到这个构造函数
	 */
	SessionPxy(SessionId _snid, ISession* _sn, ValidCode _vcode, int32 _socketType);

	//! 析构函数
	~SessionPxy();

public:
	//! 取得整型形式的Session的Id
	SessionId				getSnid						() const;

	//! 取得Session指针
	ISession*				getSession					() const;

	//! 重载运算符==
	bool					operator==					(const SessionPxy& _other) const;

	//! 重载运算符<
	bool					operator<					(const SessionPxy& _other) const;

	//! 用户基本上很少会使用
	ValidCode				getVcode					() const;

	//! 取得这个Id指向的Session的socket类型
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

//! 用于std::map的key排序
class _OrzOrzAsioExport SessionPxyLess
{
public:
	bool operator() (const SessionPxy& _s1, const SessionPxy& _s2) const;
};

}
#endif
