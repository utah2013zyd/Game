
#include <orz/View_SingleChip/ViewSingleChipConfig.h>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <orz/Framework_Base/System.h>
#include <orz/View_SingleChip/SingleChipManager.h>
#include <boost/circular_buffer.hpp>
#include <orz/Toolkit_Base/LogManager.h>
namespace Orz
{

class _OrzViewSingleChipPrivate SingleChipManagerImpl//:public SingleChipListener
{
private:
	typedef boost::mutex::scoped_lock lock;
	/*virtual void read(boost::circular_buffer<unsigned char> & data)
	{
		while(!data.empty())
		{
			std::cout<<"!!!"<<int(data.front());
			data.pop_front();
		}
		std::cout<<std::endl;
	}*/
	void _writeToBuffer(unsigned char * c, boost::system::error_code ec, 
    std::size_t bytes_transferred)
	{

		if(!ec)
		{
			//std::cout<<int(c[0])<<std::endl;
			if(!_threadOver)
			{
				{
					
					while(_cbuffer.full())
					{
						std::cout<<"_cbuffer.full()"<<std::endl;
					}

					{
						lock lk(_monitor);
						_cbuffer.push_back(c[0]);
					}
				}


				boost::asio::async_read(*_sp, boost::asio::buffer(_buf), boost::bind(&SingleChipManagerImpl::_writeToBuffer, this, _buf, _1, _2)); 
			}
		}
	}
	void _readFromBuffer(void)
	{
		
		
		if(_cbuffer.empty())
			return;
		{
			lock lk(_monitor);
			std::vector<SingleChipListener *>::iterator it;
			for(it =_listeners.begin(); it != _listeners.end(); ++it)
			{
				(*it)->read(_cbuffer);
				
			}
			_cbuffer.clear();
		}
	}

	
public:

	SingleChipManagerImpl(void):_cbuffer(128)
	{
		//addListener(this);
	}

	~SingleChipManagerImpl(void)
	{
	
	}
	void _read(void)
	{
	
		
	    boost::asio::async_read(*_sp, boost::asio::buffer(_buf), boost::bind(&SingleChipManagerImpl::_writeToBuffer, this, _buf, _1, _2)); 
	
		_iosev.run();
	}
	bool init(void)
	{
		std::string port("COM1");
		unsigned int rate = 19200;
		if(Orz::SystemInterface::getSingleton().hasParame("SingleChip_Port"))
		{
			port = SystemInterface::getSingleton().getParame<std::string>("SingleChip_Port");

			
		}
	
		
		if(Orz::SystemInterface::getSingleton().hasParame("SingleChip_BaudRate"))
		{
			rate = SystemInterface::getSingleton().getParame<int>("SingleChip_BaudRate");
		}

		try{
			_sp.reset(new boost::asio::serial_port(_iosev, port));
		}
		catch(...)
		{
			_sp.reset();
			Orz::ILogManager::getSingleton().utf8("can't create SingleChip Manager");
		}
		if(_sp)
		{
			_sp->set_option(boost::asio::serial_port::baud_rate(rate));
			_sp->set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none)); 
			_sp->set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none)); 
			_sp->set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one)); 
			_sp->set_option(boost::asio::serial_port::character_size(8)); 

			
			// 向串口写数据 
			_threadOver = false;
			_thread.reset(new boost::thread(boost::bind(&SingleChipManagerImpl::_read, this)));

		}
		return true;
	}
	void shutdown(void)
	{

		
		if(_sp)
		{
			_threadOver = true;
			_sp->close();
			//_sp->cancel();
			
			
			_thread->join();
			_thread.reset();
		}
		_sp.reset();
	}



	bool write(unsigned char * data, int num)
	{
		if(_sp)
		{
			boost::asio::write(*_sp, boost::asio::buffer(data, num)); 
			return true;
		}
		return false;
	}

	bool write(const MsgBuffer & buffer)
	{
		if(_sp)
		{
			boost::asio::write(*_sp, boost::asio::buffer(buffer, buffer.size())); 
			return true;
		}
		return false;
	}
	bool write(char * data, int num)
	{
		if(_sp)
		{
			boost::asio::write(*_sp, boost::asio::buffer(data, num)); 
			return true;
		}
		return false;
	}
	bool update(TimeType i)
	{
		_readFromBuffer();
		return true;
	}

	void addListener(SingleChipListener * listener)
	{
		_listeners.push_back(listener);
	}
	void removeListener(SingleChipListener * listener)
	{
		_listeners.erase(std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end());
	}

private:
	boost::scoped_ptr<boost::thread> _thread;
	bool _threadOver;
	boost::scoped_ptr<boost::asio::serial_port> _sp;
	boost::asio::io_service _iosev;
	std::vector<SingleChipListener *> _listeners;
	boost::circular_buffer<unsigned char> _cbuffer;
	boost::mutex _monitor;
	unsigned char _buf[1]; 

};


}