/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/
	
	#include <math.h>
	#include "quaternion.h"
	#include "matrix3.h"
	#include "matrix4.h"
	#include "vector.h"

using namespace owl;
using namespace owl::math;

//-----------------------------------------------------------------------------
const Quaternion	Quaternion::operator * (const Quaternion& b) const
{
	return	Quaternion	(	w * b.x + x * b.w + y * b.z - z * b.y,
							w * b.y - x * b.z + y * b.w + z * b.x,
							w * b.z + x * b.y - y * b.x + z * b.w,
							w * b.w - x * b.x - y * b.y - z * b.z	);
}

//-----------------------------------------------------------------------------
const Quaternion	Quaternion::operator - (const Quaternion &b) const
{	return Quaternion(x - b.x, y - b.y, z - b.z, w - b.w);	}

//-----------------------------------------------------------------------------
void				Quaternion::operator *= (const Quaternion &b)
{
	Quaternion		t = *this;
	w = t.w * b.w - (t.x * b.x + t.y * b.y + t.z * b.z);
	x = t.w * b.x + b.w * t.x + t.y * b.z - t.z * b.y;
	y = t.w * b.y + b.w * t.y + t.z * b.x - t.x * b.z;
	z = t.w * b.z + b.w * t.z + t.x * b.y - t.y * b.x;
}

//-----------------------------------------------------------------------------
void				Quaternion::Set(float _x, float _y, float _z, float _w)
{	x = _x; y = _y; z = _z; w = _w;	}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::Normalize()
{
	const float d = sqrt(x * x + y * y + z * z + w * w);
	const float k = 1.f / d;
	return Quaternion(x * k, y * k, z * k, w * k);
}

//-----------------------------------------------------------------------------
float				Quaternion::Dot(const Quaternion &b) const
{	return x * b.x + y * b.y + z * b.z + w * b.w;	}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::Conjugate()
{	return Quaternion(-x, -y, -z, w);	}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::Inverse()
{
	const float	norm_sqr = x * x + y * y + z * z + w * w;
	if (norm_sqr > 0)
	{
		const float inorm_sqr = 1.f / norm_sqr;
		return Quaternion(x * -inorm_sqr, y * -inorm_sqr, z * -inorm_sqr, w * inorm_sqr);
	}
	return *this;
}

//-----------------------------------------------------------------------------
Matrix3			Quaternion::ToMatrix3()
{
	const float	x_x = x * x,
				x_y = x * y,
				x_z = x * z,
				x_w = x * w, 
				y_y = y * y,
				y_z = y * z,
				y_w = y * w,
				z_z = z * z,
				z_w = z * w;

	return Matrix3	(	1.f - 2.f * (y_y + z_z)	, 2.f * (x_y - z_w)			, 2.f * (x_z + y_w),
						2.f * (x_y + z_w)		, 1.f - 2.f * (x_x + z_z)	, 2.f * (y_z - x_w),
						2.f * (x_z - y_w)		, 2.f * (y_z + x_w)			, 1.f - 2.f * (x_x + y_y)	);
}

//-----------------------------------------------------------------------------
Matrix4			Quaternion::ToMatrix4()
{
	const float	x_x = x * x,
				x_y = x * y,
				x_z = x * z,
				x_w = x * w, 
				y_y = y * y,
				y_z = y * z,
				y_w = y * w,
				z_z = z * z,
				z_w = z * w;

	return Matrix4	(	1.f - 2.f * (y_y + z_z),	2.f * (x_y - z_w),			2.f * (x_z + y_w),			0,
						2.f * (x_y + z_w),			1.f - 2.f * (x_x + z_z),	2.f * (y_z - x_w),			0, 
						2.f * (x_z - y_w),			2.f * (y_z + x_w),			1.f - 2.f * (x_x + y_y),	0,
						0,							0,							0,							1	);
}

