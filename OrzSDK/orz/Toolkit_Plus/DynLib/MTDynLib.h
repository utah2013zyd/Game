#ifndef __Orz_ToolkitPlus_DynLib_h__
#define __Orz_ToolkitPlus_DynLib_h__
#include <orz/Toolkit_Plus/ToolkitPlusConfig.h>
namespace Orz
{
class MTDynLibImpl;
//! 动态库类
/*!
 *	\note 封装了windows和linux平台的动态库（dll/so）
 */
class _OrzToolkitPlusExport MTDynLib
{
public:
    //! 构造函数，用户无需直接使用，而是通过MTDynLibMgr
    MTDynLib(const std::string& _name, MTDynLibId _dlid);

	//! 构造函数，用户无需直接使用，而是通过MTDynLibMgr
	MTDynLib(const MTDynLib & dl);

	//! 重载赋值操作符
	MTDynLib& operator=(const MTDynLib& dl);

	//! 析构函数
    ~MTDynLib(void);

public:
	//! 用户无需直接使用
	void				swap			(MTDynLib& dl) throw();

    //! 加载动态库
    bool				load			(void);

    //! 卸载动态库
    bool				unload			(void);

	//! 取得动态库的名字
	const std::string&	getName			(void) const;

	//! 取得动态库的Id
	MTDynLibId			getId			(void) const;

	//! 返回指定名字的动态库中符号的地址（对象、函数）
	/*!
	 *	\param[in] _strName 动态库中符号的名字
	 */
    void*				getSymbol		(const std::string& _strName) const throw();

	//! 取得上一次动态库发生的错误
	std::string			dynLibError		(void);

private:
   boost::scoped_ptr<MTDynLibImpl>		m_impl;
};

}

#endif
