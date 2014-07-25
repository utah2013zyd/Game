#include "Model.h"

namespace Orz
{

	class ExampleDirector : public Director
	{
	public:
		ExampleDirector(const std::string & name = IDManager::BLANK);
		virtual ~ExampleDirector(void);
		virtual void doEnable(void);
		virtual void doDisable(void);
	private:

	};
	
	class ExampleDirectorFactory: public IFactory<Director>
	{
	public:

		virtual const std::string & getTypeName() const;
		virtual pointer_type createInstance(const std::string& instanceName, parameter_type parameter);
	};
};