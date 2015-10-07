/**
 * @file UMSoftwareIO.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <string>
#include <assert.h>

#include "UMIO.h"
#include "UMScene.h"
#include "UMSoftwareIO.h"
#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMMesh.h"
#include "UMMeshGroup.h"
#include "UMMaterial.h"
#include "UMLight.h"
#include "UMMatrix.h"

namespace
{
	using namespace umdraw;

	double import_scale_for_debug = 1.0;
	
	UMVec3f to_float(const UMVec3d& v) { return UMVec3f((float)v.x, (float)v.y, (float)v.z); }

	UMVec4f to_float(const UMVec4d& v) { return UMVec4f((float)v.x, (float)v.y, (float)v.z, (float)v.w); }
	
	UMMat44f to_float(const UMMat44d& src)
	{
		UMMat44f dst;
		for (int i = 0; i < 4; ++i)
		{
			for (int k = 0; k < 4; ++k)
			{
				dst.m[i][k] = static_cast<float>(src.m[i][k]);
			}
		}
		return dst;
	}
	//----------------------------------------------------------------------------

	/**
	 * load vertex index from umio to umdraw
	 */
	void load_vertex_index(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.vertex_index_list().size());
		mesh->mutable_face_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::IntList& index = ummesh.vertex_index_list().at(i);
			UMVec3i umface( index.at(0), index.at(1), index.at(2) );
			mesh->mutable_face_list().at(i) = umface;
		}
		mesh->mutable_vertex_index_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::IntList& index = ummesh.vertex_index_list().at(i);
			mesh->mutable_vertex_index_list().at(i) = index;
		}
	}

	/**
	 * load vertex from umio to umdraw
	 */
	void load_vertex(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.vertex_list().size());
		mesh->mutable_vertex_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& vertex = ummesh.vertex_list().at(i);
			UMVec3d umvertex( vertex.at(0), vertex.at(1), vertex.at(2) );
			//printf("v %f %f %f.\n", umvertex.x, umvertex.y, umvertex.z);
			mesh->mutable_vertex_list().at(i) = umvertex * import_scale_for_debug;
		}
	}
	
	/**
	 * load normal from umio to umdraw
	 */
	void load_normal(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.normal_list().size());
		mesh->mutable_normal_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& normal = ummesh.normal_list().at(i);
			UMVec3d umnormal( normal.at(0), normal.at(1), normal.at(2) );
			//printf("n %f %f %f.\n", umnormal.x, umnormal.y, umnormal.z);
			mesh->mutable_normal_list().at(i) = umnormal;
		}
	}
	
	/**
	 * load uv  from umio to umdraw
	 */
	void load_uv(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.uv_list().size());
		mesh->mutable_uv_list().resize(size);
		for (int i = 0; i < size; ++i)
		{
			const umio::DoubleList& uv = ummesh.uv_list().at(i);
			UMVec2d umuv( uv.at(0), uv.at(1) );
			//printf("uv %f %f.\n", umuv.x, umuv.y);
			mesh->mutable_uv_list().at(i) = umuv;
		}
	}
	
	/**
	 * load skin from umio to umdraw
	 */
	void load_skin(UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int skin_size = static_cast<int>(ummesh.skin_list().size());
		for (int i = 0; i < skin_size; ++i)
		{
			const umio::UMSkin& skin = ummesh.skin_list().at(i);
			const int cluster_size = static_cast<int>(skin.cluster_list().size());
			for (int k = 0; k < cluster_size; ++k)
			{
				const umio::UMCluster& cluster = skin.cluster_list().at(k);
				UMSkin skin;
				skin.mutable_index_list() = cluster.index_list();
				skin.mutable_weight_list() = cluster.weight_list();
				skin.set_link_node_id(cluster.link_node_id());
				mesh->mutable_skin_list().push_back(skin);
			}
		}
	}

	
	/**
	 * load material from umio to umdraw
	 */
	void load_material(const umstring& absolute_file_path, UMMeshPtr mesh, const umio::UMMesh& ummesh)
	{
		const int size = static_cast<int>(ummesh.material_list().size());
		mesh->mutable_material_list().resize(size);
		
		//printf("material size  %d .\n", size);
		for (int i = 0; i < size; ++i)
		{
			const umio::UMMaterial& material = ummesh.material_list().at(i);
			UMMaterialPtr ummaterial = std::make_shared<UMMaterial>();
			//ummaterial->set_ambient(to_um_material_value(material.ambient()));
			ummaterial->set_diffuse(material.diffuse());
			ummaterial->set_specular(material.specular());
			ummaterial->set_emissive(material.emissive());
			ummaterial->set_refrection(material.refrection());
			ummaterial->set_transparent(material.transparent());
			ummaterial->set_shininess(material.shininess());
			ummaterial->set_transparency_factor(material.transparency_factor());
			ummaterial->set_reflection_factor(material.reflection_factor());
			ummaterial->set_diffuse_factor(material.diffuse_factor());
			ummaterial->set_specular_factor(material.specular_factor());
			ummaterial->set_emissive_factor(material.emissive_factor());
			ummaterial->set_ambient_factor(material.ambient_factor());
			
			// load texture path
			const int texture_count = static_cast<int>(material.texture_list().size());
			//printf("texture_count %d\n", texture_count);
			for (int k = 0; k < texture_count; ++k)
			{
				const umio::UMTexture& texture = material.texture_list().at(k);
				const std::string& absolute_or_relative_file_name = texture.file_name();
#ifdef WITH_EMSCRIPTEN
				umstring path = umbase::UMPath::resource_absolute_path(absolute_or_relative_file_name);
				ummaterial->mutable_texture_path_list().push_back(path);
				//printf("texture %s .\n", path.c_str());
#else
				umstring str = umbase::UMStringUtil::utf8_to_utf16(absolute_or_relative_file_name);
				umstring file_name = umbase::UMPath::get_file_name(str);
				umstring path = umbase::UMPath::get_absolute_path(absolute_file_path, file_name);
				ummaterial->mutable_texture_path_list().push_back(path);
#endif
			}

			const int polygon_count = static_cast<int>(std::count(
				ummesh.material_index_list().begin(), 
				ummesh.material_index_list().end(),
				i));

			ummaterial->set_polygon_count(polygon_count);
			mesh->mutable_material_list().at(i) = ummaterial;
		}

		// default material
		if (size == 0)
		{
			mesh->mutable_material_list().resize(1);
			UMMaterialPtr ummaterial = UMMaterial::default_material();
			const int polygon_count = static_cast<int>(ummesh.vertex_index_list().size());
			ummaterial->set_polygon_count(polygon_count);
			mesh->mutable_material_list().at(0) = ummaterial;
		}
	}
	
	/**
	 * load camera from umio to umdraw
	 */
	void load_camera(UMCameraPtr camera, const umio::UMCamera& umcamera)
	{
		camera->mutable_global_transform() = umcamera.global_transform();
		camera->mutable_local_transform() = umcamera.local_transform();
		camera->update_from_node();
	}
	//----------------------------------------------------------------------------

	/**
	 * save mesh from umdraw to umio
	 */
	void save_mesh(umio::UMMesh& mesh, UMMeshPtr ummesh)
	{
		if (!ummesh) return;

		// node parameters
		mesh.set_name(umbase::UMStringUtil::utf16_to_utf8(ummesh->name()));
		mesh.mutable_local_transform() = ummesh->local_transform();
		mesh.mutable_global_transform() = ummesh->global_transform();

		if (ummesh->mutable_face_list().empty()) {
			umio::DoubleList temp(3);

			const int vsize = static_cast<int>(ummesh->vertex_list().size());
			mesh.mutable_vertex_list().resize(vsize);
			for (int i = 0; i < vsize; ++i)
			{
				const UMVec3d& v = ummesh->vertex_list().at(i);
				temp[0] = v.x;
				temp[1] = v.y;
				temp[2] = v.z;
				mesh.mutable_vertex_list()[i] = temp;
			}

			const int nsize = static_cast<int>(ummesh->normal_list().size());
			mesh.mutable_normal_list().resize(nsize);
			for (int i = 0; i < nsize; ++i)
			{
				const UMVec3d& n = ummesh->normal_list().at(i);
				temp[0] = n.x;
				temp[1] = n.y;
				temp[2] = n.z;
				mesh.mutable_normal_list()[i] = temp;
			}

			temp.resize(2);
			const int uvsize = static_cast<int>(ummesh->uv_list().size());
			mesh.mutable_uv_list().resize(uvsize);
			for (int i = 0; i < uvsize; ++i)
			{
				const UMVec2d& uv = ummesh->uv_list().at(i);
				temp[0] = uv.x;
				temp[1] = uv.y;
				mesh.mutable_uv_list()[i] = temp;
			}
		}
	}

	/**
	* save material from umdraw to umio
	*/
	void save_material(umio::UMMesh& mesh, UMMeshPtr ummesh)
	{
		const int matsize = static_cast<int>(ummesh->material_list().size());
		for (int i = 0; i < matsize; ++i)
		{
			UMMaterialPtr mat = ummesh->material_list().at(i);
			umio::UMMaterial ummat;
			ummat.set_name(umbase::UMStringUtil::utf16_to_utf8(mat->name()));
			ummat.set_diffuse(mat->diffuse());
			ummat.set_diffuse_factor(mat->diffuse_factor());
			ummat.set_specular(mat->specular());
			ummat.set_specular_factor(mat->specular_factor());
			ummat.set_emissive(mat->emissive());
			ummat.set_emissive_factor(mat->emissive_factor());
			ummat.set_ambient(mat->ambient());
			ummat.set_ambient_factor(mat->ambient_factor());
			ummat.set_refrection(mat->refrection());
			ummat.set_reflection_factor(mat->reflection_factor());
			ummat.set_transparent(mat->transparent());
			ummat.set_transparency_factor(mat->transparency_factor());

			for (UMMaterial::TexturePathList::const_iterator it = mat->texture_path_list().begin();
				it != mat->texture_path_list().end(); ++it)
			{
				const umstring& texture_path = *it;
				std::string path = umbase::UMStringUtil::utf16_to_utf8(texture_path);
				umio::UMTexture tex;
				tex.set_file_name(path);
				// TODO tex.set_relative_file_name(path);
				ummat.mutable_texture_list().push_back(tex);
			}

			const int index_size = mat->polygon_count() * 3;
			for (int k = 0; k < index_size; ++k) {
				mesh.mutable_material_index().push_back(k);
			}
			mesh.mutable_material_list().push_back(ummat);
		}
	}

	//----------------------------------------------------------------------------
	
	typedef std::pair<int,int> IndexPair;

	/**
	 * sort umio mesh elements by material
	 */
	void sort_by_material(umio::UMMesh& src_mesh)
	{
		// sort material index with index
		size_t index_size = src_mesh.material_index_list().size();
		std::vector< IndexPair > index_pair_list;
		if (index_size > 0)
		{
			// has material index
			index_pair_list.resize(index_size);
			for (size_t i = 0; i < index_size; ++i)
			{
				index_pair_list[i] = IndexPair(src_mesh.material_index_list().at(i), static_cast<int>(i));
			}
			std::sort(index_pair_list.begin(), index_pair_list.end());
		}
		else
		{
			// one material and no material index
			index_size = src_mesh.vertex_index_list().size();
			index_pair_list.resize(index_size);
			for (size_t i = 0; i < index_size; ++i)
			{
				index_pair_list[i] = IndexPair(0, static_cast<int>(i));
			}
		}

		bool is_vertex_sized_normal = src_mesh.vertex_list().size() == src_mesh.normal_list().size();
		bool is_vetrtex_sized_uv = src_mesh.vertex_list().size() == src_mesh.uv_list().size();
		
		// store vertex index and material index by material order
		umio::IntListVec sorted_vertex_index;
		umio::IntList sorted_material_index;
		umio::DoubleListVec sorted_uv;
		umio::DoubleListVec sorted_normal;
		sorted_vertex_index.resize(index_size);
		sorted_material_index.resize(index_size);
		if (!src_mesh.uv_list().empty())
		{
			sorted_uv.resize(index_size * 3);
		}
		if (!is_vertex_sized_normal)
		{
			sorted_normal.resize(index_size * 3);
		}
		if (!is_vetrtex_sized_uv)
		{
			sorted_uv.resize(index_size * 3);
		}

		for (size_t i = 0; i < index_size; ++i)
		{
			IndexPair& pair = index_pair_list[i];
			sorted_material_index[i] = pair.first;
			sorted_vertex_index[i] = src_mesh.vertex_index_list().at(pair.second);
			if (is_vetrtex_sized_uv && !src_mesh.uv_list().empty())
			{
				for (int k = 0; k < 3; ++k)
				{
					int h = sorted_vertex_index[i][k];
					sorted_uv[i * 3 + k] = src_mesh.uv_list().at(sorted_vertex_index[i][k]);
				}
			}
			else if (!src_mesh.uv_list().empty())
			{
				for (int k = 0; k < 3; ++k)
				{
					sorted_uv[i * 3 + k] = src_mesh.uv_list().at(pair.second * 3 + k);
				}
			}
			if (!is_vertex_sized_normal && !src_mesh.normal_list().empty())
			{
				for (int k = 0; k < 3; ++k)
				{
					sorted_normal[i * 3 + k] = src_mesh.normal_list().at(pair.second * 3 + k);
				}
			}
		}

		src_mesh.mutable_vertex_index_list().swap(sorted_vertex_index);
		src_mesh.mutable_material_index().swap(sorted_material_index);

		if (!is_vetrtex_sized_uv && !src_mesh.uv_list().empty())
		{
			src_mesh.mutable_uv_list().swap(sorted_uv);
		}
		if (!is_vertex_sized_normal)
		{
			src_mesh.mutable_normal_list().swap(sorted_normal);
		}
	}

	/**
	 * convert vertices to triangle list
	 */
	void convert_to_triangle_list(umio::UMMesh& src_mesh)
	{
		const int vertex_index_size = static_cast<int>(src_mesh.vertex_index_list().size());
		umio::DoubleListVec triangle_list;
		triangle_list.resize(vertex_index_size * 3);
		
		umio::DoubleListVec normal_list;
		normal_list.resize(vertex_index_size * 3);
		
		for (int i = 0; i < vertex_index_size; ++i)
		{
			const umio::IntList& vertex_index = src_mesh.vertex_index_list().at(i);
			for (int k = 0; k < 3; ++k)
			{
				triangle_list[i * 3 + k] = src_mesh.vertex_list().at(vertex_index[k]);
				normal_list[i * 3 + k] = src_mesh.normal_list().at(vertex_index[k]);
			}
		}
		src_mesh.mutable_vertex_list().swap(triangle_list);
		src_mesh.mutable_normal_list().swap(normal_list);
	}

} // anonymouse namespace


