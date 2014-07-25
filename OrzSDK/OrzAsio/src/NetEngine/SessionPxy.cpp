#include <orz/OrzAsio/NetEngine/SessionPxy.h>
using namespace Orz;

SessionPxy::SessionPxy()
	: m_vcode(0)
	, m_sn(0)
	, m_socketType(SKTP_NULL)
{
}

SessionPxy::SessionPxy(SessionId _snid, ISession* _sn, ValidCode _vcode, int32 _socketType)
	: m_snid(_snid)
	, m_sn(_sn)
	, m_vcode(_vcode)
	, m_socketType(_socketType)
{
}

SessionPxy::~SessionPxy()
{
}

SessionId SessionPxy::getSnid() const
{
	return m_snid;
}

ISession* SessionPxy::getSession() const
{
	return m_sn;
}

bool SessionPxy::operator==(const SessionPxy& _other) const
{
	if (m_snid == _other.m_snid &&
		m_sn == _other.m_sn &&
		m_vcode == _other.m_vcode &&
		m_socketType == _other.m_socketType)
		return true;
	else
		return false;
}

bool SessionPxy::operator<(const SessionPxy& _other) const
{
	if (getSocketType() < _other.getSocketType())
		return true;
	else if (getSocketType() > _other.getSocketType())
		return false;

	return getSnid() < _other.getSnid();
}

ValidCode SessionPxy::getVcode() const
{
	return m_vcode;
}

int32 SessionPxy::getSocketType() const
{
	return m_socketType;
}

void SessionPxy::setVcode(ValidCode _vcode)
{
	m_vcode = _vcode;
}

void SessionPxy::setSession(ISession* _sn)
{
	m_sn = _sn;
}

bool SessionPxyLess::operator()(const SessionPxy& _s1, const SessionPxy& _s2) const
{
	if (_s1.getSocketType() < _s2.getSocketType())
		return true;
	else if (_s1.getSocketType() > _s2.getSocketType())
		return false;

	return _s1.getSnid() < _s2.getSnid();
}
