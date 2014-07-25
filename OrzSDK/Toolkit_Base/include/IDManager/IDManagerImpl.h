#ifndef __Orz_ToolkitBase_IDManagerImpl_h__
#define __Orz_ToolkitBase_IDManagerImpl_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/IDManager/IDManager.h>
namespace Orz
{

class _OrzToolkitBasePrivate IDManagerImpl :private boost::noncopyable//no virtual, non-capyable
{
	typedef OrzHashMap<std::string, IDManager::id_type> IDMap;
	typedef std::map<IDManager::group_type, boost::shared_ptr<IDMap> > GroupMap;
	typedef OrzHashMap<IDManager::id_type, IDManager *> PointerMap;
	
public:


	inline IDManagerImpl(const std::string & name, IDManager::group_type group):
	_name(),
	_group(group), 
	_id(0)
	{
		_id = registerID(name, group);
	}


	~IDManagerImpl();
	inline IDManager::id_type getID(void) const{return _id;}
	inline IDManager::group_type getGroup(void) const{return _group;}
	inline const std::string & getName(void) const {return _name;}
	void registerPointer(IDManager * manger);


	///通过name和类型得到ID
	static IDManager::id_type  getTargetID(const std::string & name, IDManager::group_type group);


	///通过ID得到指针
	static IDManager * getPointer(IDManager::id_type  id);
private:


	
	///当前对象字符串名称
	std::string _name;

	
	///当前对象类型
	const IDManager::group_type _group;

	
	///当前对象数字ID
	IDManager::id_type _id;


	///用于通过字符串索引到数字ID的哈西表
	static GroupMap _maps;

	///用于通过数字ID索引到指针的哈西表
	static PointerMap _id2pointer;

	///一个递增的id计数器，用于分配更大的唯一ID
	static IDManager::id_type _count_id;


	


	IDManager::id_type registerID(const std::string & name , IDManager::group_type group);

};



}

#endif
