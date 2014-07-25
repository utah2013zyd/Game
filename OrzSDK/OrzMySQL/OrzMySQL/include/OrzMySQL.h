#ifndef __Orz_OrzMySQL_OrzMySQL_h__
#define __Orz_OrzMySQL_OrzMySQL_h__
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/IDatabase.h>
namespace Orz
{
namespace Mysql
{
class OrzMySQLImpl;
//! OrzMySQL�����ݿ���ʽӿ���
/*!
 *	\note һ��OrzMySQL����ͬһʱ��ֻ�ܷ���һ�����ݿ⣬����Mysql����һ������ΪOrzCORBA�����ݿ⣬\n
 *		�����OrzMySQL�����������е�ʱ���û�ֻ�ܴ����OrzCORBA�����ݿ���ִ��SQL�������ز���������SELECT��INSERT��DELETE��
 *	\note �߳ʰ�ȫ���������û��Զ���OrzMySQL��Mysql���ݿ��������������������ÿ�����ӻ��������һ��ר���߳������У�\n
		�����û�Ҫ�����Լ��������ĸ���������������������������Խ�࣬��OrzMySQL��ͬʱ�����SQL�����Խ�࣬��������Խ��
 *	\note OrzMySQL��query��queryEx��������ͬ����������ģ�����˵һ��SQL�������ȴ��䷵�غ�query��queryEx�����Ż᷵��\n
 *		���������������һ��SQL����ֻ��ռ��һ���������ӣ�Ҳ����һ���̣߳���������SQL����ֻҪ���ж�����������ӣ��Ϳ���wait-free����ִ�У�������������������������߳�����
 *	\note OrzMySQL��asyncQuery��asyncQueryEx���������첽�ģ���Ҫ�û�����һ���ص��������첽�ص�����SQL����Ľ��
 */
class _OrzMySQLExport OrzMySQL
	: public IDatabase
	, private boost::noncopyable
{
public:
	//! Ĭ�Ϲ��캯��
	OrzMySQL();

	//! ��������
	~OrzMySQL();

public:
	//! ȡ�����OrzMySQL�����Id
	DBId			getId				() const;

	//! ��OrzMySQL���г�ʼ��
	/*!
	 *	\param[in] _dbId ���OrzMySQL�����Id���û������Լ���������������ã��Զ�����䣩
	 *	\param[in] _params ��Ҫ���ݵĲ�����Params���������̶�8��������ÿ������һ�����£�\n
	 *		#1 Mysql��������IP��ַ������192.168.1.133
	 *		#2 Mysql�������Ķ˿ڣ�����3306
	 *		#3 ��¼Mysql���������û���������root
	 *		#4 ��¼Mysql�����������룬����******
	 *		#5 Ҫʹ�õ����������ƣ�����orz_corba
	 *		#6 ����Mysql������������������5
	 *		#7 OrzMySQL�����ڲ�ʹ�õ�SQL�����ִ�н������Ķ���ز���һ������10
	 *		#8 OrzMySQL�����ڲ�ʹ�õ�SQL�����ִ�н������Ķ���ز�����������100
	 *	\return true��ɹ���false��ʧ��
	 */
	bool			initialize			(DBId _dbId, const Params<std::string>& _params);

	//! ����OrzMySQL
	bool			boost				();

	//! �ر�OrzMySQL
	void			shutdown			();

	//! �ϲ�OrzMySQL�ڲ��̵߳�����OrzMySQL::boost���߳�
	void			join				();

	//! �ڵ�ǰ�߳�ͬ������ִ��SQL����
	/*!
	 *	\param[in] _qa query����
	 *	\return �������query�Ľ��
	 *	\note ͬ���������̰߳�ȫ
	 */
	QRWrap			query				(const QueryArg& _qa, Query& _q);

	//! ��OrzMySQL�ڲ��߳��첽ִ��SQL����
	/*!
	 *	\param[in] _qs ��Ҫ˳��ִ�е�query����
	 *	\param[in] _qrHdr һ��Functor���ص����û��߳�ִ��query�Ľ��
	 *	\return ��������첽query��id
	 *	\note �첽���̰߳�ȫ
	 */
	AynQId			query				(const QueryArg& _qa, QueryQueueShrPtr _qs, IQueryResultHdrShrPtr _qrHdr);


	//! ��OrzMySQL�ڲ����߳�post����ִ��SQL�����û��Զ��庯������
	/*!
	 *	\param[in] _qa query����
	 *	\param[in] _pqHdr һ��Functor �û��ṩ������ִ��SQL���ĺ�������
	 *	\note �����query�����û���_queryHdr�������Լ�ʹ��ͬ��������query��ִ��
	 *	\note �û�Ҫע�⣬���ַ�ʽ����_pqHdr�����ں��û����̲߳�ͬ���߳��ϣ�����_pqHdr�е��̰߳�ȫ��Ҫ�û����д���
	 *	\note �û�������_pqHdr��ʹ��post��post�Լ�����Ļص��������û��Լ����߳���ִ�н����ص��߼���
	 *			���������ص��߼��ͺ��û�����postQuery���߳���ͬһ���߳���
	 */
	bool			query				(const QueryArg& _qa, IPostQueryHdrShrPtr _pqHdr);

private:
	OrzMySQLImpl*						m_impl;
};
}
}
#endif
