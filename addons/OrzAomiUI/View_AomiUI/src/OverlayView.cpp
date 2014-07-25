#include <orz/View_AomiUI/OverlayView.h>

#include <orz/Framework_Base/System/SystemInterface.h>
#include <OgrePanelOverlayElement.h>

#include <Awesomium/WebCore.h>
using namespace Orz;



void OverlayView::createTexture(void)
{
	_texName = _name+"_texture";
	_webTexture = Ogre::TextureManager::getSingleton().createManual(
		_texName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::TEX_TYPE_2D, _width, _height, 0, Ogre::PF_BYTE_BGRA,
		Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);

	Ogre::HardwarePixelBufferSharedPtr pixelBuffer = _webTexture->getBuffer();
	pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
	size_t dstBpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
	size_t dstPitch = pixelBox.rowPitch * dstBpp;

	Ogre::uint8* dstData = static_cast<Ogre::uint8*>(pixelBox.data);
	memset(dstData, 255, dstPitch * _height);
	pixelBuffer->unlock();
}

void OverlayView::draw(void)
{
	if(!_view->isDirty())
		return;

	Ogre::HardwarePixelBufferSharedPtr pixelBuffer = _webTexture->getBuffer();
	pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
	size_t dstBpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);
	size_t dstPitch = pixelBox.rowPitch * dstBpp;

	Ogre::uint8* dstData = static_cast<Ogre::uint8*>(pixelBox.data);
	
	_view->render(dstData, (int)dstPitch, (int)dstBpp, 0);

	pixelBuffer->unlock();
}
void OverlayView::destroyTexture(void)
{
	
	Ogre::TextureManager::getSingleton().remove(Ogre::ResourcePtr(_webTexture));
	_webTexture.setNull();

}


void OverlayView::onSizeChange(size_t width, size_t height)
{
	if(width == 0 || height == 0)
		return;

	_newScaleWidth = float(_width)/float(width);
	_newScaleHeight  = float(_height)/float(height);
	

	//_webTexture = Ogre::TextureManager::getSingleton().createManual(
	//_texName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	//Ogre::TEX_TYPE_2D, _width, _height, 0, Ogre::PF_BYTE_BGRA,
	//Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);



	_panel->setDimensions(width, height);/**/
		

}
OverlayView::OverlayView(Awesomium::WebCore* core, const std::string & name, int width, int height):AomiView(core, name, width, height)
{

	
	if(Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->getNonPOW2TexturesLimited())
	{
		std::cerr << "Error! Rendering device has limited/no support for NPOT textures, falling back to 512x512 dimensions.\n";
		_width = _height = 512;
	}
	else
	{
		_width = width;
		_height = height;
	}
	
	createTexture();
	createOverlay();
	_newScaleWidth = 1.f;
	_newScaleHeight = 1.f;
	if(SystemInterface::getSingleton().hasParame("WINDOW"))
	{
		WeakWindowPtr win;
		try
		{
			win = SystemInterface::getSingleton().getParame<WeakWindowPtr>("WINDOW");

		}
		catch(...)
		{
			std::cout<<"OverlayView::OverlayView(Awesomium::WebCore* core, const std::string & name, int wigth, int height):AomiView(core, name, wigth, height) error"<<std::endl;
		}
		WindowPtr wp;
		if(wp = win.lock())
		{
			wp->addListener(this);
		}
		
		
	}
	
}
	
void OverlayView::injectMouseMove(int x, int y)
{
	AomiView::injectMouseMove(x* _newScaleWidth, y* _newScaleHeight);
}
OverlayView::~OverlayView(void)
{

		
	if(SystemInterface::getSingleton().hasParame("WINDOW"))
	{
		WeakWindowPtr win;
		try
		{
			win = SystemInterface::getSingleton().getParame<WeakWindowPtr>("WINDOW");

		}
		catch(...)
		{
			std::cout<<"OverlayView::OverlayView(Awesomium::WebCore* core, const std::string & name, int wigth, int height):AomiView(core, name, wigth, height) error"<<std::endl;
		}
		WindowPtr wp;
		if(wp = win.lock())
		{
			wp->removeListener(this);
		}
		
		
	}
	destroyOverlay();
	
	destroyTexture();

}
void OverlayView::createOverlay(void)
{

	
	//_planeMaterialName = _name +"PlaneMaterial";
	static int i =0;
	_materialName =  _name +"Material";

	_material = Ogre::MaterialManager::getSingleton().create(_materialName, 
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::Pass* pass = _material->getTechnique(0)->getPass(0);
	pass->setDepthCheckEnabled(false);
	pass->setDepthWriteEnabled(false);
	pass->setLightingEnabled(false);
	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	Ogre::TextureUnitState* texUnit = pass->createTextureUnitState(_texName);
	texUnit->setTextureFiltering(Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);

//	// Create Plane Material
//	material = Ogre::MaterialManager::getSingleton().create(_planeMaterialName, 
//		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
//	pass = material->getTechnique(0)->getPass(0);
//	pass->setLightingEnabled(false);
//	pass->setSceneBlending(Ogre::SBT_REPLACE);
//	pass->setCullingMode(Ogre::CULL_NONE);
//
//	texUnit = pass->createTextureUnitState(_texName);
//	texUnit->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//	texUnit->setTextureAnisotropy(4);
//#if SUPER_QUALITY
//	texUnit->setTextureAnisotropy(16);
//#endif

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	_panel = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement("Panel", _name+"Panel"));
	_panel->setMetricsMode(Ogre::GMM_PIXELS);
	_panel->setMaterialName(_materialName);
	_panel->setDimensions(_width, _height);
	_panel->setPosition(0, 0);


	_overlay = overlayManager.create(_name+"webOverlay");
	_overlay->add2D(_panel);
	_overlay->show();

}


void OverlayView::show(void)
{
	_overlay->show();
}
void OverlayView::hide(void)
{
	_overlay->hide();
}
void OverlayView::destroyOverlay(void)
{	

	Ogre::MaterialManager::getSingleton().remove(Ogre::ResourcePtr(_material));


	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	overlayManager.destroyOverlayElement(_panel);
	overlayManager.destroy(_overlay);
}