
#ifndef __Orz_TestWebkit_h__
#define __Orz_TestWebkit_h__


#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_AomiUI/AomiUIManager.h>

#include <orz/View_AomiUI/Awesomium/WebCore.h>

namespace Orz
{
class TestWebkit
{
public:
	
	
	TestWebkit(const std::string & webfile)
	{
		Ogre::Viewport * vp = OgreGraphicsManager::getSingleton().getCamera()->getViewport();
		_view  = AomiUIManager::getSingleton().createOverlayView(webfile, vp->getActualWidth(),  vp->getActualHeight()); 
		_view->loadFile(webfile);
		_view->getWebView()->setCallback("Button");

		//_view->getWebView()->setListener(this);
		

		/*_view  = AomiUIManager::getSingleton().createOverlayView("test", vp->getActualWidth(),  vp->getActualHeight()); 
		_view->loadFile("SelectScene.html");
		_view->getWebView()->setCallback("Button"); 

		_view->getWebView()->setListener(this);
		*/
	}
	void executeJavascript(const std::string& javascript, const std::wstring& frameName = L"")
	{
		_view->getWebView()->executeJavascript(javascript, frameName);
	}


	Awesomium::FutureJSValue executeJavascriptWithResult(const std::string& javascript, const std::wstring& frameName = L"")
	{
		_view->getWebView()->executeJavascript(javascript, frameName);
	}

	void setProperty(const std::string& name, const Awesomium::JSValue& value)	
	{
		_view->getWebView()->setProperty(name, value);
	}


	void setListener(Awesomium::WebViewListener* listener)
	{
		_view->getWebView()->setListener(listener);
	}
	~TestWebkit(void)
	{
		AomiUIManager::getSingleton().destroyView(_view->getName());
	}
	
private:

	OverlayView * _view;
};

}

#endif