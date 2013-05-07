/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __BQUATERNION__
#define __BQUATERNION__

namespace owl {

class	Matrix3;
class	Matrix4;
class	Vector3;

//!	@class	Quaternion
class	Quaternion
{
public:

		float	x, y, z, w;

		Quaternion(float _x = 0, float _y = 0, float _z = 0, float _w = 1.f)
		{
			Set(_x, _y, _z, _w);
		}

const	Quaternion			operator *	(const Quaternion& b) const;
const	Quaternion			operator -	(const Quaternion &b) const;
		void				operator *= (const Quaternion &b);

		//---------------------------------------------------------------------
		//	Set quaternion's values.
		void				Set(float _x = 0, float _y = 0, float _z = 0, float _w = 1.f);

		//---------------------------------------------------------------------
		//	Return the normalize quaternion.
		Quaternion			Normalize();
		
		//---------------------------------------------------------------------
		//	Dot product.
		float				Dot(const Quaternion &b) const;

		//---------------------------------------------------------------------
		//	Return the conjugate quaternion.
		Quaternion			Conjugate();

		//---------------------------------------------------------------------
		//	Return the inverse of the quaternion.
		Quaternion			Inverse();

		//---------------------------------------------------------------------
		//	ToMatrix3 : Convert quaternion to matrix.
		//	ToAxisAngle : Convert quaternion to axis-angle.
		Matrix3			ToMatrix3();
		Matrix4			ToMatrix4();
		void				ToAxisAngle(Vector3* axis, float* angle);

		//---------------------------------------------------------------------
		//	FromAxisAngle : convert axis-angle to quaternion.
		//	FromMatrix3 : convert matrix to quaternion.
		//	FromEuler : convert from Euler Angles. Basically create 3 quaternions
		//	for pitch yaw & roll and multiply them together.
static	Quaternion			FromAxisAngle(float Q, float _x, float _y, float _z);
static	Quaternion			FromAxisAngle(float Q, Vector3& v);
static	Quaternion			FromMatrix3(const Matrix3& m);
static	Quaternion			FromEuler_ZYX(Vector3&);
static	Quaternion			FromEuler_ZYX(float pitch, float yaw, float roll);

		//---------------------------------------------------------------------
		//	Spherical linear interpolation.
static	const Quaternion	Slerp(Quaternion& a, Quaternion& b, float t);
};

}		// owl
#endif	// __BQUATERNION__