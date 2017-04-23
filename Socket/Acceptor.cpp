#include "stdafx.h"
#include "Acceptor.h"
#include <process.h>

Acceptor::Acceptor()
{
	m_stop = true;
}

Acceptor::~Acceptor()
{

}

void Acceptor::Start()
{
	m_stop = false;
	OnInitialize();
	_beginthreadex(NULL,0,ThreadProc,this,0,0);
}

void Acceptor::Stop( bool force)
{
	if( IsStopped() ) return;
	m_stop = true;
	OnStop();
}

unsigned int Acceptor::ThreadProc( void* p )
{
	Acceptor* pAcceptor = static_cast<Acceptor*>(p);
	pAcceptor->OnStart();
	return 0;
}