#ifndef C_SOCKET_H_
#define C_SOCKET_H_

namespace utility
{
	static void socket_init()
	{
#ifdef _MSC_VER
		WORD version = MAKEWORD( 2, 2 );
		WSADATA data;
		WSAStartup( version, &data );
#else
		struct sigaction sa;
		sa.sa_handler = SIG_IGN;
		sigemptyset( &sa.sa_mask );
		sa.sa_flags = 0;
		sigaction( SIGPIPE, &sa, 0 );
#endif
	}

	static void socket_term()
	{
#ifdef _MSC_VER
		WSACleanup();
#endif
	}

	static int socket_send( int s, const char* msg, size_t length )
	{
		return send( s, msg, length, 0 );
	}

	static void socket_close( int s )
	{
#ifdef _MSC_VER
		closesocket( s );
#else
		close( s );
#endif
	}

	static bool socket_fionread( int s, int& bytes )
	{
		bytes = 0;
#if defined(_MSC_VER)
		return ::ioctlsocket( s, FIONREAD, &( ( unsigned long& ) bytes ) ) == 0;
#elif defined(USING_STREAMS)
		return ::ioctl( s, I_NREAD, &bytes ) >= 0;
#else
		return ::ioctl( s, FIONREAD, &bytes ) == 0;
#endif
	}

	static bool socket_disconnected( int s )
	{
		char byte;
		return ::recv (s, &byte, sizeof (byte), MSG_PEEK) <= 0;
	}

	static int socket_setsockopt( int s, int opt, int optval )
	{
		int level = SOL_SOCKET;
		if( opt == TCP_NODELAY )
			level = IPPROTO_TCP;

#ifdef _MSC_VER
		return ::setsockopt( s, level, opt,
			( char* ) & optval, sizeof( optval ) );
#else
		return ::setsockopt( s, level, opt,
			&optval, sizeof( optval ) );
#endif
	}

	static int socket_setsockopt( int s, int opt )
	{
#ifdef _MSC_VER
		BOOL optval = TRUE;
#else
		int optval = 1;
#endif
		return socket_setsockopt( s, opt, optval );
	}

	static int socket_getsockopt( int s, int opt, int& optval )
	{
		int level = SOL_SOCKET;
		if( opt == TCP_NODELAY )
			level = IPPROTO_TCP;

#ifdef _MSC_VER
		int length = sizeof(int);
#else
		socklen_t length = sizeof(socklen_t);
#endif

		return ::getsockopt( s, level, opt, 
			( char* ) & optval, & length );
	}

#ifndef _MSC_VER
	static int socket_fcntl( int s, int opt, int arg )
	{
		return ::fcntl( s, opt, arg );
	}

	static int socket_getfcntlflag( int s, int arg )
	{
		return socket_fcntl( s, F_GETFL, arg );
	}

	static int socket_setfcntlflag( int s, int arg )
	{
		int oldValue = socket_getfcntlflag( s, arg );
		oldValue |= arg;
		return socket_fcntl( s, F_SETFL, arg );
	}
#endif

	static void socket_setnonblock( int socket )
	{
#ifdef _MSC_VER
		u_long opt = 1;
		::ioctlsocket( socket, FIONBIO, &opt );
#else
		socket_setfcntlflag( socket, O_NONBLOCK );
#endif
	}

	static bool socket_isValid( int socket )
	{
#ifdef _MSC_VER
		return socket != INVALID_SOCKET;
#else
		return socket >= 0;
#endif
	}

#ifndef _MSC_VER
	bool socket_isBad( int s )
	{
		struct stat buf;
		fstat( s, &buf );
		return errno == EBADF;
	}
#endif

	static void socket_invalidate( int& socket )
	{
#ifdef _MSC_VER
		socket = INVALID_SOCKET;
#else
		socket = -1;
#endif
	}

	static short socket_hostport( int socket )
	{
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		if( getsockname(socket, (struct sockaddr*)&addr, &len) < 0 )
			return 0;

		return ntohs( addr.sin_port );
	}

	static const char* socket_hostname( int socket )
	{
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		if( getsockname(socket, (struct sockaddr*)&addr, &len) < 0 )
			return 0;

		return inet_ntoa( addr.sin_addr );
	}

	static const char* socket_hostname( const char* name )
	{
		struct hostent* host_ptr = 0;
		struct in_addr** paddr;
		struct in_addr saddr;

#if( GETHOSTBYNAME_R_INPUTS_RESULT || GETHOSTBYNAME_R_RETURNS_RESULT )
		hostent host;
		char buf[1024];
		int error;
#endif

		saddr.s_addr = inet_addr( name );
		if ( saddr.s_addr != ( unsigned ) - 1 ) return name;

#if GETHOSTBYNAME_R_INPUTS_RESULT
		gethostbyname_r( name, &host, buf, sizeof(buf), &host_ptr, &error );
#elif GETHOSTBYNAME_R_RETURNS_RESULT
		host_ptr = gethostbyname_r( name, &host, buf, sizeof(buf), &error );
#else
		host_ptr = gethostbyname( name );
#endif

		if ( host_ptr == 0 ) return 0;

		paddr = ( struct in_addr ** ) host_ptr->h_addr_list;
		return inet_ntoa( **paddr );
	}

	static const char* socket_peername( int socket )
	{
		struct sockaddr_in addr;
		socklen_t len = sizeof(addr);
		if( getpeername( socket, (struct sockaddr*)&addr, &len ) < 0 )
			return "UNKNOWN";
		char* result = inet_ntoa( addr.sin_addr );
		if( result )
			return result;
		else
			return "UNKNOWN";
	}

	static int socket_createAcceptor(int port, bool reuse)
	{
		int socket = ::socket( PF_INET, SOCK_STREAM, 0 );
		if ( socket < 0 ) return -1;

		sockaddr_in address;
		socklen_t socklen;

		address.sin_family = PF_INET;
		address.sin_port = htons( port );
		address.sin_addr.s_addr = INADDR_ANY;
		socklen = sizeof( address );
		if( reuse )
			socket_setsockopt( socket, SO_REUSEADDR );

		int result = bind( socket, reinterpret_cast < sockaddr* > ( &address ),
			socklen );
		if ( result < 0 ) return -1;
		result = listen( socket, SOMAXCONN );
		if ( result < 0 ) return -1;
		return socket;
	}

	static int socket_createConnector()
	{
		return ::socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	}

	static int socket_connect( int socket, const char* address, int port )
	{
		const char* hostname = socket_hostname( address );
		if( hostname == 0 ) return -1;

		sockaddr_in addr;
		addr.sin_family = PF_INET;
		addr.sin_port = htons( port );
		addr.sin_addr.s_addr = inet_addr( hostname );

		int result = connect( socket, reinterpret_cast < sockaddr* > ( &addr ),
			sizeof( addr ) );

		return result;
	}

	static int socket_accept( int s )
	{
		if ( !socket_isValid( s ) ) return -1;
		return accept( s, 0, 0 );
	}

	static std::pair<int, int> socket_createpair()
	{
#ifdef _MSC_VER
		int acceptor = socket_createAcceptor(0, true);
		const char* host = socket_hostname( acceptor );
		short port = socket_hostport( acceptor );
		int client = socket_createConnector();
		socket_connect( client, "localhost", port );
		int server = socket_accept( acceptor );
		socket_close(acceptor);
		return std::pair<int, int>( client, server );
#else
		int pair[2];
		socketpair( AF_UNIX, SOCK_STREAM, 0, pair );
		return std::pair<int, int>( pair[0], pair[1] );
#endif
	}
};


#endif