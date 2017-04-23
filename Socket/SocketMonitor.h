#ifndef SOCKET_MONITOR_H_
#define SOCKET_MONITOR_H_


class SocketMonitor;
class Strategy
{
public:
	virtual void onConnect( SocketMonitor&, int s ) = 0;
	virtual void onDisconnect( SocketMonitor&, int s ) = 0;
	virtual void onWrite( SocketMonitor&, int s ) = 0;
	virtual bool onData( SocketMonitor&, int s ) = 0;
	virtual void onError() = 0;
	virtual void onTimeout( ) = 0;
};

class SocketMonitor
{
	
public:
	SocketMonitor();
	~SocketMonitor();

	bool AddConnector(int s);
	bool AddRead(int s);
	bool AddWrite( int s );
	void Block(Strategy& strategy, bool poll, double timeout );
	bool SleepIfEmpty(bool poll);
	void signal(int socket);
	void unsignal(int socket);
private:
	typedef set<int> Sockets;

	void BuildSet( const Sockets& sockets, fd_set& watchSet );
	void ProcessReadSet( Strategy&, fd_set& );
	void ProcessWriteSet( Strategy&, fd_set& );
	void ProcessExceptSet( Strategy&, fd_set& );
	Sockets m_connectSockets;
	Sockets m_readSockets;
	Sockets m_writeSockets;
	int		m_interrupt;
	int		m_signal;
	int		m_timeout;
	timeval m_timeval;

};



#endif