
#include <orz/View_SingleChip/SingleChipManager.h>
#include "SingleChipManagerImpl.h"
using namespace Orz;



template<> SingleChipManager* Singleton<SingleChipManager>::_singleton = NULL;


SingleChipManager::SingleChipManager(void):_impl(new SingleChipManagerImpl())
{

}
SingleChipManager::~SingleChipManager(void)
{

}
bool SingleChipManager::init(void)
{
	
	

	/*
	
	
	using namespace boost::asio; 
	_iosev.reset(new boost::asio::io_service());
	_sp.reset(new boost::asio::serial_port(*_iosev, "COM1"));
	_sp->set_option(boost::asio::serial_port::baud_rate(19200)); 
    _sp->set_option(boost::asio::serial_port::flow_control(serial_port::flow_control::none)); 
    _sp->set_option(boost::asio::serial_port::parity(serial_port::parity::none)); 
    _sp->set_option(boost::asio::serial_port::stop_bits(serial_port::stop_bits::one)); 
    _sp->set_option(boost::asio::serial_port::character_size(8)); 
	
	*/


	return _impl->init();
}
void SingleChipManager::shutdown(void)
{

	_impl->shutdown();
	
}



bool SingleChipManager::write(const MsgBuffer & buffer)
{
	return _impl->write(buffer);
}


bool SingleChipManager::write(unsigned char * data, int num)
{
	return _impl->write(data, num);
}

bool SingleChipManager::write(char * data, int num)
{
	return _impl->write(data, num);

}

bool SingleChipManager::update(TimeType i)
{
	return _impl->update(i);
}
void SingleChipManager::addListener(SingleChipListener * listener)
{
	_impl->addListener(listener);
}
void SingleChipManager::removeListener(SingleChipListener * listener)
{
	_impl->removeListener(listener);
}
