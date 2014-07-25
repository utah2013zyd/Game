#ifndef __Orz_ToolkitPlus_QRWrap_hpp_
#define __Orz_ToolkitPlus_QRWrap_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! ���ݿ�����ִ�н����װ��
/*!
 *	\note ��Ҫ��װ��IQueryResult�Լ����ݿ�ִ�������Ƿ�ɹ������ӵ����ݿ������Id����Ϣ
 *	\note ʹ�÷�����OrzMySQL
 */
struct QRWrap
{
	QRWrap() : cnid(0), isOk(false) {}
	QRWrap(IQueryResultShrPtr _qr, DBCnId _cnid, bool _isOk) : qr(_qr), cnid(_cnid), isOk(_isOk) {}

	IQueryResultShrPtr	qr;
	DBCnId				cnid;
	bool				isOk;
};
}
#endif
