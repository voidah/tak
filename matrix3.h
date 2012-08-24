#ifndef MATRIX3_H__
#define MATRIX3_H__

#include <string>
#include <sstream>
#include <ostream>

#include "define.h"
#include "vector3.h"

template <class T>
class Matrix3
{
    public:
        typedef T Type;

    public:
        static const Matrix3<T> ZERO;
        static const Matrix3<T> IDENTITY;

    public:
        Matrix3();
        Matrix3(const T& v);
        Matrix3(const Matrix3<T>& m);
        Matrix3(const T& m_11, const T& m_12, const T& m_13, 
                const T& m_21, const T& m_22, const T& m_23, 
                const T& m_31, const T& m_32, const T& m_33);

        Matrix3<T>& operator=(const Matrix3<T>& m);

        Matrix3<T> operator+(const Matrix3<T>& m) const;
        const Matrix3<T>& operator+=(const Matrix3<T>& m);

        Matrix3<T> operator-(const Matrix3<T>& m) const;
        Matrix3<T> operator-() const;
        const Matrix3<T>& operator-=(const Matrix3<T>& m);

        Matrix3<T> operator*(const Matrix3<T>& m) const;
        Matrix3<T> operator*(const T& v) const;
        const Matrix3<T>& operator*=(const Matrix3<T>& m);
        const Matrix3<T>& operator*=(const T& v);

        Matrix3<T> operator/(const T& v) const;
        const Matrix3<T>& operator/=(const T& v);


        bool operator==(const Matrix3<T>& m) const;
        bool operator!=(const Matrix3<T>& m) const;

        void SetZero();
        void SetIdentity();

        T GetDeterminant() const;
        Matrix3<T> GetCofactor() const;

        void Transpose();
        void Invert();

        bool IsZero() const;
        bool IsIdentity() const;

        const T* GetInternalValues() const;
        T* GetInternalValues();
        std::string ToString(const std::string& lineBegin = "|", const std::string& lineEnd = "|\n") const;

    private:
        void Swap(T& v1, T& v2);

    private:
        union
        {
            // column-major matrix
            struct { T m_11, m_21, m_31, m_12, m_22, m_32, m_13, m_23, m_33; };
            T m_values[9];
        };
};

typedef Matrix3<int> Matrix3i;
typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

template <class T>
const Matrix3<T> Matrix3<T>::ZERO = Matrix3<T>(0);

template <class T>
const Matrix3<T> Matrix3<T>::IDENTITY = Matrix3<T>(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1);

    template <class T>
std::ostream& operator<<(std::ostream& out, const Matrix3<T>& m)
{
    out << m.ToString();
    return out;
}

    template <class T>
Matrix3<T>::Matrix3()
{
    // Leave matrix uninitialized
}

    template <class T>
Matrix3<T>::Matrix3(const T& v)
{
    for(int i = 0; i < 9; ++i)
        m_values[i] = v;
}

    template <class T>
Matrix3<T>::Matrix3(const Matrix3<T>& m)
{
    for(int i = 0; i < 9; ++i)
        m_values[i] = m.m_values[i];
}

    template <class T>
Matrix3<T>::Matrix3(const T& m_11, const T& m_12, const T& m_13, 
        const T& m_21, const T& m_22, const T& m_23, 
        const T& m_31, const T& m_32, const T& m_33)
{
    this->m_11 = m_11;
    this->m_12 = m_12;
    this->m_13 = m_13;
    this->m_21 = m_21;
    this->m_22 = m_22;
    this->m_23 = m_23;
    this->m_31 = m_31;
    this->m_32 = m_32;
    this->m_33 = m_33;
}

    template <class T>
Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& m)
{
    if(this != &m)
    {
        for(int i = 0; i < 9; ++i)
            m_values[i] = m.m_values[i];
    }

    return *this;
}

template <class T>
Matrix3<T> Matrix3<T>::operator+(const Matrix3<T>& m) const
{
    return Matrix3<T>(
            m_11 + m.m_11, m_12 + m.m_12, m_13 + m.m_13,
            m_21 + m.m_21, m_22 + m.m_22, m_23 + m.m_23,
            m_31 + m.m_31, m_32 + m.m_32, m_33 + m.m_33);
}

    template <class T>
const Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& m)
{
    *this = *this + m;
    return *this;
}

template <class T>
Matrix3<T> Matrix3<T>::operator-(const Matrix3<T>& m) const
{
    return Matrix3<T>(
            m_11 - m.m_11, m_12 - m.m_12, m_13 - m.m_13,
            m_21 - m.m_21, m_22 - m.m_22, m_23 - m.m_23,
            m_31 - m.m_31, m_32 - m.m_32, m_33 - m.m_33);
}

template <class T>
Matrix3<T> Matrix3<T>::operator-() const
{
    return Matrix3<T>(
            -m_11, -m_12, -m_13,
            -m_21, -m_22, -m_23,
            -m_31, -m_32, -m_33);
}

    template <class T>
const Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& m)
{
    *this = *this - m;
    return *this;
}

template <class T>
Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& m) const
{
    return Matrix3<T>(
            m_11 * m.m_11 + m_21 * m.m_12 + m_31 * m.m_12,
            m_11 * m.m_21 + m_21 * m.m_22 + m_31 * m.m_23,
            m_11 * m.m_31 + m_21 * m.m_32 + m_31 * m.m_33,
            m_12 * m.m_11 + m_22 * m.m_12 + m_32 * m.m_12,
            m_12 * m.m_21 + m_22 * m.m_22 + m_32 * m.m_23,
            m_12 * m.m_31 + m_22 * m.m_32 + m_32 * m.m_33,
            m_12 * m.m_11 + m_23 * m.m_12 + m_33 * m.m_12,
            m_12 * m.m_21 + m_23 * m.m_22 + m_33 * m.m_23,
            m_12 * m.m_31 + m_23 * m.m_32 + m_33 * m.m_33);
}

template <class T>
Matrix3<T> Matrix3<T>::operator*(const T& v) const
{
    return Matrix3<T>(
            m_11 * v, m_12 * v, m_13 * v,
            m_21 * v, m_22 * v, m_23 * v,
            m_31 * v, m_32 * v, m_33 * v);
}

    template <class T>
const Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T>& m)
{
    *this = *this * m;
    return *this;
}

    template <class T>
const Matrix3<T>& Matrix3<T>::operator*=(const T& v)
{
    *this = *this * v;
    return *this;
}

template <class T>
Matrix3<T> Matrix3<T>::operator/(const T& v) const
{
    return Matrix3<T>(
            m_11 / v, m_12 / v, m_13 / v,
            m_21 / v, m_22 / v, m_23 / v,
            m_31 / v, m_32 / v, m_33 / v);
}

    template <class T>
const Matrix3<T>& Matrix3<T>::operator/=(const T& v)
{
    *this = *this / v;
    return *this;
}

template <class T>
bool Matrix3<T>::operator==(const Matrix3<T>& m) const
{
    for(int i = 0; i < 9; ++i)
        if(m_values[i] != m.m_values[i])
            return false;

    return true;
}

template <class T>
bool Matrix3<T>::operator!=(const Matrix3<T>& m) const
{
    return !(*this == m);
}

    template <class T>
void Matrix3<T>::SetZero()
{
    *this = ZERO;
}

template <class T>
T Matrix3<T>::GetDeterminant() const
{
    return m_11 * (m_22 * m_33 - m_32 * m_23) -
        m_21 * (m_12 * m_33 - m_32 * m_13) +
        m_31 * (m_12 * m_23 - m_22 * m_13);
}

template <class T>
Matrix3<T> Matrix3<T>::GetCofactor() const
{
    Matrix3<T> result;

    result.m_11 = m_22 * m_33 - m_32 * m_23;
    result.m_12 = -(m_21 * m_33 - m_31 * m_23);
    result.m_13 = m_21 * m_32 - m_31 * m_22;
    result.m_21 = -(m_12 * m_33 - m_32 * m_13);
    result.m_22 = m_11 * m_33 - m_31 * m_13;
    result.m_23 = -(m_11 * m_32 - m_31 * m_12);
    result.m_31 = m_12 * m_23 - m_22 * m_13;
    result.m_32 = -(m_11 * m_23 - m_21 * m_13);
    result.m_33 = m_11 * m_22 - m_21 * m_12;

    return result;
}

    template <class T>
void Matrix3<T>::SetIdentity()
{
    *this = IDENTITY;
}

    template <class T>
void Matrix3<T>::Transpose()
{
    Swap(m_21, m_12);
    Swap(m_31, m_13);
    Swap(m_32, m_23);
}

    template <class T>
void Matrix3<T>::Invert()
{
    T det = GetDeterminant();
    assert(det != 0);

    *this = GetCofactor();
    Transpose();
    *this /= det;
}

    template <class T>
T* Matrix3<T>::GetInternalValues()
{
    return m_values;
}

template <class T>
const T* Matrix3<T>::GetInternalValues() const
{
    return m_values;
}

template <class T>
std::string Matrix3<T>::ToString(const std::string& lineBegin, const std::string& lineEnd) const
{
    std::ostringstream ss;
    ss << lineBegin << m_11 << " " << m_12 << " " << m_13 << lineEnd;
    ss << lineBegin << m_21 << " " << m_22 << " " << m_23 << lineEnd;
    ss << lineBegin << m_31 << " " << m_32 << " " << m_33 << lineEnd;

    return ss.str();
}

    template <class T>
void Matrix3<T>::Swap(T& v1, T& v2)
{
    T tmp = v1;
    v1 = v2;
    v2 = tmp;
}

#endif // MATRIX3_H__
