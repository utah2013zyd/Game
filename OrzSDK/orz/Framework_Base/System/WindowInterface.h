
#ifndef __Orz_FrameworkBase_WindowInterface_h__
#define __Orz_FrameworkBase_WindowInterface_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

namespace Orz{



class _OrzFrameworkBaseExport WindowListener
{
public:
	virtual ~WindowListener(void){}
	///当窗口尺寸改变的时候被调用
	virtual void onSizeChange(size_t width, size_t height){}
	virtual void onClosed(void){}
};

//class WindowInterfaceImpl;
//template class _OrzFrameworkBaseExport boost::scoped_ptr<WindowInterfaceImpl>;

class _OrzFrameworkBaseExport WindowInterface
{
private:
	typedef std::vector<WindowListener *> ListenerList;
public:
	WindowInterface(void);
	virtual ~WindowInterface(void);
	
	
public :
	/** 得到窗口句柄
	*/
	virtual size_t getHandle() = 0;

	/** 得到窗口宽度
	*/
	virtual size_t getWidth() = 0;

	/** 得到窗口高度
	*/
	virtual size_t getHeight() = 0;

	///添加一个窗口监听
	void addListener(WindowListener * listener);

	///移除窗口监听
	void removeListener(WindowListener * listener);
	

protected:
	void _sizeChange(size_t width, size_t height);
	void _closed(void);
private:
	ListenerList _listeners;
};
}
#endif
