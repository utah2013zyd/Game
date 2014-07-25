#ifndef __Orz_ActOGServer_ActOGServerPreDeclare_H_
#define __Orz_ActOGServer_ActOGServerPreDeclare_H_
#include <orz/OrzAsio/NetEngine/SocketPreDeclare.h>
namespace Orz
{
	class ActOGClient;
	typedef boost::shared_ptr<ActOGClient> ActOGClientShrPtr;

	class game_sr_game_controller;
	class Idr;
}
#endif
