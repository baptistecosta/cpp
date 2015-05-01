#ifndef BCOSTA_CORE_MATH_MATRIX3_H
#define BCOSTA_CORE_MATH_MATRIX3_H

#include "math.h"

namespace BCosta
{
namespace Core
{
namespace Math
{
class Vector2;

class Vector3;

class Matrix4;

class Matrix3
{
public:

    float m[9];

    Matrix3()
    { }

    Matrix3(
        float m0, float m1, float m2,
        float m3, float m4, float m5,
        float m6, float m7, float m8
    )
    {
        m[0] = m0;
        m[1] = m1;
        m[2] = m2;
        m[3] = m3;
        m[4] = m4;
        m[5] = m5;
        m[6] = m6;
        m[7] = m7;
        m[8] = m8;
    }

    void LoadIdentity()
    { Set(1, 0, 0, 0, 1, 0, 0, 0, 1); }

    void Set(
        float m0, float m1, float m2,
        float m3, float m4, float m5,
        float m6, float m7, float m8
    )
    {
        m[0] = m0;
        m[1] = m1;
        m[2] = m2;
        m[3] = m3;
        m[4] = m4;
        m[5] = m5;
        m[6] = m6;
        m[7] = m7;
        m[8] = m8;
    }

    void Multiply(Matrix3 *b);

    static Matrix3 Translation(const float x, const float y);

    static Matrix3 Translation(Vector2 &v);

    Matrix3 Scale(const Vector3 &v);

    static Matrix3 RotationXAxis(const float a);

    static Matrix3 RotationYAxis(const float a);

    static Matrix3 RotationZAxis(const float a);

    static Matrix3 FromEuler(const Vector3 &euler, RotationOrder rot_order = RotOrder_Default);

    static Matrix3 FromEuler(const float x = 0, const float y = 0, const float z = 0,
                             RotationOrder rot_order = RotOrder_Default);

    Matrix3 FromMatrix4(const Matrix4 &mat);

    Matrix4 ToMatrix4();
};

}
}
}
#endif // BCOSTA_CORE_MATH_MATRIX3_H
