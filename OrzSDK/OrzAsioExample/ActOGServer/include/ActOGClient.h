#ifndef __Orz_ActOGServer_ActOGClient_h__
#define __Orz_ActOGServer_ActOGClient_h__
#include <IClient.h>
#include <common/ActOGServerPreDeclare.h>
#include <packet/packet_seer.h>
#include <common/account/account.h>
#include <common/game/game.h>
namespace Orz
{
class ActOGClient
	: public IClient
{
public:
	ActOGClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy, 
				game_sr_game_controller& _gs_game_ctrl, Idr& _actIdr);
	virtual ~ActOGClient();

public:
	void						logoff							();
	int32						process_packet					(IPacketShrPtr _pk);

private:
	//------------------------------------------------------------------------------------------
	//! process input
	int32						process_input					(IPacketShrPtr _pk);
	//!
	int32						process_input_failed			(int32 _error);
	//------------------------------------------------------------------------------------------

private:
	packet_seer					m_packet_seer;
	game_sr_game_controller&	m_gs_game_ctrl;
	Idr&						m_actIdr;
	account_ptr					m_act;
	game_ptr					m_game;

	arg_data					m_posx;
	arg_data					m_posy;
};
}
#endif
