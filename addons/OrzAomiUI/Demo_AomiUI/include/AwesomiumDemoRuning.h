#ifndef __Orz_OrzAwesomiumDemo_AwesomiumDemoRuning_h__ 
#define __Orz_OrzAwesomiumDemo_AwesomiumDemoRuning_h__

#include <Ogre/Ogre.h>
#include <orz/View_OGRE3D/OgreGraphicsManager.h>
#include <orz/View_AomiUI/AomiUIManager.h>

#include <Awesomium/WebCore.h>
namespace Orz
{

class AwesomiumDemoRuning: public Awesomium::WebViewListener
{
public:
	AwesomiumDemoRuning(void)
	{
		Ogre::Viewport * vp = OgreGraphicsManager::getSingleton().getCamera()->getViewport();
		_view  = AomiUIManager::getSingleton().createOverlayView("test", vp->getActualWidth(),  vp->getActualHeight()); 
		_view->loadFile("EnterScene.html");
		_view->getWebView()->setCallback("Button");

		_view->getWebView()->setListener(this);
		

		/*_view  = AomiUIManager::getSingleton().createOverlayView("test", vp->getActualWidth(),  vp->getActualHeight()); 
		_view->loadFile("SelectScene.html");
		_view->getWebView()->setCallback("Button"); 

		_view->getWebView()->setListener(this);
		*/
	}
	~AwesomiumDemoRuning(void)
	{
		AomiUIManager::getSingleton().destroyView("test");
	}
	bool update(void)
	{
		return true;
	}
	virtual void onBeginNavigation(const std::string& url, const std::wstring& frameName){}
	virtual void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType){}
	virtual void onFinishLoading(){}
	virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args)
	{

		if(args.at(0).isInteger())
		{
			std::cout<<args.at(0).toInteger()<<std::endl;
		}
		std::cout<<"virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args)"<<std::endl;
	}
	virtual void onReceiveTitle(const std::wstring& title, const std::wstring& frameName){}
	virtual void onChangeTooltip(const std::wstring& tooltip){}

#if defined(_WIN32)
	virtual void onChangeCursor(const HCURSOR& cursor)
	{
	}
#endif
	virtual void onChangeKeyboardFocus(bool isFocused)
	{
	}
	virtual void onChangeTargetURL(const std::string& url)
	{}
private:

	OverlayView * _view;
};
}

#endif