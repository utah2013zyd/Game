#ifndef __Orz_Test_TestActor_h__
#define __Orz_Test_TestActor_h__
#include <orz/Framework_Base/Logic.h>

namespace Orz
{

	class TestActor : public Orz::Actor
	{
	public:
		TestActor(const std::string & name):Actor(name)
		{}
	};

	class TestActorFactory:public ActorFactory
	{
	public:
		virtual const std::string & getTypeName() const
		{
			static const std::string name("test");
			return name;
		}
		pointer_type createInstance(const std::string& instanceName, parameter_type parameter)
		{
		
			return pointer_type(new TestActor(instanceName));
		}
	};
}


#endif