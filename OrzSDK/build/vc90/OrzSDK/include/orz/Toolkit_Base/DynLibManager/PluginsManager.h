#ifndef __Orz_ToolkitBase_PluginsManager_h__
#define __Orz_ToolkitBase_PluginsManager_h__
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/Singleton.h>



namespace Orz {

class PluginsManagerImpl;
template class _OrzToolkitBaseExport boost::scoped_ptr<PluginsManagerImpl>;
class _OrzToolkitBaseExport PluginsManager: public Singleton<PluginsManager>
{


public:
	PluginsManager(void);
	~PluginsManager(void);

public:
	///��ʼ�����������
	bool init(void);

	///�رղ��������
	void shutdown(void);

	///д�������Ѿ�������
	void clear();

	///������
	void loadPlugin(const std::string & pluginName);

	///���ļ�������һЩ���
	void loadPluginFromFile(const std::string & fileName);

	///ж�ز��
	void unloadPlugin(const std::string & pluginName);

private:
	boost::scoped_ptr<PluginsManagerImpl> _impl;
};

}



//\
//SYSTEMUNIT_FACTORY(Interface, PluginsManager)


#endif
