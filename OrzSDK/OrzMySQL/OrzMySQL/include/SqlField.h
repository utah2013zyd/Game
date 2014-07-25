#ifndef __Orz_ToolkitPlus_SqlField_h_
#define __Orz_ToolkitPlus_SqlField_h_
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/ISqlField.h>
namespace Orz
{
namespace Mysql
{
//! 存储SQL语句执行结果中一行中的一个字段
/*!
 *	\note 用户要根据需求，自行选择将数据转换为何种类型
 */
class _OrzMySQLExport SqlField
	: public ISqlField
{
public:
	SqlField();
	~SqlField();

public:
	void			setValue				(char* _value, uint32 _length);
	uint32			getLength				();
	const char*		getString				();
	float32			getFloat32				();
	float64			getFloat64				();
	bool			getBool					();			
	uint8			getUint8				();
	int8			getInt8					();
	uint16			getUint16				();
	int16			getInt16				();
	uint32			getUint32				();
	int32			getInt32				();
	uint64			getUint64				();
	int64			getInt64				();

private:
	char*			m_value;
	uint32			m_length;
};
}
}
#endif
