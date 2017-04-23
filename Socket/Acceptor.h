#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_



class Acceptor
{
public:
	Acceptor();
	~Acceptor();
	void Start();
	void Stop( bool force = false );

	bool IsStopped() { return m_stop;}

	virtual void OnInitialize() = 0;
	virtual void OnStart() = 0;
	virtual void OnStop() = 0;
private:
	static unsigned int __stdcall ThreadProc( void* p );
	bool m_stop;
};

#endif