/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "matrix4.h"
	#include "core/log.h"
	#include "matrix3.h"
	#include "quaternion.h"
	#include "typedefs.h"

	using namespace owl;
	using namespace owl::math;

//-----------------------------------------------------------------------------
Matrix4			Matrix4::static_identity(1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1);

//-----------------------------------------------------------------------------
void			Matrix4::LoadIdentity()
{	Set (1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  0, 0, 0, 1);   }

//-----------------------------------------------------------------------------
void			Matrix4::Set	(	
									 float m0,  float m1,  float m2,  float m3,
									 float m4,  float m5,  float m6,  float m7,
									 float m8,  float m9,  float m10, float m11,
									 float m12, float m13, float m14, float m15
								)
{
	m[0]  = m0;  m[1]  = m1;  m[2]  = m2;  m[3]  = m3;
	m[4]  = m4;  m[5]  = m5;  m[6]  = m6;  m[7]  = m7;
	m[8]  = m8;  m[9]  = m9;  m[10] = m10; m[11] = m11;
	m[12] = m12; m[13] = m13; m[14] = m14; m[15] = m15;
}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::Transpose() const
{
	return Matrix4	(	m[0], m[4], m[8],  m[12], 
						m[1], m[5], m[9],  m[13], 
						m[2], m[6], m[10], m[14], 
						m[3], m[7], m[11], m[15]	);
}

//-----------------------------------------------------------------------------
void			Matrix4::Multiply(Matrix4* b)
{
	Matrix4		r (0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0);

	for (int k = 0; k < 4; k++)
		for (int j = 0; j < 4; j++)
			for (int i = 0; i < 4; i++)
				r.m[4 * j + k] += m[4 * j + i] * b->m[4 * i + k];

	for (int i = 0; i < 16; i++)
		m[i] = r.m[i];
}

