/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

#ifndef __COLLADA__
#define __COLLADA__
		 
	#include <map>
	#include "resource.h"
	#include "core/math/matrix4.h"
	#include "core/math/vector.h"
	#include "core/containers/tree.h"
	#include "core/containers/stl.h"
	#include "externs/rapidxml/rapidxml.hpp"
	#include "typedefs.h"

using	namespace owl;
using	namespace rapidxml;
using	namespace std;

namespace owl {

class	String;
class	Geometry;
class	Material;
class	Mesh;
class	RenderingGeometry;
//class	Skeleton;

}

//-----------------------------------------------------------------------------
//	TRS vectors struct
struct VecTRS 
{	
	Vector3	t, r, s;

	VecTRS(){}
	VecTRS(const VecTRS& transf)
		:	t(transf.t)
		,	r(transf.r)
		,	s(transf.s)
	{}
};

//-----------------------------------------------------------------------------
//	TORS vectors struct
struct VecTORS 
{	
	Vector3	t, o, r, s;

	VecTORS(){}
	VecTORS(const VecTORS& transf)
		:	t(transf.t)
		,	o(transf.o)
		,	r(transf.r)
		,	s(transf.s)
	{}
};

//-----------------------------------------------------------------------------
//	Scene
class	Scene	:	public	Resource
{
public:

	String		url;

	Scene(const String& guid)	:	Resource(guid){}
};

//---------------------------------------------------------------------------
//	Library Controllers
//!
struct	VertexWeights
{
	struct	Weights
	{
		int				jid;
		float			w;

		Weights(const int _jid, const float _w) : jid(_jid), w(_w)	{}
	};

	vector<Weights>	weights;
};

//!
class	Controller	:	public	Resource
{
public:

//	//// Have to remove Accessor struct, put it in LoadController function, nothing to do here.
	struct Accessor
	{
		String				source;
		int					count, stride;

		Accessor()
		{
			count = stride = 0;
		}
	};

	Accessor				accessor_joints,
							accessor_matrices,
							accessor_weights;

	vector<VertexWeights*>	vertex_weights;
	vector<Matrix4>			inv_bind_matrices;
	owl::Matrix4			bind_shape_mtx;

//	Controller(const string& _guid):	Resource(_guid)	{}
	Controller(const String& _guid):	Resource(_guid)	{}

	~Controller()
	{
		Stl::DeleteVector(vertex_weights);
		inv_bind_matrices.clear();
	}
};

class	LibraryControllers
{
public:

	std::map< String, Controller* >	ctrl;

	~LibraryControllers()
	{	Stl::DeleteMap(ctrl);	}
};

//-----------------------------------------------------------------------------
//	Library Geometries
struct	MeshTriangles
{
	String			material;
	int				count;
	short			vert_offset, nor_offset, texcoord_offset;
	vector<uint>	p;
};

struct	ColladaMesh
{
	MeshTriangles	triangles;
	vector<Vector3>	source_pos, source_nor;
	vector<Vector2>	source_uv;
};

class	LibraryGeometries : public Resource
{
public:

	map<String, Geometry*>	geom;

	LibraryGeometries(const String& _guid):	Resource(_guid)	{}
};

//-----------------------------------------------------------------------------
//	Visual scene
struct	VisualScene
{
	struct Joint
	{
		struct Animations 
		{
			VecTORS		data;
			float		key_time;

			Animations(const VecTORS &transf) : data(transf){}
		};

		String					id;
		VecTORS					bind_pose;
		vector<Animations*>		animations;
		owl::Matrix4			current_anim_mtx;
	};

	Tree<Joint>*			joints;
	map<String, VecTRS*>	geoms;

	~VisualScene()
	{
		delete joints;
		Stl::DeleteMap(geoms);
	}
};

//-----------------------------------------------------------------------------
//	Library visual scenes
class LibraryVisualScenes :	public Resource
{
public:

	map<String, VisualScene*>	visual_scene;

	LibraryVisualScenes(const String& _guid)	:	Resource(_guid){}
	~LibraryVisualScenes()
	{
		Stl::DeleteMap(visual_scene);
	}
};


//-----------------------------------------------------------------------------
//	Main Collada class
class	Collada
{
		typedef	TreeIterator<VisualScene::Joint>	Iterator;
private:

		String					path;
		Scene*					scene;
		LibraryControllers		lib_ctrls;
		LibraryGeometries*		lib_geoms;
		LibraryVisualScenes*	lib_vs;

public:

static	Collada			collada;

