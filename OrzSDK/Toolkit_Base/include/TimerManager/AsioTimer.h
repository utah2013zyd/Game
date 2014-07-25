#ifndef __Orz_ToolkitBase_AsioTimer_h__
#define __Orz_ToolkitBase_AsioTimer_h__

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <orz/Toolkit_Base/ToolkitBaseConfig.h>
#include <orz/Toolkit_Base/TimerManager/Timer.h>
#include <orz/Toolkit_Base/AutoList.h>
#include <orz/Toolkit_Base/Exception.h>
namespace Orz{
	class _OrzToolkitBasePrivate AsioTimer:  public Timer, public AutoList<AsioTimer>
	{
	public:

		///开始定时器运行，参数分别为调用间隔、调用次数、开始时间
		virtual bool start(TimeType interval, uint32 times = 0, TimeType start = 0)
		{
			_interval = boost::posix_time::milliseconds(static_cast<uint32>(interval * 1000.f));
			_times = times;

			_other = boost::posix_time::milliseconds(0);

			_asio_timer->expires_from_now(boost::posix_time::milliseconds(static_cast<uint32>(start * 1000.f)));

			_asio_timer->async_wait(boost::bind(&AsioTimer::update, this));
			_enable = true;
			return true;
		}



		//强制停止定时器运行
		virtual void stop(void)
		{
			if(_asio_timer)
				_asio_timer->cancel();
			_enable = false;
		}


		AsioTimer(const TimerFunctorReturnBool & fun, boost::asio::io_service & io):_interval(),_other(),_times(0),_fun(fun),_asio_timer(new boost::asio::deadline_timer(io))
		{
			link();
		}
		virtual ~AsioTimer(void)
		{
			unlink();
			close();
		}

		virtual void setSpeed(float speed) 
		{

		}

		///得到运行速率
		virtual float getSpeed(void) const
		{
			return 1.0;
		}

		void update(void)
		{
			if(_enable)
			{
				boost::posix_time::ptime now = boost::asio::time_traits<boost::posix_time::ptime >::now();
				boost::posix_time::ptime expires = _asio_timer->expires_at();
				boost::posix_time::ptime next = _asio_timer->expires_at();


				for(_other += (now - expires); _other >= _interval && _interval> boost::posix_time::milliseconds(0); _other -= _interval)
				{
					if(!_fun())
						return;
					next += _interval;
					if(_times != 0)
					{
						--_times;
						if(_times == 0)
						{
							stop();
							return;
						}
					}
				}
				_asio_timer->expires_at(next);
				_asio_timer->async_wait(boost::bind(&AsioTimer::update, this));

			}
		}
		void close()
		{
			stop();
			_asio_timer.reset();
		}

	private:

		boost::posix_time::time_duration _interval;
		boost::posix_time::time_duration _other;
		uint32 _times;
		TimerFunctorReturnBool _fun;
		boost::scoped_ptr<boost::asio::deadline_timer> _asio_timer;

	};

	template<> AutoList<AsioTimer>::List AutoList<AsioTimer>::_list;

}
#endif
