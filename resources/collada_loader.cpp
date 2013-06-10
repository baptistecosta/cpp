/**
	bEngine	::	2011 - 2012
	@author		Baptiste Costa
*/

	#include "collada_loader.h"
	#include <iostream>
	#include <sstream>
	#include "geometry.h"
	#include "material.h"
	#include "mesh.h"
	#include "resource_manager.h"

//---------------------------------------------------------------------------
Collada			Collada::collada;

//---------------------------------------------------------------------------
bool			Collada::Load(const String& _path)
{
	path = _path;
	if (Resources::meshes.find(path))
		return	false;

	FILE* fp = fopen(path.cStr(), "rb");
	if (!fp)
	{
		printf("File error");
		exit(1);  
	}
	
	fseek(fp, 0, SEEK_END);
	uint size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* file = new char[size + 1];
	
	uint res = fread(file, sizeof(char)/*1*/, size, fp);
	if (res != size)
	{
		printf("Reading error");
		exit(2);
	}
	
	fclose(fp);

	file[size] = 0;

	xml_document<> doc;
	doc.parse<0>(file);

	LoadScene(doc.first_node("COLLADA"));

	delete[]file;
	return true;
}

//---------------------------------------------------------------------------
void			Collada::LoadScene(xml_node<>* cnod_collada)
{
	xml_node<>* nod_scene = cnod_collada->first_node("scene");
	xml_node<>* nod_instance_v_scene = nod_scene->first_node("instance_visual_scene");
	
	scene = new Scene(path);
	scene->url = nod_instance_v_scene->first_attribute("url")->value();
	ConvertSourceString(scene->url);

	lib_geoms = new LibraryGeometries(path);
	lib_vs = new LibraryVisualScenes(path);
	LoadVisualScenes(cnod_collada, scene);

	if (cnod_collada->first_node("library_animations"))
		LoadAnimations(cnod_collada->first_node("library_animations"));

	Resources::lib_geometries.registerRes(lib_geoms);
	Resources::lib_visual_scenes.registerRes(lib_vs);
	Resources::scenes.registerRes(scene);
}

//---------------------------------------------------------------------------
void	Collada::LoadVisualScenes(xml_node<>* cnod_collada, Scene* scene)
{
	xml_node<>* cnod_lib_visual_scenes = cnod_collada->first_node("library_visual_scenes");
	xml_node<>* cnod_visual_scene = FindNode(cnod_lib_visual_scenes, "visual_scene", "id", scene->url.cStr());
	String str_vs_id = cnod_visual_scene->first_attribute("id")->value();

	lib_vs->visual_scene[str_vs_id] = new VisualScene;
	VisualScene* vs = lib_vs->visual_scene[str_vs_id];

	for (xml_node<>* cnod_node = cnod_visual_scene->first_node("node"); cnod_node; cnod_node = cnod_node->next_sibling("node"))
	{
		// type "joint"
		if (xml_attribute<>* cattrib_type = cnod_node->first_attribute("type"))
		{
			if (strcmp(cattrib_type->value(), "JOINT") == 0)
				LoadVisualSceneJoint(cnod_node, vs);
		}

		// type "geometry"
		else
		{
			String node_id = cnod_node->first_attribute("id")->value();
			vs->geoms[node_id] = new VecTRS;
			LoadVisualScenePolygon(cnod_collada, cnod_node, vs->geoms[node_id]);
		}
	}
}

