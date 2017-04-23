#ifndef SOCKET_ACCEPTOR_H_
#define SOCKET_ACCEPTOR_H_

#include "Acceptor.h"
#include "SocketServer.h"
#include "SocketConnection.h"

class SocketAcceptor : public Acceptor, public Strategy
{
public:
	SocketAcceptor();

	virtual ~SocketAcceptor();

	void OnInitialize( );
	void OnStart();
	void OnStop();

	void onConnect( SocketMonitor&, int s);
	void onWrite( SocketMonitor&, int );
	bool onData( SocketMonitor&, int );
	void onDisconnect( SocketMonitor&, int );
	void onError();
	void onTimeout();
private:
	
	typedef std::map < int, SocketConnection* > SocketConnections;
	SocketServer* m_pServer;
	SocketConnections m_connections;
};


#endif