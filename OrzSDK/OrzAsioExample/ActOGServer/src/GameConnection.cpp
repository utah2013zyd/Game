#include <GameConnection.h>
#include <orz/OrzAsio/AsioSingleton.h>
#include <orz/OrzAsio/NetEngine/ServiceManager.h>
#include <orz/OrzAsio/NetEngine/PacketFreeList.h>
#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <ActOGClient.h>
#include <packet/packet_builder.h>
using namespace Orz;

GameConnection::GameConnection(ServiceManager* _svcMgr, uint32 _maxInc)
	: IConnection(_maxInc)
	, m_svcMgr(_svcMgr)
	, m_gs_game_ctrl(_svcMgr, m_gsc.ggc)
	, m_actIdr(20, 1000)
	, m_dt(0)
{
}

GameConnection::~GameConnection()
{
}

int32 GameConnection::load_config(const char* _conf)
{
	if (!_conf)
		return 1;

	FILE *fp = fopen(_conf, "r");
	if (fp == NULL)
	{
		printf("[ERROR]GameConnection::load_config - Unable to open %s\n", _conf);
		return 1;
	}
	
	char map_config_path[64];
	char pc_config_path[64];
	char msr_config_path[64];

	fscanf(fp, "MAX_GAME_NUM %d\n",						&m_gsc.ggc.max_game_num);
	fscanf(fp, "GAME_MAX_PLAYER %d\n",					&m_gsc.ggc.dgc.max_player);
	fscanf(fp, "DT %ld\n",								&m_dt);
	fscanf(fp, "FRAME_TIME %u\n",						&m_gsc.ggc.dgc.frame_time);
	fscanf(fp, "G %lf\n",								&m_gsc.ggc.dgc.g);
	fscanf(fp, "F_MAX_ATK_Y %u\n",						&m_gsc.ggc.dgc.f_max_atk_y);
	fscanf(fp, "MAP_CONFIG_PATH %s\n",					map_config_path);
	fscanf(fp, "PC_CONFIG_PATH %s\n",					pc_config_path);
	fscanf(fp, "MSR_CONFIG_PATH %s\n",					msr_config_path);
	fclose(fp);

	if (load_map_config(map_config_path) != 0)
	{
		fclose(fp);
		return 1;
	}

	if (load_pc_config(pc_config_path) != 0)
	{
		fclose(fp);
		return 1;
	}

	if (load_monster_config(msr_config_path) != 0)
	{
		fclose(fp);
		return 1;
	}
	
	printf("m_dt: %d\n", m_dt);

	m_gsc.run_fps = 1000000 / m_dt;
	m_gsc.ggc.dgc.run_fps = m_gsc.run_fps;
	m_gsc.ggc.dgc.dpc.run_fps = m_gsc.run_fps;
	m_gsc.ggc.dgc.dpc.frame_time = m_gsc.ggc.dgc.frame_time;
	m_gsc.ggc.dgc.dpc.g = m_gsc.ggc.dgc.g;
	m_gsc.ggc.dgc.dpc.f_max_atk_y = m_gsc.ggc.dgc.f_max_atk_y;

	m_gsc.ggc.dgc.dmc.run_fps = m_gsc.run_fps;
	m_gsc.ggc.dgc.dmc.client_fps = m_gsc.ggc.dgc.dpc.client_fps;
	m_gsc.ggc.dgc.dmc.frame_time = m_gsc.ggc.dgc.frame_time;
	m_gsc.ggc.dgc.dmc.g = m_gsc.ggc.dgc.g;
	m_gsc.ggc.dgc.dmc.f_max_atk_y = m_gsc.ggc.dgc.f_max_atk_y;

	m_gsc.ggc.dgc.mmc.client_fps = m_gsc.ggc.dgc.dpc.client_fps;

	return 0;
}

void GameConnection::shutdown()
{
	m_gs_game_ctrl.shutdown();
	sAsioMgr->shutdown();
}

void GameConnection::OnConnected(const SessionPxy& _snPxy, SessionDeputy& _snDy, bool _isInc)
{
	printf("New session - snid: [%d], [%s]\n", _snPxy.getSnid(), _isInc ? "in":"out");
	_snDy.setClient(IClientShrPtr(new ActOGClient(m_svcMgr, _snPxy, m_gs_game_ctrl, m_actIdr)));
}

