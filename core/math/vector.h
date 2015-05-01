#ifndef BCOSTA_CORE_MATH_VECTOR_H
#define BCOSTA_CORE_MATH_VECTOR_H

#include <math.h>

namespace BCosta
{
namespace Core
{
namespace Math
{
class Matrix4;

class Vector2
{
public:

    float x, y;

    Vector2()
    {
        x = 0.f;
        y = 0.f;
    }

    Vector2(float a, float b)
    {
        x = a;
        y = b;
    }

    Vector2(const Vector2 &v)
    {
        x = v.x;
        y = v.y;
    }

    Vector2 &operator =(const Vector2 &v)
    {
        x = v.x, y = v.y;
        return *this;
    }

    Vector2 operator +(const Vector2 &b)
    { return Vector2(x + b.x, y + b.y); }

    Vector2 operator +(const float &k)
    { return Vector2(x + k, y + k); }

    Vector2 operator -(const Vector2 &b)
    { return Vector2(x - b.x, y - b.y); }

    Vector2 operator -(const float &k)
    { return Vector2(x - k, y - k); }

    Vector2 operator *(const Vector2 &b)
    { return Vector2(x * b.x, y * b.y); }

    Vector2 operator *(const float &k)
    { return Vector2(x * k, y * k); }

    Vector2 operator /(const Vector2 &b)
    { return Vector2(x / b.x, y / b.y); }

    Vector2 operator /(const float &k)
    { return Vector2(x / k, y / k); }

    Vector2 operator -() const
    { return Vector2(-x, -y); }

    bool operator ==(const Vector2 &b)
    { return x == b.x && y == b.y; }

    bool operator !=(const Vector2 &b)
    { return x != b.x || y != b.y; }

    void operator +=(const Vector2 &b)
    {
        x += b.x;
        y += b.y;
    }

    void operator +=(const float &k)
    {
        x += k;
        y += k;
    }

    void operator -=(const Vector2 &b)
    {
        x -= b.x;
        y -= b.y;
    }

    void operator -=(const float &k)
    {
        x -= k;
        y -= k;
    }

    void operator *=(const Vector2 &b)
    {
        x *= b.x;
        y *= b.y;
    }

    void operator *=(const float &k)
    {
        x *= k;
        y *= k;
    }

    void Set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    float Lenght2()
    { return (float) (x * x + y * y); }

    float Lenght()
    { return sqrt((float) (x * x + y * y)); }

    Vector2 Reverse()
    { return Vector2(-x, -y); }

    void Normalize()
    {
        float l = Lenght();
        if (l) {
            x /= l;
            y /= l;
        }
    }

    void Clamp(const float l, const float h)
    {
        x = x < l ? l : (x > h ? h : x);
        y = y < l ? l : (y > h ? h : y);
    }

    void Zeroify()
    { x = y = 0.f; }

    static const Vector2 origin;

    static const Vector2 identity;

    static const float dot(const Vector2 &a, const Vector2 &b)
    { return a.x * b.x + a.y * b.y; }

    static const float dist(const Vector2 &a, const Vector2 &b)
    {
        return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    }
};

class Vector3
{
public:

    float x, y, z;

    Vector3()
    {
        x = 0.f;
        y = 0.f;
        z = 0.f;
    }

    Vector3(float v)
    {
        x = v;
        y = v;
        z = v;
    }

    Vector3(float a, float b, float c)
    {
        x = a;
        y = b;
        z = c;
    }

    Vector3(const Vector3 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Vector3 &operator =(const Vector3 &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    Vector3 operator +(const Vector3 &b)
    { return Vector3(x + b.x, y + b.y, z + b.z); }

    Vector3 operator +(const float &k)
    { return Vector3(x + k, y + k, z + k); }

    Vector3 operator -(const Vector3 &b)
    { return Vector3(x - b.x, y - b.y, z - b.z); }

    Vector3 operator -(const float &k)
    { return Vector3(x - k, y - k, z - k); }

    Vector3 operator *(const Vector3 &b)
    { return Vector3(x * b.x, y * b.y, z * b.z); }

    Vector3 operator *(const float &k)
    { return Vector3(x * k, y * k, z * k); }

    Vector3 operator /(const Vector3 &b)
    { return Vector3(x / b.x, y / b.y, z / b.z); }

    Vector3 operator /(const float &k)
    { return Vector3(x / k, y / k, z / k); }

    Vector3 operator -() const
    { return Vector3(-x, -y, -z); }

    bool operator ==(const Vector3 &b)
    { return x == b.x && y == b.y && z == b.z; }

    bool operator !=(const Vector3 &b)
    { return x != b.x || y != b.y || z != b.z; }

    void operator +=(const Vector3 &b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
    }

    void operator +=(const float &k)
    {
        x += k;
        y += k;
        z += k;
    }

    void operator -=(const Vector3 &b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
    }

    void operator -=(const float &k)
    {
        x -= k;
        y -= k;
        z -= k;
    }

    void operator *=(const Vector3 &b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;
    }

    void operator *=(const float &k)
    {
        x *= k;
        y *= k;
        z *= k;
    }

    void operator /=(const Vector3 &b)
    {
        x /= b.x;
        y /= b.y;
        z /= b.z;
    }

    void operator /=(const float &k)
    {
        x /= k;
        y /= k;
        z /= k;
    }

    Vector3 operator *(const Matrix4 &m);

    void operator *=(const Matrix4 &m);

    void Set(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void Abs()
    {
        if (x < 0) x *= -1;
        if (y < 0) y *= -1;
        if (z < 0) z *= -1;
    }

    Vector3 Cross(const Vector3 v)
    { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

    float Len()
    { return sqrt((float) (x * x + y * y + z * z)); }

    float Len2()
    { return (float) (x * x + y * y + z * z); }

    Vector3 &normalize();

    Vector3 Normalized();

    Vector3 Reverse()
    { return Vector3(-x, -y, -z); }

    void Zeroify()
    { x = y = z = 0.f; }

    void Log(const char *v = "V");

    static const Vector3 origin;

    static const Vector3 identity;

    static const void Cross(Vector3 &r, const Vector3 &a, const Vector3 &b);

    static const float Dot(const Vector3 &a, const Vector3 &b)
    { return a.x * b.x + a.y * b.y + a.z * b.z; }

    static const float Dist(const Vector3 &a, const Vector3 &b)
    {
        return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
    }
};

}
}
}
#endif // BCOSTA_CORE_MATH_VECTOR_H