//---------------------------------------------------------------------------
void			Collada::LoadVisualScenePolygon( xml_node<>* cnod_collada, xml_node<>* cnod_node, VecTRS* node_geom )
{
	if (xml_node<>* cnod_transf = FindNode(cnod_node, "translate")) node_geom->t = ExtractVector(cnod_transf->value());
	if (xml_node<>* cnod_transf = FindNode(cnod_node, "scale")) node_geom->s = ExtractVector(cnod_transf->value());
	if (xml_node<>* cnod_transf = FindNode(cnod_node, "rotate", "sid", "rotateX")) ExtractRotation(cnod_transf->value(), node_geom->r.x);
	if (xml_node<>* cnod_transf = FindNode(cnod_node, "rotate", "sid", "rotateY")) ExtractRotation(cnod_transf->value(), node_geom->r.y);
	if (xml_node<>* cnod_transf = FindNode(cnod_node, "rotate", "sid", "rotateZ")) ExtractRotation(cnod_transf->value(), node_geom->r.z);

	if (xml_node<>* cnod_inst_geom = FindNode(cnod_node, "instance_geometry"))
		LoadGeometry(cnod_collada, AccessNode(cnod_inst_geom, "url", cnod_collada->first_node("library_geometries"), "geometry", "id"));

	if (xml_node<>* cnod_inst_ctrl = FindNode(cnod_node, "instance_controller"))
		LoadControllers(cnod_collada, AccessNode(cnod_inst_ctrl, "url", cnod_collada->first_node("library_controllers"), "controller", "id"));

	if (xml_node<>* cnod_inst_mtl = FindNode(cnod_node, "instance_material"))
		LoadMaterial(cnod_collada, AccessNode(cnod_inst_mtl, "target", cnod_collada->first_node("library_materials"), "material", "id"));
}

//---------------------------------------------------------------------------
void			Collada::LoadVisualSceneJoint(xml_node<>* nod_root_joint, VisualScene* vs)
{
	vs->joints = new Tree<VisualScene::Joint>;

	ExtractVisualSceneJointData(nod_root_joint, vs->joints);
	ProcessVisualSceneJointNode(nod_root_joint, vs->joints);
}

//---------------------------------------------------------------------------
void			Collada::ProcessVisualSceneJointNode(xml_node<>* root, Tree<VisualScene::Joint>* joint)
{
	Iterator itr = joint, _itr = joint;
	for (xml_node<>* cnod = root->first_node("node"); cnod; cnod = cnod->next_sibling("node"))
	{
		itr.appendChild(new Tree<VisualScene::Joint>);
		_itr.lastChild();

		ExtractVisualSceneJointData(cnod, _itr.getChildItr().getNode()->getNodeData());

		if (cnod->first_node("node"))
			ProcessVisualSceneJointNode(cnod, _itr.getChildItr().getNode()->getNodeData());
	}
}

//---------------------------------------------------------------------------
void			Collada::ExtractVisualSceneJointData(xml_node<>* cnod, Tree<VisualScene::Joint>* tree_node)
{
	Vector3 vec;
	float rot = 0;

	tree_node->getTreeData().id = cnod->first_attribute("name")->value();
	for (xml_node<>* node = cnod->first_node(); node->next_sibling("extra"); node = node->next_sibling())
	{
		String sid = node->first_attribute("sid")->value();

		if		(sid == "translate")	tree_node->getTreeData().bind_pose.t = ExtractVector(node->value());
		else if (sid == "scale")		tree_node->getTreeData().bind_pose.s = ExtractVector(node->value());
		else if (sid == "jointOrientX")	ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.o.x);
		else if (sid == "jointOrientY")	ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.o.y);
		else if (sid == "jointOrientZ")	ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.o.z);
		else if (sid == "rotateX")		ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.r.x);
		else if (sid == "rotateY")		ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.r.y);
		else if (sid == "rotateZ")		ExtractRotation(node->value(), tree_node->getTreeData().bind_pose.r.z);
	}
}

