/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __B_MATH__
#define __B_MATH__

namespace owl {

class	Matrix4;
class	Vector3;

namespace math {

enum	RotationOrder
{
	RotOrder_ZYX		=	0
,	RotOrder_YZX
,	RotOrder_ZXY
,	RotOrder_XZY
,	RotOrder_YXZ
,	RotOrder_XYZ
,	RotOrder_Default	=	RotOrder_YXZ
};

const	float		pi = 3.1415926535897932f;
const	float		pi2 = pi / 2.f;

const	float		radians(const float degrees);
const	float		degrees(const float radians);
	
template <typename T> bool IsEqual(T v1, T v2)
{	return fabs(v1 - v2) < 0.01f;   }
template <typename T> const bool Max(const T& a, const T& b)
{	return a < b ? b : a;	}
template <typename T> const bool Min(const T& a, const T& b)
{	return a > b ? b : a;	}
template <typename T> T	Clamp(const T& v, const T& l, const T& h)
{	return v < l ? l : (v > h ? h : v);	}

// Projection
void				persp(Matrix4&, const float fov, const float ratio, const float z_near, const float z_far);
void				ortho(Matrix4&, const float l, const float r, const float b, const float t, const float n, const float f);
void				lookAt(Matrix4&, Vector3& pos, const Vector3& dir, const Vector3& up);


//!
template <typename T>	struct CallTraits
{
	template <typename U, bool big>	struct CallTraitsImpl;
			 
	template <typename U>	struct CallTraitsImpl<U, true>		{	typedef	const U&	Type;	};
	template <typename U>	struct CallTraitsImpl<U, false>		{	typedef	U			Type;	};

	typedef	typename	CallTraitsImpl<T, (sizeof(T) > 8)>::Type	ParamType;
};

template <typename T> const T tMin(typename CallTraits<T>::ParamType a, typename CallTraits<T>::ParamType b)
{	return a > b ? b : a;	}
template <typename T> const T tMax(typename CallTraits<T>::ParamType a, typename CallTraits<T>::ParamType b)
{	return a < b ? b : a;	}

}		// math
}		// owl
#endif	// __B_MATH__