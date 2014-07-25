
#ifndef __Orz_ToolkitBase_IDManager_h__
#define __Orz_ToolkitBase_IDManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Exception.h>
namespace Orz
{
 /** Specialisation of HardwareBuffer for a pixel buffer. The
    	HardwarePixelbuffer abstracts an 1D, 2D or 3D quantity of pixels
    	stored by the rendering API. The buffer can be located on the card
    	or in main memory depending on its usage. One mipmap level of a
    	texture is an example of a HardwarePixelBuffer.
    */
class IDManagerImpl;
template class _OrzToolkitBaseExport boost::scoped_ptr<IDManagerImpl>;


class _OrzToolkitBaseExport IDManager
{

public:
	///定义ID的类型
	typedef uint32 id_type;
	
	///定义Group的类型
	typedef uint32 group_type;

public:
	IDManager(const std::string & name, group_type type);

	/////拷贝构造函数
	//IDManager(const IDManager & manager);

	//
	

	//virtual void swap(IDManager& manager);

	virtual ~IDManager(void);

	///得到数字ID
	id_type getID(void) const;

	///得到Group
	group_type getGroup(void) const;

	///得到名称
	const std::string & getName(void) const;


public:
	
	
	///管理器内建常量，用来代表“不设置”，“交由系统配置”。
	static const std::string BLANK;



	///通过name和Group得到ID
	static id_type getTargetID(const std::string & name, group_type group) ;
	
	///通过id_type得到指针
	static IDManager * getPointer(id_type id);

	///通过name和Group得到指针
	inline static IDManager * getPointer(const std::string & name, group_type group);

private:
	IDManager& operator = (const IDManager& manager);//do nothing

	boost::scoped_ptr<IDManagerImpl> _impl;

};


IDManager * IDManager::getPointer(const std::string & name, group_type group)
{
	return getPointer(getTargetID(name, group));
}
}

#endif

