#ifndef __Orz_OrzAsio_ISessionDeputy_h__
#define __Orz_OrzAsio_ISessionDeputy_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class SessionDeputyImpl;
//! 连接数据保存类
/*!
 *	\note 保存Session的用户自定义数据
 */
class _OrzOrzAsioExport SessionDeputy
{
public:
	//! 回调函数的种类
	enum CallbackType
	{
		CT_INIT = 0,
		CT_RECV,
		CT_SEND,
		CT_ERROR,
		CT_TOTAL
	};

	SessionDeputy();
	virtual ~SessionDeputy();

public:
	//! 设置用户自定义数据类
	/*!
	 *	\param[in] _client Session的用户自定义数据
	 *	\note 类型可以任意定义，但是名字必须是Client，然后智能指针的名字必须是IClientShrPtr
	 */
	void					setClient				(IClientShrPtr _client);

	//! 取得用户自定义数据类
	IClientShrPtr			getClient				() const;

	//! 清除用户自定义数据类
	/*!
	 *	\note 推荐在IConnetion::OnDisconnected中调用
	 */
	void					clearClient				();

	//! 设置另一个用户自定义数据的指针
	/*!
	 *	\param[in] _tag Session的另一个用户自定义数据的指针
	 */
	void					setTag					(void* _tag);

	//! 取得另一个用户自定义数据的指针
	void*					getTag					() const;

	//! 清除另一个用户自定义数据的指针
	/*!
	 *	\note 推荐在IConnetion::OnDisconnected中调用
	 */
	void					clearTag				();

	//! 是否还有效，如果否则不要使用
	bool					isValid					() const;

	//! 系统内部使用，用户无需使用
	SessionDeputyImpl&		impl					();

private:
	SessionDeputyImpl*		m_impl;
};
}
#endif
