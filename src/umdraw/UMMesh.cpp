/**
 * @file UMMesh.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMMesh.h"
#include "UMMacro.h"
#include "UMVector.h"
#include "UMMatrix.h"
#include <vector>

namespace
{
	using namespace umdraw;
	bool create_smooth_normals(UMMesh& mesh)
	{
		return false;
	}

	bool create_flat_normals(UMMesh& mesh)
	{
		return false;
	}
}

namespace umdraw
{
/** 
 * create normals
 * @param [in] is_smooth smooth or flat
 */
bool UMMesh::create_normals(bool is_smooth)
{
	if (vertex_list().empty()) return false;
	
	const int face_size = static_cast<int>(face_list().size());
	const int vertex_size = static_cast<int>(vertex_list().size());
	mutable_normal_list().resize(vertex_size);
	
	for (int i = 0; i < vertex_size; ++i)
	{
		mutable_normal_list().at(i) = UMVec3d(0);
	}

	if (face_size > 0)
	{
		for (int i = 0; i < face_size; ++i)
		{
			UMVec3i face = face_list().at(i);
			const UMVec3d& v0 = vertex_list().at(face.x);
			const UMVec3d& v1 = vertex_list().at(face.y);
			const UMVec3d& v2 = vertex_list().at(face.z);
			UMVec3d normal = (v0-v1).cross(v1-v2);

			mutable_normal_list().at(face.x) += normal;
			mutable_normal_list().at(face.y) += normal;
			mutable_normal_list().at(face.z) += normal;
		}
	}
	else
	{
		for (int i = 0, size = vertex_size / 3; i < size; ++i)
		{
			int vi0 = i * 3 + 0;
			int vi1 = i * 3 + 1;
			int vi2 = i * 3 + 2;
			const UMVec3d& v0 = vertex_list().at(vi0);
			const UMVec3d& v1 = vertex_list().at(vi1);
			const UMVec3d& v2 = vertex_list().at(vi2);
			UMVec3d normal = (v0-v1).cross(v1-v2);

			mutable_normal_list().at(vi0) += normal;
			mutable_normal_list().at(vi1) += normal;
			mutable_normal_list().at(vi2) += normal;
		}
	}
	
	for (int i = 0; i < vertex_size; ++i)
	{
		mutable_normal_list().at(i) = normal_list().at(i).normalized();
	}
	return false;
}

/**
 * update AABB
 */
void UMMesh::update_box()
{
	box_.set_minimum(UMVec3d(std::numeric_limits<double>::infinity()));
	box_.set_maximum(UMVec3d(-std::numeric_limits<double>::infinity()));
	
	const int vertex_size = static_cast<int>(vertex_list().size());
	for (int i = 0; i < vertex_size; ++i)
	{
		box_.extend(vertex_list().at(i));
	}
}

/** 
 * get material from vertex index
 */
UMMaterialPtr UMMesh::material_from_face_index(int face_index) const
{
	int pos = 0;
	UMMaterialList::const_iterator it = material_list_.begin();
	for (; it != material_list_.end(); ++it)
	{
		const int polygon_count = (*it)->polygon_count();
		if (face_index >= pos && face_index < (pos+polygon_count)) {
			return *it;
		}
		pos += polygon_count;
	}
	return UMMaterialPtr();
}

/**
 * update from skin
 */
