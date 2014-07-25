//////////////////////////////////////////////////////////////
//
//      FileName: game_server.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-12-07
//          Time: 16:05:00
//   Description: 所有类型的游戏副本都在这个server中
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-12-07  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef GAME_SERVER_H_
#define GAME_SERVER_H_

#include <common/framer.h>
#include <server.h>
#include <game_server/game_sr_account_controller.h>
#include <game_server/game_sr_game_controller.h>


namespace CoreEngine
{


class game_server
{
public:
	game_server();
	~game_server();

public:
	//! 读取关于游戏服务器的一些配置参数
	int								load_config							(const char* _conf);

	//! get listen client port
	net_port						get_listen_client_port				() const;

	//!
	std::string						get_ip								() const;

	//!
	unsigned						get_io_num							() const;

	//!
	unsigned						get_hdr_num							() const;

	//!
	unsigned						get_run_num							() const;

	//!
	unsigned						get_max_client						() const;

	//! get loop dt
	clock_time						get_dt								() const;

	//!
	void							set_server							(server& _sr);

public:
	//!
	void							client_accept_handler				(session_proxy& _sn, std::vector<packet>& _pkts);

	//!
	void							client_recv_handler					(session_proxy& _sn, std::vector<packet>& _pkts);

	//!
	void							client_error_handler				(session_proxy& _sn, std::vector<packet>& _pkts);

public:
	//!
	void							run_handler							();

private:
	//! load map
	int								load_map_config						(const char* _file);

	//! load pc
	int								load_pc_config						(const char* _file);

	//! load monster
	int								load_monster_config					(const char* _file);

	//! check packet 返回error code 0 合格 >1 error code
	err_pkt							check_packet						(packet& _pkt);

	//------------------------------------------------------------------------------------------
	//! process input
	int								process_input						(session_proxy& _sn, packet& _pkt);
	//!
	int								process_input_failed				(session_proxy& _sn, int _error);
	//------------------------------------------------------------------------------------------


	//! sever manager
	server*																m_server;

	//!
	packet_seer															m_packet_seer;

	//!
	game_sr_account_controller											m_gs_account_ctrl;

	//!
	game_sr_game_controller												m_gs_game_ctrl;

	//! framer
	framer																m_framer;

	clock_time															m_dt;

	clock_time															m_ct;
	unsigned long														m_nums;

	//! config
	game_server_config													m_gsc;

	//! config attrs
	char																m_ip[32];
	//! 处理io事件的线程数量
	unsigned															m_io_num;
	//! 处理hdr的线程数量
	unsigned															m_hdr_num;
	//! 处理run hdr的线程数量
	unsigned															m_run_num;
	//!
	unsigned															m_max_client;
	
};

}

#endif
