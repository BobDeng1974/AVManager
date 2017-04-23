#ifndef SOCKET_CONNECTION_H_
#define SOCKET_CONNECTION_H_



class SocketConnection
{
public:
	SocketConnection(int uid);
	~SocketConnection();

	bool Read();
	bool Write();

	int GetUID() { return m_uid; };
private:
	int m_uid;

};

#endif