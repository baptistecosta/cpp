#include "matrix3.h"
#include "matrix4.h"
#include "vector.h"

using namespace BCosta::Core::Math;

void Matrix3::Multiply(Matrix3 *b)
{
    Matrix3 r(0, 0, 0, 0, 0, 0, 0, 0, 0);

    for (int k = 0; k < 3; k++) {
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 3; i++) {
                r.m[3 * j + k] += m[3 * j + i] * b->m[3 * i + k];
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        m[i] = r.m[i];
    }
}

Matrix3 Matrix3::Translation(const float x, const float y)
{ return Matrix3(1, 0, 0, 0, 1, 0, x, y, 1); }

Matrix3 Matrix3::Translation(Vector2 &v)
{ return Matrix3(1, 0, 0, 0, 1, 0, v.x, v.y, 1); }

Matrix3 Matrix3::Scale(const Vector3 &v)
{ return Matrix3(v.x, 0, 0, 0, v.y, 0, 0, 0, v.z); }

Matrix3 Matrix3::RotationXAxis(float a)
{
    return Matrix3(
        1, 0, 0,
        0, cos(a), sin(a),
        0, -sin(a), cos(a)
    );
}

Matrix3 Matrix3::RotationYAxis(float a)
{
    return Matrix3(
        cos(a), 0, -sin(a),
        0, 1, 0,
        sin(a), 0, cos(a)
    );
}

Matrix3 Matrix3::RotationZAxis(float a)
{
    return Matrix3(
        cos(a), sin(a), 0,
        -sin(a), cos(a), 0,
        0, 0, 1
    );
}

Matrix3 Matrix3::FromEuler(const Vector3 &euler, RotationOrder rot_order)
{ return Matrix3::FromEuler(euler.x, euler.y, euler.z, rot_order); }

Matrix3 Matrix3::FromEuler(const float x, const float y, const float z, RotationOrder rot_order)
{
    const float c_x = cos(radians(x)),
        c_y = cos(radians(y)),
        c_z = cos(radians(z)),
        s_x = sin(radians(x)),
        s_y = sin(radians(y)),
        s_z = sin(radians(z));

    switch (rot_order) {
        case RotOrder_XZY:
            return Matrix3(
                c_y * c_z, s_x * s_y + c_x * c_y * s_z, -c_x * s_y + c_y * s_x * s_z,
                -s_z, c_x * c_z, c_z * s_x,
                c_z * s_y, -c_y * s_x + c_x * s_y * s_z, c_x * c_y + s_x * s_y * s_z
            );
        case RotOrder_ZYX:
            return Matrix3(
                c_y * c_z, c_y * s_z, -s_y,
                c_z * s_x * s_y - c_x * s_z, c_x * c_z + s_x * s_y * s_z, c_y * s_x,
                c_x * c_z * s_y + s_x * s_z, -c_z * s_x + c_x * s_y * s_z, c_x * c_y
            );

        case RotOrder_XYZ:
            return Matrix3(
                c_y * c_z, c_z * s_x * s_y + c_x * s_z, -c_x * c_z * s_y + s_x * s_z,
                -c_y * s_z, c_x * c_z - s_x * s_y * s_z, c_z * s_x + c_x * s_y * s_z,
                s_y, -c_y * s_x, c_x * c_y
            );

        case RotOrder_ZXY:
            return Matrix3(
                c_y * c_z - s_x * s_y * s_z, c_z * s_x * s_y + c_y * s_z, -c_x * s_y,
                -c_x * s_z, c_x * c_z, s_x,
                c_z * s_y + c_y * s_x * s_z, -c_y * c_z * s_x + s_y * s_z, c_x * c_y
            );

        case RotOrder_YZX:
            return Matrix3(
                c_y * c_z, s_z, -c_z * s_y,
                s_x * s_y - c_x * c_y * s_z, c_x * c_z, c_y * s_x + c_x * s_y * s_z,
                c_x * s_y + c_y * s_x * s_z, -c_z * s_x, c_x * c_y - s_x * s_y * s_z
            );

        case RotOrder_YXZ:
            return Matrix3(
                c_y * c_z + s_x * s_y * s_z, c_x * s_z, -c_z * s_y + c_y * s_x * s_z,
                c_z * s_x * s_y - c_y * s_z, c_x * c_z, c_y * c_z * s_x + s_y * s_z,
                c_x * s_y, -s_x, c_x * c_y
            );
    }
}

Matrix3 Matrix3::FromMatrix4(const Matrix4 &mat)
{
    return Matrix3(
        mat.m[0], mat.m[1], mat.m[2],
        mat.m[3], mat.m[4], mat.m[5],
        mat.m[6], mat.m[7], mat.m[8]
    );
}

Matrix4 Matrix3::ToMatrix4()
{
    return Matrix4(
        m[0], m[1], m[2], 0,
        m[3], m[4], m[5], 0,
        m[6], m[7], m[8], 0,
        0, 0, 0, 1
    );
}
