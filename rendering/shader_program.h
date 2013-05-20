/*
	bEngine - 2011
	Baptiste Costa
*/

#ifndef		__SHADER_PROGRAM__
#define		__SHADER_PROGRAM__

	#include	"shader.hpp"
	#include	"rendering_geometry.h"
	#include	"core\math\vector.h"

struct	SkinningJoints;
struct	SkinningWeights;

//!
class ShaderProgram
{
private:

		GLuint			program;

public:

inline	void			init(const String& pathA, const String& pathB)				{	program = LoadShaders(pathA, pathB);   }

		void			setUniform(const char* name, int x);
		void			setUniform(const char* name, float x);
		void			setUniform(const char* name, float x, float y, float z);
		void			setUniform(const char* name, Vector3& v);
		void			setUniform(const char* name, Matrix4& mtx);
		void			setUniform(const char* name, vector<owl::SkinningJoints*>& joints);
		void			setUniform(const char* name, vector<owl::SkinningWeights*>& weights);
		void			setUniform(const char* name, float vmtx[], uint count);
		void			setUniform(const char* name, Matrix4 vmtx[], uint count);
		void			setUniform(const char* name, vMatrix4& vmtx);

inline	int				GetUniformLocation( const char* name )						{	return glGetUniformLocation(program, name);   }
inline	GLuint&			getProgram()												{	return program;   }
};

#endif /*__SHADER_PROGRAM__*/