void GameConnection::OnReceived(const SessionPxy& _snPxy, SessionDeputy& _snDy, IPacketShrPtr _pk)
{
	//printf("Session [%d, %d], port: [%d] - recv data!\n", _snPxy.getClrId(), _snPxy.getSnId(), _snPxy.getNetPort());
	if (!_pk)
		return;

	IClientShrPtr cl = _snDy.getClient();
	if (!cl)
		return;

	cl->process_packet(_pk);
}

void GameConnection::OnSended(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
}

void GameConnection::OnDisconnected(const SessionPxy& _snPxy, SessionDeputy& _snDy)
{
	_snDy.getClient()->logoff();
	_snDy.clearClient();
	printf("Session disconnected - snid: [%d]\n", _snPxy.getSnid());
}

int32 GameConnection::load_map_config(const char* _file)
{
	if (!_file)
		return 1;

	printf("_config: %s\n", _file);

	FILE* fp = fopen(_file, "r");
	if (fp == NULL)
	{
		printf("[ERROR]game_server::load_map_config - Unable to open %s\n", _file);
		return 1;
	}

	fscanf(fp, "BP1_X %lf\n",							&m_gsc.ggc.dgc.bornPoint1.x);
	fscanf(fp, "BP1_Y %lf\n",							&m_gsc.ggc.dgc.bornPoint1.y);
	fscanf(fp, "BP1_Z %lf\n",							&m_gsc.ggc.dgc.bornPoint1.z);
	fscanf(fp, "BP2_X %lf\n",							&m_gsc.ggc.dgc.bornPoint2.x);
	fscanf(fp, "BP2_Y %lf\n",							&m_gsc.ggc.dgc.bornPoint2.y);
	fscanf(fp, "BP2_Z %lf\n",							&m_gsc.ggc.dgc.bornPoint2.z);
	fscanf(fp, "SIZE_X %d\n",							&m_gsc.ggc.dgc.dmapc.size_x);
	fscanf(fp, "SIZE_Y %d\n",							&m_gsc.ggc.dgc.dmapc.size_y);
	fscanf(fp, "GAME_ZONE_POS_X %d\n",					&m_gsc.ggc.dgc.dmapc.game_zone.x);
	fscanf(fp, "GAME_ZONE_POS_Y %d\n",					&m_gsc.ggc.dgc.dmapc.game_zone.y);
	fscanf(fp, "GAME_ZONE_W %u\n",						&m_gsc.ggc.dgc.dmapc.game_zone.w);
	fscanf(fp, "GAME_ZONE_H %u\n",						&m_gsc.ggc.dgc.dmapc.game_zone.h);
	fscanf(fp, "BLOCK_ZONE_NUM %u\n",					&m_gsc.ggc.dgc.dmapc.block_zone_num);
	for (unsigned i=0; i<m_gsc.ggc.dgc.dmapc.block_zone_num; ++i)
	{
		cfg_zone bz;
		fscanf(fp, "BZ_X %d\n",							&bz.x);
		fscanf(fp, "BZ_Y %d\n",							&bz.y);
		fscanf(fp, "BZ_W %u\n",							&bz.w);
		fscanf(fp, "BZ_H %u\n",							&bz.h);
		m_gsc.ggc.dgc.dmapc.block_zones.push_back(bz);
	}
	
	fclose(fp);

	std::cout << "load map config finished!" << std::endl;

	return 0;
}

