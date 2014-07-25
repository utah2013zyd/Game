#ifndef __Orz_FrameworkBase_Theater_h__
#define __Orz_FrameworkBase_Theater_h__
#include <orz/Framework_Base/FrameworkBaseConfig.h>

#include <orz/Framework_Base/Logic/Entity.h>

namespace Orz{
class TheaterImpl;
template class _OrzFrameworkBaseExport boost::scoped_ptr<TheaterImpl>;
class _OrzFrameworkBaseExport Theater :public Entity
{
public:
	Theater(const std::string & name);
	virtual ~Theater(void);
	void addDirector(const std::string & type, const std::string & name, NameValueList * parameter = NULL);
	void activeDirector(const std::string & name);
protected:
	virtual void doAddDirector(const std::string & type, const std::string & name, NameValueList * parameter = NULL);
	virtual void doActiveDirector(DirectorPtr director);
	boost::scoped_ptr<TheaterImpl> _impl;
};

}
#endif