//---------------------------------------------------------------------------
Geometry*		Collada::LoadGeometry(xml_node<>* cnod_collada, xml_node<>* cnod_geom, Controller* ctrl)
{
	String geom_guid = cnod_geom->first_attribute("id")->value();

	Geometry* geom = Resources::geometries.find(geom_guid);
	if (geom != null_ptr) return geom;
	geom = new Geometry(geom_guid);
	
	ColladaMesh mesh;

	xml_node<>* cnod_triangles = FindNode(cnod_geom, "triangles");
	mesh.triangles.count = atoi(cnod_triangles->first_attribute("count")->value());
	mesh.triangles.material = cnod_triangles->first_attribute("material")->value();

	for (xml_node<>* cnod_input = cnod_triangles->first_node("input"); cnod_input; cnod_input = cnod_input->next_sibling("input"))
	{
		String semantic = cnod_input->first_attribute("semantic")->value();

		if (semantic == "VERTEX")
		{
			mesh.triangles.vert_offset = atoi(cnod_input->first_attribute("offset")->value());

			xml_node<>* cnod_vertices = AccessNode(cnod_input, "source", cnod_geom, "vertices", "id");
			xml_node<>* cnod_source = AccessNode(cnod_vertices->first_node("input"), "source", cnod_geom, "source", "id");
			ExtractVertexData(cnod_source->first_node("float_array")->value(), mesh.source_pos);
		}
		else if (semantic == "NORMAL")
		{
			mesh.triangles.nor_offset = atoi(cnod_input->first_attribute("offset")->value());

			xml_node<>* cnod_source = AccessNode(cnod_input, "source", cnod_geom, "source", "id");
			ExtractVertexData(cnod_source->first_node("float_array")->value(), mesh.source_nor);
		}
		else if (semantic == "TEXCOORD")
		{
			mesh.triangles.texcoord_offset = atoi(cnod_input->first_attribute("offset")->value());

			xml_node<>* cnod_source = AccessNode(cnod_input, "source", cnod_geom, "source", "id");
			ExtractVertexUV(cnod_source->first_node("float_array")->value(), mesh.source_uv);
		}
	}
	mesh.triangles.p = ExtractVectorInt(cnod_triangles->first_node("p")->value());
	uint n_vertices = mesh.triangles.count * 3;
	
	for (uint i = 0; i < n_vertices; i++)
	{
		Vector3 pos(mesh.source_pos[mesh.triangles.p[i * 3 + mesh.triangles.vert_offset]]);
		Vector3 nor(mesh.source_nor[mesh.triangles.p[i * 3 + mesh.triangles.nor_offset]]);
		Vector2 uv(mesh.source_uv[mesh.triangles.p[i * 3 + mesh.triangles.texcoord_offset]]);

		geom->vertices.Push(new Vertex(pos, nor, uv));

		uint p_index = mesh.triangles.p[i * 3 + mesh.triangles.vert_offset];
		
		if (ctrl)
		{
			geom->is_skinned = true;
			uint w_count = ctrl->vertex_weights[p_index]->weights.size();
			for (uint j = 0; j < w_count; j++)
			{
				if (j == MAX_BONES_PER_VERT) break;
				geom->vertices[i]->weights[j].jid = ctrl->vertex_weights[p_index]->weights[j].jid;
				geom->vertices[i]->weights[j].w = ctrl->vertex_weights[p_index]->weights[j].w;
			}
		}
	}
	geom->ComputeTangentBasis();

	lib_geoms->geom[geom_guid] = geom;
	Resources::geometries.registerRes(geom);
	return geom;
}

//---------------------------------------------------------------------------
xml_node<>*		Collada::FindNode(xml_node<>* search_location, const char* tag_name, const char* attrib_name, const char* attrib_value)
{
	if (search_location->first_node(tag_name))
	{
		for (xml_node<>* child = search_location->first_node(tag_name); child; child = child->next_sibling(tag_name))
		{
			if (attrib_name == 0) return child;

			else if (attrib_name != 0 && attrib_value == 0)
			{
				if (child->first_attribute(attrib_name))
					return child;
			}
			else if (attrib_name != 0 && attrib_value != 0)
			{
				if (strcmp(child->first_attribute(attrib_name)->value(), attrib_value) == 0)
					return child;
			}
		}
	}
	else
	{
		for (xml_node<>* child = search_location->first_node(); child; child = child->next_sibling())
		{
			xml_node<>* find_in_child = FindNode(child, tag_name, attrib_name, attrib_value);
			if (find_in_child)
				return find_in_child;
		}
	}
	return null_ptr;
}

