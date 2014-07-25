//
//#ifndef __Orz_FrameworkBase_WindowInterfaceImpl_h__
//#define __Orz_FrameworkBase_WindowInterfaceImpl_h__
//#include <orz/Framework_Base/FrameworkBaseConfig.h>
//#include <orz/Framework_Base/System/WindowInterface.h>
//
//namespace Orz{
//
//
//
//class _OrzToolkitBasePrivate WindowInterfaceImpl
//{
//	typedef std::vector<WindowListener *> ListenerList;
//public:
//	WindowInterfaceImpl(void){}
//	~WindowInterfaceImpl(void){}
//	
//public :
//
//	void addListener(WindowListener * listener)
//	{
//		_listeners.push_back(listener);
//	}
//	void removeListener(WindowListener * listener)
//	{
//		_listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
//	}
//	void visitor(WindowListener::VisitorFun fun)
//	{
//		for(ListenerList::iterator it = _listeners.begin(); it != _listeners.end(); ++it)
//		{
//			fun(*it);
//		}
//	}
//
//
//private:
//	ListenerList _listeners;
//
//};
//}
//#endif
