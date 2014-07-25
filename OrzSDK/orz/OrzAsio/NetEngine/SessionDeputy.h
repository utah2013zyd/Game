#ifndef __Orz_OrzAsio_ISessionDeputy_h__
#define __Orz_OrzAsio_ISessionDeputy_h__
#include <orz/OrzAsio/OrzAsioConfig.h>
namespace Orz
{
class SessionDeputyImpl;
//! �������ݱ�����
/*!
 *	\note ����Session���û��Զ�������
 */
class _OrzOrzAsioExport SessionDeputy
{
public:
	//! �ص�����������
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
	//! �����û��Զ���������
	/*!
	 *	\param[in] _client Session���û��Զ�������
	 *	\note ���Ϳ������ⶨ�壬�������ֱ�����Client��Ȼ������ָ������ֱ�����IClientShrPtr
	 */
	void					setClient				(IClientShrPtr _client);

	//! ȡ���û��Զ���������
	IClientShrPtr			getClient				() const;

	//! ����û��Զ���������
	/*!
	 *	\note �Ƽ���IConnetion::OnDisconnected�е���
	 */
	void					clearClient				();

	//! ������һ���û��Զ������ݵ�ָ��
	/*!
	 *	\param[in] _tag Session����һ���û��Զ������ݵ�ָ��
	 */
	void					setTag					(void* _tag);

	//! ȡ����һ���û��Զ������ݵ�ָ��
	void*					getTag					() const;

	//! �����һ���û��Զ������ݵ�ָ��
	/*!
	 *	\note �Ƽ���IConnetion::OnDisconnected�е���
	 */
	void					clearTag				();

	//! �Ƿ���Ч���������Ҫʹ��
	bool					isValid					() const;

	//! ϵͳ�ڲ�ʹ�ã��û�����ʹ��
	SessionDeputyImpl&		impl					();

private:
	SessionDeputyImpl*		m_impl;
};
}
#endif
