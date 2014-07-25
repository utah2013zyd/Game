//////////////////////////////////////////////////////////////
//
//      FileName: account.h
//        Author: 熊小磊
//       Version: 1.0.0
//          Date: 2008-10-13
//          Time: 15:38:00
//   Description: 帐户基类
//
// History:
//      <author>    <time>      <version>   <descript>
//      熊小磊		2008-10-13  1.0.0       add
//////////////////////////////////////////////////////////////
#ifndef ACCOUNT_H_
#define ACCOUNT_H_


namespace Orz
{

typedef unsigned int account_id;

class string;
class SessionPxy;

class account
{
public:
	account() {}
	virtual ~account() {}

public:
	//!
	virtual account_id				get_id				() const = 0;

	//!
	virtual const SessionPxy&	get_session_proxy	() const = 0;

	//!
	virtual const std::string&		get_name			() const = 0;

	//!
	virtual const std::string&		get_nick_name		() const = 0;
};

typedef boost::shared_ptr<account> account_ptr;

}	// end of namespace Orz

#endif
