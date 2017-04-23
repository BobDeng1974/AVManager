#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_
#include "SocketMonitor.h"


struct SocketInfo
{
	SocketInfo()
		: m_socket( -1 ), m_port( 0 ), m_noDelay( false ),
		m_sendBufSize( 0 ), m_rcvBufSize( 0 ) {}

	SocketInfo( int socket, short port, bool noDelay, int sendBufSize, int rcvBufSize )
		: m_socket( socket ), m_port( port ), m_noDelay( noDelay ), 
		m_sendBufSize( sendBufSize ), m_rcvBufSize( rcvBufSize ) {}

	int m_socket;
	short m_port;
	bool m_noDelay;
	int m_sendBufSize;
	int m_rcvBufSize;
};

class SocketServer
{
public:
	SocketServer();
	~SocketServer();
	int add( int port, bool reuse = false, bool noDelay = false, int sendBufSize = 0, int rcvBufSize = 0 ) ;
	int accept( int socket );
	void close();
	bool block( Strategy& strategy, bool poll = 0, double timeout = 0.0 );

	//size_t numConnections() { return m_monitor.numSockets() - 1; }
	SocketMonitor& getMonitor() { return m_monitor; }
	int SocketToPort( int socket );
	int PortToSocket( int port );

private:
	typedef std::map<int, SocketInfo>	SocketToInfo;
	typedef std::map<int, SocketInfo>	PortToInfo;

	SocketToInfo m_socketToInfo;  
	PortToInfo m_portToInfo;
	SocketMonitor m_monitor;
};

#endif