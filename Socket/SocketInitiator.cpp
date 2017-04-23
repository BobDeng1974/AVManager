#include "stdafx.h"
#include "SocketInitiator.h"


SocketInitiator::SocketInitiator()
	:Initiator()
{
	m_noDelay = true;
	m_rcvBufSize = 0;
	m_sendBufSize = 0;
	m_lastConnect = 0;
	m_reconnectInterval = 30;
}

SocketInitiator::~SocketInitiator()
{

}

void SocketInitiator::OnInitialize()
{
	for (int i = 0; i < 2; i++)
	SetDisconnected(i);
}

void SocketInitiator::OnStart()
{
	Connect();

	while ( !IsStopped() ) {
 		m_connector.Block( *this, false, 1.0 );
 		onTimeout();
	}

	time_t start = 0;
	time_t now = 0;

	::time( &start );
	while ( /*isLoggedOn()*/true )
	{
		m_connector.Block( *this );
		if( ::time(&now) -5 >= start )
			break;
	}
}

void SocketInitiator::OnStop()
{
}

void SocketInitiator::DoConnect(int uid)
{
	try
	{
		std::string address;
		short port = 0;

		int result = m_connector.Connect( address, port, m_noDelay, m_sendBufSize, m_rcvBufSize );
		SetPending( uid );
		m_pendingConnections[ result ] 
		= new SocketConnection(uid);
	}
	catch ( std::exception& ) {}
}

void SocketInitiator::onConnect( SocketMonitor&, int s )
{
	SocketConnections::iterator i = m_pendingConnections.find( s );
	if( i == m_pendingConnections.end() ) return;
	SocketConnection* pSocketConnection = i->second;

	m_connections[s] = pSocketConnection;
	m_pendingConnections.erase( i );

	SetConnected(pSocketConnection->GetUID());
}

void SocketInitiator::onDisconnect( SocketMonitor&, int s )
{
	SocketConnections::iterator i = m_connections.find( s );
	SocketConnections::iterator j = m_pendingConnections.find( s );

	SocketConnection* pSocketConnection = 0;
	if( i != m_connections.end() ) 
		pSocketConnection = i->second;
	if( j != m_pendingConnections.end() )
		pSocketConnection = j->second;
	if( !pSocketConnection )
		return;

	delete pSocketConnection;
	m_connections.erase( s );
	m_pendingConnections.erase( s );
	SetDisconnected(pSocketConnection->GetUID());
}

void SocketInitiator::onWrite( SocketMonitor&, int s )
{
	SocketConnections::iterator i = m_connections.find( s );
	if ( i == m_connections.end() ) return ;
	SocketConnection* pSocketConnection = i->second;
	if( pSocketConnection->Write() )
		//pSocketConnection->unsignal();
	{

	}
}

bool SocketInitiator::onData( SocketMonitor&, int s )
{
	SocketConnections::iterator i = m_connections.find( s );
	if ( i == m_connections.end() ) return false;
	SocketConnection* pSocketConnection = i->second;
	return pSocketConnection->Read();
}

void SocketInitiator::onError()
{
	onTimeout();
}

void SocketInitiator::onTimeout()
{
	time_t now;
	::time( &now );

	if ( (now - m_lastConnect) >= m_reconnectInterval )
	{
		Connect();
		m_lastConnect = now;
	}
}