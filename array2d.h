#ifndef TAK__ARRAY2D_H__
#define TAK__ARRAY2D_H__
#include "define.h"
#include <cassert>

template <class T>
class Array2d
{
    public:
    Array2d(unsigned int x, unsigned int y);
    ~Array2d();
    Array2d(const Array2d<T>& array);

    void Set(unsigned int x, unsigned int y, T type);
    T Get(unsigned int x, unsigned int y) const;

    unsigned int SizeX() const;
    unsigned int SizeY() const;

    void Reset(T type);

    private:
        unsigned int m_x, m_y;
        T* m_array;

        T& GetElement(unsigned int x, unsigned int y);
        const T& GetElement(unsigned int x, unsigned int y) const;
};

template <class T>
Array2d<T>::Array2d(unsigned int x, unsigned int y) : m_x(x), m_y(y)
{
    m_array = new T[m_x * m_y];
}

template <class T>
Array2d<T>::~Array2d()
{
    delete [] m_array;
}

template <class T>
Array2d<T>::Array2d(const Array2d& array)
{
    m_x = array.m_x;
    m_y = array.m_y;

    m_array = new T[m_x * m_y];
    for(unsigned int i = 0; i < m_x * m_y; ++i)
        m_array[i] = array.m_array[i];
}

template <class T>
void Array2d<T>::Set(unsigned int x, unsigned int y, T type)
{
    GetElement(x, y) = type;
}

template <class T>
T Array2d<T>::Get(unsigned int x, unsigned int y) const
{
    return GetElement(x, y);
}

template <class T>
unsigned int Array2d<T>::SizeX() const
{
    return m_x;
}

template <class T>
unsigned int Array2d<T>::SizeY() const
{
    return m_y;
}

template <class T>
void Array2d<T>::Reset(T type)
{
    for(unsigned int i = 0; i < m_x * m_y; ++i)
        m_array[i] = type;
}

template <class T>
T& Array2d<T>::GetElement(unsigned int x, unsigned int y)
{
    assert(x < m_x);
    assert(y < m_y);
    return m_array[x + (y * m_x)];
}

template <class T>
const T& Array2d<T>::GetElement(unsigned int x, unsigned int y) const
{
    assert(x < m_x);
    assert(y < m_y);
    return m_array[x + (y * m_x)];
}

#endif // ARRAY2D_H__
