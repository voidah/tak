#ifndef VECTOR3_H__
#define VECTOR3_H__

#include <iostream>
#include <cmath>
#include <SFML/System.hpp>


template <class T>
class Vector3 : public sf::Vector3<T>
{
    public:
    Vector3(const T& x = 0, const T& y = 0, const T& z = 0);

    Vector3<T> operator+(const Vector3<T>& v) const;
    Vector3<T> operator+(const T& v) const;
    Vector3<T> operator-(const Vector3<T>& v) const;
    Vector3<T> operator*(const Vector3<T>& v) const;
    Vector3<T> operator*(const T& v) const;
    T Length() const;
    T Dot(const Vector3<T>& v) const;
    Vector3<T> Cross(const Vector3<T>& v) const;
    T GetAngle(const Vector3<T>& v) const;
    void Normalize();
    void Zero();
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

template <class T>
std::ostream& operator<<(std::ostream& out, const Vector3<T>& v)
{
    out << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return out;
}


template <class T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : sf::Vector3<T>(x, y, z)
{
}

template <class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
    return Vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator+(const T& v) const
{
    return Vector3<T>(this->x + v, this->y + v, this->z + v);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const
{
    return Vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator*(const Vector3<T>& v) const
{
    return Vector3<T>(this->x * v.x, this->y * v.y, this->z * v.z);
}


template <class T>
Vector3<T> Vector3<T>::operator*(const T& v) const
{
    return Vector3<T>(this->x * v, this->y * v, this->z * v);
}


template <class T>
T Vector3<T>::Length() const
{
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

template <class T>
T Vector3<T>::Dot(const Vector3<T>& v) const
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
}

template <class T>
Vector3<T> Vector3<T>::Cross(const Vector3<T>& v) const
{
    return Vector3<T>(this->y * v.z - v.y * this->z,
                      this->z * v.x - v.z * this->x,
                      this->x * v.y - v.x * this->y);
}

template <class T>
T Vector3<T>::GetAngle(const Vector3<T>& v) const
{
    T nm = Length() * v.Length();
    if(nm == 0)
        return 0;

    return acos(Dot(v) / nm);
}

template <class T>
void Vector3<T>::Normalize()
{
    T n = Length();
    if(n != 0)
    {
        this->x /= n;
        this->y /= n;
        this->z /= n;
    }
}

template <class T>
void Vector3<T>::Zero()
{
	this->x = this->y = this->z = 0;

}

#endif // VECTOR3_H__
