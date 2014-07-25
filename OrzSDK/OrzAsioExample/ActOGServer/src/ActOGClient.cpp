#include <ActOGClient.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <game_server/game_sr_game_controller.h>
#include <common/Idr.h>
#include <game_server/game_account.h>
using namespace Orz;

ActOGClient::ActOGClient(ServiceManager* _svcMgr, const SessionPxy& _snPxy, 
						 game_sr_game_controller& _gs_game_ctrl, Idr& _actIdr)
	: IClient(_svcMgr, _snPxy)
	, m_gs_game_ctrl(_gs_game_ctrl)
	, m_actIdr(_actIdr)
	, m_act(new game_account(m_actIdr.generate(), sn(), "pc", "pc"))
	, m_game(m_gs_game_ctrl.join_game(m_act, _snPxy, GT_DEMO1))
{
	if (!m_game)
		svcmgr()->kick(sn(), true);
}

ActOGClient::~ActOGClient()
{
}

void ActOGClient::logoff()
{
	BoostLock lock(m_shr_mutex);
	if (m_act)
	{
		if (m_game)
		{
			game_id gid = m_game->get_id();
			m_game->leave(sn());
			m_gs_game_ctrl.leave_game(gid, sn());
			m_game = game_ptr();
		}
		m_actIdr.release(m_act->get_id());
		m_act = account_ptr();
	}
}

int32 ActOGClient::process_packet(IPacketShrPtr _pk)
{
	BoostLock lock(m_shr_mutex);
	if (check_packet(_pk) != 0)
		return 1;

	Args args_list;
	m_packet_seer.analyse_packet(_pk, args_list, g_spts);
	std::vector<Arg>& args = args_list.argList;
	if (args.empty())
		return 1;

	m_posx.clear();
	m_posx = args[args.size()-2].arg;
	m_posy.clear();
	m_posy = args[args.size()-1].arg;

	m_game->updatePosition(sn(), s2i(m_posx.c_str()), s2i(m_posy.c_str()));
	m_game->broadcast(_pk, &sn());

	msg_id type = (msg_id)_pk->getMsg();	

	switch (type)
	{
	case M_INPUT:		process_input(_pk);		break;
	default:	
		{ 
			printf("[ERROR]ActOGClient::process_packet - no matched msg found!\n", 0);	
			return 1; 
		}break;
	}

	return 0;
}

int32 ActOGClient::process_input(IPacketShrPtr _pk)
{
	Args args_list;
	m_packet_seer.analyse_packet(_pk, args_list, g_spts);
	std::vector<Arg>& args = args_list.argList;

	if (args.size() < 1)
		return 1;

	if (!m_game)
		return 1;

	player_id prid = m_game->get_player_id_by_sn(sn());
	int it = 0;
	try
	{
		it = boost::lexical_cast<int>(args[0].arg.c_str());
	}
	catch(...)
	{
		std::cout << "[Catch!]recv client's input msg: " << args[0].arg.c_str() << std::endl;
		return 0;
	}
	//std::cout << "recv client's input msg: " << _input.c_str() << std::endl;

	m_game->set_input(prid, it);

	//m_gs_game_ctrl.process_input(sn(), args[0].arg);

	return 0;
}

int32 ActOGClient::process_input_failed(int32 _error)
{
	return 0;
}
