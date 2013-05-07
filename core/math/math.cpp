/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "math.h"
	#include "matrix4.h"

using namespace owl;
using namespace owl::math;

//-----------------------------------------------------------------------------
const float		math::radians(const float degrees)
{	return degrees / 180.f * pi;	}
const float		math::degrees(const float radians)
{	return radians / pi * 180.f;	}

//-----------------------------------------------------------------------------
void			math::persp(Matrix4& m, const float fov, const float ratio, const float z_near, const float z_far)
{
	const float q = radians(fov);
	const float f = 1 / tan (q / 2);

	m *= Matrix4(	f / ratio, 0, 0, 0,
					0, f, 0, 0,
					0, 0, (z_near + z_far) / (z_near - z_far), (2 * z_far * z_near) / (z_near - z_far),
					0, 0, -1, 0		);
}

//-----------------------------------------------------------------------------
void			math::ortho(Matrix4& m, const float l, const float r, const float b, const float t, const float n, const float f)
{
	const float	q = 1.f / (f - n);

	m *= Matrix4(	2 / (r - l), 0, 0, 0,
					0, 2 / (t - b), 0, 0,
					0, 0, -2 * q, -q * n,
					0, 0, 0, 1	);
}

//-----------------------------------------------------------------------------
void			math::lookAt(Matrix4& m, Vector3& pos, const Vector3& dir, const Vector3& up)
{
	Vector3 center = pos + dir;
	Vector3 regard(center.x - pos.x, center.y - pos.y, center.z - pos.z);
	Vector3 normal(0.f, 0.f, 0.f);
	Vector3 new_axe(0.f, 0.f, 0.f);

	Vector3::Cross(normal, regard, -up);
	Vector3::Cross(new_axe, normal, regard);

	normal.normalize();
	new_axe.normalize();
	regard.normalize();

	Matrix4 mat(	normal.x,	normal.y,	normal.z,	0.f,
					new_axe.x,	new_axe.y,	new_axe.z,	0.f,
					-regard.x,	-regard.y,	-regard.z,	0.f,
					0.f,		0.f,		0.f,		1.f		);

	m = m * mat * Matrix4::Translation(pos.Reverse());
}