namespace umdraw
{

/** 
 * import umdraw mesh list
 */
bool UMSoftwareIO::import_mesh_list(UMMeshList& dst, const umio::UMObjectPtr src, const umstring& absolute_file_path)
{
	if (!src) return false;

	bool result = false;
	umio::UMMesh::IDToMeshMap::iterator it = src->mutable_mesh_map().begin();
	for (; it != src->mutable_mesh_map().end(); ++it)
	{
		umio::UMMesh& ummesh = it->second;

		UMMeshPtr mesh(std::make_shared<UMMesh>());
		dst.push_back(mesh);
		result = true;
		
		sort_by_material(ummesh);
		load_material(absolute_file_path, mesh, ummesh);
		load_vertex_index(mesh, ummesh);
		load_vertex(mesh, ummesh);
		load_normal(mesh, ummesh);
		load_uv(mesh, ummesh);
		load_skin(mesh, ummesh);
		mesh->update_box();
	}
	return result;
}

/** 
 * import umdraw node list
 * @param [out] dst destination mesh list
 * @param [in] src source object
 */
bool UMSoftwareIO::import_node_list(
	UMNodeList& dst,
	UMMeshList& mesh_list, 
	const umio::UMObjectPtr src)
{
	if (!src) return false;

	typedef std::map<int, UMNodePtr> IDNodeMap;
	IDNodeMap id_node_map;
	bool result = false;
	{
		umio::UMSkeleton::IDToSkeletonMap::iterator it = src->mutable_skeleton_map().begin();
		for (; it != src->mutable_skeleton_map().end(); ++it)
		{
			int id = it->first;
			umio::UMSkeleton& skeleton = it->second;
		
			UMNodePtr node = std::make_shared<UMNode>();
			node->set_name(umbase::UMStringUtil::utf8_to_utf16(skeleton.name()));
			node->mutable_local_transform() = skeleton.local_transform();
			node->mutable_global_transform() = skeleton.global_transform();
			node->mutable_initial_global_transform() = node->global_transform();
			node->mutable_initial_local_transform() = node->local_transform();
			id_node_map[id] = node;
		}
	}
	{
		umio::UMSkeleton::IDToNodeMap::iterator it = src->mutable_other_node_map().begin();
		for (; it != src->mutable_other_node_map().end(); ++it)
		{
			int id = it->first;
			umio::UMNode& umnode = it->second;
			UMNodePtr node = std::make_shared<UMNode>();
			node->set_name(umbase::UMStringUtil::utf8_to_utf16(umnode.name()));
			node->mutable_local_transform() = umnode.local_transform();
			node->mutable_global_transform() = umnode.global_transform();
			node->mutable_initial_global_transform() = node->global_transform();
			node->mutable_initial_local_transform() = node->local_transform();
			id_node_map[id] = node;
		}
	}
	
	// connect parent - children
	{
		umio::UMSkeleton::IDToSkeletonMap::iterator it = src->mutable_skeleton_map().begin();
		for (int i = 0; it != src->mutable_skeleton_map().end(); ++it, ++i)
		{
			int id = it->first;
			umio::UMSkeleton& skeleton = it->second;
			if (skeleton.parent_id() >= 0 &&
				id_node_map.find(skeleton.parent_id()) != id_node_map.end())
			{
				UMNodePtr node = id_node_map[id];
				UMNodePtr parent = id_node_map[skeleton.parent_id()];
				parent->mutable_children().push_back(node);
				node->set_parent(parent);
			}
		}
	}

	// result node list
	{
		IDNodeMap::iterator it = id_node_map.begin();
		for (; it != id_node_map.end(); ++it)
		{
			dst.push_back(it->second);
		}
	}

	// assign skin node
	{
		UMMeshList::iterator it = mesh_list.begin();
		for (; it != mesh_list.end(); ++it)
		{
			UMMeshPtr mesh = *it;
			UMSkinList::iterator st = mesh->mutable_skin_list().begin();
			for (; st != mesh->mutable_skin_list().end(); ++st)
			{
				if (id_node_map.find(st->link_node_id()) != id_node_map.end())
				{
					st->set_link_node(id_node_map[st->link_node_id()]);
				}
			}
		}
	}
	return true;
}

/** 
 * import umdraw camera list
 * @param [out] dst destination mesh list
 * @param [in] src source object
 */
bool UMSoftwareIO::import_camera_list(
	UMCameraList& dst,
	const umio::UMObjectPtr src,
	int initial_width,
	int initial_height)
{
	if (!src) return false;

	umio::UMCamera::IDToCameraMap::iterator it = src->mutable_camera_map().begin();
	for (; it != src->mutable_camera_map().end(); ++it)
	{
		umio::UMCamera& umcamera = it->second;
		UMCameraPtr camera(std::make_shared<UMCamera>(false, initial_width, initial_height));
		dst.push_back(camera);
		load_camera(camera, umcamera);
	}
	return true;
}

/**
* export mesh list
* @param [out] dst destination
*/
bool UMSoftwareIO::export_mesh_list(
	umio::UMObjectPtr dst,
	const UMMeshList& src)
{
	if (src.empty()) return false;
	if (!dst) return false;

	UMMeshList::const_iterator it = src.begin();
	for (; it != src.end(); ++it) 
	{
		UMMeshPtr ummesh = *it;
		umio::UMMesh mesh;
		save_mesh(mesh, ummesh);
		save_material(mesh, ummesh);
		dst->add_mesh(mesh);
	}
	return true;
}

bool UMSoftwareIO::convert_node_to_octahedron(
	std::vector<UMVec3d>& dst_octahedron,
	std::vector<UMVec3d>& dst_triangles,
	UMNodePtr src_node)
{
	dst_triangles.resize(24);
	dst_octahedron.resize(6);
		
	UMMat44d global = src_node->global_transform();
	UMMat44d parent_global;
	if (src_node->parent())
	{
		parent_global = src_node->parent()->global_transform();
	}
	else
	{
		parent_global = src_node->global_transform();
	}
	// remove scale
	umbase::um_matrix_remove_scale(global, global);
	umbase::um_matrix_remove_scale(parent_global, parent_global);

	UMMat44d global_rot = parent_global;
	global_rot.m[3][0] = global_rot.m[3][1] = global_rot.m[3][2] = 0.0f;
	UMVec3d start(parent_global.m[3][0], parent_global.m[3][1], parent_global.m[3][2]);
	UMVec3d end(global.m[3][0], global.m[3][1], global.m[3][2]);
	double length = (end - start).length();
	if (length <= FLT_EPSILON) { length = 1.0; }
	UMVec3d middle = (start + end) * 0.5;
	UMVec3d dir = (end - start).normalized();
	UMVec3d global_y(global_rot.m[1][0], global_rot.m[1][1], global_rot.m[1][2]);
	UMVec3d global_z(global_rot.m[2][0], global_rot.m[2][1], global_rot.m[2][2]);
	dst_octahedron[0] = end;
	dst_octahedron[1] = middle + dir.cross(global_y) * 0.1 * length;
	dst_octahedron[2] = middle + dir.cross(global_z) * 0.1 * length;
	dst_octahedron[3] = start;
	dst_octahedron[4] = middle + dir.cross(global_y) * -0.1 * length;
	dst_octahedron[5] = middle + dir.cross(global_z) * -0.1 * length;
			
	dst_triangles[0] = dst_octahedron[1];
	dst_triangles[1] = dst_octahedron[0];
	dst_triangles[2] = dst_octahedron[5];
			
	dst_triangles[3] = dst_octahedron[1];
	dst_triangles[4] = dst_octahedron[5];
	dst_triangles[5] = dst_octahedron[3];
			
	dst_triangles[6] = dst_octahedron[1];
	dst_triangles[7] = dst_octahedron[3];
	dst_triangles[8] = dst_octahedron[2];
			
	dst_triangles[9] = dst_octahedron[1];
	dst_triangles[10] = dst_octahedron[2];
	dst_triangles[11] = dst_octahedron[0];
			
	dst_triangles[12] = dst_octahedron[0];
	dst_triangles[13] = dst_octahedron[4];
	dst_triangles[14] = dst_octahedron[5];
			
	dst_triangles[15] = dst_octahedron[5];
	dst_triangles[16] = dst_octahedron[4];
	dst_triangles[17] = dst_octahedron[3];
			
	dst_triangles[18] = dst_octahedron[3];
	dst_triangles[19] = dst_octahedron[4];
	dst_triangles[20] = dst_octahedron[2];
			
	dst_triangles[21] = dst_octahedron[2];
	dst_triangles[22] = dst_octahedron[4];
	dst_triangles[23] = dst_octahedron[0];

	return true;
}

} // umdraw
