/**
 * @file UMMesh.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include "UMVector.h"
#include "UMMaterial.h"
#include "UMMathTypes.h"
#include "UMBox.h"
#include "UMNode.h"
#include "UMSkin.h"

#include <vector>
#include <deque>

namespace umdraw
{

class UMMesh;
typedef std::shared_ptr<UMMesh> UMMeshPtr;
typedef std::weak_ptr<UMMesh> UMMeshWeakPtr;

typedef std::vector<UMMeshPtr> UMMeshList;

/**
 * a mesh object
 */
class UMMesh : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMMesh);
public:
	UMMesh() {}
	~UMMesh() {}
	
	typedef std::vector<UMVec3d> Vec4dList;
	typedef std::vector<UMVec3d> Vec3dList;
	typedef std::vector<UMVec2d> Vec2dList;
	typedef std::vector<UMVec3i> Vec3iList;
	typedef std::vector<int> IndexList;
	typedef std::vector<std::pair<int, int>> TimeList;
	typedef std::vector<IndexList> VertexIndexList;
	typedef std::vector<bool> BoolList;
	
	/**
	 * get face list
	 */
	const Vec3iList& face_list() const { return face_list_; }

	/**
	 * get face list
	 */
	Vec3iList& mutable_face_list() { return face_list_; }

	/**
	 * get vertex index list
	 */
	const VertexIndexList& vertex_index_list() const { return vertex_index_list_; }

	/**
	 * get vertex index list
	 */
	VertexIndexList& mutable_vertex_index_list() { return vertex_index_list_; }

	/** 
	 * get vertex list
	 */
	const Vec3dList& vertex_list() const { return vertex_list_; }

	/**
	 * get vertex list
	 */
	Vec3dList& mutable_vertex_list() { return vertex_list_; }

	/**
	 * get valid list
	 * using on creation mesh
	 * if using face list, validlist's size = face list size
	 * else validlist's size = vertex list size / 3
	 */
	BoolList& mutable_valid_list() { return valid_list_; }

	/**
	 * get valid list
	 */
	const BoolList& valid_list() const { return valid_list_; }

	/**
	* get time list
	*/
	const TimeList& time_list() const { return time_list_; }

	/**
	* get time list
	*/
	TimeList& mutable_time_list() { return time_list_; }

	/**
	 * get valid count
	 */
	int valid_count() const;

	/**
	 * get normal list
	 */
	const Vec3dList& normal_list() const { return normal_list_; }

	/**
	 * get normal list
	 */
	Vec3dList& mutable_normal_list() { return normal_list_; }

	/**
	 * get vertex color list
	 */
	const Vec4dList& vertex_color_list() const { return vertex_color_list_; }

	/**
	 * get vertex color list
	 */
	Vec4dList& mutable_vertex_color_list() { return vertex_color_list_; }

	/**
	 * get uv list
	 */
	const Vec2dList& uv_list() const { return uv_list_; }

	/**
	 * get uv list
	 */
	Vec2dList& mutable_uv_list() { return uv_list_; }

	/**
	 * get uv index list
	 */
	const IndexList& uv_index_list() const { return uv_index_list_; }

	/**
	 * get uv index list
	 */
	IndexList& mutable_uv_index_list() { return uv_index_list_; }

	/**
	 * get material list
	 */
	const UMMaterialList& material_list() const { return material_list_; }
	
	/**
	 * get material list
	 */
	UMMaterialList& mutable_material_list() { return material_list_; }

	/**
	 * get skin list
	 */
	const UMSkinList& skin_list() const { return skin_list_; }

	/** 
	 * get skin list
	 */
	UMSkinList& mutable_skin_list() { return skin_list_; }

	/** 
	 * create normals
	 * @param [in] is_smooth smooth or flat
	 */
	bool create_normals(bool is_smooth);

	/**
	 * get centroid
	 */
	UMVec3d centroid() const { return box().center(); }
	
	/**
	 * get box
	 */
	const umbase::UMBox& box() const { return box_; }
	
	/**
	 * update AABB
	 */
	void update_box();

	/**
	 * update from skin
	 */
	void update(unsigned long time);
	
	/**
	 * get material from face index
	 */ 
	UMMaterialPtr material_from_face_index(int face_index) const;

	/**
	 * clear deform cache(original_vertex_list, original_normal_list)
	 */
	void clear_deform_cache();
	
	/**
	 * is changed or not
	 */
	bool is_changed() const { return is_changed_; }
	
	/**
	 * is changed or not
	 */
	void set_changed(bool is_changed) { is_changed_ = is_changed; }

private:
	Vec3iList face_list_;
	VertexIndexList vertex_index_list_;
	Vec4dList vertex_list_;
	Vec4dList original_vertex_list_;
	Vec4dList normal_list_;
	Vec4dList original_normal_list_;
	Vec4dList vertex_color_list_;
	Vec2dList uv_list_;
	IndexList uv_index_list_;
	BoolList valid_list_;
	TimeList time_list_;
	UMSkinList skin_list_;
	typedef std::pair<int, int> IndexPair;
	typedef std::vector<IndexPair> IndexPairList;
	std::map<int, IndexPairList > vertex_index_to_face_index_map_;

	umbase::UMBox box_;
	UMMaterialList material_list_;
	bool is_changed_;
};

} //umdraw
