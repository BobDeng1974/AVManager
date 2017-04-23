#pragma once

#ifndef _C_EVENT_H_
#define _C_EVENT_H_

#ifdef _MSC_VER

#define t_event		HANDLE

#else

#include <semaphore.h>
#define t_event		sem_t
	
#endif

namespace utility 
{

class c_event
{
public:
	c_event(void)
	{
#ifdef _MSC_VER
	   m_event = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
	int res = sem_init(&m_event, 0, 0);
	if (res != 0)
	{
		perror("Semaphore initialization failed");
	}
#endif
	}

	virtual ~c_event(void)
	{
#ifdef _MSC_VER
		if (m_event != NULL)
		  CloseHandle(m_event);
#else
		sem_destroy(&m_event);
#endif
	}

	int wait_event(int second  = 0xffff) 
	{
#ifdef _MSC_VER
	return WaitForSingleObject(m_event, second * 1000);
#else
		time_t _timout = time(0) + second;
		
		while (true)
		{
			if (sem_trywait(&m_event) == 0)
				break;

			 if (time(0) > _timout)
                break;

            _sleep(100);

		}
		
	return 0;
#endif
	}

	int set_event(void)
	{
#ifdef _MSC_VER
		return SetEvent(m_event);
#else
		sem_post(&m_event);

		return 0;
#endif
	}

private:
	t_event				m_event;

#ifndef _MSC_VER
	pthread_mutex_t		m_mutex;
#endif
};
}



#endif //_C_EVENT_H_
