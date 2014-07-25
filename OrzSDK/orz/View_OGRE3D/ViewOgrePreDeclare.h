#ifndef __Orz_ViewOgre_PreDeclare_H_
#define __Orz_ViewOgre_PreDeclare_H_
namespace Orz
{
class OgreGraphicsManager;
class OgreWindow;
typedef  boost::shared_ptr<OgreWindow>  OgreWindowPtr;
}
namespace Ogre
{
class Camera;
class SceneManager;
class RenderWindow;
class Root;
class Viewport;
}

namespace CEGUI
{
class Renderer;
class System;
}
	
#endif