//-----------------------------------------------------------------------------
void				Quaternion::ToAxisAngle(Vector3* axis, float* angle)
{
	float scale = sqrt(x * x + y * y + z * z);
	axis->x = x / scale;
	axis->y = y / scale;
	axis->z = z / scale;
	*angle = acos(w) * 2.f;
}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::FromAxisAngle(float q, float _x, float _y, float _z)
{
	const float	s = sin(q / 2);
	const float	c = cos(q / 2);
	return Quaternion(_x * s, _y * s, _z * s, c).Normalize();
}
//-----------------------------------------------------------------------------
Quaternion			Quaternion::FromAxisAngle(float q, Vector3& v)
{	return Quaternion::FromAxisAngle(q, v.x, v.y, v.z);	}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::FromMatrix3(const Matrix3& m)
{
	float _x, _y, _z, _w;
	const float	trace = m.m[0] + m.m[4] + m.m[8];

	if (trace > 0.f)
	{
		const float scale = 2.f * sqrt(trace);
		_x = (m.m[5] - m.m[7]) / scale;
		_y = (m.m[6] - m.m[2]) / scale;
		_z = (m.m[1] - m.m[3]) / scale;
		_w = 0.25f * scale;
	}

	else if (m.m[0] > m.m[4] && m.m[0] > m.m[8])
	{
		const float	scale = sqrt(1.f + m.m[0] - m.m[4] - m.m[8]) * 2.f;
		_x = 0.25f * scale;
		_y = (m.m[1] + m.m[3]) / scale;
		_z = (m.m[6] + m.m[2]) / scale;
		_w = (m.m[5] - m.m[7]) / scale;
	}

	else if (m.m[4] > m.m[8])
	{
		const float	scale = sqrt(1.f + m.m[4] - m.m[0] - m.m[8]) * 2.f;
		_x = (m.m[1] + m.m[3]) / scale;
		_y = 0.25f * scale;
		_z = (m.m[5] + m.m[7]) / scale;
		_w = (m.m[6] - m.m[2]) / scale;
	}

	else
	{
		const float	scale = sqrt(1.f + m.m[8] - m.m[0] - m.m[4]) * 2.f;
		_x = (m.m[6] + m.m[2]) / scale;
		_y = (m.m[5] + m.m[7]) / scale;
		_z = 0.25f * scale;
		_w = (m.m[1] - m.m[3]) / scale;
	}

	return Quaternion(_x, _y, _z, _w).Normalize();
}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::FromEuler_ZYX(Vector3& v)
{	return Quaternion::FromEuler_ZYX(v.x, v.y, v.z);	}

//-----------------------------------------------------------------------------
Quaternion			Quaternion::FromEuler_ZYX(float pitch, float yaw, float roll)
{
	Quaternion	qx(Quaternion::FromAxisAngle(radians(pitch), 1, 0, 0)),
				qy(Quaternion::FromAxisAngle(radians(yaw), 0, 1, 0)),
				qz(Quaternion::FromAxisAngle(radians(roll), 0, 0, 1)),
				q;
	
	q = qz * qy * qx;
	return q.Normalize();
}

//-----------------------------------------------------------------------------
const Quaternion	Quaternion::Slerp(Quaternion& a, Quaternion& b, float t)
{
	float cosQ = a.Dot(b);

	if (cosQ < 0.f)
	{
		b.x = -b.x; b.y = -b.y; b.z = -b.z; b.w = -b.w;
		cosQ = -cosQ;
	}
	
	float k0, k1;

	// Lerp
	if (cosQ > 0.9999f)
	{
		k0 = 1.f - t;
		k1 = t;
	}

	// Slerp
	else
	{
		float	sinQ = sqrt(1.f - cosQ * cosQ),
				Q = atan2(sinQ, cosQ),
				inv_sinQ = 1.f / sinQ;

		k0 = sin((1.f - t) * Q) * inv_sinQ;
		k1 = sin(t * Q) * inv_sinQ;
	}
	// Interpolation
	return Quaternion(a.x * k0 + b.x * k1, a.y * k0 + b.y * k1, a.z * k0 + b.z * k1, a.w * k0 + b.w * k1);
}

