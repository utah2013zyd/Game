#ifndef __Orz_ViewMd5Checker_Md5Checker_h__
#define __Orz_ViewMd5Checker_Md5Checker_h__

#include <orz/Toolkit_Base/Singleton.h>
namespace Orz
{
class Md5CheckerListener
{
public:
	virtual bool fail(const std::string & filename) = 0;
	virtual ~Md5CheckerListener(void){}

};
class Md5Checker// : public Singleton<Md5CheckerManager>
{
public:
	Md5Checker(void);
	~Md5Checker(void);
	bool checkerFile(const std::string & filename, char * data, Md5CheckerListener * listener = NULL);

};
}



#endif