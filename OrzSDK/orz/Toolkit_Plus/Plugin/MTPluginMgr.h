#ifndef __Orz_ToolkitPlus_PluginsManager_h__
#define __Orz_ToolkitPlus_PluginsManager_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
#include <orz/Toolkit_Plus/Plugin/PluginWrap.hpp>
#include <orz/Toolkit_Plus/Toolkit/Params.hpp>
namespace Orz
{
class MTPluginMgrImpl;
//! Orz���������
class _OrzToolkitPlusExport MTPluginMgr
{
public:
	//! ���캯��
	/*!
	 *	\param[in] _plgsHash �洢��������HashMap��hash��С��һ��1~10��1����1��Ͱ��10����10��Ͱ����ϵͳ���ض�����
	 *	\param[in] _dynLibMgr ��������������ڹ���̬��/������ʽ��Orz���
	 */
	MTPluginMgr(uint32 _plgsHash, MTDynLibMgr* _dynLibMgr);

	//! ��������
	~MTPluginMgr();

public:
	//! ���ض�̬����ʽ��Orz���
	/*!
	 *	\param[in] _pluginName ������֣�Ҳ���Ƕ�̬�����ƣ���MTDynLibMgr
	 *	\param[in] _params Ҫ���ݵĲ�����ʹ�÷�����orz/Toolkit_Plus/Toolkit/Params.hpp
	 *	\return ���ش����õĲ������İ�װ�����ڰ���Orz������󣬼�PluginWrap
	 *	\note ���صĲ������İ�װ������Ҫ�������û�ʹ��addPlugin������¼��صĲ��
	 */
	PluginWrap		loadPlugin			(const char* _pluginName, const Params<std::string>& _params);

	//! ж�ض�̬����ʽ��Orz���
	/*!
	 *	\param[in] _plgWrap Ҫж�صĲ������İ�װ����
	 *	\return ���ر�ж�ص�Orz���Id
	 *	\note ���ص�Id�������û�ʹ��removePlugin���Ƴ������ж�صĲ��
	 */
	bool			unloadPlugin		(PluginWrap _plgWrap);

	//! ���Orz���
	/*!
	 *	\param[in] _plgWrap �������İ�װ����
	 *	\return ������ӵ�Orz�����Id
	 *	\note ʹ��loadPlugin���سɹ��󣬻���Ҫ��addPlugin������ϵͳ
	 *	\note �û�Ҳ�������д����������ʹ��loadPlugin����Ȼ����addPlugin������ϵͳ������������ϵͳ������ֻ�ܹ���̬����ʽ��Orz���
	 *	\note ����û��Լ�����PluginWrap������ʹ��PluginWrap(IOrzPlugin* _plg)������캯��������ʹ��һ������Ϊ�գ���PluginWrap
	 */
	OrzPluginId		addPlugin			(PluginWrap _plgWrap);

	//! �Ƴ�Orz���
	/*!
	 *	\param[in] _id Orz�����Id
	 *	\return ���ر�ж�صĲ������İ�װ����
	 *	\note ���صĲ������İ�װ������Ҫ�������û�ʹ��unloadPlugin��ж������������Ȼǰ�����������Ƕ�̬����ʽ��
	 */
	PluginWrap		removePlugin		(OrzPluginId _id);

	//! �������������
	bool			boost				();

	//! ����ָ��Id�Ĳ��
	bool			boostPlugin			(OrzPluginId _plgId);

	//! �ر�ָ��Id�Ĳ��
	void			shutdownPlugin		(OrzPluginId _plgId);

	//! �ϲ�ָ��Id�Ĳ���ڲ��߳�
	void			joinPlugin			(OrzPluginId _plgId);

	//! �رղ��������
	void			shutdown			();

	//! �ϲ����в���ڲ��߳�
	void			join				();

private:
	MTPluginMgrImpl*						m_impl;
};
}
#endif