//---------------------------------------------------------------------------
Controller*		Collada::LoadControllers(xml_node<>* cnod_collada, xml_node<>* cnod_ctrl)
{
	String ctrl_guid = cnod_ctrl->first_attribute("id")->value();
	
	Controller* controller = Resources::controllers.find(ctrl_guid);
	if (controller)
		return controller;

	lib_ctrls.ctrl[ctrl_guid] = new Controller(ctrl_guid);
	controller = lib_ctrls.ctrl[ctrl_guid];

	// Node <skin> and <bind_shape_matrix>
	xml_node<>* node_skin = FindNode(cnod_ctrl, "skin", "source");
	controller->bind_shape_mtx = ExtractMatrix(node_skin->first_node("bind_shape_matrix")->value());

	// Node <joints>
	xml_node<>* node_joints = FindNode(cnod_ctrl, "joints");
	xml_node<>* node_joints_input = FindNode(node_joints, "input", "semantic", "INV_BIND_MATRIX");
	String str_ibmtx_source = node_joints_input->first_attribute("source")->value();

	// Node <source> for joints
	xml_node<>* node_v_weights = FindNode(cnod_ctrl, "vertex_weights");
	xml_node<>* cnod_input_joint = FindNode(node_v_weights, "input", "semantic", "JOINT");

	String str_input_joint_accessor = cnod_input_joint->first_attribute("source")->value();
	string str_joints = AccessSourceData(cnod_ctrl, controller->accessor_joints, str_input_joint_accessor, "Name_array").cStr();

	vector<string> str_joints_name(controller->accessor_joints.count);
	istringstream iss(str_joints);
	for (int i = 0; i < controller->accessor_joints.count; i++)
		iss >> str_joints_name[i];

	// Node <source> for inverse bind matrices
	String str_ibmtx = AccessSourceData(cnod_ctrl, controller->accessor_matrices, str_ibmtx_source, "float_array");
	controller->inv_bind_matrices = ExtractMatrices(str_ibmtx, controller->accessor_matrices.count, controller->accessor_matrices.stride);

	// Node <source> for weights
	xml_node<>* cnod_input_weight = FindNode(node_v_weights, "input", "semantic", "WEIGHT");

	String str_input_weight_accessor = cnod_input_weight->first_attribute("source")->value();
	String str_source_weights_data = AccessSourceData(cnod_ctrl, controller->accessor_weights,str_input_weight_accessor, "float_array");
	std::vector<float> weights = ExtractVectorFloat(str_source_weights_data);

	// Node <vertex_weights>
	uint v_weight_count = atoi(node_v_weights->first_attribute("count")->value());

	std::vector<uint> vcount = ExtractVectorInt(node_v_weights->first_node("vcount")->value());
	std::vector<uint> v = ExtractVectorInt(node_v_weights->first_node("v")->value());

	uint current_v_index = 0;
	for (uint i = 0; i < v_weight_count; i++)
	{
		controller->vertex_weights.push_back(new VertexWeights);
		for (uint j = 0; j < vcount[i]; j++)
		{
			uint joint_index = v[current_v_index++];
			uint weight_index = v[current_v_index++];

			controller->vertex_weights[i]->weights.push_back(VertexWeights::Weights(joint_index, weights[weight_index]));
		}
	}

	if (xml_node<>* cnod_skin_source = FindNode(cnod_ctrl, "skin", "source"))
		LoadGeometry(cnod_collada, AccessNode(cnod_skin_source, "source", cnod_collada->first_node("library_geometries"), "geometry", "id"), controller);

	Resources::controllers.registerRes(controller);
	return controller;
}

