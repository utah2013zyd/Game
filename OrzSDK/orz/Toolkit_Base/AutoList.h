
#ifndef __Orz_ToolkitBase_AutoList_h__
#define __Orz_ToolkitBase_AutoList_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>


namespace Orz{

template<typename T> class 
AutoList
{

public:
	///列表类型
	typedef typename std::vector<T *> List;

	///迭代器类型
	typedef typename List::iterator Iterator;

	///迭代器常量类型
	typedef typename List::const_iterator ConstIterator;

public:
	
	AutoList(void):_inlist(false){}
	virtual ~AutoList(void){}

	///连接入列表
	void link(void)
	{
		if(!_inlist)
		{
			_list.push_back(static_cast<T *>(this));
			_inlist = true;
		}
	}
	///从列表中移除
	void unlink(void)
	{
		if(_inlist)
		{
			_list.erase(std::remove(_list.begin(), _list.end(), this), _list.end());
			_inlist = false;
		}
	}

	///检查是否在列表中
	bool islink(void)
	{
		return _inlist;
	}

	///移除所有列表中实例，并清空列表
	static void unlinkAll(void)
	{
		for(Iterator it = begin(); it != end(); ++it)
		{
			(*it)->_inlist = false;
		}
		_list.clear();
	}

	///得到起始位置迭代器
	static Iterator begin(void)
	{
		return(_list.begin());
	}
	///得到最终位置迭代器
	static Iterator end(void)
	{
		return (_list.end());
	}

	///得到起始位置常量迭代器
	static ConstIterator const_begin(void)
	{
		const List & clist = _list;
		return(clist.begin());
	}

	///得到最终位置常量迭代器
	static ConstIterator const_end(void)
	{
		const List & clist = _list;
		return(clist.end());
	}
protected:
	static List _list;
private:
	bool _inlist;
};



}
#endif

