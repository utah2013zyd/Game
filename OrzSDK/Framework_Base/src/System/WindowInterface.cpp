
#include <orz/Framework_Base/System/WindowInterface.h>
//#include "System/WindowInterfaceImpl.h"

using namespace Orz;

WindowInterface::WindowInterface(void)//:_impl(new WindowInterfaceImpl())
{

}
WindowInterface::~WindowInterface(void)
{

}
void WindowInterface::addListener(WindowListener * listener)
{
	_listeners.push_back(listener);
}
void WindowInterface::removeListener(WindowListener * listener)
{
	_listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
}


//void visitor(WindowListener::VisitorFun fun);


//boost::scoped_ptr<WindowInterfaceImpl> _impl;
void WindowInterface::_sizeChange(size_t width, size_t height)
{
	BOOST_FOREACH(WindowListener * listener, _listeners)
	{
		listener->onSizeChange(width, height);
	}
}

void WindowInterface::_closed(void)
{
	BOOST_FOREACH(WindowListener * listener, _listeners)
	{
		listener->onClosed();
	}
}
//
//void WindowInterface::addListener(WindowListener * listener)
//{
//	_impl->addListener(listener);
//}
//void WindowInterface::removeListener(WindowListener * listener)
//{
//	_impl->removeListener(listener);
//}
//
//
//void  WindowInterface::visitor(WindowListener::VisitorFun fun)
//{
//	_impl->visitor(fun);
//}