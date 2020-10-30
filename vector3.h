#ifndef TAK__VECTOR3_H__
#define TAK__VECTOR3_H__

#include <iostream>
#include <cmath>
//#include <SFML/System.hpp>


template <class T>
class Vector3
{
    public:
        Vector3(const T& x = 0, const T& y = 0, const T& z = 0);

        Vector3<T> operator+(const Vector3<T>& v) const;
        Vector3<T> operator-(const Vector3<T>& v) const;
        Vector3<T> operator-() const;
        Vector3<T> operator+(const T& v) const;
        Vector3<T> operator-(const T& v) const;
        Vector3<T> operator/(const T& v) const;
        Vector3<T> operator*(const T& v) const;

        Vector3<T>& operator=(const Vector3<T>& v);

        Vector3<T>& operator+=(const Vector3<T>& v);
        Vector3<T>& operator-=(const Vector3<T>& v);
        Vector3<T>& operator+=(const T& v);
        Vector3<T>& operator-=(const T& v);
        Vector3<T>& operator/=(const T& v);
        Vector3<T>& operator*=(const T& v);

        bool operator==(const Vector3<T>& v) const;
        bool operator!=(const Vector3<T>& v) const;

        T Length() const;
        T Dot(const Vector3<T>& v) const;
        Vector3<T> Cross(const Vector3<T>& v) const;
        T GetAngle(const Vector3<T>& v) const;
        void Normalize();
        void Zero();

        void RotateAroundAxis(const Vector3<T>& axis, const T& angle);

    public:
        T x, y, z;
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
Vector3<T>::Vector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z)
{
}

template <class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& v) const
{
	return Vector3<T>(x + v.x, y + v.y, z + v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& v) const
{
	return Vector3<T>(x - v.x, y - v.y, z - v.z);
}

template <class T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(-x, -y, -z);
}

template <class T>
Vector3<T> Vector3<T>::operator+(const T& v) const
{
	return Vector3<T>(x + v, y + v, z + v);
}

template <class T>
Vector3<T> Vector3<T>::operator-(const T& v) const
{
	return Vector3<T>(x - v, y - v, z - v);
}

template <class T>
Vector3<T> Vector3<T>::operator/(const T& v) const
{
	return Vector3<T>(x / v, y / v, z / v);
}

template <class T>
Vector3<T> Vector3<T>::operator*(const T& v) const
{
	return Vector3<T>(x * v, y * v, z * v);
}

template <class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

template <class T>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& v)
{
	return (*this = *this + v);
}

template <class T>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& v)
{
	return (*this = *this - v);
}

template <class T>
Vector3<T>& Vector3<T>::operator+=(const T& v)
{
	return (*this = *this + v);
}

template <class T>
Vector3<T>& Vector3<T>::operator-=(const T& v)
{
	return (*this = *this - v);
}

template <class T>
Vector3<T>& Vector3<T>::operator/=(const T& v)
{
	return (*this = *this / v);
}

template <class T>
Vector3<T>& Vector3<T>::operator*=(const T& v)
{
	return (*this = *this * v);
}

template <class T>
bool Vector3<T>::operator==(const Vector3<T>& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

template <class T>
bool Vector3<T>::operator!=(const Vector3<T>& v) const
{
	return !(*this == v);
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
        T inv = T(1) / n;
        this->x *= inv;
        this->y *= inv;
        this->z *= inv;
    }
}

    template <class T>
void Vector3<T>::Zero()
{
    this->x = this->y = this->z = 0;

}

    template <class T>
void Vector3<T>::RotateAroundAxis(const Vector3<T>& axis, const T& angle)
{
    // http://stackoverflow.com/questions/3982877/opengl-rotation-of-an-object-around-a-line/4240058#4240058
    Vector3<T> v = *this;
    *this = ((v - axis * (axis * v)) * cos(DEGTORAD(angle))) + (axis.DotProduct(v) * sin(DEGTORAD(angle))) + (axis * (axis * v));
}

#endif // VECTOR3_H__
