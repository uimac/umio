/**
*
* @file UMPython.cpp
*
* @author tori31001 at gmail.com
*
*
* Copyright (C) 2014 Kazuma Hatta
* Licensed under the MIT or GPL Version 3 licenses.
*
*/
#ifdef WITH_V8

#include <stdio.h>
#include <iostream>
#include <string>

#include "UMIO.h"
#include "UMObject.h"
#include "UMAnimation.h"
#

#include <node.h>
#include "v8pp/context.hpp"
#include "v8pp/convert.hpp"
#include "v8pp/module.hpp"
#include "v8pp/class.hpp"
#include "v8pp/property.hpp"

using namespace v8;

namespace umio {

	template<class T, class WrapT>
	v8::Handle<v8::Object> wrap_map(T& map) {
		Isolate *isolate = Isolate::GetCurrent();
		v8::Local<v8::Object> obj = v8::Object::New(isolate);
		for (auto &pair : map) {
			obj->Set(pair.first,
				v8pp::class_<WrapT>::import_external(
					Isolate::GetCurrent(), new WrapT(&pair.second)));

		}
		return obj;
	}

	template<class T, class WrapT>
	v8::Handle<v8::Object> wrap_vec(T& vec) {
		Isolate *isolate = Isolate::GetCurrent();
		v8::Local<v8::Object> obj = v8::Object::New(isolate);
		for (int i = 0; i < vec.size(); ++i) {
			obj->Set(i, v8pp::class_<WrapT>::import_external(Isolate::GetCurrent(), new WrapT(&vec[i])));
		}
		return obj;
	}

	class WrapUMNode {
	public:
		WrapUMNode() {}
		WrapUMNode(UMNode* node) : node_(node) {}
		virtual ~WrapUMNode() {}

		int id() const { return node_->id(); }
		int parent_id() const { return node_->parent_id(); }
		const std::string& name() const { return node_->name(); }
		const UMNode* parent() const { return node_->parent(); }
		//const NodePointerList children() const { return node_->children(); }
		const UMVec4d& local_translation() const { return node_->local_translation(); }
		const UMVec4d& local_rotation() const { return node_->local_rotation(); }
		const UMVec4d& local_scaling() const { return node_->local_scaling(); }
		const UMVec4d& rotation_offset() const { return node_->rotation_offset(); }
		const UMVec4d& rotation_pivot() const { return node_->rotation_pivot(); }
		const UMVec4d& pre_rotation() const { return node_->pre_rotation(); }
		const UMVec4d& post_rotation() const { return node_->post_rotation(); }
		const UMVec4d& scaling_offset() const { return node_->scaling_offset(); }
		const UMVec4d& scaling_pivot() const { return node_->scaling_pivot(); }
		const UMVec4d& geometric_translation() const { return node_->geometric_translation(); }
		const UMVec4d& geometric_rotation() const { return node_->geometric_rotation(); }
		const UMVec4d& geometric_scaling() const { return node_->geometric_scaling(); }
		const UMMat44d& local_transform() const { return node_->local_transform(); }
		const UMMat44d& global_transform() const { return node_->global_transform(); }

		void set_id(int id) { node_->set_id(id); }
		void set_parent_id(int parent_id) { node_->set_parent_id(parent_id); }
		void set_name(const std::string& name) { node_->set_name(name); }
		void set_local_translation(const UMVec4d& trans) { node_->set_local_translation(trans); }
		void set_local_rotation(const UMVec4d& rot) { node_->set_local_rotation(rot); }
		void set_local_scaling(const UMVec4d& scale) { node_->set_local_scaling(scale); }
		void set_rotation_offset(const UMVec4d& offset) { node_->set_rotation_offset(offset); }
		void set_rotation_pivot(const UMVec4d& pivot) { node_->set_rotation_pivot(pivot); }
		void set_pre_rotation(const UMVec4d& rot) { node_->set_pre_rotation(rot); }
		void set_post_rotation(const UMVec4d& rot) { node_->set_post_rotation(rot); }
		void set_scaling_offset(const UMVec4d& offset) { node_->set_scaling_offset(offset); }
		void set_scaling_pivot(const UMVec4d& pivot) { node_->set_scaling_pivot(pivot); }
		void set_geometric_translation(const UMVec4d& trans) { node_->set_geometric_translation(trans); }
		void set_geometric_rotation(const UMVec4d& rot) { node_->set_geometric_rotation(rot); }
		void set_geometric_scaling(const UMVec4d& scale) { node_->set_geometric_scaling(scale); }
		void set_local_transform(const UMMat44d& mat) { node_->set_local_transform(mat); }
		void set_global_transform(const UMMat44d& mat) { node_->set_global_transform(mat); }

		UMNode* node_;
	};

	class WrapUMDeformer {
	public:
		WrapUMDeformer() {}
		WrapUMDeformer(UMDeformer* deformer) : deformer_(deformer) {}
		virtual ~WrapUMDeformer() {}

		int geometry_node_id() const { return deformer_->geometry_node_id(); }
		int deformer_type() const { return deformer_->deformer_type(); }
		void set_geometry_node_id(int geometry_id) { deformer_->set_geometry_node_id(geometry_id); }

		UMDeformer* deformer_;
	};

	class WrapUMShape {
	public:
		WrapUMShape() {}
		WrapUMShape(UMShape* shape) : shape_(shape) {}
		~WrapUMShape() {}

		const std::string& name() const { return shape_->name(); }
		//const UMNode* base_geometry_node() const { return base_geometry_node_; }
		int base_geometry_node_id() const { return shape_->base_geometry_node_id(); }

