
#ifndef __Orz_ToolkitBase_Singleton_h__
#define __Orz_ToolkitBase_Singleton_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>


namespace Orz{

template<typename T> class Singleton: private boost::noncopyable
{
protected:
	static T * _singleton;
public:
	Singleton(void)
	{
		assert( !_singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200
        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
        _singleton = (T*)((int)this + offset);
#else
	    _singleton = static_cast< T* >( this );
#endif
	}


	virtual ~Singleton(void)
	{
		assert(_singleton);
		_singleton = NULL;
	}

	///得到单件引用
	static T& getSingleton(void)
	{
		assert(_singleton);
		return(*_singleton);
	}

	///得到单件指针
	static T* getSingletonPtr(void)
	{
		return (_singleton);
	}

};
}
#endif

