#ifndef __Orz_ToolkitPlus_IQueryResult_h_
#define __Orz_ToolkitPlus_IQueryResult_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 数据库命令执行结果接口类
/*!
 *	\note 用户可以通过继承来自定义自己的数据库命令执行结果类
 *	\note 使用方法见OrzMySQL
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
