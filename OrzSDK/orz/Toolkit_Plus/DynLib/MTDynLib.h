#ifndef __Orz_ToolkitPlus_DynLib_h__
#define __Orz_ToolkitPlus_DynLib_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class MTDynLibImpl;
//! ��̬����
/*!
 *	\note ��װ��windows��linuxƽ̨�Ķ�̬�⣨dll/so��
 */
class _OrzToolkitPlusExport MTDynLib
{
public:
    //! ���캯�����û�����ֱ��ʹ�ã�����ͨ��MTDynLibMgr
    MTDynLib(const std::string& _name, MTDynLibId _dlid);

	//! ���캯�����û�����ֱ��ʹ�ã�����ͨ��MTDynLibMgr
	MTDynLib(const MTDynLib & dl);

	//! ���ظ�ֵ������
	MTDynLib& operator=(const MTDynLib& dl);

	//! ��������
    ~MTDynLib(void);

public:
	//! �û�����ֱ��ʹ��
	void				swap			(MTDynLib& dl) throw();

    //! ���ض�̬��
    bool				load			(void);

    //! ж�ض�̬��
    bool				unload			(void);

	//! ȡ�ö�̬�������
	const std::string&	getName			(void) const;

	//! ȡ�ö�̬���Id
	MTDynLibId			getId			(void) const;

	//! ����ָ�����ֵĶ�̬���з��ŵĵ�ַ�����󡢺�����
	/*!
	 *	\param[in] _strName ��̬���з��ŵ�����
	 */
    void*				getSymbol		(const std::string& _strName) const throw();

	//! ȡ����һ�ζ�̬�ⷢ���Ĵ���
	std::string			dynLibError		(void);

private:
   boost::scoped_ptr<MTDynLibImpl>		m_impl;
};

}

#endif