//---------------------------------------------------------------------------
String			Collada::AccessSourceData(xml_node<>* search_location, Controller::Accessor& accessor, String& source, const String& data_param_node)
{
	xml_node<>* node_source = FindNode(search_location, "source", "id", ConvertSourceString(source).cStr());
	xml_node<>* node_accessor = FindNode(node_source, "accessor");

	if (node_accessor->first_attribute("source"))	accessor.source = node_accessor->first_attribute("source")->value();
	if (node_accessor->first_attribute("count"))	accessor.count = atoi(node_accessor->first_attribute("count")->value());
	if (node_accessor->first_attribute("stride"))	accessor.stride = atoi(node_accessor->first_attribute("stride")->value());

	xml_node<>* node_source_data = FindNode(node_source, data_param_node.cStr(), "id", ConvertSourceString(accessor.source).cStr());
	return node_source_data->value();
}

//---------------------------------------------------------------------------
bool			Collada::LoadMaterial(xml_node<>* cnod_collada, xml_node<>* cnod_material)
{
	if (Resources::materials.find(path))
		return false;

	Material* mtl = new Material(path);

	xml_node<>* cnod_instance_effect = cnod_material->first_node("instance_effect");
	xml_node<>* cnod_effect = AccessNode(cnod_instance_effect, "url", cnod_collada->first_node("library_effects"), "effect", "id");
	
//	xml_node<>* cnod_emission = FindNode(cnod_effect, "emission");
//	if (cnod_emission) ExtractVector(cnod_emission->first_node("color")->value());

	xml_node<>* cnod_ambient = FindNode(cnod_effect, "ambient");
//	if (cnod_ambient) ExtractVector(cnod_ambient->first_node("color")->value(), mtl->getKa());
	if (cnod_ambient) mtl->setKa(ExtractVector(cnod_ambient->first_node("color")->value()));

	xml_node<>* cnod_diffuse = FindNode(cnod_effect, "diffuse");
//	if (cnod_diffuse) ExtractVector(cnod_diffuse->first_node("color")->value(), mtl->getKd());
	if (cnod_diffuse) mtl->setKd(ExtractVector(cnod_diffuse->first_node("color")->value()));

	xml_node<>* cnod_specular = FindNode(cnod_effect, "specular");
//	if (cnod_specular) ExtractVector(cnod_specular->first_node("color")->value(), mtl->getKs());
	if (cnod_specular) mtl->setKs(ExtractVector(cnod_specular->first_node("color")->value()));

	xml_node<>* cnod_shininess = FindNode(cnod_effect, "shininess");
//	if (cnod_shininess) ExtractFloat(cnod_shininess->first_node("color")->value(), mtl->getNs());
	if (cnod_shininess) mtl->setNs(ExtractFloat(cnod_shininess->first_node("color")->value()));

	Resources::materials.registerRes(mtl);
	return true;
}

