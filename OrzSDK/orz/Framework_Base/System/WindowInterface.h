
#ifndef __Orz_FrameworkBase_WindowInterface_h__
#define __Orz_FrameworkBase_WindowInterface_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

namespace Orz{



class _OrzFrameworkBaseExport WindowListener
{
public:
	virtual ~WindowListener(void){}
	///�����ڳߴ�ı��ʱ�򱻵���
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
	/** �õ����ھ��
	*/
	virtual size_t getHandle() = 0;

	/** �õ����ڿ��
	*/
	virtual size_t getWidth() = 0;

	/** �õ����ڸ߶�
	*/
	virtual size_t getHeight() = 0;

	///���һ�����ڼ���
	void addListener(WindowListener * listener);

	///�Ƴ����ڼ���
	void removeListener(WindowListener * listener);
	

protected:
	void _sizeChange(size_t width, size_t height);
	void _closed(void);
private:
	ListenerList _listeners;
};
}
#endif
