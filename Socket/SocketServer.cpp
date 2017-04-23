#include "stdafx.h"
#include "SocketServer.h"


SocketServer::SocketServer()
{

}

SocketServer::~SocketServer()
{

}

int SocketServer::add( int port, bool reuse, bool noDelay, int sendBufSize, int rcvBufSize)
{
	if( m_portToInfo.find(port) != m_portToInfo.end() )
		return m_portToInfo[port].m_socket;

	int socket = socket_createAcceptor( port, reuse );
	if( socket < 0 )
		return 0;
	if( noDelay )
		socket_setsockopt( socket, TCP_NODELAY );
	if( sendBufSize )
		socket_setsockopt( socket, SO_SNDBUF, sendBufSize );
	if( rcvBufSize )
		socket_setsockopt( socket, SO_RCVBUF, rcvBufSize );
	m_monitor.AddRead( socket );
	
	SocketInfo info( socket, port, noDelay, sendBufSize, rcvBufSize );
	m_socketToInfo[socket] = info;
	m_portToInfo[port] = info;
	return socket;
}

int SocketServer::accept( int socket )
{
	SocketInfo info = m_socketToInfo[socket];

	int result = socket_accept( socket );
	if( info.m_noDelay )
		socket_setsockopt( result, TCP_NODELAY );
	if( info.m_sendBufSize )
		socket_setsockopt( result, SO_SNDBUF, info.m_sendBufSize );
	if( info.m_rcvBufSize )
		socket_setsockopt( result, SO_RCVBUF, info.m_rcvBufSize );
	if ( result >= 0 )
		m_monitor.AddConnector( result );
	return result;
}

void SocketServer::close()
{
	SocketToInfo::iterator i = m_socketToInfo.begin();
	for( ; i != m_socketToInfo.end(); ++i )
	{
		int s = i->first;
		socket_close( s );
		socket_invalidate( s );
	}
}

bool SocketServer::block( Strategy& strategy, bool poll, double timeout )
{
	std::set<int> sockets;
	SocketToInfo::iterator i = m_socketToInfo.begin();
	for( ; i != m_socketToInfo.end(); ++i )
	{
		if( !socket_isValid(i->first) )
			return false;
		sockets.insert( i->first );
	}
	m_monitor.Block(strategy,poll,timeout);
	return 0;
}

int SocketServer::SocketToPort( int socket )
{
	SocketToInfo::iterator find = m_socketToInfo.find( socket );
	if( find == m_socketToInfo.end() ) return 0;
	return find->second.m_port;
}

int SocketServer::PortToSocket( int port )
{
	SocketToInfo::iterator find = m_portToInfo.find( port );
	if( find == m_portToInfo.end() ) return 0;
	return find->second.m_socket;
}



