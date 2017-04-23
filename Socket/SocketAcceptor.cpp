#include "stdafx.h"
#include "SocketAcceptor.h"


SocketAcceptor::SocketAcceptor( )
{

}

SocketAcceptor::~SocketAcceptor()
{

}

void SocketAcceptor::OnInitialize( )
{
	short port = 0;
	bool reuseAddress = true;
	bool noDelay = true;
	int sendBufSize = 8096;
	int rcvBufSize = 8096;
	m_pServer = new SocketServer;
	m_pServer->add( port, reuseAddress, noDelay, sendBufSize, rcvBufSize );
}

void SocketAcceptor::OnStart()
{
	while ( !IsStopped() && m_pServer && m_pServer->block( *this ) ) {}

	if( !m_pServer )
		return;

	time_t start = 0;
	time_t now = 0;

	::time( &start );
	while ( /*isLoggedOn()*/ true)
	{
		m_pServer->block( *this );
		if( ::time(&now) -5 >= start )
			break;
	}

	m_pServer->close();
	delete m_pServer;
	m_pServer = 0;
}

void SocketAcceptor::OnStop()
{

}

void SocketAcceptor::onConnect( SocketMonitor&, int s )
{
	if ( !socket_isValid( s ) ) return;
	SocketConnections::iterator i = m_connections.find( s );
	if ( i != m_connections.end() ) return;
	m_connections[ s ] = new SocketConnection(s);
}

void SocketAcceptor::onWrite( SocketMonitor&, int s)
{
	SocketConnections::iterator i = m_connections.find(s);
	if (i == m_connections.end()) return;
	SocketConnection* pSocketConnection = i->second;
	if( pSocketConnection->Write() )
	{

	}
}

bool SocketAcceptor::onData( SocketMonitor& monitor, int s/*server port*/)
{
	int port = m_pServer->SocketToPort( s );
	if( port != 0 )
	{
		int sRet = m_pServer->accept(s);
		onConnect(monitor,sRet);
	}
	else
	{
		SocketConnections::iterator i = m_connections.find( s );
		if ( i == m_connections.end() ) return false;
		SocketConnection* pSocketConnection = i->second;
		return pSocketConnection->Read();
	}
	return true;
}

void SocketAcceptor::onDisconnect( SocketMonitor&, int s)
{
	SocketConnections::iterator i = m_connections.find( s );
	if ( i == m_connections.end() ) return ;
	SocketConnection* pSocketConnection = i->second;

	delete pSocketConnection;
	m_connections.erase( s );
}

void SocketAcceptor::onError( )
{

}

void SocketAcceptor::onTimeout( )
{

}