void UMMesh::update(unsigned long time)
{
	// validate update
	for (int i = 0, size = static_cast<int>(time_list_.size()); i < size; ++i)
	{
		bool old = valid_list_.at(i);
		valid_list_.at(i) = 
			(time >= time_list_.at(i).first)
			&& (time <= time_list_.at(i).second);
		if (old != valid_list_.at(i)) {
			is_changed_ = true;
		}
	}
	if (!time_list_.empty()) { return; }

	// skin update
	if (original_vertex_list_.empty())
	{
		original_vertex_list_ = vertex_list_;
	}
	if (original_normal_list_.empty())
	{
		original_normal_list_ = normal_list_;
	}
	if (vertex_index_to_face_index_map_.empty())
	{
		const int fsize = static_cast<int>(face_list_.size());
		for (int i = 0; i < fsize; ++i)
		{
			const UMVec3i& face = face_list_.at(i);
			for (int k = 0; k < 3; ++k)
			{
				IndexPairList& pair_list = vertex_index_to_face_index_map_[face[k]];
				pair_list.push_back(IndexPair(i, k));
			}
		}
	}
	
	vertex_list_.clear();
	vertex_list_.resize(original_vertex_list_.size());
	normal_list_.clear();
	normal_list_.resize(original_normal_list_.size());

	if (skin_list_.empty())
	{
		// no skin
		const UMMat44d& global_transform = UMNode::global_transform();
		const UMMat44d& initial_global_transform = UMNode::initial_global_transform();
		UMMat44d global_rot = global_transform;
		global_rot.m[3][0] = global_rot.m[3][1] = global_rot.m[3][2] = 0.0;
		UMMat44d initial_global_rot = initial_global_transform;
		initial_global_rot.m[3][0] = initial_global_rot.m[3][1] = initial_global_rot.m[3][2] = 0.0;
		UMMat44d initial_global_rot_inv = initial_global_rot.inverted();

		for (int i = 0, isize = static_cast<int>(original_vertex_list_.size()); i < isize; ++i)
		{
			const UMVec3d& v = initial_global_transform.inverted() * original_vertex_list_[i];
			vertex_list_.at(i) = global_transform * v;
		}
		for (int i = 0, isize = static_cast<int>(original_normal_list_.size()); i < isize; ++i)
		{
			const UMVec3d& n = initial_global_rot_inv * original_normal_list_[i];
 			normal_list_[i] = global_rot * n;
		}
	}
	else
	{
		// skin deform
		UMSkinList::iterator it = skin_list_.begin();
		for (; it != skin_list_.end(); ++it)
		{
			UMSkin& skin = *it;
			UMNodePtr link = skin.link_node();
			if (link)
			{
				const UMMat44d& global_transform = link->global_transform();
				const UMMat44d& initial_global_transform = link->initial_global_transform();

				UMMat44d global_rot = global_transform;
				global_rot.m[3][0] = global_rot.m[3][1] = global_rot.m[3][2] = 0.0;
				UMMat44d initial_global_rot = initial_global_transform;
				initial_global_rot.m[3][0] = initial_global_rot.m[3][1] = initial_global_rot.m[3][2] = 0.0;
				UMMat44d initial_global_rot_inv = initial_global_rot.inverted();

				UMSkin::IndexList::const_iterator st = skin.index_list().begin();
				for (int k = 0; st != skin.index_list().end(); ++st, ++k)
				{
					int index = *st;
					double weight = skin.weight_list().at(k);
					const UMVec3d& v = initial_global_transform.inverted() * original_vertex_list_[index];
					vertex_list_.at(index) += global_transform * v * weight;

					if (original_normal_list_.size() > original_vertex_list_.size())
					{
						const IndexPairList& pair_list = vertex_index_to_face_index_map_[index];
						const int pair_size = static_cast<int>(pair_list.size());
						for (int p = 0; p < pair_size; ++p)
						{
							const IndexPair& pair = pair_list[p];
							const int ni = pair.first * 3 + pair.second;
							const UMVec3d& n = initial_global_rot_inv * original_normal_list_[ni];
 							normal_list_[ni] += global_rot * n * weight;
						}
					}
					else if (original_normal_list_.size() == original_vertex_list_.size())
					{
						const UMVec3d& n = initial_global_rot_inv * original_normal_list_[index];
						normal_list_[index] += global_rot * n * weight;
					}
				}
			}
		}
	}
	
	{
		Vec4dList::iterator it = normal_list_.begin();
		for (; it != normal_list_.end(); ++it)
		{
			*it = it->normalized();
		}
	}
}

void UMMesh::clear_deform_cache()
{
	original_vertex_list_.clear();
	original_normal_list_.clear();
}

int UMMesh::valid_count() const
{
	int valid_count = 0;
	for (int i = 0, size = valid_list_.size(); i < size; ++i)
	{
		if (valid_list_[i]) { ++valid_count; }
	}
	return valid_count;
}

} //umdraw