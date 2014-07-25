#ifndef ACTIONFRAMEQUEUE_H_
#define ACTIONFRAMEQUEUE_H_

#include <orz/OrzAsio/OrzAsioConfig.h>
#include <common/game/rect.h>
#include <common/static_any.h>
#include <orz/Toolkit_Plus/Toolkit/WeakTimer.hpp>
#include <game_server/gs.h>

#define FSLCDS 64

namespace Orz
{

class frame_segment;
class action_frame_sequence;
class action_frame_sequence_manager;

//!
class action_frame
{
public:
	//! 
	action_frame(action_frame_sequence& _afs, unsigned int _index, const std::string& _label, const array2d& _dpos, unsigned int _frame_time, unsigned int _unit_frame_time, 
				 bool _has_iz, const zone& _iz, bool _has_az, const zone& _az, unsigned int _attack_y)
		: m_afs(&_afs)
		, m_is_start(false)
		, m_index(_index)
		, m_label(_label)
		, m_dpos(_dpos)
		, m_loop_curr_time(now())
		, m_start_time(now())
		, m_frame_time(_frame_time)
		, m_unit_frame_time(_unit_frame_time)
		, m_has_iz(_has_iz)
		, m_injure_zone(_iz)
		, m_has_az(_has_az)
		, m_attack_zone(_az)
		, m_attack_y(_attack_y)
	{
		//printf("action_frame::action_frame - " << "_index: " << _index << ", _label: " << _label << ", _dpos.x: " << _dpos.x << ", _dpos.y: " << _dpos.y
		//	<< ", _frame_time: " << _frame_time << ", _unit_frame_time: " << _unit_frame_time << ", _iz.x: " << _iz.x << ", _az.x: " << _az.x << ", _attack_y: " << _attack_y << std::endl;
	}

	~action_frame()
	{
	}

public:
	//!
	bool								operator==					(const std::string& _label) const
	{
		if (_label == m_label)
			return true;
		else
			return false;
	}

public:

	//!
	ORZ_PLUS_INLINE unsigned int					get_index					() const
	{
		return m_index;
	}

	//!
	ORZ_PLUS_INLINE const std::string&			get_label					() const
	{
		return m_label;
	}

	//!
	ORZ_PLUS_INLINE const array2d&				get_dpos					() const
	{
		return m_dpos;
	}

	//!_unit_frame_time: 默认为40ms
	ORZ_PLUS_INLINE unsigned int					get_frame_time				() const
	{
		return m_frame_time * m_unit_frame_time;
	}

	//!
	ORZ_PLUS_INLINE unsigned int					get_ft_num					() const
	{
		return m_frame_time;
	}

	//!
	ORZ_PLUS_INLINE unsigned int					get_unit_ft					() const
	{
		return m_unit_frame_time;
	}

	//!
	ORZ_PLUS_INLINE bool							has_injure_zone				() const
	{
		return m_has_iz;
	}

	//!
	ORZ_PLUS_INLINE const zone&					get_injure_zone				() const
	{
		return m_injure_zone;
	}

	//!
	ORZ_PLUS_INLINE bool							has_attack_zone				() const
	{
		return m_has_az;
	}

	//!
	ORZ_PLUS_INLINE const zone&					get_attack_zone				() const
	{
		return m_attack_zone;
	}

	//!
	ORZ_PLUS_INLINE unsigned int					get_attack_y				() const
	{
		return m_attack_y;
	}

	//!
	ORZ_PLUS_INLINE void							run							()
	{
	}

private:
	action_frame_sequence*									m_afs;

	bool													m_is_start;

	unsigned int											m_index;
	//!
	std::string												m_label;
	//!
	array2d													m_dpos;

	ClockTime												m_loop_curr_time;
	ClockTime												m_start_time;

	//! 有几个单位关键针时间
	unsigned int											m_frame_time;
	//! 单位关键针时间
	unsigned int											m_unit_frame_time;

	//! 
	bool													m_has_iz;
	zone													m_injure_zone;
	//!
	bool													m_has_az;
	zone													m_attack_zone;

	unsigned int											m_attack_y;

