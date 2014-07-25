#include "Model.h"

namespace Orz
{

	class ExampleActor : public Actor//, private FSMLogic<GameLogic, TheaterBase>
	{
	public:
		ExampleActor(const std::string & name = IDManager::BLANK);
		virtual ~ExampleActor(void);
		virtual void doEnable(void);
		virtual void doDisable(void);
	private:

	};
	
	class ExampleActorFactory: public IFactory<Actor>
	{
	public:

		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName, parameter_type parameter);
	};
};