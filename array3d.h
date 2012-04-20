#ifndef TAK__ARRAY3D_H__
#define TAK__ARRAY3D_H__

#include "define.h"
#include <cassert>

template <class T>
class Array3d
{
    public:
        Array3d(unsigned int x, unsigned int y, unsigned int z);
        ~Array3d();
        Array3d(const Array3d<T>& array);

        void Set(unsigned int x, unsigned int y, unsigned int z, T type);
        T Get(unsigned int x, unsigned int y, unsigned int z) const;

        void Reset(T type);

        unsigned int SizeX() const;
        unsigned int SizeY() const;
        unsigned int SizeZ() const;

    private:
        unsigned int m_x, m_y, m_z;
        T* m_array;

        T& GetElement(unsigned int x, unsigned int y, unsigned int z);
        const T& GetElement(unsigned int x, unsigned int y, unsigned int z) const;
};

    template <class T>
Array3d<T>::Array3d(unsigned int x, unsigned int y, unsigned int z) : m_x(x), m_y(y), m_z(z)
{
    m_array = new T[m_x * m_y * m_z];
}

    template <class T>
Array3d<T>::~Array3d()
{
    delete [] m_array;
}

    template <class T>
Array3d<T>::Array3d(const Array3d& array)
{
    m_x = array.m_x;
    m_y = array.m_y;
    m_z = array.m_z;

    m_array = new T[m_x * m_y * m_z];
    for(unsigned int i = 0; i < m_x * m_y * m_z; ++i)
        m_array[i] = array.m_blocs[i];
}

    template <class T>
void Array3d<T>::Set(unsigned int x, unsigned int y, unsigned int z, T type)
{
    GetElement(x, y, z) = type;
}

template <class T>
T Array3d<T>::Get(unsigned int x, unsigned int y, unsigned int z) const
{
    return GetElement(x, y, z);
}

    template <class T>
void Array3d<T>::Reset(T type)
{
    for(unsigned int i = 0; i < m_x * m_y * m_z; ++i)
        m_array[i] = type;
}

    template <class T>
unsigned int Array3d<T>::SizeX() const
{
    return m_x;
}

    template <class T>
unsigned int Array3d<T>::SizeY() const
{
    return m_y;
}

    template <class T>
unsigned int Array3d<T>::SizeZ() const
{
    return m_z;
}


    template <class T>
T& Array3d<T>::GetElement(unsigned int x, unsigned int y, unsigned int z)
{
    assert(x < m_x);
    assert(y < m_y);
    assert(z < m_z);
    return m_array[x + (z * m_x) + (y * m_z * m_x)];
}

template <class T>
const T& Array3d<T>::GetElement(unsigned int x, unsigned int y, unsigned int z) const
{
    assert(x < m_x);
    assert(y < m_y);
    assert(z < m_z);
    return m_array[x + (z * m_x) + (y * m_z * m_x)];
}

#endif // ARRAY3D_H__
