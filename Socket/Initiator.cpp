#include "stdafx.h"
#include "Initiator.h"
#include <process.h>

Initiator::Initiator()
{

}

Initiator::~Initiator()
{

}

void Initiator::Initialize()
{
	OnInitialize();
}


void Initiator::Start()
{
	m_stop = false;
	Initialize();
	m_threadID = _beginthreadex( NULL, 0, &ThreadProc, this, 0, 0 );
}

void Initiator::Stop()
{
	if (m_stop) return;

	OnStop();
	m_stop = true;

}

void Initiator::Connect()
{
	Sockets::iterator i = m_disconnected.begin();
	for (; i != m_disconnected.end(); i++)
	{
		DoConnect(*i);
	}
	
}

void Initiator::SetConnected( const int  uid )
{
	c_wlock l(&m_lock);

	m_connected.insert( uid );
	m_disconnected.erase( uid );
	m_pending.erase(uid);
}

void Initiator::SetPending(const int uid)
{
	c_wlock l(&m_lock);

	m_connected.erase( uid );
	m_disconnected.erase( uid );
	m_pending.insert(uid);
}

void Initiator::SetDisconnected( const int uid )
{
	c_wlock l(&m_lock);
	m_connected.erase( uid );
	m_disconnected.insert( uid );
	m_pending.erase(uid);
}

unsigned int Initiator::ThreadProc(void *p)
{
	Initiator * pInitiator = static_cast < Initiator* > ( p );
	pInitiator->OnStart();
	return 0;
}