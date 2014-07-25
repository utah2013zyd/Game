#ifndef GAMEACCOUNT_H_
#define GAMEACCOUNT_H_
#include <orz/OrzAsio/NetEngine/SessionPxy.h>
#include <common/account/account.h>
namespace Orz
{

class game_account : public account
{
public:
	game_account(account_id _aid, const SessionPxy& _sid, const char* _pName, const char* _pNickName)
		: m_aid(_aid)
		, m_sid(_sid)
		, m_name(_pName)
		, m_nickname(_pNickName)
	{
	}

	virtual ~game_account()
	{
	}

public:
	//! 
	account_id				get_id				() const 
	{
		return m_aid;
	}

	//!
	const SessionPxy&	get_session_proxy	() const
	{
		return m_sid;
	}

	//! 
	const std::string&		get_name			() const
	{
		return m_name;
	}

	//!
	const std::string&		get_nick_name		() const
	{
		return m_nickname;
	}

private:
	account_id					m_aid;
	SessionPxy					m_sid;
	std::string					m_name;
	std::string					m_nickname;
};

typedef boost::shared_ptr<game_account> game_account_ptr;

}

#endif