//-----------------------------------------------------------------------------
Matrix4		Matrix4::Translation(const float x, const float y, const float z)
{
	return Matrix4	(	1, 0, 0, x,
						0, 1, 0, y,
						0, 0, 1, z,
						0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
Matrix4		Matrix4::Translation(const Vector3 &v)
{	return Matrix4::Translation(v.x, v.y, v.z);	}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::Scale(const float x, const float y, const float z)
{
	return Matrix4	(	x, 0, 0, 0,
						0, y, 0, 0,
						0, 0, z, 0,
						0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::Scale(const float s)
{	return Matrix4::Scale(s, s, s);	}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::Scale(const Vector3 &v)
{	return Matrix4::Scale(v.x, v.y, v.z);	}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::RotationXAxis(const float _a)
{
	const float a = radians(_a);
	return Matrix4	(	1, 0, 0, 0,
						0, cos(a), sin(a), 0,
						0, -sin(a), cos(a), 0,
						0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::RotationYAxis(const float _a)
{
	const float a = radians(_a);
	return Matrix4	(	cos(a), 0, -sin(a), 0,
						0, 1, 0, 0,
						sin(a), 0, cos(a), 0,
						0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
const Matrix4	Matrix4::RotationZAxis(const float _a)
{
	const float a = radians(_a);
	return Matrix4	(	cos(a), sin(a), 0, 0,
						-sin(a), cos(a), 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
Matrix4		Matrix4::Transform(Quaternion& rotation, Vector3& translation, float scale)
{	
	Matrix4 s = Matrix4::Scale(scale);
	Matrix4 q = rotation.ToMatrix4();
	Matrix4 t = Matrix4::Translation(translation);

	return Matrix4(q * t * s);
}

//-----------------------------------------------------------------------------
Matrix4		Matrix4::Transform(const Vector3& translation, const Vector3& rotation, const Vector3& scale)
{
	Matrix4 t = Matrix4::Translation(translation);
	Matrix4 r = (Matrix3::FromEuler(rotation)).ToMatrix4();
	Matrix4 s = Matrix4::Scale(scale);

	return Matrix4( t * r * s );
}

//-----------------------------------------------------------------------------
float			Matrix4::Determinant()
{
	return	(		m[0] * m[5] * m[10] * m[15] + m[0] * m[6] * m[11] * m[13] + m[0] * m[7] * m[9] * m[14]
				+	m[1] * m[4] * m[11] * m[14] + m[1] * m[6] * m[8] * m[15] + m[1] * m[7] * m[10] * m[12]
				+	m[2] * m[4] * m[9] * m[15]	 + m[2] * m[5] * m[11] * m[12] + m[2] * m[7] * m[8] * m[13]
				+	m[3] * m[4] * m[10] * m[13] + m[3] * m[5] * m[8] * m[14] + m[3] * m[6] * m[9] * m[12]
				-	m[0] * m[5] * m[11] * m[14] + m[0] * m[6] * m[9] * m[15] + m[0] * m[7] * m[10] * m[13]
				-	m[1] * m[4] * m[10] * m[15] + m[1] * m[6] * m[11] * m[12] + m[1] * m[7] * m[8] * m[14]
				-	m[2] * m[4] * m[11] * m[13] + m[2] * m[5] * m[8] * m[15] + m[2] * m[7] * m[9] * m[12]
				-	m[3] * m[4] * m[9] * m[14] + m[3] * m[5] * m[10] * m[12] + m[3] * m[6] * m[8] * m[13]	);
}

//-----------------------------------------------------------------------------
Matrix4		Matrix4::Inverse()
{
	Matrix4 dst;
	float tmp[12];
	float src[16];
	
	for (uint i = 0; i < 4; i++)
	{
		src[i] = m[i * 4];
		src[i + 4] = m[i * 4 + 1];
		src[i + 8] = m[i * 4 + 2];
		src[i + 12] = m[i * 4 + 3];
	}

	// calculate pairs for first 8 elements (cofactors)
	tmp[0] = src[10] * src[15];		tmp[1] = src[11] * src[14];		tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];		tmp[4] = src[9] * src[14];		tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];		tmp[7] = src[11] * src[12];		tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];		tmp[10] = src[8] * src[13];		tmp[11] = src[9] * src[12];

	// calculate first 8 elements (cofactors)
	dst.m[0] = (tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7]) - (tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7]);
	dst.m[1] = (tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7]) - (tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7]);
	dst.m[2] = (tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7]) - (tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7]);
	dst.m[3] = (tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6]) - (tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6]);
	dst.m[4] = (tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3]) - (tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3]);
	dst.m[5] = (tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3]) - (tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3]);
	dst.m[6] = (tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3]) - (tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3]);
	dst.m[7] = (tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2]) - (tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2]);

	// calculate pairs for second 8 elements (cofactors)
	tmp[0] = src[2] * src[7];		tmp[1] = src[3] * src[6];		tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];		tmp[4] = src[1] * src[6];		tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];		tmp[7] = src[3] * src[4];		tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];		tmp[10] = src[0] * src[5];		tmp[11] = src[1] * src[4];

	// calculate second 8 elements (cofactors)
	dst.m[8] = (tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15]) - (tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15]);
	dst.m[9] = (tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15]) - (tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15]);
	dst.m[10] = (tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15]) - (tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15]);
	dst.m[11] = (tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14]) - (tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14]);
	dst.m[12] = (tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9]) - (tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10]);
	dst.m[13] = (tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10]) - (tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8]);
	dst.m[14] = (tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8]) - (tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9]);
	dst.m[15] = (tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9]) - (tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8]);

	// calculate determinant
	float det = src[0] * dst.m[0] + src[1] * dst.m[1] + src[2] * dst.m[2] + src[3] * dst.m[3];

	// calculate matrix inverse
	det = 1 / det;

	for (uint i = 0; i < 16; i++)
		dst.m[i] *= det;

	return dst;
}

//---------------------------------------------------------------------------
void			Matrix4::Log()
{
	Log::i("%.02f, %.02f, %.02f, %.02f", m[0], m[1], m[2], m[3]);
	Log::i("%.02f, %.02f, %.02f, %.02f", m[4], m[5], m[6], m[7]);
	Log::i("%.02f, %.02f, %.02f, %.02f", m[8], m[9], m[10], m[11]);
	Log::i("%.02f, %.02f, %.02f, %.02f", m[12], m[13], m[14], m[15]);
	Log::NewLine();
}
