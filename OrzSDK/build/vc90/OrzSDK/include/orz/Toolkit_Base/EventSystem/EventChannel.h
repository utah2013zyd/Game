
#ifndef __Orz_ToolkitBase_Channel_h__
#define __Orz_ToolkitBase_Channel_h__

#include <orz/Toolkit_Base/ToolkitBaseConfig.h>



namespace Orz
{





///这个类型是保存频道位用的类型，如果需要可以改变成64位整数，以提供更多频道位
/// ----------------------------------------------------------------------------------
//	这里由于gcc不支持类内区域的模版特化，故只能将其变为全局 （以下到Line87） - note by Nous
/// ----------------------------------------------------------------------------------

typedef uint32 event_channel_type;

template<class T,int I = (sizeof(T)>>2)>
class ChannelLength
{
public:
	enum
	{
		Left = (I<<3),
		Right = (sizeof(T) - I)<<3,
		Length = Left + Right
	};
};


template<class T>
class ChannelLength<T, 0>
{
public:
	enum
	{
		Left = 8,
		Right = (sizeof(T) - 1)<<3,
		Length = Left + Right
	};
};

enum
{
	SystemChannelLength = ChannelLength<event_channel_type>::Left,
	UserChannelLength = ChannelLength<event_channel_type>::Right,
	AllChannelLength = ChannelLength<event_channel_type>::Length
};

/**
用户使用频道 
*/
template<int N> class UserChannel;
template<int N>
class UserChannel
{
public:
	enum{Val = UserChannel<N-1>::Val<<1};
};


template<>
class UserChannel<0>
{
public:
	enum{Val = 1<< SystemChannelLength};
};


/**
系统使用频道 
*/
template<int N>
class SystemChannel
{
public:
	enum{Val = SystemChannel<N-1>::Val<<1};
};


template<>
class SystemChannel<0>
{
public:
	enum{Val = 1};
};
///
// ----------------------------------------------------------------------------------
//	由于gcc不支持类内区域的模版特化，故只能将其变为全局 （以上从Line16开始） - note by Nous
// ----------------------------------------------------------------------------------

/**
这个EventChannel对象代表着频道的概念，是对一个简单数据event_channel_type的封装，
其保证构造函数和析构函数极其简单，没有任何一个虚函数，没有non-inline函数，并sizeof(EventChannel) == sizeof(event_channel_type)：即，你可以把这个类当成一个内置类型来使用（比如传值）;
其中把频道类型分为两种，即系统保留类型与用户使用类型，
如果使用32位event_channel_type，将会产生8位系统频道，24位用户频道（默认情况）
如果使用16位event_channel_type，将会产生8位系统频道，8位用户频道（需要重新编译）
如果使用64位event_channel_type，将会产生16位系统频道，48位用户频道（需要重新编译）

为了快速分发事件，Orz提供了“频道EventChannel”的概念，每个事件消息可以发送到几个不同的频道中去，而每个实体可以监听几个不同的频道。

*/
class EventChannel
{
public:


	
public:
	///极其简单的构造函数
	inline EventChannel(void);
	///拷贝构造函数
	inline EventChannel(const EventChannel & channel);

	///极其简单的析构函数
	inline ~EventChannel(void);
	


	///可以处理自赋值的=重载
	inline EventChannel& operator = (const EventChannel& channel);



	///提供一个绝不抛出异常的swap，可以用于异常安全函数使用
	inline void swap(EventChannel & channel) throw();//never throw

	///增加监听（发送）用户频道
	template<int N>
	inline EventChannel & addUserChannel(void);


	///增加监听（发送）系统频道
	template<int N>
	inline EventChannel & addSystemChannel(void);


	///移除监听（发送）用户频道
	template<int N>
	inline EventChannel & removeUserChannel(void);


	///移除监听（发送）系统频道
	template<int N>
	inline EventChannel & removeSystemChannel(void);

	///检查是否监听当前频道对象所包含的频道
	inline bool listening(const EventChannel & channel) const;
	

	///检查是否没有包含频道，当没有包含时，被用来作为Event内部专用频道。
	inline bool empty() const;

	///清空监听（发送）频道
	inline EventChannel & clear(void);
	///把当前频道对象所见听得频道合并到本对象中
	inline void add(const EventChannel & channel);

	///把当前对象包含的所有频道移除本对象
	inline void remove(const EventChannel & channel);

	///填满所有频道
	inline EventChannel & fill(void);

	///填满用户频道
	inline EventChannel & fillUserChannel(void);
	
