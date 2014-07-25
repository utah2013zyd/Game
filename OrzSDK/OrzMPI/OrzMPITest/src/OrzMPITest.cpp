#include <orz/Toolkit_Plus/Toolkit/Arithmetic.hpp>
#include <orz/Toolkit_Plus/Log/LogSingleton.h>
#include <orz/Toolkit_Plus/Toolkit/OrzAsioException.hpp>
#include <orz/Toolkit_Plus/Toolkit/FreeList.hpp>
#include <orz/Toolkit_Plus/Plugin/MTPluginMgr.h>
#include <orz/OrzAsio/NetEngine/Packet.hpp>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
namespace mpi = boost::mpi;
using namespace Orz;


int main(int argc, char* argv[])
{
	mpi::environment env(argc, argv);
	mpi::communicator world;
	std::cout << "I am process " << world.rank() << " of " << world.size()
			<< "." << std::endl;
	return 0;
}