	WeakTimer												m_timer;
};

//!
class frame_segment
{
public:
	frame_segment(const std::string& _label, action_frame_sequence& _afs, action_frame_sequence_manager& _afs_mgr, 
				  const boost::function<void (const std::string&, action_frame_sequence&)>& _fs_start_handler,
				  const boost::function<void (const action_frame&, frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64)>& _fs_loop_handler, 
				  const boost::function<void (frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64, bool)>& _fs_finish_handler)
		: m_label(_label)
		, m_afs(&_afs)
		, m_afs_mgr(&_afs_mgr)
		, m_start_time(now())
		, m_is_start(false)
		, m_current_af(0)
		, m_life_cycle(0)
		, m_curr_af_lc(0)
		, m_fs_start_handler(_fs_start_handler)
		, m_fs_loop_handler(_fs_loop_handler)
		, m_fs_finish_handler(_fs_finish_handler)
	{
	}
	~frame_segment()
	{
	}

public:
	//!
	bool								operator==					(const std::string& _label) const
	{
		if (_label == m_label)
			return true;
		else
			return false;
	}

public:
	//!
	ORZ_PLUS_INLINE int32							add_af						(const action_frame& _af)
	{
		m_action_frames.push_back(_af);
		m_life_cycle += _af.get_frame_time();
		return 0;
	}

	//!
	ORZ_PLUS_INLINE unsigned						get_size					() const
	{
		return (unsigned int)m_action_frames.size();
	}

	//!
	ORZ_PLUS_INLINE const std::string&			get_label					() const
	{
		return m_label;
	}

	//!
	ORZ_PLUS_INLINE void							start						(const static_any<FSLCDS>& _lcd)
	{
		m_start_time = now();
		m_current_af = 0;
		m_is_start = true;

		if (!check_run())
		{
			m_is_start = false;
			printf("frame_segment::start - !check_run() \n");
			return;
		}

		m_timer.setDuration((ClockTime)m_life_cycle);
		m_timer.start(boost::bind(&frame_segment::loop_handler, this, _1, _2, _3), boost::bind(&frame_segment::finish_handler, this, _1, _2, _3));

		m_life_cycle_data = _lcd;
		m_is_start = true;
		m_curr_af_lc = m_action_frames.front().get_frame_time();
		m_fs_start_handler(m_label, *m_afs);
	}

	//!
	ORZ_PLUS_INLINE void							stop						()
	{
		if (!check_run())
		{
			//printf("frame_segment::stop - !check_run()!\n");
			return;
		}

		m_timer.stop();
		m_is_start = false;
	}

	//!
	ORZ_PLUS_INLINE bool							is_start					() const
	{
		return m_is_start;
	}

	//!
	ORZ_PLUS_INLINE void							run							()
	{
		if (!check_run())
		{
			printf("frame_segment::run!\n");
			return;
		}

		m_timer.run();
	}

	ORZ_PLUS_INLINE const action_frame*			get_current_af				() const
	{
		if (!check_run())
			return 0;

		return &m_action_frames[m_current_af];
	}

	//!
	ORZ_PLUS_INLINE void							loop_handler				(ClockTime _duration_time, ClockTime _past_time, ClockTime _dt)
	{
		float64 past_time = (float64)_past_time/1000000.0f;
		float64 dt = (float64)_dt/1000000.0f;

		if (m_current_af < m_action_frames.size())
			af_loop_handler(m_action_frames[m_current_af], *m_afs, past_time, dt);
		if (_past_time >= m_curr_af_lc)
			next_action_frame(past_time, dt);
	}
	//! 
	ORZ_PLUS_INLINE void							finish_handler				(ClockTime _past_time, ClockTime _dt, bool _is_break)
	{
		m_is_start = false;

		float64 past_time = (float64)_past_time/1000000.0f;
		float64 dt = (float64)_dt/1000000.0f;

		af_finish_handler(m_action_frames.back(), *m_afs, past_time, dt, _is_break);
	}

	//!
	ORZ_PLUS_INLINE const static_any<FSLCDS>*	get_lcd						() const
	{
		return &m_life_cycle_data;
	}

private:
	//!
	ORZ_PLUS_INLINE void							af_loop_handler				(const action_frame& _af, action_frame_sequence& _afs, float64 _past_time, float64 _dt)
	{
		m_fs_loop_handler(_af, *this, _afs, *m_afs_mgr, _past_time, _dt);		
	}

	//! one action_frame finished
	ORZ_PLUS_INLINE void							af_finish_handler			(const action_frame& _af, action_frame_sequence& _afs, float64 _past_time, float64 _dt, bool _is_break)
	{
		m_is_start = false;
		m_fs_finish_handler(*this, _afs, *m_afs_mgr, _past_time, _dt, _is_break);
	}
	