	///填满系统频道
	inline EventChannel & fillSystemChannel(void);
		
	///填满用户频道
	inline EventChannel & clearUserChannel(void);

	
	///填满系统频道
	inline EventChannel & clearSystemChannel(void);

	///创建频道对象
	static inline EventChannel create(void);




	///增加监听（发送）用户频道
	inline EventChannel & addUserChannel(uint i)
	{
		_channel |= getUserChannel(i);
		return (*this);
	}	
	
	///移除监听（发送）用户频道
	inline EventChannel & removeUserChannel(uint i)
	{
		_channel &=~ getUserChannel(i);
		return (*this);
	}


private:
	inline event_channel_type getUserChannel(uint i)
	{
		return static_cast<event_channel_type>(1)<<i;
	}
	inline void init(const EventChannel & channel);
	inline void addChannel(const event_channel_type channel);
	inline void removeChannel(const event_channel_type channel);
	event_channel_type _channel;


	friend std::ostream& operator <<(::std::ostream &os, const EventChannel & channel)
	{
		for(int i =0; i<SystemChannelLength; ++i)
		{
			os<<"system["<<i<<"]:"<<static_cast</*EventChannel::*/event_channel_type>((channel._channel>>i)&1)<<std::endl;
		}

		for(int i =SystemChannelLength; i<AllChannelLength; ++i)
		{
			os<<"user["<<i - SystemChannelLength<<"]:"<<static_cast</*EventChannel::*/event_channel_type>((channel._channel>>i)&1)<<std::endl;
		}
		return os;
	}
};

EventChannel EventChannel::create(void)
{
	return EventChannel();
}

bool EventChannel::listening(const EventChannel & channel) const
{
	return (_channel & channel._channel) != 0;
}
	


bool EventChannel::empty() const
{
	return _channel == 0;
}



EventChannel & EventChannel::clear(void)
{
	_channel = 0;
	return (*this);
}

void EventChannel::add(const EventChannel & channel)
{
	addChannel(channel._channel);
}

void EventChannel::remove(const EventChannel & channel)
{
	removeChannel(channel._channel);
}

void EventChannel::addChannel(const event_channel_type channel)
{
	_channel |= channel;
}	
void EventChannel::removeChannel(const event_channel_type channel)
{
	_channel &= ~channel;
}


EventChannel::EventChannel(void):_channel(0)
{
}

EventChannel & EventChannel::fill(void)
{
	_channel = ~(static_cast<event_channel_type>(0));
	return (*this);
}


EventChannel & EventChannel::fillUserChannel(void)
{
	//fix
	fill();
	return (*this);
}


EventChannel & EventChannel::fillSystemChannel(void)
{
	//fix
	fill();
	return (*this);
}


EventChannel & EventChannel::clearUserChannel(void)
{
	//fix
	clear();
	return (*this);
}


EventChannel & EventChannel::clearSystemChannel(void)
{
	clear();
	//fix
	return (*this);
}



EventChannel::EventChannel(const EventChannel & channel)
{
	init(channel);
}

EventChannel::~EventChannel(void){};



EventChannel& EventChannel::operator = (const EventChannel& channel)
{
	init(channel);
	return (*this);
	
}
void EventChannel::init(const EventChannel & channel)
{
	_channel = channel._channel;
}


void EventChannel::swap(EventChannel & channel) throw()
{

	using std::swap;
	swap(_channel, channel._channel);
}


template<int N>
EventChannel & EventChannel::addUserChannel(void)
{

	
	BOOST_STATIC_ASSERT(N >= 0);
	BOOST_STATIC_ASSERT(N < UserChannelLength);

	addChannel(UserChannel<N>::Val);
	return (*this);
}

template<int N>
EventChannel & EventChannel::addSystemChannel(void)
{

	
	BOOST_STATIC_ASSERT(N >= 0);
	BOOST_STATIC_ASSERT(N < SystemChannelLength);

	addChannel(SystemChannel<N>::Val);
	return (*this);
}


template<int N>
EventChannel & EventChannel::removeUserChannel(void)
{

	
	BOOST_STATIC_ASSERT(N >= 0);
	BOOST_STATIC_ASSERT(N < UserChannelLength);

	removeChannel(UserChannel<N>::Val);
	return (*this);
}


template<int N>
EventChannel & EventChannel::removeSystemChannel(void)
{

	
	BOOST_STATIC_ASSERT(N >= 0);
	BOOST_STATIC_ASSERT(N < SystemChannelLength);

	removeChannel(SystemChannel<N>::Val);
	return (*this);
}


}

#endif
