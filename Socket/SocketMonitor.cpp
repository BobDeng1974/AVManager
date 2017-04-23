#include "stdafx.h"
#include "SocketMonitor.h"


SocketMonitor::SocketMonitor()
{
	socket_init();
	std::pair<int, int> sockets = socket_createpair();
	m_signal = sockets.first;
	m_interrupt = sockets.second;
	socket_setnonblock( m_signal );
	socket_setnonblock( m_interrupt );
	m_readSockets.insert( m_interrupt );
	m_timeout = 1;
	m_timeval.tv_sec = 1;
	m_timeval.tv_usec = 0;
}

SocketMonitor::~SocketMonitor()
{
	socket_term();
}

void SocketMonitor::signal( int socket )
{
	socket_send( m_signal, (char*)&socket, sizeof(socket) );
}

void SocketMonitor::unsignal( int s )
{
	Sockets::iterator i = m_writeSockets.find( s );
	if( i == m_writeSockets.end() ) return;

	m_writeSockets.erase( s );
}

bool SocketMonitor::AddConnector(int s)
{ 
	Sockets::iterator i = m_connectSockets.find( s );
	if( i != m_connectSockets.end() ) return false;

	m_connectSockets.insert( s );
	return true;
}

bool SocketMonitor::AddRead
	(int s)
{
	socket_setnonblock( s );
	Sockets::iterator i = m_readSockets.find( s );
	if( i != m_readSockets.end() ) return false;

	m_readSockets.insert( s );
	return true;

}

bool SocketMonitor::AddWrite( int s )
{
	if( m_readSockets.find(s) == m_readSockets.end() )
		return false;

	socket_setnonblock(s);
	Sockets::iterator i = m_writeSockets.find( s );
	if( i != m_writeSockets.end() ) return false;

	m_writeSockets.insert( s );
	return true;
}

void SocketMonitor::Block(Strategy& strategy, bool poll, double timeout )
{
	fd_set readSet;
	FD_ZERO( &readSet );
	BuildSet( m_readSockets, readSet );
	fd_set writeSet;
	FD_ZERO( &writeSet );
	BuildSet( m_connectSockets, writeSet );
	BuildSet( m_writeSockets, writeSet );
//	If processing a connect call (nonblocking), connection attempt failed. 
//	OOB data is available for reading (only if SO_OOBINLINE is disabled). 
// 	fd_set exceptSet;
// 	FD_ZERO( &exceptSet );
// 	BuildSet( m_connectSockets, exceptSet );

	if ( SleepIfEmpty(poll) )
	{
		return;
	}


	int result = select( FD_SETSIZE, &readSet, &writeSet, /*&exceptSet*/0,  &m_timeval);
	if ( result == 0 )
	{
		return;
	}
	else if ( result > 0 )
	{
		//ProcessExceptSet( strategy, exceptSet );
		ProcessWriteSet( strategy, writeSet );
		ProcessReadSet( strategy, readSet );
	}
	else
	{
		strategy.onError();  //socket error
	}
}

bool SocketMonitor::SleepIfEmpty(bool poll)
{
	if( poll )
		return false;

	if ( m_readSockets.empty() && 
		m_writeSockets.empty() &&
		m_connectSockets.empty() )
	{
		Sleep( m_timeout * 1000 );
		return true;
	}
	else
		return false;
}

void SocketMonitor::BuildSet( const Sockets& sockets, fd_set& watchSet )
{
	Sockets::const_iterator iter;
	for ( iter = sockets.begin(); iter != sockets.end(); ++iter ) {
		FD_SET( *iter, &watchSet );
	}
}

void SocketMonitor::ProcessReadSet( Strategy& strategy, fd_set& readSet)
{					
	for ( unsigned i = 0; i < readSet.fd_count; ++i )
	{
		int s = readSet.fd_array[ i ];
		if( s == m_interrupt )
		{
			int socket = 0;
			recv( s, (char*)&socket, sizeof(socket), 0 );
			AddWrite( socket );
		}
		else
		{
			if (!strategy.onData( *this, s ))
			{
				strategy.onDisconnect(*this, s);
			}
		}
	}				
}

void SocketMonitor::ProcessWriteSet( Strategy& strategy, fd_set& writeSet )
{					
	for ( unsigned i = 0; i < writeSet.fd_count; ++i )
	{
		int s = writeSet.fd_array[ i ];
		if( m_connectSockets.find(s) != m_connectSockets.end() )
		{
			m_connectSockets.erase( s );
			m_readSockets.insert( s );
			strategy.onConnect( *this, s );
		}
		else
		{
			strategy.onWrite( *this, s );
		}
	}
}					

void SocketMonitor::ProcessExceptSet( Strategy& strategy, fd_set& exceptSet)
{
	for ( unsigned i = 0; i < exceptSet.fd_count; ++i )
	{
		int s = exceptSet.fd_array[ i ];
		strategy.onDisconnect( *this, s );
	}
}