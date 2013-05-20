/*
	bEngine - 2011
	Baptiste Costa
*/

#ifndef		__OBJ_LOADER__
#define		__OBJ_LOADER__

	#include	"geometry.h"
	#include	"material.h"

class OBJ
{
private:

	

public:

	Mesh*		Load			( const char* path );

	//-----------------------------------------------------------------------------
	//	Load geometry from an OBJ file.
	//	\param	vd : dynamic arrays of 3D vectors holding vertices coordinates.
	//	\param	path : OBJ file path.
	Geometry*	LoadOBJGeometry ( const char* path );

	//-----------------------------------------------------------------------------
	//	Load material from a MTL file. Return a pointer on Material.
	//	\param	path : MTL file path.
	Material*	LoadOBJMaterial ( const char* path );
};



#endif	/*__OBJ_LOADER__*/