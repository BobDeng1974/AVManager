#ifndef INITIATOR_H_
#define INITIATOR_H_


class Initiator
{
public:
	Initiator();
	~Initiator();
	void Initialize();
	void Start();
	void Stop();
	void Connect();

	/// Implemented to initialize initiator
	virtual void OnInitialize() = 0;
	virtual void OnStart() = 0;
	virtual void OnStop() = 0;
	virtual void DoConnect(int uid) = 0;

	void SetConnected(const int uid);
	void SetDisconnected(const int uid);
	void SetPending(const int uid);
	bool IsStopped() { return m_stop; }
	static unsigned int (_stdcall ThreadProc)(void *);

private:
	typedef std::set < int > Sockets;
	bool m_stop;
	int  m_threadID;
	rw_lock m_lock;
	Sockets m_connected;
	Sockets m_disconnected;
	Sockets m_pending;
};


#endif