		const v8::Handle<v8::Object> vertex_index_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), shape_->vertex_index_list());
		}
		const v8::Handle<v8::Object> normal_index_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), shape_->normal_index_list());
		}
		const v8::Handle<v8::Object> vertex_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), shape_->vertex_list());
		}
		const v8::Handle<v8::Object> normal_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), shape_->normal_list());
		}
		UMShape* shape_;
	};

	class WrapUMTexture {
	public:
		WrapUMTexture() {}
		WrapUMTexture(UMTexture* tex) : tex_(tex) {}
		~WrapUMTexture() {}

		const std::string& name() const { return tex_->name(); }
		const std::string& file_name() const { return tex_->file_name(); }
		const std::string& relative_file_name() const { return tex_->relative_file_name(); }
		const int blend_mode() const { return tex_->blend_mode(); }
		const double alpha() const { return tex_->alpha(); }

		void set_name(const std::string& name) { tex_->set_name(name); }
		void set_file_name(const std::string& file_name) { tex_->set_file_name(file_name); }
		void set_relative_file_name(const std::string& relative_file_name) { tex_->set_relative_file_name(relative_file_name); }
		void set_blend_mode(int blend_mode) { tex_->set_blend_mode(blend_mode); }
		void set_alpha(double alpha) { tex_->set_alpha(alpha); }

		UMTexture* tex_;
	};

	class WrapUMMaterial {
	public:
		WrapUMMaterial() {}
		WrapUMMaterial(UMMaterial* mat) : mat_(mat) {}
		~WrapUMMaterial() {}

		const std::string& name() const { return mat_->name(); }
		const UMVec4d& ambient() const { return mat_->ambient(); }
		const UMVec4d& diffuse() const { return mat_->diffuse(); }
		const UMVec4d& specular() const { return mat_->specular(); }
		const UMVec4d& emissive() const { return mat_->emissive(); }
		const UMVec4d& refrection() const { return mat_->refrection(); }
		const UMVec4d& transparent() const { return mat_->transparent(); }
		const UMVec4d& bump() const { return mat_->bump(); }
		const UMVec4d& vector_displacement() const { return mat_->vector_displacement(); }
		const double transparency_factor() const { return mat_->transparency_factor(); }
		const double shininess() const { return mat_->shininess(); }
		const double reflection_factor() const { return mat_->reflection_factor(); }
		const double diffuse_factor() const { return mat_->diffuse_factor(); }
		const double specular_factor() const { return mat_->specular_factor(); }
		const double emissive_factor() const { return mat_->emissive_factor(); }
		const double ambient_factor() const { return mat_->ambient_factor(); }
		const double bump_factor() const { return mat_->bump_factor(); }
		const double vector_displacement_factor() const { return mat_->vector_displacement_factor(); }
		const v8::Handle<v8::Object> texture_list() const {
			return wrap_vec<decltype(mat_->mutable_texture_list()), WrapUMTexture>(mat_->mutable_texture_list());
		}
		//const v8::Handle<v8::Object> layered_texture_list() const { 
		//	return mat_->layered_texture_list();
		//}
		UMMaterial* mat_;
	};

	class WrapUMCluster {
	public:
		WrapUMCluster() {}
		WrapUMCluster(UMCluster* cluster) : cluster_(cluster) {}
		~WrapUMCluster() {}

		const std::string& name() const { return cluster_->name(); }
		const v8::Handle<v8::Object> index_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), cluster_->index_list());
		}
		const  v8::Handle<v8::Object> weight_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), cluster_->weight_list());
		}
		int link_mode() const { return cluster_->link_mode(); }
		//const UMNode* link_node() const { return cluster_->link_node(); }
		int link_node_id() const { return cluster_->link_node_id(); }

		void set_name(const std::string& name) { cluster_->set_name(name); }
		void set_link_mode(int link_mode) { cluster_->set_link_mode(link_mode); }
		//void set_link_node(UMNode* link_node) { cluster_->set_link_node(link_node); }
		void set_link_node_id(int link_node_id) { cluster_->set_link_node_id(link_node_id); }

		void add_index(int index) { cluster_->mutable_index_list().push_back(index); }
		void add_weight(double weight) { cluster_->mutable_weight_list().push_back(weight); }

		UMCluster* cluster_;
	};

	class WrapUMSkin : public WrapUMDeformer {
	public:
		WrapUMSkin() {}
		WrapUMSkin(UMSkin* skin) : WrapUMDeformer(skin), skin_(skin) {}
		~WrapUMSkin() {}

		const std::string& name() const {
			return skin_->name();
		}
		const v8::Handle<v8::Object> cluster_list() const {
			return wrap_vec<decltype(skin_->mutable_cluster_list()), WrapUMCluster>(skin_->mutable_cluster_list());
		}
		void set_name(const std::string& name) { skin_->set_name(name); }

		void add_cluster(v8::Handle<v8::Object> cluster) {
			WrapUMCluster* converted = v8pp::from_v8<WrapUMCluster*>(Isolate::GetCurrent(), cluster);
			if (converted) {
				skin_->mutable_cluster_list().push_back(*converted->cluster_);
			}
		}
		UMSkin* skin_;
	};

	class WrapUMBlendShapeChannel {
	public:
		WrapUMBlendShapeChannel() {}
		WrapUMBlendShapeChannel(UMBlendShapeChannel* chan) :  chan_(chan) {}
		~WrapUMBlendShapeChannel() {}

		UMBlendShapeChannel* chan_;
	};

	class WrapUMBlendShape : public WrapUMDeformer {
	public:
		WrapUMBlendShape() {}
		WrapUMBlendShape(UMBlendShape* shape) : WrapUMDeformer(shape), shape_(shape) {}
		~WrapUMBlendShape() {}

		const std::string& name() const { return shape_->name(); }
		const v8::Handle<v8::Object> blend_shape_channel_list() const {
			return wrap_vec<decltype(shape_->mutable_blend_shape_channel_list()), WrapUMBlendShapeChannel>(shape_->mutable_blend_shape_channel_list());
		}

		void set_name(const std::string& name) { shape_->set_name(name); }
		//void add_blend_shape_channel(const UMBlendShapeChannel& channel) { blend_shape_channel_list_.push_back(channel); }

		UMBlendShape* shape_;
	};

	class WrapUMSkeleton : public WrapUMNode {
	public:
		WrapUMSkeleton() {}
		WrapUMSkeleton(UMSkeleton* skeleton) : WrapUMNode(skeleton), skeleton_(skeleton) {}
		~WrapUMSkeleton() {}

		// getter
		int type() const { return skeleton_->type(); }
		const double limb_length() const { return skeleton_->limb_length(); }
		const double size() const { return skeleton_->size(); }

		// setter
		void set_type(int type) { skeleton_->set_type(type); }
		void set_limb_length(double value) { skeleton_->set_limb_length(value); }
		void set_size(double value) { skeleton_->set_size(value); }

		UMSkeleton* skeleton_;
	};

	//
	// UMMesh
	//
	class WrapUMMesh : public WrapUMNode {
	public:
		WrapUMMesh() {}
		WrapUMMesh(UMMesh* mesh) :  mesh_(mesh) {}
		~WrapUMMesh() {}

		const v8::Handle<v8::Object> vertex_index_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->vertex_index_list());
		}
		const v8::Handle<v8::Object> vertex_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->vertex_list());
		}
		const v8::Handle<v8::Object> material_index_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->material_index_list());
		}
		const v8::Handle<v8::Object> material_list() const {
			return wrap_vec<decltype(mesh_->mutable_material_list()), WrapUMMaterial>(mesh_->mutable_material_list());
		}
		const v8::Handle<v8::Object> skin_list() const {
			return wrap_vec<decltype(mesh_->mutable_skin_list()), WrapUMSkin>(mesh_->mutable_skin_list());
		}
		const v8::Handle<v8::Object> blend_shape_list() const {
			return wrap_vec<decltype(mesh_->mutable_blend_shape_list()), WrapUMBlendShape>(mesh_->mutable_blend_shape_list());
		}
		const v8::Handle<v8::Object> normal_list(int layer = 0) const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_normal_list().at(layer));
		}
		const v8::Handle<v8::Object> vertex_color_list(int layer = 0) const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_vertex_color_list().at(layer));
		}
		const v8::Handle<v8::Object> uv_list(int layer = 0) const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_uv_list().at(layer));
		}
		const v8::Handle<v8::Object> layered_uv_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_uv_list());
		}
		const v8::Handle<v8::Object> layered_normal_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_normal_list());
		}
		const v8::Handle<v8::Object> layered_vertex_color_list() const {
			return v8pp::to_v8(Isolate::GetCurrent(), mesh_->layered_vertex_color_list());
		}

		UMMesh* mesh_;
	};

	//
	// UMObject
	//
	class WrapUMObject {
	public:
		WrapUMObject() {}
		WrapUMObject(UMObjectPtr obj) : obj_(obj) {}
		~WrapUMObject() {}

		// mesh
		v8::Handle<v8::Object> mesh_map()  {
			return wrap_map<decltype(obj_->mutable_mesh_map()), WrapUMMesh>(obj_->mutable_mesh_map());
		}

		//// skeleton
		//v8::Handle<v8::Object> skeleton_map() {
		//	return wrap_map<decltype(obj_->mutable_skeleton_map()), WrapUMSkeleton>(obj_->mutable_skeleton_map());
		//}

		//// camera
		//v8::Handle<v8::Object> camera_map() {
		//	return v8pp::to_v8(Isolate::GetCurrent(), obj_->mutable_camera_map());
		////	return wrap_map<decltype(obj_->mutable_camera_map()), UMCamera>(obj_->mutable_camera_map());
		//}

		//// other node list
		//v8::Handle<v8::Object> other_node_map() const {
		//	return wrap_map<decltype(obj_->mutable_other_node_map()), WrapUMNode>(obj_->mutable_other_node_map());
		//}

		//// pose
		//const UMPose::PoseList& pose_list() const { return pose_list_; }

		UMObjectPtr obj_;
	};

	class WrapUMIO {
		umio::UMIO umio;
	public:
		WrapUMIO() {}
		~WrapUMIO() {}

		v8::Handle<v8::Object> load(std::string path, const UMIOSetting& setting) {
			UMObjectPtr obj = umio.load(path, setting);
			return v8pp::class_<umio::WrapUMObject>::import_external(Isolate::GetCurrent(), new WrapUMObject(obj));
		}

		bool save(std::string path, v8::Handle<v8::Object> object, const UMIOSetting& setting) {
			WrapUMObject* converted = v8pp::from_v8<WrapUMObject*>(Isolate::GetCurrent(), object);
			return umio.save(path, converted->obj_, setting);
		}

		//v8::Handle<v8::Object> load_from_memory(const std::string& src, const UMIOSetting& setting) {
		//	UMObjectPtr obj = umio.load_from_memory(src, setting);
		//	return v8pp::class_<umio::UMObject, true>::reference_external(Isolate::GetCurrent(), obj);
		//}

		////char* save_to_memory(UMObjectPtr object, const UMIOSetting& setting) {
		////	int dst_size;
		////	return umio.save_to_memory(dst_size, object, setting);
		////}

		//bool load_setting(std::string path, UMIOSetting& setting) {
		//	return umio.load_setting(path, setting);
		//}

		//bool save_setting(std::string path, const UMIOSetting& setting) {
		//	return umio.save_setting(path, setting);
		//}
	};

	static void umio_module(v8::Handle<v8::Object> exports)
	{
		Isolate* isolate = Isolate::GetCurrent();

		v8pp::class_<UMIOSetting> UMIOSetting_class(isolate);
		UMIOSetting_class
			// setter
			.ctor<>() //constructor
			.set("set_exp_bool_prop", &UMIOSetting::set_exp_bool_prop)
			.set("set_imp_bool_prop", &UMIOSetting::set_imp_bool_prop)
			.set("set_bl_exp_bool_prop", &UMIOSetting::set_bl_exp_bool_prop)
			.set("set_bl_imp_bool_prop", &UMIOSetting::set_bl_imp_bool_prop)
			.set("set_axis_type", &UMIOSetting::set_axis_type)
			.set("set_system_unit_type", &UMIOSetting::set_system_unit_type)
			;

		//v8pp::class_<UMIOSetting::EExpSettingType> EExpSettingType_class(isolate);
		//EExpSettingType_class
		//	.set_const("ExpColladaTriangulate", UMIOSetting::eExpColladaTriangulate)
		//	.set_const("ExpColladaSingleMatrix", UMIOSetting::eExpColladaSingleMatrix)
		//	.set_const("ExpColladaFrameRate", UMIOSetting::eExpColladaFrameRate)
		//	.set_const("ExpDxfTriangulate", UMIOSetting::eExpDxfTriangulate)
		//	.set_const("ExpDxfDeformation", UMIOSetting::eExpDxfDeformation)
		//	.set_const("ExpObjTriangulate", UMIOSetting::eExpObjTriangulate)
		//	.set_const("ExpObjDeformation", UMIOSetting::eExpObjDeformation)
		//	.set_const("Exp3dsReferenceNode", UMIOSetting::eExp3dsReferenceNode)
		//	.set_const("Exp3dsTexture", UMIOSetting::eExp3dsTexture)
		//	.set_const("Exp3dsMaterial", UMIOSetting::eExp3dsMaterial)
		//	.set_const("Exp3dsAnimation", UMIOSetting::eExp3dsAnimation)
		//	.set_const("Exp3dsMesh", UMIOSetting::eExp3dsMesh)
		//	.set_const("Exp3dsLight", UMIOSetting::eExp3dsLight)
		//	.set_const("Exp3dsCamera", UMIOSetting::eExp3dsCamera)
		//	.set_const("Exp3dsAmbientLight", UMIOSetting::eExp3dsAmbientLight)
		//	.set_const("Exp3dsRescaling", UMIOSetting::eExp3dsRescaling)
		//	.set_const("Exp3dsTexuvbypoly", UMIOSetting::eExp3dsTexuvbypoly)
		//	.set_const("ExpFbxTemplate", UMIOSetting::eExpFbxTemplate)
		//	.set_const("ExpFbxPivot", UMIOSetting::eExpFbxPivot)
		//	.set_const("ExpFbxGlobalSettings", UMIOSetting::eExpFbxGlobalSettings)
		//	.set_const("ExpFbxCharacter", UMIOSetting::eExpFbxCharacter)
		//	.set_const("ExpFbxConstraint", UMIOSetting::eExpFbxConstraint)
		//	.set_const("ExpFbxGobo", UMIOSetting::eExpFbxGobo)
		//	.set_const("ExpFbxShape", UMIOSetting::eExpFbxShape)
		//	.set_const("ExpFbxMaterial", UMIOSetting::eExpFbxMaterial)
		//	.set_const("ExpFbxTexture", UMIOSetting::eExpFbxTexture)
		//	.set_const("ExpFbxModel", UMIOSetting::eExpFbxModel)
		//	.set_const("ExpFbxAnimation", UMIOSetting::eExpFbxAnimation)
		//	.set_const("ExpFbxEmbedded", UMIOSetting::eExpFbxEmbedded)
		//	.set_const("ExpFbxPassword", UMIOSetting::eExpFbxPassword)
		//	.set_const("ExpFbxPasswordEnable", UMIOSetting::eExpFbxPasswordEnable)
		//	.set_const("ExpFbxCollapseExternals", UMIOSetting::eExpFbxCollapseExternals)
		//	.set_const("ExpFbxCompressArrays", UMIOSetting::eExpFbxCompressArrays)
		//	.set_const("ExpFbxCompressLevel", UMIOSetting::eExpFbxCompressLevel)
		//	.set_const("ExpFbxCompressMinsize", UMIOSetting::eExpFbxCompressMinsize)
		//	.set_const("ExpFbxEmbeddedPropertiesSkipExpFbx", UMIOSetting::eExpFbxEmbeddedPropertiesSkipExpFbx)
		//	.set_const("ExpFbxExportFileVersion", UMIOSetting::eExpFbxExportFileVersion)
		//	;

		//v8pp::class_<UMIOSetting::EImpSettingType> EImpSettingType_class(isolate);
		//EImpSettingType_class
		//	.set_const("ImpDxfWeldVertices", UMIOSetting::eImpDxfWeldVertices)
		//	.set_const("ImpDxfObjectDerivation", UMIOSetting::eImpDxfObjectDerivation)
		//	.set_const("ImpDxfReferenceNode", UMIOSetting::eImpDxfReferenceNode)
		//	.set_const("ImpObjReferenceNode", UMIOSetting::eImpObjReferenceNode)
		//	.set_const("Imp3dsReferenceNode", UMIOSetting::eImp3dsReferenceNode)
		//	.set_const("Imp3dsTexture", UMIOSetting::eImp3dsTexture)
		//	.set_const("Imp3dsMaterial", UMIOSetting::eImp3dsMaterial)
		//	.set_const("Imp3dsAnimation", UMIOSetting::eImp3dsAnimation)
		//	.set_const("Imp3dsMesh", UMIOSetting::eImp3dsMesh)
		//	.set_const("Imp3dsLight", UMIOSetting::eImp3dsLight)
		//	.set_const("Imp3dsCamera", UMIOSetting::eImp3dsCamera)
		//	.set_const("Imp3dsAmbientLight", UMIOSetting::eImp3dsAmbientLight)
		//	.set_const("Imp3dsRescaling", UMIOSetting::eImp3dsRescaling)
		//	.set_const("Imp3dsFilter", UMIOSetting::eImp3dsFilter)
		//	.set_const("Imp3dsSmoothgroup", UMIOSetting::eImp3dsSmoothgroup)
		//	.set_const("ImpFbxModelCount", UMIOSetting::eImpFbxModelCount)
		//	.set_const("ImpFbxDeviceCount", UMIOSetting::eImpFbxDeviceCount)
		//	.set_const("ImpFbxCharacterCount", UMIOSetting::eImpFbxCharacterCount)
		//	.set_const("ImpFbxActorCount", UMIOSetting::eImpFbxActorCount)
		//	.set_const("ImpFbxConstraintCount", UMIOSetting::eImpFbxConstraintCount)
		//	.set_const("ImpFbxMediaCount", UMIOSetting::eImpFbxMediaCount)
		//	.set_const("ImpFbxTemplate", UMIOSetting::eImpFbxTemplate)
		//	.set_const("ImpFbxPivot", UMIOSetting::eImpFbxPivot)
		//	.set_const("ImpFbxGlobalSettings", UMIOSetting::eImpFbxGlobalSettings)
		//	.set_const("ImpFbxCharacter", UMIOSetting::eImpFbxCharacter)
		//	.set_const("ImpFbxConstraint", UMIOSetting::eImpFbxConstraint)
		//	.set_const("ImpFbxMergeLayerAndTimewarp", UMIOSetting::eImpFbxMergeLayerAndTimewarp)
		//	.set_const("ImpFbxGobo", UMIOSetting::eImpFbxGobo)
		//	.set_const("ImpFbxShape", UMIOSetting::eImpFbxShape)
		//	.set_const("ImpFbxLink", UMIOSetting::eImpFbxLink)
		//	.set_const("ImpFbxMaterial", UMIOSetting::eImpFbxMaterial)
		//	.set_const("ImpFbxTexture", UMIOSetting::eImpFbxTexture)
		//	.set_const("ImpFbxModel", UMIOSetting::eImpFbxModel)
		//	.set_const("ImpFbxAnimation", UMIOSetting::eImpFbxAnimation)
		//	.set_const("ImpFbxPassword", UMIOSetting::eImpFbxPassword)
		//	.set_const("ImpFbxPasswordEnable", UMIOSetting::eImpFbxPasswordEnable)
		//	.set_const("ImpFbxCurrentTakeName", UMIOSetting::eImpFbxCurrentTakeName)
		//	;

		//v8pp::class_<UMIOSetting::EUMExpSettingType> EUMExpSettingType_class(isolate);
		//EUMExpSettingType_class
		//	.set_const("UMExpFBX", UMIOSetting::eUMExpFBX)
		//	.set_const("UMExpText", UMIOSetting::eUMExpText)
		//	.set_const("UMExpOldFBX", UMIOSetting::eUMExpOldFBX)
		//	;

		//v8pp::class_<UMIOSetting::EUMImpSettingType> EUMImpSettingType_class(isolate);
		//EUMImpSettingType_class
		//	.set_const("UMImpFBX", UMIOSetting::eUMImpFBX)
		//	.set_const("UMImpTriangulate", UMIOSetting::eUMImpTriangulate)
		//	.set_const("UMImpNurbs", UMIOSetting::eUMImpNurbs)
		//	.set_const("UMImpPatch", UMIOSetting::eUMImpPatch)
		//	;

		//v8pp::class_<UMIOSetting::EFbxAxisType> EFbxAxisType_class(isolate);
		//EFbxAxisType_class
		//	.set_const("AxisMayaZUp", UMIOSetting::eFbxAxisMayaZUp)
		//	.set_const("AxisMayaYUp", UMIOSetting::eFbxAxisMayaYUp)
		//	.set_const("AxisMax", UMIOSetting::eFbxAxisMax)
		//	.set_const("AxisMotionBuilder", UMIOSetting::eFbxAxisMotionBuilder)
		//	.set_const("AxisOpenGL", UMIOSetting::eFbxAxisOpenGL)
		//	.set_const("AxisDirectX", UMIOSetting::eFbxAxisDirectX)
		//	.set_const("AxisLightWave", UMIOSetting::eFbxAxisLightWave)
		//	;

		//v8pp::class_<UMIOSetting::EFbxSystemUnitType> EFbxSystemUnitType_class(isolate);
		//EFbxSystemUnitType_class
		//	.set_const("SystemUnitMM", UMIOSetting::eFbxSystemUnitMM)
		//	.set_const("SystemUnitDM", UMIOSetting::eFbxSystemUnitDM)
		//	.set_const("SystemUnitCM", UMIOSetting::eFbxSystemUnitCM)
		//	.set_const("SystemUnitM", UMIOSetting::eFbxSystemUnitM)
		//	.set_const("SystemUnitKM", UMIOSetting::eFbxSystemUnitKM)
		//	.set_const("SystemUnitInch", UMIOSetting::eFbxSystemUnitInch)
		//	.set_const("SystemUnitFoot", UMIOSetting::eFbxSystemUnitFoot)
		//	.set_const("SystemUnitMile", UMIOSetting::eFbxSystemUnitMile)
		//	.set_const("SystemUnitYard", UMIOSetting::eFbxSystemUnitYard)
		//	;

		//
		// UMVec4d
		//
		v8pp::class_<umio::UMVec4d> UMVec4d_class(isolate);
		UMVec4d_class
			.ctor<>() //constructor
			.set("x", &UMVec4d::x)
			.set("y", &UMVec4d::y)
			.set("z", &UMVec4d::z)
			.set("w", &UMVec4d::w)
			;

		//
		// UMMat44d
		//
		v8pp::class_<umio::UMMat44d> UMMat44d_class(isolate);
		UMMat44d_class
			.ctor<>() //constructor
			.set("set", &UMMat44d::set)
			.set("get", &UMMat44d::get)
			;

		//
		// UMNode
		//
		v8pp::class_<umio::WrapUMNode> WrapUMNode_class(isolate);
		WrapUMNode_class
			.ctor<>() //constructor
			.set("id", &UMNode::id)
			.set("parent_id", &UMNode::parent_id)
			//.set("parent", &UMNode::parent, return_value_policy::reference)
			.set("name", &UMNode::name)
			.set("local_translation", &UMNode::local_translation)
			.set("local_rotation", &UMNode::local_rotation)
			.set("local_scaling", &UMNode::local_scaling)
			.set("rotation_offset", &UMNode::rotation_offset)
			.set("rotation_pivot", &UMNode::rotation_pivot)
			.set("pre_rotation", &UMNode::pre_rotation)
			.set("post_rotation", &UMNode::post_rotation)
			.set("scaling_offset", &UMNode::scaling_offset)
			.set("scaling_pivot", &UMNode::scaling_pivot)
			.set("geometric_translation", &UMNode::geometric_translation)
			.set("geometric_rotation", &UMNode::geometric_rotation)
			.set("geometric_scaling", &UMNode::geometric_scaling)
			.set("local_transform", &UMNode::local_transform)
			.set("global_transform", &UMNode::global_transform)
			// setter
			.set("set_id", &UMNode::set_id)
			.set("set_parent_id", &UMNode::set_parent_id)
			.set("set_name", &UMNode::set_name)
			.set("set_local_translation", &UMNode::set_local_translation)
			.set("set_local_rotation", &UMNode::set_local_rotation)
			.set("set_local_scaling", &UMNode::set_local_scaling)
			.set("set_rotation_offset", &UMNode::set_rotation_offset)
			.set("set_rotation_pivot", &UMNode::set_rotation_pivot)
			.set("set_pre_rotation", &UMNode::set_pre_rotation)
			.set("set_post_rotation", &UMNode::set_post_rotation)
			.set("set_scaling_offset", &UMNode::set_scaling_offset)
			.set("set_scaling_pivot", &UMNode::set_scaling_pivot)
			.set("set_geometric_translation", &UMNode::set_geometric_translation)
			.set("set_geometric_rotation", &UMNode::set_geometric_rotation)
			.set("set_geometric_scaling", &UMNode::set_geometric_scaling)
			.set("set_local_transform", &UMNode::set_local_transform)
			.set("set_global_transform", &UMNode::set_global_transform)
			;

		////
		//// UMCamera
		////
		//v8pp::class_<umio::UMCamera> UMCamera_class(isolate);
		//UMCamera_class
		//	.ctor<>() //constructor
		//	.inherit<umio::UMNode>()
		//	// getter
		//	.set("position", &UMCamera::position)
		//	.set("up_vector", &UMCamera::up_vector)
		//	.set("interest_position", &UMCamera::interest_position)
		//	.set("roll", &UMCamera::roll)
		//	.set("optical_center_x", &UMCamera::optical_center_x)
		//	.set("optical_center_y", &UMCamera::optical_center_y)
		//	.set("background_color", &UMCamera::background_color)
		//	.set("turn_table", &UMCamera::turn_table)
		//	.set("display_turn_table_icon", &UMCamera::display_turn_table_icon)
		//	.set("use_motion_blur", &UMCamera::use_motion_blur)
		//	.set("use_real_time_motion_blur", &UMCamera::use_real_time_motion_blur)
		//	.set("motion_blur_intensity", &UMCamera::motion_blur_intensity)
		//	.set("aspect_ratio_mode", &UMCamera::aspect_ratio_mode)
		//	.set("aspect_width", &UMCamera::aspect_width)
		//	.set("aspect_height", &UMCamera::aspect_height)
		//	.set("pixel_aspect_ration", &UMCamera::pixel_aspect_ration)
		//	.set("aperture_mode", &UMCamera::aperture_mode)
		//	.set("gate_fit", &UMCamera::gate_fit)
		//	.set("field_of_view", &UMCamera::field_of_view)
		//	.set("field_of_view_x", &UMCamera::field_of_view_x)
		//	.set("field_of_view_y", &UMCamera::field_of_view_y)
		//	.set("focal_length", &UMCamera::focal_length)
		//	.set("camera_format", &UMCamera::camera_format)
		//	.set("use_frame_color", &UMCamera::use_frame_color)
		//	.set("frame_color", &UMCamera::frame_color)
		//	.set("show_name", &UMCamera::show_name)
		//	.set("show_info_on_moving", &UMCamera::show_info_on_moving)
		//	.set("show_grid", &UMCamera::show_grid)
		//	.set("show_optical_center", &UMCamera::show_optical_center)
		//	.set("show_azimut", &UMCamera::show_azimut)
		//	.set("show_time_code", &UMCamera::show_time_code)
		//	.set("show_audio", &UMCamera::show_audio)
		//	.set("audio_color", &UMCamera::audio_color)
		//	.set("near_plane", &UMCamera::near_plane)
		//	.set("far_plane", &UMCamera::far_plane)
		//	.set("auto_compute_clip_planes", &UMCamera::auto_compute_clip_planes)
		//	.set("film_width", &UMCamera::film_width)
		//	.set("film_height", &UMCamera::film_height)
		//	.set("film_aspect_ration", &UMCamera::film_aspect_ration)
		//	.set("film_squeeze_ration", &UMCamera::film_squeeze_ration)
		//	.set("film_format", &UMCamera::film_format)
		//	.set("film_offset_x", &UMCamera::film_offset_x)
		//	.set("film_offset_y", &UMCamera::film_offset_y)
		//	.set("pre_scale", &UMCamera::pre_scale)
		//	.set("film_translate_x", &UMCamera::film_translate_x)
		//	.set("film_translate_y", &UMCamera::film_translate_y)
		//	.set("film_roll_pivot_x", &UMCamera::film_roll_pivot_x)
		//	.set("film_roll_pivot_y", &UMCamera::film_roll_pivot_y)
		//	.set("film_roll_value", &UMCamera::film_roll_value)
		//	.set("film_roll_order", &UMCamera::film_roll_order)
		//	.set("view_camera_to_look_at", &UMCamera::view_camera_to_look_at)
		//	.set("view_frustum_near_far_plane", &UMCamera::view_frustum_near_far_plane)
		//	.set("view_frustum_back_plane_mode", &UMCamera::view_frustum_back_plane_mode)
		//	.set("back_plane_distance", &UMCamera::back_plane_distance)
		//	.set("back_plane_distance_mode", &UMCamera::back_plane_distance_mode)
		//	.set("view_frustum_front_plane_mode", &UMCamera::view_frustum_front_plane_mode)
		//	.set("front_plane_distance", &UMCamera::front_plane_distance)
		//	.set("front_plane_distance_mode", &UMCamera::front_plane_distance_mode)
		//	.set("lock_mode", &UMCamera::lock_mode)
		//	.set("lock_interest_navigation", &UMCamera::lock_interest_navigation)
		//	.set("back_plate_fit_image", &UMCamera::back_plate_fit_image)
		//	.set("back_plate_crop", &UMCamera::back_plate_crop)
		//	.set("back_plate_center", &UMCamera::back_plate_center)
		//	.set("back_plate_keep_ration", &UMCamera::back_plate_keep_ration)
		//	.set("background_alpha_treshold", &UMCamera::background_alpha_treshold)
		//	.set("back_plane_offset_x", &UMCamera::back_plane_offset_x)
		//	.set("back_plane_offset_y", &UMCamera::back_plane_offset_y)
		//	.set("back_plane_rotation", &UMCamera::back_plane_rotation)
		//	.set("back_plane_scale_x", &UMCamera::back_plane_scale_x)
		//	.set("back_plane_scale_y", &UMCamera::back_plane_scale_y)
		//	.set("show_backplate", &UMCamera::show_backplate)
		//	.set("front_plate_fit_image", &UMCamera::front_plate_fit_image)
		//	.set("front_plate_crop", &UMCamera::front_plate_crop)
		//	.set("front_plate_center", &UMCamera::front_plate_center)
		//	.set("front_plate_keep_ratio", &UMCamera::front_plate_keep_ratio)
		//	.set("show_frontplate", &UMCamera::show_frontplate)
		//	.set("front_plane_offset_x", &UMCamera::front_plane_offset_x)
		//	.set("front_plane_offset_y", &UMCamera::front_plane_offset_y)
		//	.set("front_plane_rotation", &UMCamera::front_plane_rotation)
		//	.set("front_plane_scale_x", &UMCamera::front_plane_scale_x)
		//	.set("front_plane_scale_y", &UMCamera::front_plane_scale_y)
		//	.set("foreground_opacity", &UMCamera::foreground_opacity)
		//	.set("display_safe_area", &UMCamera::display_safe_area)
		//	.set("display_safe_area_on_render", &UMCamera::display_safe_area_on_render)
		//	.set("safe_area_display_style", &UMCamera::safe_area_display_style)
		//	.set("safe_area_display_ratio", &UMCamera::safe_area_display_ratio)
		//	.set("use_2d_magnifier_zoom", &UMCamera::use_2d_magnifier_zoom)
		//	.set("_2d_magnifier_zoom", &UMCamera::_2d_magnifier_zoom)
		//	.set("_2d_magnifier_x", &UMCamera::_2d_magnifier_x)
		//	.set("_2d_magnifier_y", &UMCamera::_2d_magnifier_y)
		//	.set("projection_type", &UMCamera::projection_type)
		//	.set("ortho_zoom", &UMCamera::ortho_zoom)
		//	.set("use_real_time_dof_and_aa", &UMCamera::use_real_time_dof_and_aa)
		//	.set("use_depth_of_field", &UMCamera::use_depth_of_field)
		//	.set("focus_source", &UMCamera::focus_source)
		//	.set("focus_angle", &UMCamera::focus_angle)
		//	.set("focus_distance", &UMCamera::focus_distance)
		//	.set("use_antialiasing", &UMCamera::use_antialiasing)
		//	.set("antialiasing_intensity", &UMCamera::antialiasing_intensity)
		//	.set("antialiasing_method", &UMCamera::antialiasing_method)
		//	.set("use_accumulation_buffer", &UMCamera::use_accumulation_buffer)
		//	.set("frame_sample_count", &UMCamera::frame_sample_count)
		//	.set("frame_sampling_type", &UMCamera::frame_sampling_type)

		//	// setter
		//	.set("set_position", &UMCamera::set_position)
		//	.set("set_up_vector", &UMCamera::set_up_vector)
		//	.set("set_interest_position", &UMCamera::set_interest_position)
		//	.set("set_roll", &UMCamera::set_roll)
		//	.set("set_optical_center_x", &UMCamera::set_optical_center_x)
		//	.set("set_optical_center_y", &UMCamera::set_optical_center_y)
		//	.set("set_background_color", &UMCamera::set_background_color)
		//	.set("set_turn_table", &UMCamera::set_turn_table)
		//	.set("set_display_turn_table_icon", &UMCamera::set_display_turn_table_icon)
		//	.set("set_use_motion_blur", &UMCamera::set_use_motion_blur)
		//	.set("set_use_real_time_motion_blur", &UMCamera::set_use_real_time_motion_blur)
		//	.set("set_motion_blur_intensity", &UMCamera::set_motion_blur_intensity)
		//	.set("set_aspect_ratio_mode", &UMCamera::set_aspect_ratio_mode)
		//	.set("set_aspect_width", &UMCamera::set_aspect_width)
		//	.set("set_aspect_height", &UMCamera::set_aspect_height)
		//	.set("set_pixel_aspect_ration", &UMCamera::set_pixel_aspect_ration)
		//	.set("set_aperture_mode", &UMCamera::set_aperture_mode)
		//	.set("set_gate_fit", &UMCamera::set_gate_fit)
		//	.set("set_field_of_view", &UMCamera::set_field_of_view)
		//	.set("set_field_of_view_x", &UMCamera::set_field_of_view_x)
		//	.set("set_field_of_view_y", &UMCamera::set_field_of_view_y)
		//	.set("set_focal_length", &UMCamera::set_focal_length)
		//	.set("set_camera_format", &UMCamera::set_camera_format)
		//	.set("set_use_frame_color", &UMCamera::set_use_frame_color)
		//	.set("set_frame_color", &UMCamera::set_frame_color)
		//	.set("set_show_name", &UMCamera::set_show_name)
		//	.set("set_show_info_on_moving", &UMCamera::set_show_info_on_moving)
		//	.set("set_show_grid", &UMCamera::set_show_grid)
		//	.set("set_show_optical_center", &UMCamera::set_show_optical_center)
		//	.set("set_show_azimut", &UMCamera::set_show_azimut)
		//	.set("set_show_time_code", &UMCamera::set_show_time_code)
		//	.set("set_show_audio", &UMCamera::set_show_audio)
		//	.set("set_audio_color", &UMCamera::set_audio_color)
		//	.set("set_near_plane", &UMCamera::set_near_plane)
		//	.set("set_far_plane", &UMCamera::set_far_plane)
		//	.set("set_auto_compute_clip_planes", &UMCamera::set_auto_compute_clip_planes)
		//	.set("set_film_width", &UMCamera::set_film_width)
		//	.set("set_film_height", &UMCamera::set_film_height)
		//	.set("set_film_aspect_ration", &UMCamera::set_film_aspect_ration)
		//	.set("set_film_squeeze_ration", &UMCamera::set_film_squeeze_ration)
		//	.set("set_film_format", &UMCamera::set_film_format)
		//	.set("set_film_offset_x", &UMCamera::set_film_offset_x)
		//	.set("set_film_offset_y", &UMCamera::set_film_offset_y)
		//	.set("set_pre_scale", &UMCamera::set_pre_scale)
		//	.set("set_film_translate_x", &UMCamera::set_film_translate_x)
		//	.set("set_film_translate_y", &UMCamera::set_film_translate_y)
		//	.set("set_film_roll_pivot_x", &UMCamera::set_film_roll_pivot_x)
		//	.set("set_film_roll_pivot_y", &UMCamera::set_film_roll_pivot_y)
		//	.set("set_film_roll_value", &UMCamera::set_film_roll_value)
		//	.set("set_film_roll_order", &UMCamera::set_film_roll_order)
		//	.set("set_view_camera_to_look_at", &UMCamera::set_view_camera_to_look_at)
		//	.set("set_view_frustum_near_far_plane", &UMCamera::set_view_frustum_near_far_plane)
		//	.set("set_view_frustum_back_plane_mode", &UMCamera::set_view_frustum_back_plane_mode)
		//	.set("set_back_plane_distance", &UMCamera::set_back_plane_distance)
		//	.set("set_back_plane_distance_mode", &UMCamera::set_back_plane_distance_mode)
		//	.set("set_view_frustum_front_plane_mode", &UMCamera::set_view_frustum_front_plane_mode)
		//	.set("set_front_plane_distance", &UMCamera::set_front_plane_distance)
		//	.set("set_front_plane_distance_mode", &UMCamera::set_front_plane_distance_mode)
		//	.set("set_lock_mode", &UMCamera::set_lock_mode)
		//	.set("set_lock_interest_navigation", &UMCamera::set_lock_interest_navigation)
		//	.set("set_back_plate_fit_image", &UMCamera::set_back_plate_fit_image)
		//	.set("set_back_plate_crop", &UMCamera::set_back_plate_crop)
		//	.set("set_back_plate_center", &UMCamera::set_back_plate_center)
		//	.set("set_back_plate_keep_ration", &UMCamera::set_back_plate_keep_ration)
		//	.set("set_background_alpha_treshold", &UMCamera::set_background_alpha_treshold)
		//	.set("set_back_plane_offset_x", &UMCamera::set_back_plane_offset_x)
		//	.set("set_back_plane_offset_y", &UMCamera::set_back_plane_offset_y)
		//	.set("set_back_plane_rotation", &UMCamera::set_back_plane_rotation)
		//	.set("set_back_plane_scale_x", &UMCamera::set_back_plane_scale_x)
		//	.set("set_back_plane_scale_y", &UMCamera::set_back_plane_scale_y)
		//	.set("set_show_backplate", &UMCamera::set_show_backplate)
		//	.set("set_front_plate_fit_image", &UMCamera::set_front_plate_fit_image)
		//	.set("set_front_plate_crop", &UMCamera::set_front_plate_crop)
		//	.set("set_front_plate_center", &UMCamera::set_front_plate_center)
		//	.set("set_front_plate_keep_ratio", &UMCamera::set_front_plate_keep_ratio)
		//	.set("set_show_frontplate", &UMCamera::set_show_frontplate)
		//	.set("set_front_plane_offset_x", &UMCamera::set_front_plane_offset_x)
		//	.set("set_front_plane_offset_y", &UMCamera::set_front_plane_offset_y)
		//	.set("set_front_plane_rotation", &UMCamera::set_front_plane_rotation)
		//	.set("set_front_plane_scale_x", &UMCamera::set_front_plane_scale_x)
		//	.set("set_front_plane_scale_y", &UMCamera::set_front_plane_scale_y)
		//	.set("set_foreground_opacity", &UMCamera::set_foreground_opacity)
		//	.set("set_display_safe_area", &UMCamera::set_display_safe_area)
		//	.set("set_display_safe_area_on_render", &UMCamera::set_display_safe_area_on_render)
		//	.set("set_safe_area_display_style", &UMCamera::set_safe_area_display_style)
		//	.set("set_safe_area_display_ratio", &UMCamera::set_safe_area_display_ratio)
		//	.set("set_use_2d_magnifier_zoom", &UMCamera::set_use_2d_magnifier_zoom)
		//	.set("set__2d_magnifier_zoom", &UMCamera::set__2d_magnifier_zoom)
		//	.set("set__2d_magnifier_x", &UMCamera::set__2d_magnifier_x)
		//	.set("set__2d_magnifier_y", &UMCamera::set__2d_magnifier_y)
		//	.set("set_projection_type", &UMCamera::set_projection_type)
		//	.set("set_ortho_zoom", &UMCamera::set_ortho_zoom)
		//	.set("set_use_real_time_dof_and_aa", &UMCamera::set_use_real_time_dof_and_aa)
		//	.set("set_use_depth_of_field", &UMCamera::set_use_depth_of_field)
		//	.set("set_focus_source", &UMCamera::set_focus_source)
		//	.set("set_focus_angle", &UMCamera::set_focus_angle)
		//	.set("set_focus_distance", &UMCamera::set_focus_distance)
		//	.set("set_use_antialiasing", &UMCamera::set_use_antialiasing)
		//	.set("set_antialiasing_intensity", &UMCamera::set_antialiasing_intensity)
		//	.set("set_antialiasing_method", &UMCamera::set_antialiasing_method)
		//	.set("set_use_accumulation_buffer", &UMCamera::set_use_accumulation_buffer)
		//	.set("set_frame_sample_count", &UMCamera::set_frame_sample_count)
		//	.set("set_frame_sampling_type", &UMCamera::set_frame_sampling_type)
		//	;


		//v8pp::class_<umio::UMCameraTypes> UMCameraTypes_class(isolate);
		//UMCameraTypes_class
		//	;

		//v8pp::class_<UMCameraTypes::EProjectionType> EProjectionType_class(isolate);
		//EProjectionType_class
		//	.set_const("Perspective", UMCameraTypes::ePerspective)
		//	.set_const("Orthogonal", UMCameraTypes::eOrthogonal)
		//	;

		//v8pp::class_<UMCameraTypes::EFormat> EFormat_class(isolate);
		//EFormat_class
		//	.set_const("CustomFormat", UMCameraTypes::eCustomFormat)
		//	.set_const("D1NTSC", UMCameraTypes::eD1NTSC)
		//	.set_const("NTSC", UMCameraTypes::eNTSC)
		//	.set_const("PAL", UMCameraTypes::ePAL)
		//	.set_const("D1PAL", UMCameraTypes::eD1PAL)
		//	.set_const("HD", UMCameraTypes::eHD)
		//	.set_const("640x480", UMCameraTypes::e640x480)
		//	.set_const("320x200", UMCameraTypes::e320x200)
		//	.set_const("320x240", UMCameraTypes::e320x240)
		//	.set_const("128x128", UMCameraTypes::e128x128)
		//	.set_const("Fullscreen", UMCameraTypes::eFullscreen)
		//	;

		//v8pp::class_<UMCameraTypes::EAspectRatioMode> EAspectRatioMode_class(isolate);
		//EAspectRatioMode_class
		//	.set_const("WindowSize", UMCameraTypes::eWindowSize)
		//	.set_const("FixedRatio", UMCameraTypes::eFixedRatio)
		//	.set_const("FixedResolution", UMCameraTypes::eFixedResolution)
		//	.set_const("FixedWidth", UMCameraTypes::eFixedWidth)
		//	.set_const("FixedHeight", UMCameraTypes::eFixedHeight)
		//	;

		//v8pp::class_<UMCameraTypes::EApertureFormat> EApertureFormat_class(isolate);
		//EApertureFormat_class
		//	.set_const("CustomAperture", UMCameraTypes::eCustomAperture)
		//	.set_const("16mmTheatrical", UMCameraTypes::e16mmTheatrical)
		//	.set_const("Super16mm", UMCameraTypes::eSuper16mm)
		//	.set_const("35mmAcademy", UMCameraTypes::e35mmAcademy)
		//	.set_const("35mmTVProjection", UMCameraTypes::e35mmTVProjection)
		//	.set_const("35mmFullAperture", UMCameraTypes::e35mmFullAperture)
		//	.set_const("35mm185Projection", UMCameraTypes::e35mm185Projection)
		//	.set_const("35mmAnamorphic", UMCameraTypes::e35mmAnamorphic)
		//	.set_const("70mmProjection", UMCameraTypes::e70mmProjection)
		//	.set_const("VistaVision", UMCameraTypes::eVistaVision)
		//	.set_const("DynaVision", UMCameraTypes::eDynaVision)
		//	.set_const("IMAX", UMCameraTypes::eIMAX)
		//	;

		//v8pp::class_<UMCameraTypes::EApertureMode> EApertureMode_class(isolate);
		//EApertureMode_class
		//	.set_const("HorizAndVert", UMCameraTypes::eHorizAndVert)
		//	.set_const("Horizontal", UMCameraTypes::eHorizontal)
		//	.set_const("Vertical", UMCameraTypes::eVertical)
		//	.set_const("FocalLength", UMCameraTypes::eFocalLength)
		//	;

		//v8pp::class_<UMCameraTypes::EGateFit> EGateFit_class(isolate);
		//EGateFit_class
		//	.set_const("FitNone", UMCameraTypes::eFitNone)
		//	.set_const("FitVertical", UMCameraTypes::eFitVertical)
		//	.set_const("FitHorizontal", UMCameraTypes::eFitHorizontal)
		//	.set_const("FitFill", UMCameraTypes::eFitFill)
		//	.set_const("FitOverscan", UMCameraTypes::eFitOverscan)
		//	.set_const("FitStretch", UMCameraTypes::eFitStretch)
		//	;

		//v8pp::class_<UMCameraTypes::EFilmRollOrder> EFilmRollOrder_class(isolate);
		//EFilmRollOrder_class
		//	.set_const("RotateFirst", UMCameraTypes::eRotateFirst)
		//	.set_const("TranslateFirst", UMCameraTypes::eTranslateFirst)
		//	;

		//v8pp::class_<UMCameraTypes::EPlateDrawingMode> EPlateDrawingMode_class(isolate);
		//EPlateDrawingMode_class
		//	.set_const("PlateBackground", UMCameraTypes::ePlateBackground)
		//	.set_const("PlateForeground", UMCameraTypes::ePlateForeground)
		//	.set_const("PlateBackAndFront", UMCameraTypes::ePlateBackAndFront)
		//	;

		//v8pp::class_<UMCameraTypes::EFrontBackPlaneDistanceMode> EFrontBackPlaneDistanceMode_class(isolate);
		//EFrontBackPlaneDistanceMode_class
		//	.set_const("RelativeToInterest", UMCameraTypes::eRelativeToInterest)
		//	.set_const("RelativeToCamera", UMCameraTypes::eRelativeToCamera)
		//	;

		//v8pp::class_<UMCameraTypes::EFrontBackPlaneDisplayMode> EFrontBackPlaneDisplayMode_class(isolate);
		//EFrontBackPlaneDisplayMode_class
		//	.set_const("PlanesDisabled", UMCameraTypes::ePlanesDisabled)
		//	.set_const("PlanesAlways", UMCameraTypes::ePlanesAlways)
		//	.set_const("PlanesWhenMedia", UMCameraTypes::ePlanesWhenMedia)
		//	;

		//v8pp::class_<UMCameraTypes::ESafeAreaStyle> ESafeAreaStyle_class(isolate);
		//ESafeAreaStyle_class
		//	.set_const("SafeAreaRound", UMCameraTypes::eSafeAreaRound)
		//	.set_const("SafeAreaSquare", UMCameraTypes::eSafeAreaSquare)
		//	;

		//v8pp::class_<UMCameraTypes::ERenderOptionsUsageTime> ERenderOptionsUsageTime_class(isolate);
		//ERenderOptionsUsageTime_class
		//	.set_const("Interactive", UMCameraTypes::eInteractive)
		//	.set_const("OnDemand", UMCameraTypes::eOnDemand)
		//	;

		//v8pp::class_<UMCameraTypes::EAntialiasingMethod> EAntialiasingMethod_class(isolate);
		//EAntialiasingMethod_class
		//	.set_const("AAOversampling", UMCameraTypes::eAAOversampling)
		//	.set_const("AAHardware", UMCameraTypes::eAAHardware)
		//	;

		//v8pp::class_<UMCameraTypes::ESamplingType> ESamplingType_class(isolate);
		//ESamplingType_class
		//	.set_const("SamplingUniform", UMCameraTypes::eSamplingUniform)
		//	.set_const("SamplingStochastic", UMCameraTypes::eSamplingStochastic)
		//	;

		//v8pp::class_<UMCameraTypes::EFocusDistanceSource> EFocusDistanceSource_class(isolate);
		//EFocusDistanceSource_class
		//	.set_const("FocusSrcCameraInterest", UMCameraTypes::eFocusSrcCameraInterest)
		//	.set_const("FocusSpecificDistance", UMCameraTypes::eFocusSpecificDistance)
		//	;

		//
		// UMSkeleton
		//
		v8pp::class_<umio::WrapUMSkeleton> WrapUMSkeleton_class(isolate);
		WrapUMSkeleton_class
			.ctor<>() //constructor
			.inherit<umio::WrapUMNode>()
			// getter
			.set("type", &UMSkeleton::type)
			.set("limb_length", &UMSkeleton::limb_length)
			.set("size", &UMSkeleton::size)
			// setter
			.set("set_type", &UMSkeleton::set_type)
			.set("set_limb_length", &UMSkeleton::set_limb_length)
			.set("set_size", &UMSkeleton::set_size)
			;

		//v8pp::class_<UMSkeleton::EType> EType_class(isolate);
		//EType_class
		//	.ctor<>() //constructor
		//	// getter
		//	.set_const("Root", UMSkeleton::eRoot)
		//	.set_const("Limb", UMSkeleton::eLimb)
		//	.set_const("LimbNode", UMSkeleton::eLimbNode)
		//	.set_const("Effector", UMSkeleton::eEffector)
		//	;

		// UMTexture
		v8pp::class_<umio::WrapUMTexture> WrapUMTexture_class(isolate);
		WrapUMTexture_class
			.ctor<>() //constructor
			// getter
			.set("name", &WrapUMTexture::name)
			.set("file_name", &WrapUMTexture::file_name)
			.set("relative_file_name", &WrapUMTexture::relative_file_name)
			.set("blend_mode", &WrapUMTexture::blend_mode)
			.set("alpha", &WrapUMTexture::alpha)
			// setter
			.set("set_name", &WrapUMTexture::set_name)
			.set("set_file_name", &WrapUMTexture::set_file_name)
			.set("set_relative_file_name", &WrapUMTexture::set_relative_file_name)
			.set("set_blend_mode", &WrapUMTexture::set_blend_mode)
			.set("set_alpha", &WrapUMTexture::set_alpha)
			;

		//v8pp::class_<UMTexture::EBlendMode> EBlendMode_class(isolate);
		//EBlendMode_class
		//	.ctor<>() //constructor
		//	.set_const("Translucent", UMTexture::eTranslucent)
		//	.set_const("Additive", UMTexture::eAdditive)
		//	.set_const("Modulate", UMTexture::eModulate)
		//	.set_const("Modulate2", UMTexture::eModulate2)
		//	.set_const("Over", UMTexture::eOver)
		//	.set_const("Normal", UMTexture::eNormal)
		//	.set_const("Dissolve", UMTexture::eDissolve)
		//	.set_const("Darken", UMTexture::eDarken)
		//	.set_const("ColorBurn", UMTexture::eColorBurn)
		//	.set_const("LinearBurn", UMTexture::eLinearBurn)
		//	.set_const("DarkerColor", UMTexture::eDarkerColor)
		//	.set_const("Lighten", UMTexture::eLighten)
		//	.set_const("Screen", UMTexture::eScreen)
		//	.set_const("ColorDodge", UMTexture::eColorDodge)
		//	.set_const("LinearDodge", UMTexture::eLinearDodge)
		//	.set_const("LighterColor", UMTexture::eLighterColor)
		//	.set_const("SoftLight", UMTexture::eSoftLight)
		//	.set_const("HardLight", UMTexture::eHardLight)
		//	.set_const("VividLight", UMTexture::eVividLight)
		//	.set_const("LinearLight", UMTexture::eLinearLight)
		//	.set_const("PinLight", UMTexture::ePinLight)
		//	.set_const("HardMix", UMTexture::eHardMix)
		//	.set_const("Difference", UMTexture::eDifference)
		//	.set_const("Exclusion", UMTexture::eExclusion)
		//	.set_const("Subtract", UMTexture::eSubtract)
		//	.set_const("Divide", UMTexture::eDivide)
		//	.set_const("Hue", UMTexture::eHue)
		//	.set_const("Saturation", UMTexture::eSaturation)
		//	.set_const("Color", UMTexture::eColor)
		//	.set_const("Luminosity", UMTexture::eLuminosity)
		//	.set_const("Overlay", UMTexture::eOverlay)
		//	.set_const("BlendModeCount", UMTexture::eBlendModeCount)
		//	;

		//
		// UMMaterial
		//
		v8pp::class_<umio::WrapUMMaterial> WrapUMMaterial_class(isolate);
		WrapUMMaterial_class
			.ctor<>() //constructor
			// getter
			.set("name", &WrapUMMaterial::name)
			.set("ambient", &WrapUMMaterial::ambient)
			.set("ambient_factor", &WrapUMMaterial::ambient_factor)
			.set("diffuse", &WrapUMMaterial::diffuse)
			.set("diffuse_factor", &WrapUMMaterial::diffuse_factor)
			.set("specular", &WrapUMMaterial::specular)
			.set("specular_factor", &WrapUMMaterial::specular_factor)
			.set("emissive", &WrapUMMaterial::emissive)
			.set("emissive_factor", &WrapUMMaterial::emissive_factor)
			.set("transparent", &WrapUMMaterial::transparent)
			.set("transparency_factor", &WrapUMMaterial::transparency_factor)
			.set("refrection", &WrapUMMaterial::refrection)
			.set("reflection_factor", &WrapUMMaterial::reflection_factor)
			.set("bump", &WrapUMMaterial::bump)
			.set("bump_factor", &WrapUMMaterial::bump_factor)
			.set("vector_displacement", &WrapUMMaterial::vector_displacement)
			.set("vector_displacement_factor", &WrapUMMaterial::vector_displacement_factor)
			.set("shininess", &WrapUMMaterial::shininess)
			.set("texture_list", &WrapUMMaterial::texture_list)
			//.set("layered_texture_list", &WrapUMMaterial::layered_texture_list)
			//// setter
			//.set("set_name", &UMMaterial::set_name)
			//.set("set_ambient", &UMMaterial::set_ambient)
			//.set("set_ambient_factor", &UMMaterial::set_ambient_factor)
			//.set("set_diffuse", &UMMaterial::set_diffuse)
			//.set("set_diffuse_factor", &UMMaterial::set_diffuse_factor)
			//.set("set_specular", &UMMaterial::set_specular)
			//.set("set_specular_factor", &UMMaterial::set_specular_factor)
			//.set("set_emissive", &UMMaterial::set_emissive)
			//.set("set_emissive_factor", &UMMaterial::set_emissive_factor)
			//.set("set_transparency", &UMMaterial::set_transparent)
			//.set("set_transparency_factor", &UMMaterial::set_transparency_factor)
			//.set("set_refrection", &UMMaterial::set_refrection)
			//.set("set_reflection_factor", &UMMaterial::set_reflection_factor)
			//.set("set_bump", &UMMaterial::set_bump)
			//.set("set_bump_factor", &UMMaterial::set_bump_factor)
			//.set("set_vector_displacement", &UMMaterial::set_vector_displacement)
			//.set("set_vector_displacement_factor", &UMMaterial::set_vector_displacement_factor)
			//.set("set_shininess", &UMMaterial::set_shininess)
			//.set("add_texture", &UMMaterial::add_texture)
			//.set("add_layered_texture", &UMMaterial::add_layered_texture)
			;

		// UMCluster
		v8pp::class_<umio::WrapUMCluster> WrapUMCluster_class(isolate);
		WrapUMCluster_class
			.ctor<>() //constructor
			// getter
			.set("name", &WrapUMCluster::name)
			.set("index_list", &WrapUMCluster::index_list)
			.set("weight_list", &WrapUMCluster::weight_list)
			.set("link_mode", &WrapUMCluster::link_mode)
			//.set("link_node", &WrapUMCluster::link_node, return_value_policy::reference)
			.set("link_node_id", &WrapUMCluster::link_node_id)
			// setter
			.set("set_name", &WrapUMCluster::set_name)
			.set("set_link_mode", &WrapUMCluster::set_link_mode)
			.set("set_link_node_id", &WrapUMCluster::set_link_node_id)
			.set("add_weight", &WrapUMCluster::add_weight)
			.set("add_index", &WrapUMCluster::add_index)
			;

		////v8pp::class_<UMCluster::ELinkMode> ELinkMode_class(isolate);
		////ELinkMode_class
		////	.set_const("Normalize", UMCluster::eNormalize)
		////	.set_const("Additive", UMCluster::eAdditive)
		////	.set_const("TotalOne", UMCluster::eTotalOne)
		////	;

		// UMDeformer
		v8pp::class_<umio::WrapUMDeformer> WrapUMDeformer_class(isolate);
		WrapUMDeformer_class
			.ctor<>() //constructor
			// getter
			//.set("geometry_node", &UMDeformer::geometry_node, return_value_policy::reference)
			.set("geometry_node_id", &WrapUMDeformer::geometry_node_id)
			.set("deformer_type", &WrapUMDeformer::deformer_type)
			// setter
			.set("set_geometry_node_id", &WrapUMDeformer::set_geometry_node_id)
			;

		////v8pp::class_<UMDeformer::EDeformerType> EDeformerType_class(isolate);
		////EDeformerType_class
		////	.set_const("Unknown", UMDeformer::eUnknown)
		////	.set_const("Skin", UMDeformer::eSkin)
		////	.set_const("BlendShape", UMDeformer::eBlendShape)
		////	.set_const("VertexCache", UMDeformer::eVertexCache)
		////	;

		// UMShape
		v8pp::class_<umio::WrapUMShape> WrapUMShape_class(isolate);
		WrapUMShape_class
			.ctor<>() //constructor
			// getter
			.set("name", &WrapUMShape::name)
			//.set("base_geometry_node", &WrapUMShape::base_geometry_node, return_value_policy::reference)
			.set("base_geometry_node_id", &WrapUMShape::base_geometry_node_id)
			.set("vertex_index_list", &WrapUMShape::vertex_index_list)
			.set("normal_index_list", &WrapUMShape::normal_index_list)
			.set("vertex_list", &WrapUMShape::vertex_list)
			.set("normal_list", &WrapUMShape::normal_list)
			// setter
			//.set("set_name", &WrapUMShape::set_name)
			//.set("add_vertex_index", &WrapUMShape::add_vertex_index)
			//.set("add_normal_index", &WrapUMShape::add_normal_index)
			//.set("add_vertex", &WrapUMShape::add_vertex)
			//.set("add_normal", &WrapUMShape::add_normal)
			//.set("set_base_geometry_node_id", &WrapUMShape::set_base_geometry_node_id)
			;

		// UMBlendShapeChannel
		v8pp::class_<umio::WrapUMBlendShapeChannel> WrapUMBlendShapeChannel_class(isolate);
		WrapUMBlendShapeChannel_class
			.ctor<>() //constructor
			//// getter
			//.set("target_shape_list", &UMBlendShapeChannel::target_shape_list)
			//.set("full_weight_list", &UMBlendShapeChannel::full_weight_list)
			//.set("deform_percent", &UMBlendShapeChannel::deform_percent)
			//// setter
			//.set("add_target_shape", &UMBlendShapeChannel::add_target_shape)
			//.set("add_full_weight", &UMBlendShapeChannel::add_full_weight)
			//.set("set_deform_percent", &UMBlendShapeChannel::set_deform_percent)
			;

		// UMBlendShape
		v8pp::class_<umio::WrapUMBlendShape> WrapUMBlendShape_class(isolate);
		WrapUMBlendShape_class
			.ctor<>() //constructor
			.inherit<umio::WrapUMDeformer>()
			// getter
			.set("name", &WrapUMBlendShape::name)
			.set("blend_shape_channel_list", &WrapUMBlendShape::blend_shape_channel_list)
			// setter
			.set("set_name", &WrapUMBlendShape::set_name)
			//.set("add_blend_shape_channel", &WrapUMBlendShape::add_blend_shape_channel)
			;

		// UMSkin
		v8pp::class_<umio::WrapUMSkin> WrapUMSkin_class(isolate);
		WrapUMSkin_class
			.ctor<>() //constructor
			.inherit<umio::WrapUMDeformer>()
			.set("name", &WrapUMSkin::name)
			.set("cluster_list", &WrapUMSkin::cluster_list)
			// setter
			.set("add_cluster", &WrapUMSkin::add_cluster)
			.set("set_name", &WrapUMSkin::set_name)
			;

		//
		// UMMesh
		//
		v8pp::class_<umio::WrapUMMesh> WrapUMMesh_class(isolate);
		WrapUMMesh_class
			.ctor<>() //constructor
			.inherit<umio::WrapUMNode>()
			// getter
			.set("vertex_index_list", &WrapUMMesh::vertex_index_list)
			.set("vertex_list", &WrapUMMesh::vertex_list)
			.set("normal_list", &WrapUMMesh::normal_list)
			.set("vertex_color_list", &WrapUMMesh::vertex_color_list)
			.set("uv_list", &WrapUMMesh::uv_list)
			.set("material_index_list", &WrapUMMesh::material_index_list)
			.set("material_list", &WrapUMMesh::material_list)
			.set("skin_list", &WrapUMMesh::skin_list)
			.set("blend_shape_list", &WrapUMMesh::blend_shape_list)
			.set("layered_uv_list", &WrapUMMesh::layered_uv_list)
			.set("layered_normal_list", &WrapUMMesh::layered_normal_list)
			.set("layered_vetex_color_list", &WrapUMMesh::layered_vertex_color_list)
			//// setter
			//.set("add_vertex_index", &WrapUMMesh::add_vertex_index)
			//.set("add_vertex", &WrapUMMesh::add_vertex)
			//.set("add_normal", &WrapUMMesh::add_normal)
			//.set("add_vertex_color", &WrapUMMesh::add_vertex_color)
			//.set("add_uv", &WrapUMMesh::add_uv)
			//.set("add_material_index", &WrapUMMesh::add_material_index)
			//.set("add_material", &WrapUMMesh::add_material)
			//.set("add_skin", &WrapUMMesh::add_skin)
			//.set("add_blend_shape", &WrapUMMesh::add_blend_shape)
			//.set("add_uv_layer", &WrapUMMesh::add_uv_layer)
			//.set("add_normal_layer", &WrapUMMesh::add_normal_layer)
			//.set("add_vertex_color_layer", &WrapUMMesh::add_vertex_color_layer)
			;

		//
		// UMObject
		//
		v8pp::class_<umio::WrapUMObject> WrapUMObject_class(isolate);
		WrapUMObject_class
			.ctor<>() //constructor
			// getter
			.set("mesh_map", &WrapUMObject::mesh_map)
			//.set("skeleton_map", &WrapUMObject::skeleton_map)
			//.set("camera_map", &WrapUMObject::camera_map)
			//.set("other_node_map", &WrapUMObject::other_node_map)
			//.set("next_id", &WrapUMObject::next_id)
			//// setter
			//.set("add_mesh", &WrapUMObject::add_mesh)
			//.set("add_skeleton", &WrapUMObject::add_skeleton)
			//.set("add_camera", &WrapUMObject::add_camera)
			//.set("add_other_node", &WrapUMObject::add_other_node)
			;

		//
		// UMIO
		//
		v8pp::class_<WrapUMIO> WrapUMIO_class(isolate);
		WrapUMIO_class
			.ctor<>() //constructor
			.set("load", &WrapUMIO::load)
			//.set("save", &WrapUMIO::save)
			//.set("load_from_memory", &WrapUMIO::load_from_memory)
			////.set("save_to_memory", &WrapUMIO::save_to_memory)
			//.set("load_setting", &WrapUMIO::load_setting)
			//.set("save_setting", &WrapUMIO::save_setting)
			//.set("create_object", &WrapUMIO::create_object)
			;


		v8pp::module umio(isolate);
		// set bindings... 
		umio
			.set("UMIOSetting", UMIOSetting_class)
			////.set("EExpSettingType", EExpSettingType_class)
			////.set("EImpSettingType", EImpSettingType_class)
			////.set("EUMExpSettingType", EUMExpSettingType_class)
			////.set("EUMImpSettingType", EUMImpSettingType_class)
			////.set("EFbxAxisType", EFbxAxisType_class)
			////.set("EFbxSystemUnitType", EFbxSystemUnitType_class)
			//.set("UMCamera", UMCamera_class)
			//.set("UMCameraTypes", UMCameraTypes_class)
			////.set("EProjectionType", EProjectionType_class)
			////.set("EFormat", EFormat_class)
			////.set("EAspectRatioMode", EAspectRatioMode_class)
			////.set("EApertureFormat", EApertureFormat_class)
			////.set("EApertureMode", EApertureMode_class)
			////.set("EGateFit", EGateFit_class)
			////.set("EFilmRollOrder", EFilmRollOrder_class)
			////.set("EPlateDrawingMode", EPlateDrawingMode_class)
			////.set("EFrontBackPlaneDistanceMode", EFrontBackPlaneDistanceMode_class)
			////.set("EFrontBackPlaneDisplayMode", EFrontBackPlaneDisplayMode_class)
			////.set("ESafeAreaStyle", ESafeAreaStyle_class)
			////.set("ERenderOptionsUsageTime", ERenderOptionsUsageTime_class)
			////.set("EAntialiasingMethod", EAntialiasingMethod_class)
			////.set("ESamplingType", ESamplingType_class)
			////.set("EFocusDistanceSource", EFocusDistanceSource_class)
			.set("UMVec4d", UMVec4d_class)
			.set("UMMat44d", UMMat44d_class)
			.set("UMNode", WrapUMNode_class)
			.set("UMSkeleton", WrapUMSkeleton_class)
			////.set("EType", EType_class)
			.set("UMTexture", WrapUMTexture_class)
			////.set("EBlendMode", EBlendMode_class)
			.set("UMMaterial", WrapUMMaterial_class)
			.set("UMCluster", WrapUMCluster_class)
			////.set("ELinkMode", ELinkMode_class)
			.set("UMDeformer", WrapUMDeformer_class)
			////.set("EDeformerType", EDeformerType_class)
			.set("UMShape", WrapUMShape_class)
			.set("UMBlendShapeChannel", WrapUMBlendShapeChannel_class)
			.set("UMBlendShape", WrapUMBlendShape_class)
			.set("UMSkin", WrapUMSkin_class)
			.set("UMMesh", WrapUMMesh_class)
			.set("UMObject", WrapUMObject_class)
			.set("UMIO", WrapUMIO_class)
			;

		// set bindings as exports object prototype
		exports->SetPrototype(umio.new_instance());
	}

	NODE_MODULE(umio, umio_module)

} // namespace umio

#endif // WITH_V8
