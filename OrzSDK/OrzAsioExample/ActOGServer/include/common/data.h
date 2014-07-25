#ifndef DATA_H_
#define DATA_H_
#include <orz/OrzAsio/OrzAsioConfig.h>
class data
{
public:
	data()
	{
	}

	template <typename D>
	data(const D& _data)
		: m_data(boost::any(_data))
	{
	}
	
	~data() 
	{
	}

	template <typename D>
	D* get_data()
	{
		return boost::any_cast<D>(&m_data);
	}

	template <typename D>
	const D* get_const_data() const
	{
		return boost::any_cast<D>(&m_data);
	}

private:
	boost::any			m_data;
};

#endif
