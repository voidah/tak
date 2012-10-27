#ifndef TAK_MUTEX_H__
#define TAK_MUTEX_H__

#include "define.h"

#ifdef OS_WINDOWS
class Mutex
{
    public:
        enum ERRORSTATUS {SUCCESS, FAILED};

        Mutex()
        {
            InitializeCriticalSection(&m_mutex);
        }

        ~Mutex()
        {
            DeleteCriticalSection(&m_mutex);
        }

        ERRORSTATUS Lock()
        {
            EnterCriticalSection(&m_mutex);
            return SUCCESS;
        }

        ERRORSTATUS TryLock()
        {
            return TryEnterCriticalSection(&m_mutex) ? SUCCESS : FAILED;
        }

        ERRORSTATUS UnLock()
        {
            LeaveCriticalSection(&m_mutex);
            return SUCCESS;
        }

    private:
        CRITICAL_SECTION m_mutex;
};

#else
#include <pthread.h>

class Mutex
{
    public:
        enum ERROR {SUCCESS, FAILED};

        Mutex()
        {
            pthread_mutexattr_init(&m_mutexAttr);
            pthread_mutexattr_settype(&m_mutexAttr, PTHREAD_MUTEX_RECURSIVE);

            pthread_mutex_init(&m_mutex, &m_mutexAttr);
        }

        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }

        ERROR Lock()
        {
            return pthread_mutex_lock(&m_mutex) == 0 ? SUCCESS : FAILED;
        }

        ERROR TryLock()
        {
            return pthread_mutex_trylock(&m_mutex) == 0 ? SUCCESS : FAILED;
        }

        ERROR UnLock()
        {
            return pthread_mutex_unlock(&m_mutex) == 0 ? SUCCESS : FAILED;
        }

    private:
        pthread_mutexattr_t m_mutexAttr;
        pthread_mutex_t     m_mutex;
};
#endif

#endif // TAK_MUTEX_H__
