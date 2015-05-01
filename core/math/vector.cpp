#include "vector.h"
#include "matrix4.h"

using namespace BCosta::Core::Math;

const Vector2 Vector2::origin(0.f, 0.f);

const Vector2 Vector2::identity(1.f, 1.f);

const Vector3 Vector3::origin(0.f, 0.f, 0.f);

const Vector3 Vector3::identity(1.f, 1.f, 1.f);

Vector3 Vector3::operator *(const Matrix4 &m)
{
    return Vector3(
        x * m.m[0] + y * m.m[1] + z * m.m[2] + m.m[3],
        x * m.m[4] + y * m.m[5] + z * m.m[6] + m.m[7],
        x * m.m[8] + y * m.m[9] + z * m.m[10] + m.m[11]
    );
}

void Vector3::operator *=(const Matrix4 &m)
{
    float _x = x, _y = y, _z = z;
    x = _x * m.m[0] + _y * m.m[1] + _z * m.m[2] + m.m[3];
    y = _x * m.m[4] + _y * m.m[5] + _z * m.m[6] + m.m[7];
    z = _x * m.m[8] + _y * m.m[9] + _z * m.m[10] + m.m[11];
}

Vector3 Vector3::Normalized()
{
    float l = 1.f / Len();
    return Vector3(x * l, y * l, z * l);
}

Vector3 &Vector3::normalize()
{
    float l = Len();
    if (l) {
        x /= l, y /= l, z /= l;
    }
    return *this;
}

const void Vector3::Cross(Vector3 &r, const Vector3 &a, const Vector3 &b)
{
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
}