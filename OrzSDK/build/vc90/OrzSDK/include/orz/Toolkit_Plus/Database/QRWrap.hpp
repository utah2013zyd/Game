#ifndef __Orz_ToolkitPlus_QRWrap_hpp_
#define __Orz_ToolkitPlus_QRWrap_hpp_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 数据库命令执行结果包装类
/*!
 *	\note 主要封装了IQueryResult以及数据库执行命令是否成功和连接到数据库的连接Id等信息
 *	\note 使用方法见OrzMySQL
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
