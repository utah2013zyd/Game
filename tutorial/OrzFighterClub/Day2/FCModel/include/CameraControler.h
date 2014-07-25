#include "FCConfig.h"

namespace Orz
{
class CameraControler :public KeyListener, public MouseListener
{
public:
	CameraControler(Ogre::Camera * camera);
	virtual ~CameraControler(void);
	bool onMousePressed(const MouseEvent & evt);
	bool onMouseReleased(const MouseEvent & evt);
	bool onMouseMoved(const MouseEvent & evt);

	bool onKeyPressed(const KeyEvent & evt);
	bool onKeyReleased(const KeyEvent & evt);
	void update(void);
private:
	void log(void);
	Ogre::Camera * _camera;
	Ogre::Vector3 _translateVector;
	Ogre::Radian _rotX, _rotY;
	float _moveScale;
};

}
