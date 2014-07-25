#ifndef __Orz_ToolkitPlus_ISqlField_h_
#define __Orz_ToolkitPlus_ISqlField_h_
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
//! 数据库查询结果接口类
/*!
 *	\note 用户可以通过继承来自定义自己的查询结果类
 *	\note 使用方法见OrzMySQL
 */
class ISqlField
{
public:
	ISqlField() {}
	virtual ~ISqlField() {}

public:
	virtual void			setValue				(char* _value, uint32 _length) = 0;
	virtual uint32			getLength				() = 0;
	virtual const char*		getString				() = 0;
	virtual float32			getFloat32				() = 0;
	virtual float64			getFloat64				() = 0;
	virtual bool			getBool					() = 0;			
	virtual uint8			getUint8				() = 0;
	virtual int8			getInt8					() = 0;
	virtual uint16			getUint16				() = 0;
	virtual int16			getInt16				() = 0;
	virtual uint32			getUint32				() = 0;
	virtual int32			getInt32				() = 0;
	virtual uint64			getUint64				() = 0;
	virtual int64			getInt64				() = 0;
};
}
#endif