//---------------------------------------------------------------------------
void			Collada::LoadAnimations(xml_node<>* cnod_lib_anims)
{
	VisualScene::Joint* joint = null_ptr;

	for (xml_node<>* cnod_anim = cnod_lib_anims->first_node("animation"); cnod_anim; cnod_anim = cnod_anim->next_sibling("animation"))
	{
		// If cnod_anim is a visibility animation, pass here only once after a new animation joint is encounter.
		if (cnod_anim->first_attribute("id"))
		{
			String str_joint_name = cnod_anim->first_attribute("name")->value();
			joint = FindJoint(str_joint_name, lib_vs->visual_scene[""/*hard coded, shize*/]->joints);
		}
		
		xml_node<>* cnod_channel = FindNode(cnod_anim, "channel");
		if (!cnod_channel)
			break;
		
		string str_channel_target = cnod_channel->first_attribute("target")->value();

		size_t len_to_slash = str_channel_target.find("/");

		char str_channel_joint_target[48] = {0};
		char str_channel_transf_target[48] = {0};
		str_channel_target.copy(str_channel_joint_target, len_to_slash, 0);
		str_channel_target.copy(str_channel_transf_target, str_channel_target.length() - len_to_slash, len_to_slash);

		if (strcmp(str_channel_transf_target, "/visibility") == 0) continue;

		xml_node<>* cnod_anim_sampler = AccessNode(cnod_channel, "source", cnod_anim, "sampler", "id");
		xml_node<>*	cnod_anim_sampler_input = FindNode(cnod_anim_sampler, "input", "semantic", "INPUT");
		xml_node<>* cnod_anim_sampler_output = FindNode(cnod_anim_sampler, "input", "semantic", "OUTPUT");

		xml_node<>* cnod_anim_source_input = AccessNode(cnod_anim_sampler_input, "source", cnod_anim, "source", "id");
		xml_node<>* cnod_anim_source_output = AccessNode(cnod_anim_sampler_output, "source", cnod_anim, "source", "id");

		xml_node<>* cnod_anim_source_input_accessor = FindNode(cnod_anim_source_input, "accessor");
		xml_node<>* cnod_anim_source_output_accessor = FindNode(cnod_anim_source_output, "accessor");
		xml_node<>* cnod_anim_source_input_data = AccessNode(cnod_anim_source_input_accessor, "source", cnod_anim_source_input, "float_array", "id");
		xml_node<>* cnod_anim_source_output_data = AccessNode(cnod_anim_source_output_accessor, "source", cnod_anim_source_output, "float_array", "id");

		std::vector<float> input_vector_float = ExtractVectorFloat(cnod_anim_source_input_data->value());
		std::vector<float> output_vector_float = ExtractVectorFloat(cnod_anim_source_output_data->value());
		
		// Create enough animations in the joint so they can store animation data.
		if (joint->animations.size() == 0)
		{
			for (uint i = 0; i < output_vector_float.size(); i++)
			{
				joint->animations.push_back(new VisualScene::Joint::Animations(lib_vs->visual_scene[""]->joints->getTreeData().bind_pose));
				joint->animations[i]->key_time = input_vector_float[i];
			}
		}

		str_channel_target = str_channel_transf_target;
		for (uint i = 0; i < joint->animations.size(); i++)
		{
			// process animation source data in joint.animation vector.
			if		(str_channel_target == "/translate.X") joint->animations[i]->data.t.x = output_vector_float[i];
			else if (str_channel_target == "/translate.Y") joint->animations[i]->data.t.y = output_vector_float[i];
			else if (str_channel_target == "/translate.Z") joint->animations[i]->data.t.z = output_vector_float[i];
			else if	(str_channel_target == "/rotateX.ANGLE") joint->animations[i]->data.r.x = output_vector_float[i];
			else if	(str_channel_target == "/rotateY.ANGLE") joint->animations[i]->data.r.y = output_vector_float[i];
			else if	(str_channel_target == "/rotateZ.ANGLE") joint->animations[i]->data.r.z = output_vector_float[i];
			else if (str_channel_target == "/scale.X") joint->animations[i]->data.s.x = output_vector_float[i];
			else if (str_channel_target == "/scale.Y") joint->animations[i]->data.s.y = output_vector_float[i];
			else if (str_channel_target == "/scale.Z") joint->animations[i]->data.s.z = output_vector_float[i];
		}
	}
}

//---------------------------------------------------------------------------
VisualScene::Joint*	Collada::FindJoint(const String& name, Tree<VisualScene::Joint>* root)
{
	if (root->getTreeData().id == name)
		return &root->getTreeData();

	Iterator itr = root;

	for (itr.firstChild(); itr.isChildValid(); itr.nextChild())
	{
		if (itr.childData().id == name)
			return &itr.childData();

		VisualScene::Joint* find_in_child = FindJoint(name, itr.getChildItr().getNode()->getNodeData());
	
		if (find_in_child)
			return find_in_child;
	}
	return 0;
}

