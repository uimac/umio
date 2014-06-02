/**
 *
 * @file UMTinyObj.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#include "UMTinyObj.h"
#include "UMObject.h"

#ifdef WITH_TINYOBJ
#include <tiny_obj_loader.h>
#include <iostream>
#endif

namespace umio
{

#if !defined(WITH_TINYOBJ)
UMObjectPtr UMTinyObj::load(std::string path, const UMIOSetting& setting) { return UMObjectPtr(); }

#else

/**
 * load obj from tinyobjloader
 */
UMObjectPtr UMTinyObj::load(std::string path, const UMIOSetting& setting)
{
	std::vector<tinyobj::shape_t> shapes;
	std::string err = tinyobj::LoadObj(shapes, path.c_str());
	if (!err.empty()) {
		return UMObjectPtr();
	}
	UMObjectPtr umobj = UMObject::create_object();
	if (!umobj)
	{
		return umobj;
	}

	for (size_t i = 0; i < shapes.size(); i++)
	{
		tinyobj::shape_t& shape = shapes.at(i);
		UMMesh ummesh;

		const size_t face_count = shapes[i].mesh.indices.size() / 3;
		// vertex index
		{
			for (size_t f = 0; f < face_count; ++f)
			{
				IntList vertex_index_list(3);
				vertex_index_list[0] = shapes[i].mesh.indices[f*3+0];
				vertex_index_list[1] = shapes[i].mesh.indices[f*3+1];
				vertex_index_list[2] = shapes[i].mesh.indices[f*3+2];
				ummesh.mutable_vertex_index_list().push_back(vertex_index_list);
			}
		}
		// vertex
		{
			for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; ++v)
			{
				DoubleList vertex_list(3);
				vertex_list[0] = shapes[i].mesh.positions[3*v+0];
				vertex_list[1] = shapes[i].mesh.positions[3*v+1];
				vertex_list[2] = shapes[i].mesh.positions[3*v+2];
				ummesh.mutable_vertex_list().push_back(vertex_list);
			}
		}
		// normal
		{
			for (size_t n = 0; n < shapes[i].mesh.normals.size() / 3; ++n) 
			{
				DoubleList normal_list(3);
				normal_list[0] = shapes[i].mesh.normals[3*n+0];
				normal_list[1] = shapes[i].mesh.normals[3*n+1];
				normal_list[2] = shapes[i].mesh.normals[3*n+2];
				ummesh.mutable_normal_list().push_back(normal_list);
			}
		}
		// uv
		{
			for (size_t uv = 0; uv < shapes[i].mesh.texcoords.size() / 2; ++uv)
			{
				DoubleList uv_list(2);
				uv_list[0] = shapes[i].mesh.texcoords[2*uv+0];
				uv_list[1] = shapes[i].mesh.texcoords[2*uv+1];
				ummesh.mutable_uv_list().push_back(uv_list);
			}
		}
		// material
		{
			UMMaterial ummat;
			tinyobj::material_t& mat = shapes[i].material;
			ummat.set_name(mat.name);
			ummat.set_ambient(UMVec4d(
				static_cast<double>(mat.ambient[0]),
				static_cast<double>(mat.ambient[1]),
				static_cast<double>(mat.ambient[2]), 1.0));
			ummat.set_diffuse(UMVec4d(
				static_cast<double>(mat.diffuse[0]),
				static_cast<double>(mat.diffuse[1]),
				static_cast<double>(mat.diffuse[2]), 1.0));
			ummat.set_specular(UMVec4d(
				static_cast<double>(mat.specular[0]),
				static_cast<double>(mat.specular[1]),
				static_cast<double>(mat.specular[2]), 1.0));
			ummat.set_transparent(UMVec4d(
				static_cast<double>(mat.transmittance[0]),
				static_cast<double>(mat.transmittance[1]),
				static_cast<double>(mat.transmittance[2]), 1.0));
			ummat.set_emissive(UMVec4d(
				static_cast<double>(mat.emission[0]),
				static_cast<double>(mat.emission[1]),
				static_cast<double>(mat.emission[2]), 1.0));
			ummat.set_shininess(
				static_cast<double>(mat.shininess));

			if (!mat.diffuse_texname.empty())
			{
				UMTexture texture;
				texture.set_file_name(mat.diffuse_texname);
				ummat.add_texture(texture);
			}
			if (!mat.specular_texname.empty())
			{
				UMTexture texture;
				texture.set_file_name(mat.specular_texname);
				ummat.add_texture(texture);
			}
			if (!mat.normal_texname.empty())
			{
				UMTexture texture;
				texture.set_file_name(mat.normal_texname);
				ummat.add_texture(texture);
			}
			if (!mat.ambient_texname.empty())
			{
				UMTexture texture;
				texture.set_file_name(mat.ambient_texname);
				ummat.add_texture(texture);
			}
			for (size_t k = 0; k < ummesh.vertex_index_list().size(); ++k)
			{
				ummesh.add_material_index(static_cast<int>(ummesh.material_list().size()));
			}
			ummesh.add_material(ummat);
		}
		umobj->add_mesh(ummesh);
		return umobj;
	}
	return UMObjectPtr();
}

#endif

} // namespace umio
