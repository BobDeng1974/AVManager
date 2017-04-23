#include "stdafx.h"
#include "SocketConnector.h"

SocketConnector::SocketConnector()
{

}

SocketConnector::~SocketConnector()
{

}

int SocketConnector::Connect( const std::string& address, int port, bool noDelay,
			 int sendBufSize, int rcvBufSize)
{
	int socket = ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( socket != -1 )
	{

		if( noDelay )
			socket_setsockopt(socket, TCP_NODELAY, noDelay);
		if( sendBufSize )
			socket_setsockopt(socket, SO_SNDBUF, sendBufSize);
		if( rcvBufSize )
			socket_setsockopt(socket, SO_RCVBUF, rcvBufSize);
		if (true)
			socket_setnonblock(socket);
		m_monitor.AddConnector( socket );

		socket_connect(socket, address.c_str(), port);

	}
	return socket;
}

int SocketConnector::Block(Strategy& strategy, bool poll, double timeout )
{
	m_monitor.Block(strategy,poll,timeout);
	return 0;
}