//---------------------------------------------------------------------------
bool			Collada::ExtractVertexData(char* v_ascii, std::vector<Vector3>& v_float)
{
	v_ascii = RemoveNewlineAndSpaceBeforeString(v_ascii);
	while (v_ascii[0] != 0 && v_ascii[1] != 0)
	{
		float x = static_cast<float>(atof(&v_ascii[0]));
		v_ascii = JumpToNextChar(v_ascii, ' ');
		if (v_ascii[0] == 0) return false;

		float y = static_cast<float>(atof(&v_ascii[0]));
		v_ascii = JumpToNextChar(v_ascii, ' ');
		if (v_ascii[0] == 0)	return false;

		float z = static_cast<float>(atof(&v_ascii[0]));
		v_ascii = JumpToNextChar(v_ascii, 10);

		v_float.push_back(Vector3(x, y, z));
	}
	return	true;
}

//---------------------------------------------------------------------------
bool			Collada::ExtractVertexUV(char* v_ascii, std::vector<Vector2>& v)
{
	v_ascii = RemoveNewlineAndSpaceBeforeString(v_ascii);
	while (v_ascii[0] != 0 && v_ascii[1] != 0)
	{
		float x = static_cast<float>(atof(&v_ascii[0]));
		v_ascii = JumpToNextChar(v_ascii, ' ');
		if (v_ascii[0] == 0) return false;

		float y = static_cast<float>(atof(&v_ascii[0]));
		v_ascii = JumpToNextChar(v_ascii, 10);

		v.push_back(Vector2(x, y));
	}
	return	true;
}

//---------------------------------------------------------------------------
vector<uint>	Collada::ExtractVectorInt(String& str)
{
//	vector<char> vn_ascii(str.size() + 1);
	vector<char> vn_ascii(str.Size() + 1);
	copy(str.Begin(), str.End(), vn_ascii.begin());

	char* n_ascii = &vn_ascii[0];
	n_ascii = RemoveNewlineAndSpaceBeforeString(n_ascii);

	vector<uint> v;
	while (n_ascii[0] != 0)
	{
		v.push_back(atoi(&n_ascii[0]));
		n_ascii = JumpToNextChar(n_ascii, ' ');
	}
	return v;
}

//---------------------------------------------------------------------------
vector<uint>	Collada::ExtractVectorInt(char* n_ascii)
{
	n_ascii = RemoveNewlineAndSpaceBeforeString(n_ascii);
	vector<uint> v;
	while (n_ascii[0] != 0)
	{
		v.push_back(atoi(&n_ascii[0]));
		n_ascii = JumpToNextChar(n_ascii, ' ');
	}
	return v;
}

//---------------------------------------------------------------------------
std::vector<float> Collada::ExtractVectorFloat(const String& str)
{
//	vector<char> vn_ascii(str.size() + 1);
	vector<char> vn_ascii(str.Size() + 1);
	copy(str.Begin(), str.End(), vn_ascii.begin());
	char* n_ascii = &vn_ascii[0];
	n_ascii = RemoveNewlineAndSpaceBeforeString(n_ascii);

	std::vector<float> v;
	while (n_ascii[0] != 0)
	{
		v.push_back(static_cast<float>(atof(&n_ascii[0])));
		n_ascii = JumpToNextChar(n_ascii, ' ');
	}
	return v;
}

//---------------------------------------------------------------------------
bool			Collada::ExtractJointPoseData(char* a_ascii, float* a)
{
	if (a_ascii[0] == 0)
		return false;

	uint i = 0;

	while (a_ascii[0] != 0)
	{
		a[i] = static_cast<float>(atof(a_ascii));
		a_ascii = JumpToNextChar(a_ascii, ' ');
		i++;
	}
	return true;
}

//---------------------------------------------------------------------------
/*
bool			Collada::ExtractVector(char* v_ascii, Vector3& v)
{
	v.x = static_cast<float>(atof(&v_ascii[0]));
	v_ascii = JumpToNextChar(v_ascii, ' ');
	if (v_ascii[0] == 0) return false;

	v.y = static_cast<float>(atof(&v_ascii[0]));
	v_ascii = JumpToNextChar(v_ascii, ' ');
	if (v_ascii[0] == 0) return false;

	v.z = static_cast<float>(atof(&v_ascii[0]));

	return true;
}
*/

