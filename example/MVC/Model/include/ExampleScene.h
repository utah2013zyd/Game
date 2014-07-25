#include "Model.h"

namespace Orz
{

	class ExampleScene : public Scene
	{
	public:
		ExampleScene(const std::string & name = IDManager::BLANK);
		virtual ~ExampleScene(void);
		virtual void doEnable(void);
		virtual void doDisable(void);
	private:

	};
	
	class ExampleSceneFactory: public IFactory<Scene>
	{
	public:

		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName, parameter_type parameter);
	};
};