#ifndef __Orz_ViewAomiUI_AomiUIManagerImpl_h__
#define __Orz_ViewAomiUI_AomiUIManagerImpl_h__

#include <orz/View_AomiUI/AomiView.h>
#include <orz/View_AomiUI/OverlayView.h>
#include <orz/View_AomiUI/ViewAomiUIConfig.h>
#include <orz/Toolkit_Base/Singleton.h>
#include <Ogre.h>
#include <orz/Framework_Base/System/IInputManager.h>
#include <Awesomium/WebCore.h>
#include <OgrePanelOverlayElement.h>


namespace Orz
{


class _OrzViewAomiUIPrivate AomiUIManagerImpl :public Orz::MouseListener
{
public:
	AomiUIManagerImpl(void);
	~AomiUIManagerImpl(void);
	bool init(void);
	void shutdown(void);
	bool update(TimeType i);
	
	//AomiView * createAomiView(const std::string & name, int width, int height);
	OverlayView * createOverlayView(const std::string & name, int width, int height);
	void destroyView(const std::string & name);


	
	void mouseEvent(bool enable)
	{
		if(_enableMouseEvent == enable)
			return;
		_enableMouseEvent = enable;
		if(_enableMouseEvent)
			Orz::IInputManager::getSingleton().addMouseListener(this);
		else
			Orz::IInputManager::getSingleton().removeMouseListener(this);
	}

	//virtual bool  keyEvent(bool enable)
	//{
	//	if(_enableKeyEvent == enable)
	//		return;
	//	_enableKeyEvent = enable;
	//	if(_enableKeyEvent)
	//		Orz::IInputManager::getSingleton().addKeyListener(this);
	//	else
	//		Orz::IInputManager::getSingleton().removeKeyListener(this);

	//}
	//

	virtual bool onMousePressed(const MouseEvent & evt);
	virtual bool onMouseReleased(const MouseEvent & evt);
	virtual bool onMouseMoved(const MouseEvent & evt);

	
private:
	//Ogre::TexturePtr _webTexture;
	
	Ogre::Viewport* _viewport;
	std::map<std::string, AomiViewPtr> _views;

	boost::scoped_ptr<Awesomium::WebCore> _core;
	//Awesomium::WebView* _view;
	
private:
	



	//void createOverlayAndMaterials(void);
	void setupAwesomium(void);
	//void resizeOverlay(int width, int height);
	void draw(void);


	
	bool _enableMouseEvent;
	//bool _enableKeyEvent;

};
}

#endif