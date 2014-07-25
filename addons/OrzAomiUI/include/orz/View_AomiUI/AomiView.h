#ifndef __Orz_ViewAomiUI_AomiView_h__
#define __Orz_ViewAomiUI_AomiView_h__

#include <orz/View_AomiUI/ViewAomiUIConfig.h> 
#include <orz/Toolkit_Base/Singleton.h>
#include <orz/Framework_Base/System/IInputManager.h>
#include <Ogre/Ogre.h>

#include <orz/View_AomiUI/KeyboardHook.h>

namespace Awesomium
{
	class WebCore;
	class WebView;
}
namespace Orz
{


class _OrzViewAomiUIExport AomiView :public HookListener
{

public:

	AomiView(Awesomium::WebCore* core, const std::string & name, int width, int height);
	
	virtual ~AomiView(void);
	virtual void draw(void) = 0;
	std::string & getName(void);
	
	/**
	* Loads a local file into the WebView asynchronously.
	*
	* @param	file	The file to load.
	*
	* @param	frameName	Optional, the name of the frame to load the file in; leave this blank to load in the main frame.
	*
	* @note	The file should exist within the base directory (specified via WebCore::setBaseDirectory).
	*/
	void loadFile(const std::string& file, const std::wstring& frameName = L"");


	virtual void injectMouseMove(int x, int y);

	virtual void injectMouseDown(MouseButtonID button);

	virtual void injectMouseUp(MouseButtonID button);

	virtual void injectMouseWheel(int scrollAmount);

	void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	

	inline Awesomium::WebView * getWebView(void){return _view;}
protected:

	std::string _name; 
	Awesomium::WebView * _view;
	//bin.wang
	KeyboardHook* hook;
};



typedef boost::shared_ptr<AomiView> AomiViewPtr;


}

#endif