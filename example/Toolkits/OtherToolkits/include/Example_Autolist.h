#ifndef __Orz_Example_Autolist__
#define __Orz_Example_Autolist__
#include <orz/Toolkit_Base/Autolist.h>

namespace Orz
{

class Foo2: public AutoList<Foo2>
{
public:
	Foo2(const std::string & text):_text(text)
	{
		link();
	}
	~Foo2(void)
	{
		unlink();
	}

	void print(void)
	{
		std::cout<<"class Foo2:"<<_text<<std::endl;
	}
private:
	std::string _text;
};


inline void Example_Autolist(void)
{
	
	Foo2 * foo2_1 = new Foo2("foo2_1");
	Foo2 * foo2_2 = new Foo2("foo2_2");
	Foo2 * foo2_3 = new Foo2("foo2_3");
	//Example_Single();
	{
		
		for(Foo2::Iterator it = Foo2::begin(); it!=Foo2::end(); ++it)
		{
			(*it)->print();
		}

	}
	delete(foo2_1);
	delete(foo2_2);
	delete(foo2_3);


}


}
#endif