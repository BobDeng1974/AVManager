#ifndef SOCKET_CONNECTOR_H_
#define SOCKET_CONNECTOR_H_
#include "SocketMonitor.h"



class SocketConnector
{
public:
	SocketConnector();
	~SocketConnector();

	int Connect( const std::string& address, int port, bool noDelay,
		int sendBufSize, int rcvBufSize);

	int Block(Strategy& strategy, bool poll = 0, double timeout = 0.0 );

private:
	SocketMonitor m_monitor;
};


#endif