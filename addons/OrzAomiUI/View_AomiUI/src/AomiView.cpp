#include <orz/View_AomiUI/AomiView.h>
#include <Awesomium/WebCore.h>
using namespace Orz;

Awesomium::MouseButton Orz2AwButton(int button)
{
	switch(button)
	{
	case Orz::MB_Left:
		return Awesomium::LEFT_MOUSE_BTN;
	case Orz::MB_Middle:
		return Awesomium::MIDDLE_MOUSE_BTN;
	case Orz::MB_Right:
		return Awesomium::RIGHT_MOUSE_BTN;
	}
	return Awesomium::RIGHT_MOUSE_BTN;
}

void AomiView::loadFile(const std::string& file, const std::wstring& frameName)
{
	_view->loadFile(file, frameName);
	_view->setTransparent(true);
}

AomiView::AomiView(Awesomium::WebCore* core, const std::string & name, int width, int height): _name(name)
{



	_view = core->createWebView(width, height, false, 1, 90);

	hook = new KeyboardHook(this);
	
}

AomiView::~AomiView(void)
{	
	if(hook)
			delete hook;
	_view->destroy();
	



}

std::string & AomiView::getName(void)
{
	return _name;
}


void AomiView::injectMouseMove(int x, int y)
{
	_view->injectMouseMove(x, y);
}

void AomiView::injectMouseDown(MouseButtonID button)
{
	_view->injectMouseDown(Orz2AwButton(button));
}
void AomiView::injectMouseUp(MouseButtonID button)
{
	_view->injectMouseUp(Orz2AwButton(button));
}

void AomiView::injectMouseWheel(int scrollAmount)
{
	_view->injectMouseWheel(scrollAmount);
}



void AomiView::handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	_view->injectKeyboardEvent(hwnd, msg, wParam, lParam);
}