	//!
	ORZ_PLUS_INLINE void							next_action_frame			(float64 _past_time, float64 _dt)
	{
		++m_current_af;
		if (check_run())
			m_curr_af_lc += m_action_frames[m_current_af].get_frame_time();
	}

	//!
	ORZ_PLUS_INLINE bool							check_run					() const
	{
		if (!m_is_start)
		{
		//	printf("frame_segment::check_run### [ 1 ] ! label: " << m_label << std::endl;
			return false;
		}

		if (m_action_frames.empty())
		{
		//	printf("frame_segment::check_run### [ 2 ] ! label: " << m_label << std::endl;
			return false;
		}

		if (m_current_af >= m_action_frames.size())
		{
		//	printf("frame_segment::check_run### [ 3 ] ! label: " << m_label << std::endl;
			return false;
		}

		return true;
	}

private:
	std::string							m_label;
	action_frame_sequence*				m_afs;
	action_frame_sequence_manager*		m_afs_mgr;
	std::vector<action_frame>			m_action_frames;

	//!
	ClockTime							m_start_time;

	bool								m_is_start;

	//! 当前的action_frame
	unsigned int						m_current_af;

	WeakTimer							m_timer;
	unsigned int						m_life_cycle;
	//! 当前af的生存时间
	ClockTime							m_curr_af_lc;

	//!
	static_any<FSLCDS>					m_life_cycle_data;

	boost::function<void (const std::string&, action_frame_sequence&)> 
										m_fs_start_handler;
	boost::function<void (const action_frame&, frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64)>
										m_fs_loop_handler;
	boost::function<void (frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64, bool)>
										m_fs_finish_handler;
};

typedef unsigned int afs_id;

//! 
class action_frame_sequence
{
public:
	action_frame_sequence(action_frame_sequence_manager& _afs_mgr, afs_id _afsid, 
						  const boost::function<void (const std::string&, action_frame_sequence&)>& _fs_start_handler,
						  const boost::function<void (const std::string&, action_frame_sequence&, const std::string&)>& _change_handler,
						  const boost::function<void (const action_frame&, frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64)>& _fs_loop_handler, 
						  const boost::function<void (frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64, bool)>& _fs_finish_handler)
		: m_afs_mgr(&_afs_mgr)
		, m_afsid(_afsid)
		, m_current_fs_label(null_label)
		, m_current_fs(0)
		, m_fs_start_handler(_fs_start_handler)
		, m_change_handler(_change_handler)
		, m_fs_loop_handler(_fs_loop_handler)
		, m_fs_finish_handler(_fs_finish_handler)
	{
	}
	~action_frame_sequence()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE afs_id					get_id							() const
	{
		return m_afsid;
	}

	//!
	bool							operator==						(const afs_id _id) const
	{
		if (m_afsid == _id)
			return true;
		else
			return false;
	}

	//!
	ORZ_PLUS_INLINE const static_any<FSLCDS>*	get_fs_lcd						(const std::string& _label) const
	{
		std::vector<frame_segment>::const_iterator itr(std::find(m_frame_segments.begin(), m_frame_segments.end(), _label));
		if (itr == m_frame_segments.end())
		{
			printf("action_frame_sequence::get_fs_lcd - itr == m_frame_segments.end()\n");
			return 0;
		}

		return (*itr).get_lcd();
	}

	//!
	ORZ_PLUS_INLINE int32						create_action_frame			
	(
		const std::string& _label, const array2d& _dpos, unsigned int _frame_time, unsigned int _unit_frame_time, 
		bool _has_iz, const zone& _iz, bool _has_az, const zone& _az,  unsigned int _attack_y
	)
	{
		unsigned af_num = 0;
		std::vector<frame_segment>::iterator itr(m_frame_segments.begin());
		for (; itr!=m_frame_segments.end(); ++itr)
		{
			af_num += (*itr).get_size();
		}

		itr = std::find(m_frame_segments.begin(), m_frame_segments.end(), _label);
		if (itr == m_frame_segments.end())
			return 1;
		else
		{
			action_frame af(*this, af_num, _label, _dpos, _frame_time, _unit_frame_time, _has_iz, _iz, _has_az, _az, _attack_y);
			(*itr).add_af(af);
		}

		return 0;
	}

public:
	//! < 0 无限次 
	ORZ_PLUS_INLINE void						start						(const std::string& _label, const static_any<FSLCDS>& _lcd=static_any<FSLCDS>())
	{
		std::vector<frame_segment>::iterator itr(std::find(m_frame_segments.begin(), m_frame_segments.end(), _label));
		if (itr == m_frame_segments.end())
		{
			printf("action_frame_sequence::start - itr == m_frame_segments.end()\n");
			return;
		}

		m_current_fs = &(*itr);
		m_current_fs_label = (*itr).get_label();
		(*itr).start(_lcd);
	}