		//---------------------------------------------------------------------
		//	Load a Collada file using RapidXML parse() function.
		//	RapidXML parse() get a char[] parameter.
		//	Conversion from FILE to an array of char is necessary.
		//	!! Call LoadGeometry() !!
		//	!! Call LoadMaterial() !!
		//	!! Call LoadVisualScenesAnimations() !!
		//	\param	path : Collada file path.
		bool				Load(const String& path);

//		//---------------------------------------------------------------------
// 		//							<scene>
// 		//---------------------------------------------------------------------
		void				LoadScene(xml_node<>* scene);

// 		//---------------------------------------------------------------------
// 		//						<library_visual_scenes>
// 		//---------------------------------------------------------------------
		//	Set up all the basics coordinates for each joint of the skeleton.
		void				LoadVisualScenes(xml_node<>* cnod_collada, Scene*);
		void				LoadVisualSceneJoint(xml_node<>* nod_root_joint, VisualScene*);
		void				LoadVisualScenePolygon(xml_node<>* cnod_collada, xml_node<>* cnod_node, VecTRS*);

		//---------------------------------------------------------------------
		//	Recursively process every node in visual scenes library.
		//	!! Call LoadJointData() !! for each node.
		//	\param	node : skeleton root.
		//	\param	count : number of node in the skeleton, 0 = root.
		void				ProcessVisualSceneJointNode(xml_node<>* _root, Tree<VisualScene::Joint>* joint);

		//---------------------------------------------------------------------
		//	Iterate through each <animation> node trying to find if a joint name
		//	match in the tree skeleton structure. It should.
		//	When the joint is found, load joint transformations data.
		void				ExtractVisualSceneJointData(xml_node<>* _xmlnode, Tree<VisualScene::Joint>* tree_node);
		
//		//---------------------------------------------------------------------
// 		//						<library_geometries>
// 		//---------------------------------------------------------------------
		//	Parsing geometry node from collada files.
		//	Contain vertex informations in 3D coordinates.
		//	!! Call ExtractVertex() !!
		//	\param	geometry : first child of library geometries.
		//	\return a VertexData instance if the process went well, either return 0.
		Geometry*			LoadGeometry(xml_node<>* cnod_collada, xml_node<>* lib_geometry, Controller* ctrl = NULL);
				
		//---------------------------------------------------------------------
		//	Deindexer conditionner : Collada files provide a 3 parameters indexation which is not okay
		//	for opengl uses. Once deindexed, you need to pass the vertex info into a VBO indexer,
		//	which will provide a compatible indexation with glDrawElements() function.
		//	\param	vertex data in & out.
		void				Deindex(RenderingGeometry*);

//		//---------------------------------------------------------------------
//		//						<library_material>
//		//---------------------------------------------------------------------
		//	Extract material informations.
		//	\return a Material instance if th process went well, either return 0.
		bool				LoadMaterial(xml_node<>* cnod_collada, xml_node<>* cnod_material);

// 		//---------------------------------------------------------------------
// 		//						<library_animation>
// 		//---------------------------------------------------------------------
		void				LoadAnimations(xml_node<>* cnod_lib_anims);

// 		//---------------------------------------------------------------------
// 		//						<library_controllers>
// 		//---------------------------------------------------------------------
		Controller*			LoadControllers(xml_node<>* cnod_collada, xml_node<>* cnod_ctrl);
		VisualScene::Joint*	FindJoint(const String& name, Tree<VisualScene::Joint>* root);
		xml_node<>*			FindNode(xml_node<>* search_location, const char* tag_name, const char* attrib_name = NULL, const char* attrib_value = NULL);
		String				AccessSourceData(xml_node<>* search_location, Controller::Accessor& accessor, String& source, const String& data_param_node);

//		//---------------------------------------------------------------------
//		//							Collada tools
//		//---------------------------------------------------------------------
//		bool				ExtractVector(char* v_ascii, Vector3&);
		Vector3				ExtractVector(char* v_ascii);
		bool				ExtractFloat(char* f_ascii, float&);
		float				ExtractFloat(char* f_ascii);
		Matrix4				ExtractMatrix(char* mtx_ascii);
		vector<Matrix4>		ExtractMatrices(String str, int count, int stride);
		vector<uint>		ExtractVectorInt(String& n_ascii);
		vector<uint>		ExtractVectorInt(char* n_ascii);
		bool				ExtractVertexData(char* ascii_vertex_pos, vector<Vector3>&);
		bool				ExtractVertexUV(char* ascii_vertex_uv, vector<Vector2>&);
		bool				ExtractRotation(char* r_ascii, float&);
		vector<float>		ExtractVectorFloat(const String& str);
		bool				ExtractJointPoseData(char* a_ascii, float*);

		//---------------------------------------------------------------------
		//	String manipulations.
		//	JumpToNextChar :
		//	\param	code	Define the character you want to jump to in the str string.
		char*				JumpToNextChar(char* str, char code);
		char*				RemoveNewlineAndSpaceBeforeString(char* str);
		String				ConvertSourceString(String& str);
		xml_node<>*			AccessNode	( 	
											xml_node<>* cnod_source,
											const String& str_source_attrib,
											xml_node<>* cnod_target,
											const String& str_target_tag,
											const String& str_target_attrib = "id"
										);
};

#endif	//__COLLADA__

