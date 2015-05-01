#ifndef BCOSTA_CORE_MATH_MATRIX4_H
#define BCOSTA_CORE_MATH_MATRIX4_H

#include "math.h"
#include "vector.h"

namespace BCosta
{
namespace Core
{
namespace Math
{
class Quaternion;

// A normal matrix is the inverse transpose of the upper-left 3x3 portion of the model-view matrix.
class Matrix4
{
public:

    static Matrix4 static_identity;

    float m[16];

    Matrix4()
    { }

    Matrix4(
        float m0, float m1, float m2, float m3,
        float m4, float m5, float m6, float m7,
        float m8, float m9, float m10, float m11,
        float m12, float m13, float m14, float m15
    )
    {
        Set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);
    }

    void operator *=(const Matrix4 &b)
    { *this = *this * b; }

    Matrix4 operator *(const Matrix4 &b)
    {
        return Matrix4(
            m[0] * b.m[0] + m[1] * b.m[4] + m[2] * b.m[8] + m[3] * b.m[12],
            m[0] * b.m[1] + m[1] * b.m[5] + m[2] * b.m[9] + m[3] * b.m[13],
            m[0] * b.m[2] + m[1] * b.m[6] + m[2] * b.m[10] + m[3] * b.m[14],
            m[0] * b.m[3] + m[1] * b.m[7] + m[2] * b.m[11] + m[3] * b.m[15],

            m[4] * b.m[0] + m[5] * b.m[4] + m[6] * b.m[8] + m[7] * b.m[12],
            m[4] * b.m[1] + m[5] * b.m[5] + m[6] * b.m[9] + m[7] * b.m[13],
            m[4] * b.m[2] + m[5] * b.m[6] + m[6] * b.m[10] + m[7] * b.m[14],
            m[4] * b.m[3] + m[5] * b.m[7] + m[6] * b.m[11] + m[7] * b.m[15],

            m[8] * b.m[0] + m[9] * b.m[4] + m[10] * b.m[8] + m[11] * b.m[12],
            m[8] * b.m[1] + m[9] * b.m[5] + m[10] * b.m[9] + m[11] * b.m[13],
            m[8] * b.m[2] + m[9] * b.m[6] + m[10] * b.m[10] + m[11] * b.m[14],
            m[8] * b.m[3] + m[9] * b.m[7] + m[10] * b.m[11] + m[11] * b.m[15],

            m[12] * b.m[0] + m[13] * b.m[4] + m[14] * b.m[8] + m[15] * b.m[12],
            m[12] * b.m[1] + m[13] * b.m[5] + m[14] * b.m[9] + m[15] * b.m[13],
            m[12] * b.m[2] + m[13] * b.m[6] + m[14] * b.m[10] + m[15] * b.m[14],
            m[12] * b.m[3] + m[13] * b.m[7] + m[14] * b.m[11] + m[15] * b.m[15]
        );
    }

    void LoadIdentity();

    void Set(
        float m0, float m1, float m2, float m3,
        float m4, float m5, float m6, float m7,
        float m8, float m9, float m10, float m11,
        float m12, float m13, float m14, float m15
    );

    float Determinant();

    Matrix4 Inverse();

    const Matrix4 Transpose() const;

    void Multiply(Matrix4 *b);

    static Matrix4 Translation(const float x, const float y, const float z);

    static Matrix4 Translation(const Vector3 &v);

    static const Matrix4 Scale(const float x, const float y, const float z);

    static const Matrix4 Scale(const Vector3 &v);

    static const Matrix4 Scale(const float s);

    static const Matrix4 RotationXAxis(const float a);

    static const Matrix4 RotationYAxis(const float a);

    static const Matrix4 RotationZAxis(const float a);

    static Matrix4 Transform(Quaternion &rotation, Vector3 &translation, float scale);

    static Matrix4 Transform(const Vector3 &translation, const Vector3 &rotation, const Vector3 &scale);
};

}
}
}
#endif // BCOSTA_CORE_MATH_MATRIX4_H