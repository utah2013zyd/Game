#ifndef __Orz_TriggerConditions_h__
#define __Orz_TriggerConditions_h__

#include <orz/Toolkit_Trigger/ToolkitTriggerConfig.h>
namespace Orz
{
class _OrzToolkitTriggerExport TriggerConditions
{

	
public:
	/*virtual void enable(void) = 0;
	virtual void disable(void) = 0;*/
	virtual bool operator()(Event * evt) = 0;
	virtual ~TriggerConditions(void){}
};

class _OrzToolkitTriggerExport TriggerConditionsProxy: public TriggerConditions
{
private:
	typedef boost::function< bool ( Event * ) > TriggerFunctor;
public:
	TriggerConditionsProxy(const TriggerFunctor & functor):_functor(functor){}
	//virtual void enable(void){}
	//virtual void disable(void){}
	virtual bool operator()(Event * evt){return _functor(evt);}
	virtual ~TriggerConditionsProxy(void){}
private:
	TriggerFunctor _functor;
};

	
typedef boost::shared_ptr<TriggerConditions>  TriggerConditionsPtr;

}


#endif