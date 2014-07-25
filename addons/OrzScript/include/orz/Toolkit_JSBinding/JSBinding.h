#ifndef __Orz_ToolkitJSBinding_JSBinding_h__
#define __Orz_ToolkitJSBinding_JSBinding_h__

#include "ToolkitJSBindingConfig.h"
#include <orz/Toolkit_Base/EventSystem.h>


#include <orz/View_AomiUI/Awesomium/WebCore.h>
namespace Orz
{
class ScriptInterface;
class  _ToolkitJSBindingExport JSBinding: public Awesomium::WebViewListener
{
	
public:
	JSBinding(void);
	~JSBinding(void);
	void enable(ScriptInterface * face);
	void disable(void);

private:
	virtual void onBeginNavigation(const std::string& url, const std::wstring& frameName);
	virtual void onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType);
	virtual void onFinishLoading();
	virtual void onCallback(const std::string& name, const Awesomium::JSArguments& args);
	virtual void onReceiveTitle(const std::wstring& title, const std::wstring& frameName);
	virtual void onChangeTooltip(const std::wstring& tooltip);

	virtual void onChangeKeyboardFocus(bool isFocused);
	virtual void onChangeTargetURL(const std::string& url);
#if defined(_WIN32)
	
	virtual void onChangeCursor(const HCURSOR& cursor);
#endif


private:

	ScriptInterface * _face;
};

}

#endif