	//!
	ORZ_PLUS_INLINE void						stop						()
	{
		if (check_run())
		{
			//printf("action_frame_sequence::stop!\n");
			m_current_fs->stop();
		}
	}

	//!
	ORZ_PLUS_INLINE void						change_to					(const std::string& _label, const static_any<FSLCDS>& _lcd=static_any<FSLCDS>())
	{
		std::vector<frame_segment>::iterator itr(std::find(m_frame_segments.begin(), m_frame_segments.end(), _label));
		if (itr == m_frame_segments.end())
			return;

		if (m_current_fs && m_current_fs->is_start())
			m_current_fs->stop();

		const std::string& old_label = m_current_fs->get_label();
		m_current_fs = &(*itr);
		m_current_fs_label = (*itr).get_label();
		const std::string& new_label = m_current_fs->get_label();

		(*itr).start(_lcd);

		m_change_handler(old_label, *this, new_label);
	}

	//!
	ORZ_PLUS_INLINE void						run							()
	{
		if (!check_run())
			return;

		m_current_fs->run();
	}

	ORZ_PLUS_INLINE const action_frame*		get_current_af				() const
	{
		if (!check_run())
			return 0;

		return m_current_fs->get_current_af();
	}

	//!
	ORZ_PLUS_INLINE int32						create_frame_segment		(const std::string& _label)
	{
		std::vector<frame_segment>::iterator itr(std::find(m_frame_segments.begin(), m_frame_segments.end(), _label));
		if (itr != m_frame_segments.end())
			return 1;

		m_frame_segments.push_back(frame_segment(_label, *this, *m_afs_mgr, m_fs_start_handler, m_fs_loop_handler, m_fs_finish_handler));

		return 0;
	}

private:

	//!
	ORZ_PLUS_INLINE bool						check_run					() const
	{
		if (m_frame_segments.empty())
			return false;

		if (!m_current_fs)
			return false;

		return true;
	}

private:
	action_frame_sequence_manager*			m_afs_mgr;
	std::vector<frame_segment>				m_frame_segments;
	std::string								m_current_fs_label;
	frame_segment*							m_current_fs;

	afs_id									m_afsid;

	boost::function<void (const std::string&, action_frame_sequence&)>
											m_fs_start_handler;
	boost::function<void (const std::string&, action_frame_sequence&, const std::string&)>
											m_change_handler;
	boost::function<void (const action_frame&, frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64)>
											m_fs_loop_handler;
	boost::function<void (frame_segment&, action_frame_sequence&, action_frame_sequence_manager&, float64, float64, bool)>
											m_fs_finish_handler;

};


//!
class action_frame_sequence_manager
{
public:
	action_frame_sequence_manager()
		: m_current_afs(0)
	{
	}
	~action_frame_sequence_manager()
	{
	}

public:
	//!
	ORZ_PLUS_INLINE void							init						(std::vector<action_frame_sequence>* _afss)
	{
		m_afss = _afss;
	}

public:
	//!
	ORZ_PLUS_INLINE int32							start_afs					(afs_id _afsid, const char* _label, const static_any<FSLCDS>& _lcd=static_any<FSLCDS>())
	{
		if (_afsid >= m_afss->size())
		{
			printf("action_frame_sequence_manager::start_afs - _afsid >= m_afss.size()\n");
			return 1;
		}

		if (m_current_afs)
			m_current_afs->stop();

		m_current_afs = &(*m_afss)[_afsid];
		std::string label(_label);
		m_current_afs->start(label, _lcd);

		return 0;
	}

	//!
	ORZ_PLUS_INLINE const action_frame*			get_current_af				() const
	{
		if (m_current_afs)
			return m_current_afs->get_current_af();
		else
			return 0;
	}

	//!
	ORZ_PLUS_INLINE void							run							()
	{
		if (m_current_afs)
			m_current_afs->run();
	}

	//!
	ORZ_PLUS_INLINE void							stop						()
	{
		if (m_current_afs)
			m_current_afs->stop();
	}

private:
	std::vector<action_frame_sequence>*								m_afss;
	action_frame_sequence*											m_current_afs;
};

}

#endif
