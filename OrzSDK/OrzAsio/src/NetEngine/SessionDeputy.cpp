#include <orz/OrzAsio/NetEngine/SessionDeputy.h>
#include <NetEngine/SessionDeputyImpl.h>
using namespace Orz;

SessionDeputy::SessionDeputy()
	: m_impl(new SessionDeputyImpl)
{
}

SessionDeputy::~SessionDeputy()
{
	delete m_impl;
}

void SessionDeputy::setClient(IClientShrPtr _client)
{
	m_impl->setClient(_client);
}

IClientShrPtr SessionDeputy::getClient() const
{
	return m_impl->getClient();
}

void SessionDeputy::clearClient()
{
	m_impl->clearClient();
}

void SessionDeputy::setTag(void* _tag)
{
	m_impl->setTag(_tag);
}
	
void* SessionDeputy::getTag() const
{
	return m_impl->getTag();
}
	
void SessionDeputy::clearTag()
{
	m_impl->clearTag();
}

bool SessionDeputy::isValid() const
{
	return m_impl->isValid();
}

SessionDeputyImpl& SessionDeputy::impl()
{
	return *m_impl;
}
