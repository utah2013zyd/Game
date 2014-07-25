#ifndef __Orz_ToolkitPlus_SqlField_h_
#define __Orz_ToolkitPlus_SqlField_h_
#include <OrzMySQLConfig.h>
#include <orz/Toolkit_Plus/Database/ISqlField.h>
namespace Orz
{
namespace Mysql
{
//! �洢SQL���ִ�н����һ���е�һ���ֶ�
/*!
 *	\note �û�Ҫ������������ѡ������ת��Ϊ��������
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
