
#include "FCConfig.h"
#include "FCFighter.h"
namespace Orz
{


class CameraController /*:public KeyListener, public MouseListener*/
{
public:
	CameraController(Ogre::Camera * camera);
	virtual ~CameraController(void);
	//bool onMousePressed(const MouseEvent & evt);
	//bool onMouseReleased(const MouseEvent & evt);
	//bool onMouseMoved(const MouseEvent & evt);

	//bool onKeyPressed(const KeyEvent & evt);
	//bool onKeyReleased(const KeyEvent & evt);
	void update(void);
private:
	void log(void);
	Ogre::Camera * _camera;
	Ogre::SceneNode * _node;
	Ogre::Vector3 _translateVector;
	Ogre::Radian _rotX, _rotY;
	float _moveScale;
};

}
