#ifndef TAK_AUTOLOCK_H__
#define TAK_AUTOLOCK_H__

template <class T>
class AutoLock
{
    public:
        AutoLock(T& lock);
        ~AutoLock();

    private:
        T& m_lock;
};

    template <class T>
AutoLock<T>::AutoLock(T& lock) : m_lock(lock)
{
    m_lock.Lock();
}

    template <class T>
AutoLock<T>::~AutoLock()
{
    m_lock.UnLock();
}

#endif // TAK_AUTOLOCK_H__