int32 GameConnection::load_pc_config(const char* _file)
{
	if (!_file)
		return 1;

	printf("_config: %s\n", _file);

	FILE* fp = fopen(_file, "r");
	if (fp == NULL)
	{
		printf("[ERROR]game_server::load_pc_config - Unable to open %s\n", _file);
		return 1;
	}

	fscanf(fp, "RAW_COL_BOX_W %u\n",					&m_gsc.ggc.dgc.dpc.raw_collision_box.w);
	fscanf(fp, "RAW_COL_BOX_H %u\n",					&m_gsc.ggc.dgc.dpc.raw_collision_box.h);
	fscanf(fp, "MIN_MOVE_DISTANCE %lf\n",				&m_gsc.ggc.dgc.dpc.min_move_distance);
	fscanf(fp, "MOVE_SPEED %lf\n",						&m_gsc.ggc.dgc.dpc.move_speed);
	fscanf(fp, "MIN_RUN_DISTANCE %lf\n",				&m_gsc.ggc.dgc.dpc.min_run_distance);
	fscanf(fp, "RUN_SPEED %lf\n",						&m_gsc.ggc.dgc.dpc.run_speed);
	fscanf(fp, "JUMP_H %lf\n",							&m_gsc.ggc.dgc.dpc.jump_high);
	fscanf(fp, "CLIENT_FPS %u\n",						&m_gsc.ggc.dgc.dpc.client_fps);
	fscanf(fp, "SKILL_ACT_TIME %lf\n",					&m_gsc.ggc.dgc.dpc.skill_act_time);
	fscanf(fp, "ATTACK1_DMG %d\n",						&m_gsc.ggc.dgc.dpc.attack1_dmg);
	fscanf(fp, "ATTACK2_DMG %d\n",						&m_gsc.ggc.dgc.dpc.attack2_dmg);
	fscanf(fp, "ATTACK3_DMG %d\n",						&m_gsc.ggc.dgc.dpc.attack3_dmg);
	fscanf(fp, "SKT10_DMG %d\n",						&m_gsc.ggc.dgc.dpc.skt10_dmg);
	fscanf(fp, "SKT13_DMG %d\n",						&m_gsc.ggc.dgc.dpc.skt13_dmg);

	for (unsigned i=0; i<AFSPCT_TOTAL; ++i)
	{
		fscanf(fp, "#\n");
		std::string erase_str;
		erase_str += g_apcts[i];
		std::vector<cfg_key_frame> ckfs;
		m_gsc.ggc.dgc.dpc.cfg_key_frames.push_back(ckfs);
		load_key_frames(fp, erase_str.c_str(), m_gsc.ggc.dgc.dpc.cfg_key_frames[i]);
	}
	
	fclose(fp);

	std::cout << "load pc config finished!" << std::endl;

	return 0;
}

int32 GameConnection::load_monster_config(const char* _file)
{
	if (!_file)
		return 1;

	printf("_config: %s\n", _file);

	FILE* fp = fopen(_file, "r");
	if (fp == NULL)
	{
		printf("[ERROR]game_server::load_monster_config - Unable to open %s\n", _file);
		return 1;
	}

	fscanf(fp, "RAW_COL_BOX_W %u\n",					&m_gsc.ggc.dgc.dmc.raw_collision_box.w);
	fscanf(fp, "RAW_COL_BOX_H %u\n",					&m_gsc.ggc.dgc.dmc.raw_collision_box.h);
	fscanf(fp, "MIN_MOVE_DISTANCE %lf\n",				&m_gsc.ggc.dgc.dmc.min_move_distance);
	fscanf(fp, "MOVE_SPEED %lf\n",						&m_gsc.ggc.dgc.dmc.move_speed);
	fscanf(fp, "ATTACK1_ACT_TIME %lf\n",				&m_gsc.ggc.dgc.dmc.attack1_act_time);
	fscanf(fp, "GUARD_MOVE_INTERVAL_TIME %lf\n",		&m_gsc.ggc.dgc.mmc.guard_move_interval_time);
	fscanf(fp, "SHORT_SIDE_MOVE_TIME %lf\n",			&m_gsc.ggc.dgc.mmc.short_side_move_time);
	fscanf(fp, "LONG_SIDE_MOVE_TIME %lf\n",				&m_gsc.ggc.dgc.mmc.long_side_move_time);

	//for (unsigned i=0; i<AFSMSRT_TOTAL; ++i)
	//{
	//	fscanf(fp, "#\n");
	//	std::string erase_str;
	//	erase_str += g_amsrts[i];
	//	std::vector<cfg_key_frame> ckfs;
	//	m_gsc.ggc.dgc.dmc.cfg_key_frames.push_back(ckfs);
	//	load_key_frames(fp, erase_str.c_str(), m_gsc.ggc.dgc.dmc.cfg_key_frames[i]);
	//}
	fclose(fp);

	std::cout << "load monster config finished!" << std::endl;

	return 0;
}
