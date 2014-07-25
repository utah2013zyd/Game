//
//
//#ifndef __Orz_FrameworkBase_IInputManagerImpl_h__
//#define __Orz_FrameworkBase_IInputManagerImpl_h__
//#include <orz/Framework_Base/FrameworkBaseConfig.h>
//#include <orz/Framework_Base/System/IInputManager.h>
//
//
//namespace Orz{
//
//class _OrzToolkitBasePrivate IInputManagerImpl
//{
//
//public:
//
//
//	IInputManagerImpl(void){}
//	~IInputManagerImpl(void){}
//
//
//
//public:
//	void addMouseListener(MouseListener * listener)
//	{
//		_mouseListener.push_back(listener);
//	}
//	void removeMouseListener(MouseListener * listener)
//	{
//		_mouseListener.erase(std::remove(_mouseListener.begin(), _mouseListener.end(), listener), _mouseListener.end());
//	}
//	void addKeyListener(KeyListener * listener)
//	{
//		_keyListener.push_back(listener);
//	}
//	void removeKeyListener(KeyListener * listener)
//	{
//		_keyListener.erase(std::remove(_keyListener.begin(), _keyListener.end(), listener), _keyListener.end());
//	
//	}
//	
//
//	void visitor(IInputManager::MouseVisitorFun fun)
//	{
//		for(MouseListenerList::iterator it = _mouseListener.begin(); it != _mouseListener.end(); ++it)
//		{
//			fun(*it);
//		}
//	}
//
//	void visitor(IInputManager::KeyVisitorFun fun)
//	{
//		for(KeyListenerList::iterator it = _keyListener.begin(); it != _keyListener.end(); ++it)
//		{
//			fun(*it);
//		}
//	}
//private:
//	MouseListenerList _mouseListener;
//	KeyListenerList _keyListener;
//};
//
//
//}
//#endif
//
//
