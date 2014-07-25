#ifndef __Orz_ToolkitPlus_DynLibManager_h__
#define __Orz_ToolkitPlus_DynLibManager_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class MTDynLibMgrImpl;
//! ��̬�������
/*!
 *	\note ��װ��windows��linuxƽ̨�Ķ�̬�⣨dll/so��
 */
class _OrzToolkitPlusExport MTDynLibMgr
{
public:
	//! ���캯��
	/*!
	 *	\param[in] _dynLibsHash �洢��̬���HashMap��hash��С��һ��1~10��1����1��Ͱ��10����10��Ͱ����ϵͳ���ض�����
	 */
	explicit MTDynLibMgr(uint32 _dynLibsHash);

	//! ��������
    ~MTDynLibMgr();

public:
	//! ����ָ�����ƵĶ�̬��
	/*!
	 *	\param[in] _filename ��̬�������
	 *	\return �����õĶ�̬������������ʧ�ܣ��򷵻ص�ָ��Ϊ��
	 *	\note _filename��windowsƽ̨��Ҫȥ��.dll��׺������Linuxƽ̨��Ҫ����ȫ�ƣ�����windows�¡�MyDLL.dll��Ҫ���MyDLL����Linux��Ҫ���MyDLL.so
	 */
	MTDynLibShrPtr		load			(const std::string& _filename);

	//! ж�ض�̬��
	/*!
	 *	\param[in] _lib ��̬�����
	 */
	void				unload			(MTDynLibShrPtr _lib);

private:
	MTDynLibMgrImpl*	m_impl;
};
}
#endif
