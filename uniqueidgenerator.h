#ifndef UNIQUEIDGENERATOR_H__
#define UNIQUEIDGENERATOR_H__

// Generate sequential (unique) values for any types that 
// has a default ctor and supports the post-increment operator
template <class T>
class UniqueIdGenerator
{
    public:
        typedef T Type;

    public:
        UniqueIdGenerator();
        T Get();

    private:
        T m_current;
};

template <class T>
UniqueIdGenerator<T>::UniqueIdGenerator() : m_current(T())
{
}

template <class T>
T UniqueIdGenerator<T>::Get()
{
    return m_current++;
}

#endif // UNIQUEIDGENERATOR_H__
