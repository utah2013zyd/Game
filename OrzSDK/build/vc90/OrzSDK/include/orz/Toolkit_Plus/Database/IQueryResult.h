#ifndef __Orz_ToolkitPlus_IQueryResult_h_
#define __Orz_ToolkitPlus_IQueryResult_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! ���ݿ�����ִ�н���ӿ���
/*!
 *	\note �û�����ͨ���̳����Զ����Լ������ݿ�����ִ�н����
 *	\note ʹ�÷�����OrzMySQL
 */
class IQueryResult
{
public:
	IQueryResult() {}
	virtual ~IQueryResult() {}

public:
	virtual bool			nextRow				() = 0;
	virtual void			clear				() = 0;
	virtual ISqlField*		fetch				(uint32 _offset) = 0;
	virtual uint32			getFieldCount		() const = 0;
	virtual uint32			getRowCount			() const = 0;
	virtual DBCnId			getConnection		() const = 0;
};
}
#endif
