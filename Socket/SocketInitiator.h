#ifndef SOCKET_INITIATOR_H_
#define SOCKET_INITIATOR_H_

#include "Initiator.h"
#include "SocketConnector.h"
#include "SocketConnection.h"

class SocketInitiator : public Initiator ,public Strategy
{
public:
	SocketInitiator();
	~SocketInitiator();
	void OnInitialize();
	void OnStart();
	void OnStop();
	void DoConnect(int uid);

	void onConnect( SocketMonitor&, int s );
	void onWrite( SocketMonitor&, int s );
	bool onData( SocketMonitor&, int s );
	void onDisconnect( SocketMonitor&, int s );
	void onError();
	void onTimeout();
private:
	typedef map<int, SocketConnection*> SocketConnections;
	SocketConnections m_pendingConnections;
	SocketConnections m_connections;
	SocketConnector m_connector;
	bool	m_noDelay;
	int		m_sendBufSize;
	int		m_rcvBufSize;
	int		m_reconnectInterval;
	time_t	m_lastConnect;

};


#endif