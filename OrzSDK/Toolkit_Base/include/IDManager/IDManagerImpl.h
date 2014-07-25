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


	///ͨ��name�����͵õ�ID
	static IDManager::id_type  getTargetID(const std::string & name, IDManager::group_type group);


	///ͨ��ID�õ�ָ��
	static IDManager * getPointer(IDManager::id_type  id);
private:


	
	///��ǰ�����ַ�������
	std::string _name;

	
	///��ǰ��������
	const IDManager::group_type _group;

	
	///��ǰ��������ID
	IDManager::id_type _id;


	///����ͨ���ַ�������������ID�Ĺ�����
	static GroupMap _maps;

	///����ͨ������ID������ָ��Ĺ�����
	static PointerMap _id2pointer;

	///һ��������id�����������ڷ�������ΨһID
	static IDManager::id_type _count_id;


	


	IDManager::id_type registerID(const std::string & name , IDManager::group_type group);

};



}

#endif
