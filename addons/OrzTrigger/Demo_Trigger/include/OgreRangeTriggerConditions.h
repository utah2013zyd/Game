#include <orz/Toolkit_Trigger/TriggerConditions.h>


namespace Orz
{
class  OgreRangeTriggerConditions: public TriggerConditions
{
public:
	OgreRangeTriggerConditions(const Ogre::Vector3  & position, Ogre::SceneNode * node):
	_position(position),_node(node),_in(false)
	{}
	~OgreRangeTriggerConditions(){}
	void enable(void){}
	void disable(void){}
	bool operator()(Event * evt)
	{
		bool in = _node->getPosition().distance(_position) < 5.f;
		if(in && !_in)
		{
			_in = true;
			return true;
		}
		if(!in && _in)
		{
			_in = false;
		}
		return false;
			
	}
	
private:
	const Ogre::Vector3 _position;
	Ogre::SceneNode * _node;
	bool _in;
}; 

}