//---------------------------------------------------------------------------
Vector3			Collada::ExtractVector(char* v_ascii)
{
	Vector3 v;
	v.x = static_cast<float>(atof(&v_ascii[0]));
	v_ascii = JumpToNextChar(v_ascii, ' ');
	if (v_ascii[0] == 0) return false;

	v.y = static_cast<float>(atof(&v_ascii[0]));
	v_ascii = JumpToNextChar(v_ascii, ' ');
	if (v_ascii[0] == 0) return false;

	v.z = static_cast<float>(atof(&v_ascii[0]));

	return v;
}

//---------------------------------------------------------------------------
bool			Collada::ExtractFloat(char* f_ascii, float& f)
{
	if (f_ascii[0] == 0)
		return false;

	f = static_cast<float>(atof(&f_ascii[0]));

	return true;
}

//---------------------------------------------------------------------------
float			Collada::ExtractFloat(char* f_ascii)
{
	if (f_ascii[0] == 0)
		return 0.f;
	return static_cast<float>(atof(&f_ascii[0]));
}

//---------------------------------------------------------------------------
bool			Collada::ExtractRotation(char* r_ascii, float& _rot)
{
	if (r_ascii[0] == 0)
		return false;

	for (uint i = 0; i < 3; i++)
		r_ascii = JumpToNextChar(r_ascii, ' ');

	_rot = static_cast<float>(atof(r_ascii));

	return true;	
}

//---------------------------------------------------------------------------
owl::Matrix4	Collada::ExtractMatrix(char* mtx_ascii)
{
	owl::Matrix4 mtx;
	uint i = 0;
	while (mtx_ascii[0] != 0)
	{
		mtx.m[i] = static_cast<float>(atof(&mtx_ascii[0]));
		mtx_ascii = JumpToNextChar(mtx_ascii, ' ');
		i++;
	}
	return mtx;
}

//---------------------------------------------------------------------------
std::vector<Matrix4> Collada::ExtractMatrices(String str, int count, int stride)
{
//	vector<char> vmtx_ascii(str.size() + 1);
	vector<char> vmtx_ascii(str.Size() + 1);
	copy(str.Begin(), str.End(), vmtx_ascii.begin());
	char* mtx_ascii = &vmtx_ascii[0];
	mtx_ascii = RemoveNewlineAndSpaceBeforeString(mtx_ascii);

	std::vector<Matrix4> mtx(count);
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < stride; j++)
		{
			mtx[i].m[j] = static_cast<float>(atof(mtx_ascii));
			mtx_ascii = JumpToNextChar(mtx_ascii, 32 /*space*/);
		}
	}
	return mtx;
}

//---------------------------------------------------------------------------
char*			Collada::JumpToNextChar(char* str, char code)
{
	while (str[0] != code)
	{
		if (str[0] == 0) return str;
		if (str[0] == 10 /*newline*/) break;
		str++;
	}
	str++;
	return	str;
}

//---------------------------------------------------------------------------
char*			Collada::RemoveNewlineAndSpaceBeforeString(char* str)
{
	while (str[0] == 10 || str[0] == 32)
	{
		int pop = str[0];
		str++;
		pop = str[0];
		pop = str[1];
	}
	return str;
}

//---------------------------------------------------------------------------
String			Collada::ConvertSourceString(String& str)
{	
	if (str[0] == '#')
		str.Erase(0, 1);
	return str;
}

//---------------------------------------------------------------------------
xml_node<>*		Collada::AccessNode	(	
										xml_node<>* cnod_accessor,
										const String& str_accessor_attrib,
										xml_node<>* cnod_parent_target,
										const String& str_target_tag,
										const String& str_target_attrib
									)
{	
	String str_accessor_value = cnod_accessor->first_attribute(str_accessor_attrib.cStr())->value();
	return FindNode(cnod_parent_target, str_target_tag.cStr(), str_target_attrib.cStr(), ConvertSourceString(str_accessor_value).cStr());
}

