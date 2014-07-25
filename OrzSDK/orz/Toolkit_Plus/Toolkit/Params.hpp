#ifndef __Orz_ToolkitPlus_Params_hpp__
#define __Orz_ToolkitPlus_Params_hpp__
#include <orz/Toolkit_Plus/GlobalPlus.h>
namespace Orz
{
//! 用于传递不定数目参数的时候使用。std::vector版本
/*!
 *	\note 最多传递20个参数
 example:
 \code
	// fun
	void fun(const Params<std::string>& _attrs)
	{
		const std::vector<std::string*>& attrs = _attrs.getParamList();
		if (attrs.empty())
			return;
		size_t size = attrs.size();
		for (unsigned i=0; i<size; ++i)
		{
			if (attrs[i])
			{
				//...do something, user code
			}
		}
	}

	// outside code use fun
	{
		...
		fun(Params<std::string>(&std::string("id"), &std::string("="), &std::string("1")));
		...
	}
 \endcode
*/
template <typename T>
class Params
{
public:
	ORZ_PLUS_INLINE
	Params()
	{
	}

	ORZ_PLUS_INLINE
	Params(T* _p1)
	{
		m_paramList.push_back(_p1);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15, T* _p16)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
		m_paramList.push_back(_p16);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15, T* _p16, T* _p17)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
		m_paramList.push_back(_p16);
		m_paramList.push_back(_p17);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15, T* _p16, T* _p17, T* _p18)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
		m_paramList.push_back(_p16);
		m_paramList.push_back(_p17);
		m_paramList.push_back(_p18);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15, T* _p16, T* _p17, T* _p18, T* _p19)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
		m_paramList.push_back(_p16);
		m_paramList.push_back(_p17);
		m_paramList.push_back(_p18);
		m_paramList.push_back(_p19);
	}

	ORZ_PLUS_INLINE
	Params(T* _p1, T* _p2, T* _p3, T* _p4, T* _p5, T* _p6, T* _p7, T* _p8, T* _p9, T* _p10, T* _p11, T* _p12, T* _p13, T* _p14, T* _p15, T* _p16, T* _p17, T* _p18, T* _p19, T* _p20)
	{
		m_paramList.push_back(_p1);
		m_paramList.push_back(_p2);
		m_paramList.push_back(_p3);
		m_paramList.push_back(_p4);
		m_paramList.push_back(_p5);
		m_paramList.push_back(_p6);
		m_paramList.push_back(_p7);
		m_paramList.push_back(_p8);
		m_paramList.push_back(_p9);
		m_paramList.push_back(_p10);
		m_paramList.push_back(_p11);
		m_paramList.push_back(_p12);
		m_paramList.push_back(_p13);
		m_paramList.push_back(_p14);
		m_paramList.push_back(_p15);
		m_paramList.push_back(_p16);
		m_paramList.push_back(_p17);
		m_paramList.push_back(_p18);
		m_paramList.push_back(_p19);
		m_paramList.push_back(_p20);
	}

	ORZ_PLUS_INLINE
	~Params()
	{
	}

	ORZ_PLUS_INLINE
	const std::vector<T*>& getParamList() const
	{
		return m_paramList;
	}

private:
	std::vector<T*>		m_paramList;
};

}

#endif
