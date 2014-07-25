#include <orz/Toolkit_JSBinding/JSBinding.h>
#include <orz/Toolkit_Base/LogManager.h>
using namespace Orz;

JSBinding::JSBinding(void):_face(NULL)
{
	
}

JSBinding::~JSBinding(void)
{
	
}


void JSBinding::enable(ScriptInterface * face)
{
	_face = face;
}


void JSBinding::disable(void)
{
	_face = NULL;
}

void JSBinding::onBeginNavigation(const std::string& url, const std::wstring& frameName)
{
	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onBeginNavigation");
	}
}
void JSBinding::onBeginLoading(const std::string& url, const std::wstring& frameName, int statusCode, const std::wstring& mimeType)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onBeginLoading");
	}
}
void JSBinding::onFinishLoading()
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onFinishLoading");
	}
}
void JSBinding::onCallback(const std::string& name, const Awesomium::JSArguments& args)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onCallback");
	}
}
void JSBinding::onReceiveTitle(const std::wstring& title, const std::wstring& frameName)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onBeginNavigation");
	}
}
void JSBinding::onChangeTooltip(const std::wstring& tooltip)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onChangeTooltip");
	}
}

void JSBinding::onChangeKeyboardFocus(bool isFocused)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onChangeKeyboardFocus");
	}
}
void JSBinding::onChangeTargetURL(const std::string& url)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onChangeTargetURL");
	}
}
#if defined(_WIN32)
	
void JSBinding::onChangeCursor(const HCURSOR& cursor)
{

	if(Orz::ILogManager::getSingletonPtr())
	{
		ILogManager::getSingleton().utf8("onChangeCursor");
	}
}
#endif

