#ifndef __Orz_ViewAomiUI_OverlayView_h__
#define __Orz_ViewAomiUI_OverlayView_h__

#include <orz/View_AomiUI/ViewAomiUIConfig.h>
#include <orz/View_AomiUI/AomiView.h>
#include <orz/Framework_Base/System/WindowInterface.h>
namespace Ogre
{
	class PanelOverlayElement;
}
namespace Orz
{
class _OrzViewAomiUIExport OverlayView:public AomiView, public WindowListener, private boost::noncopyable
{

public:

	OverlayView(Awesomium::WebCore* core, const std::string & name, int width, int height);
	virtual void onSizeChange(size_t width, size_t height);
	~OverlayView(void);
	
	virtual void injectMouseMove(int x, int y);
	virtual void draw(void);
private:

	
	void createTexture(void);
	void destroyTexture(void);
	void createOverlay(void);
	void destroyOverlay(void);
	void show(void);
	void hide(void);

	Ogre::PanelOverlayElement* _panel;
	Ogre::Overlay* _overlay;

	
	float _newScaleWidth;
	float _newScaleHeight; 
	Ogre::String _planeMaterialName;
	Ogre::String _materialName;
	Ogre::MaterialPtr _material;

	
	
	int _width, _height; 
	Ogre::TexturePtr _webTexture;
	
	std::string _texName;

};

typedef boost::shared_ptr<OverlayView> OverlayViewPtr;
}

#endif