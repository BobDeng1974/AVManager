
#ifndef rw_lock_h
#define rw_lock_h

#ifdef _MSC_VER
#pragma	warning(disable: 4127) 
#endif


namespace utility
{
	//读写锁
	class rw_lock
	{
	public:
		rw_lock(void) 
		{
#ifdef _MSC_VER
			InitializeCriticalSection(&m_rw_lock);
#else
			pthread_rwlock_init(&m_rw_lock, NULL);
#endif
		}
		~rw_lock(void)
		{
#ifdef _MSC_VER
			DeleteCriticalSection(&m_rw_lock);
#else
			pthread_rwlock_destroy(&m_rw_lock);
#endif
		}
	public:
		//读锁进入
		void r_enter(void)
		{
#ifdef _MSC_VER
			EnterCriticalSection(&m_rw_lock);
#else
			pthread_rwlock_rdlock(&m_rw_lock);
#endif
		}
		//读锁离开
		void r_leave(void)
		{
#ifdef _MSC_VER
			LeaveCriticalSection(&m_rw_lock);
#else
			 pthread_rwlock_unlock(&m_rw_lock);
#endif
		}
		//写锁保护进入
		void w_enter(void)
		{
#ifdef _MSC_VER
			EnterCriticalSection(&m_rw_lock);
#else
			pthread_rwlock_wrlock(&m_rw_lock);
#endif
		}
		//写锁保护离开
		void w_leave(void)
		{
#ifdef _MSC_VER
			LeaveCriticalSection(&m_rw_lock);
#else
			 pthread_rwlock_unlock(&m_rw_lock);
#endif
		}
	private:
#ifdef _MSC_VER
		CRITICAL_SECTION		m_rw_lock;
#else
		pthread_rwlock_t 		m_rw_lock;
#endif
	};


	class c_wlock
	{
	public:
		c_wlock(rw_lock* lock): m_wlock(lock) 
		{
			m_wlock->w_enter(); 
		}

		~c_wlock() 
		{
			m_wlock->w_leave();
		}
	private:
		rw_lock*	m_wlock;
	};

	class c_rlock
	{
	public:
		c_rlock(rw_lock* lock): m_rlock(lock) 
		{
			m_rlock->r_enter(); 
		}

		~c_rlock() 
		{
			m_rlock->r_leave();
		}
	private:
		rw_lock*	m_rlock;
	};

}

#endif //rw_lock_h
