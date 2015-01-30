/**
 *
 * UMFbx.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_FBX2015

/**
 * if you build with FBXSDK2015, you will need to write below to your about box.
 *
 * This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2014 Autodesk, Inc. All rights, reserved.
 * Such code is provided “as is” and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including 
 * without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights.
 * In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, 
 * or consequential damages (including, but not limited to, procurement of substitute goods or services; 
 * loss of use, data, or profits; or business interruption) however caused and on any theory of liability, 
 * whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of such code.
 *
 */

#include <fbxsdk.h>
#include <vector>
#include <algorithm>
#include "UMFbx.h"
#include "UMObject.h"
#include "UMAnimation.h"

#ifdef WITH_PYTHON
	#include <boost/python.hpp>
	//#define printf(fmt) boost::python::exec("print('" #fmt "')\n")
	//#define printfm(fmt, ...) boost::python::exec("print('" #fmt "'," #__VA_ARGS__ ")\n")
	#define printf(fmt)
	#define printfm(fmt, ...)
#else	
	#define printfm(fmt, ...)
#endif//WITH_PYTHON

namespace umio
{
	
/**
 * UMFbx Load implementation.
 */
class UMFbxLoadImpl
{
public:

	/**
	 * constructor
	 */
	UMFbxLoadImpl() : 
		manager_(NULL), 
		importer_(NULL), 
		current_scene_(NULL), 
		is_triangulate_(false),
		is_load_patch_(false), 
		is_load_nurbs_(false) {}
	
	/**
	 * destructor
	 */
	~UMFbxLoadImpl() {
		if (importer_)
		{
			importer_->Destroy();
			importer_ = NULL;
		}
		if (manager_)
		{
			manager_->Destroy();
			manager_ = NULL;
		}
	}

	/**
	 * load fbx file to UMObject
	 */
	UMObjectPtr load(std::string path, const UMIOSetting& setting);
	
	/**
	 * load setting
	 */
	void set_load_setting(FbxIOSettings& fbx_settings, const UMIOSetting& setting);

	/**
	 * set axis type to scene
	 */
	void set_axis_type(FbxScene* fbx_scene, const UMIOSetting& setting);

	/**
	 * set system unit type to scene
	 */
	void set_system_unit_type(FbxScene* fbx_scene, const UMIOSetting& setting);

	/**
	 * UM original load options
	 */
	void set_triangulate(bool triangulate) { is_triangulate_ = triangulate; }
	bool is_triangulate() { return is_triangulate_; }
	
	void set_load_nurbs(bool val) { is_load_nurbs_ = val; }
	bool is_load_nurbs() { return is_load_nurbs_; }

	void set_load_patch(bool val) { is_load_patch_ = val; }
	bool is_load_patch() { return is_load_patch_; }
	
	/**
	 * create object from fbx's manager, scene, importer
	 */
	UMObjectPtr create_object(FbxScene* scene);

	/**
	 * create animation from fbx's manager, scene, importer, and UMObject
	 */
	UMAnimationPtr create_animation(FbxScene* scene, UMObjectPtr object);
	
	/**
	 * assign all animation stack to UMAnimation.
	 */
	bool assign_all_animation_stacks(UMAnimationPtr animation);

	/**
	 * assign all node to UMObject.
	 */
	bool assign_all_nodes(UMObjectPtr object, FbxNode* node);
	
	/**
	 * assign all poses in scene to UMObject.
	 */
	bool assing_all_poses(UMObjectPtr object);

	/**
	 * assign imported mesh to UMObject.
	 */
	bool assign_mesh(UMObjectPtr object, FbxNode* node);
	
	/**
	 * assign imported nurbs to UMObject.
	 */
	bool assign_nurbs(UMObjectPtr object, FbxNode* node);

	/**
	 * assign imported patch to UMObject.
	 */
	bool assign_patch(UMObjectPtr object, FbxNode* node);

	/**
	 * assign imported skins to UMObject.
	 * @param node mesh's node
	 */
	bool assign_skins(UMObjectPtr object, UMMesh& mesh, FbxNode* node);

	/**
	 * assign imported skins to UMObject.
	 * @param node mesh's node
	 */
	bool assign_blend_shapes(UMObjectPtr object, UMMesh& mesh, FbxNode* node);

	/**
	 * assign imported materials to UMObject.
	 * @param node mesh's node
	 */
	bool assign_materials(UMObjectPtr object, UMMesh& mesh, FbxNode* node);

	/**
	 * assign imported textures to UMMaterial.
	 */
	bool assign_textures(UMObjectPtr object, UMMaterial& mesh, FbxProperty& fbx_property);
	
	/**
	 * assign imported skeleton to UMObject.
	 */
	bool assing_skeleton(UMObjectPtr object, FbxNode* node);
	
	/**
	 * assign imported camera to UMObject.
	 */
	bool assign_camera(UMObjectPtr object, FbxNode* node);

	/**
	 * assign imported animation stacks to UMAnimationStack
	 */
	bool assign_animation_layers(UMAnimationPtr animation, UMAnimationStack& stack, FbxAnimStack* fbx_stack);

	/**
	 * assign imported animation curve stacks to UMAnimationLayer
	 */
	bool assign_animation_curve_stacks(UMAnimationPtr animation, UMAnimationLayer& layer, FbxAnimLayer* fbx_layer, FbxNode* node);
	
	/**
	 * assign imported animation curves to UMAnimationCurveStack
	 */
	bool assign_animation_curves(UMAnimationPtr animation, UMAnimationCurveStack& stack, FbxAnimLayer* fbx_layer, FbxNode* node);

	/**
	 * assign imported animation curve to UMAnimationCurve
	 */
	bool assign_animation_curve(UMAnimationPtr animation, UMAnimationCurve& curve, FbxAnimCurve* fbx_layer);


	/**
	 * convert FbxNode to UMNode.
	 */
	bool assign_one_node(UMObjectPtr object, UMNode* dst_node, FbxNode* node);
		
	// internal use
	const IntList& skeleton_id_list() const { return skeleton_id_list_; }
	IntList& mutable_skeleton_id_list() { return skeleton_id_list_; }
protected:
	
	// internal use
	IntList skeleton_id_list_;

	FbxManager* manager() { return manager_; }
	FbxImporter* importer() { return importer_; }
	FbxScene* current_scene() { return current_scene_; }

	FbxManager* manager_;
	FbxImporter* importer_;
	FbxScene* current_scene_;

	bool is_triangulate_;
	bool is_load_nurbs_;
	bool is_load_patch_;

	std::map<FbxLayer*, int> uv_layer_to_index_map_;
	std::map<FbxLayer*, int> normal_layer_to_index_map_;
	std::map<FbxLayer*, int> vertex_color_layer_to_index_map_;
	
	/**
	 * assing normals to UMMesh
	 */
	bool assign_normals(
		UMMesh& mesh, 
		FbxMesh* fbx_mesh, 
		FbxLayer* layer,
		int polygon_index,
		int point_index,
		int polygon_vertex_index,
		int control_point_index);
	
	/**
	 * assing uvs to UMMesh
	 */
	bool assign_uvs(
		UMMesh& mesh, 
		FbxMesh* fbx_mesh, 
		FbxLayer* layer,
		int polygon_index,
		int point_index,
		int polygon_vertex_index,
		int control_point_index);
	
	/**
	 * assing vertex colors to UMMesh
	 */
	bool assign_vertex_colors(
		UMMesh& mesh, 
		FbxMesh* fbx_mesh, 
		FbxLayer* layer,
		int polygon_index,
		int point_index,
		int polygon_vertex_index,
		int control_point_index);
};

	
/**
 * UMFbx Save implementation.
 */
class UMFbxSaveImpl
{
public:
	/**
	 * constructor
	 */
	UMFbxSaveImpl() : 
		manager_(NULL), 
		exporter_(NULL), 
		current_scene_(NULL),
		is_save_text_(false),
		is_old_fbx_(false)
		{}
	
	/**
	 * destructor
	 */
	~UMFbxSaveImpl() {
		if (exporter_)
		{
			exporter_->Destroy();
			exporter_ = NULL;
		}
		if (manager_)
		{
			manager_->Destroy();
			manager_ = NULL;
		}
	}
	
	/**
	 * save fbx file to UMObject
	 */
	bool save(std::string path, UMObjectPtr object, const UMIOSetting& setting);

	/**
	 * save setting
	 */
	void set_save_setting(FbxIOSettings& fbx_settings, const UMIOSetting& setting);
	
	/**
	 * set axis type to scene
	 */
	void set_axis_type(FbxScene* fbx_scene, const UMIOSetting& setting);
	
	/**
	 * set system unit type to scene
	 */
	void set_system_unit_type(FbxScene* fbx_scene, const UMIOSetting& setting);

	/**
	 * UM original load options
	 */
	void set_save_text(bool val) { is_save_text_ = val; }
	bool is_save_text() { return is_save_text_; }
	
	void set_old_fbx(bool val) { is_old_fbx_ = val; }
	bool is_old_fbx() { return is_old_fbx_; }

	/**
	 * create scene from UMObject
	 */
	FbxScene* create_scene(UMObjectPtr object);

	/**
	 * export skeleton to scene
	 */
	bool export_skeleton(FbxScene* scene, UMObjectPtr object);
	
	/**
	 * export mesh materials to node
	 */ 
	bool export_materials(FbxNode* fbx_node, const UMMesh& mesh);
	
	/**
	 * export bindpose to scene
	 */
	bool export_bind_pose(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh);

	/**
	 * export mesh skin to scene
	 */
	bool export_skins(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh);

	/**
	 * export blend shape to scene
	 */
	bool export_blend_shapes(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh);
	
	/**
	 * export blend shape to scene
	 */
	bool export_shape(FbxShape* fbx_shape, const UMShape& shape);

	/**
	 * export mesh to scene
	 */
	bool export_mesh(FbxScene* scene, UMObjectPtr object);
	
protected:
	FbxManager* manager() { return manager_; }
	FbxExporter* exporter() { return exporter_; }
	FbxScene* current_scene() { return current_scene_; }

	FbxManager* manager_;
	FbxExporter* exporter_;
	FbxScene* current_scene_;

	bool is_save_text_;
	bool is_old_fbx_;
};

/**
 * convert FbxAMatrix to DoubleListVec
 */
static void matrix_to_UMMat44d(UMMat44d& dst, const FbxAMatrix& src_matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			dst.m[i][k] = src_matrix.Get(i, k);
		}
	}
}

/**
 * convert FbxMatrix to DoubleListVec
 */
static void matrix_to_UMMat44d(UMMat44d& dst, const FbxMatrix& src_matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int k = 0; k < 4; ++k)
		{
			dst.m[i][k] = src_matrix.Get(i, k);
		}
	}
}

/*---------------------------------------------------------------------------*
 * UMFbxLoadImpl
 *---------------------------------------------------------------------------*/

/**
 * load setting
 */
void UMFbxLoadImpl::set_load_setting(FbxIOSettings& fbx_settings, const UMIOSetting& setting)
{
	UMIOSetting::ImpSettingMap::const_iterator it = setting.imp_prop_map().begin();
	for (; it != setting.imp_prop_map().end(); ++it)
	{
		UMIOSetting::EImpSettingType type = it->first;
		bool val = it->second;

		if (type == UMIOSetting::eImpDxfWeldVertices)
			fbx_settings.SetBoolProp(IMP_DXF_WELD_VERTICES, val);
		else if (type == UMIOSetting::eImpDxfObjectDerivation)
			fbx_settings.SetBoolProp(IMP_DXF_OBJECT_DERIVATION, val);
		else if (type == UMIOSetting::eImpDxfReferenceNode)
			fbx_settings.SetBoolProp(IMP_DXF_REFERENCE_NODE, val);
		else if (type == UMIOSetting::eImpObjReferenceNode)
			fbx_settings.SetBoolProp(IMP_OBJ_REFERENCE_NODE, val);
		else if (type == UMIOSetting::eImp3dsReferenceNode)
			fbx_settings.SetBoolProp(IMP_3DS_REFERENCENODE, val);
		else if (type == UMIOSetting::eImp3dsTexture)
			fbx_settings.SetBoolProp(IMP_3DS_TEXTURE, val);
		else if (type == UMIOSetting::eImp3dsMaterial)
			fbx_settings.SetBoolProp(IMP_3DS_MATERIAL, val);
		else if (type == UMIOSetting::eImp3dsAnimation)
			fbx_settings.SetBoolProp(IMP_3DS_ANIMATION, val);
		else if (type == UMIOSetting::eImp3dsMesh)
			fbx_settings.SetBoolProp(IMP_3DS_MESH, val);
		else if (type == UMIOSetting::eImp3dsLight)
			fbx_settings.SetBoolProp(IMP_3DS_LIGHT, val);
		else if (type == UMIOSetting::eImp3dsCamera)
			fbx_settings.SetBoolProp(IMP_3DS_CAMERA, val);
		else if (type == UMIOSetting::eImp3dsAmbientLight)
			fbx_settings.SetBoolProp(IMP_3DS_AMBIENT_LIGHT, val);
		else if (type == UMIOSetting::eImp3dsRescaling)
			fbx_settings.SetBoolProp(IMP_3DS_RESCALING, val);
		else if (type == UMIOSetting::eImp3dsFilter)
			fbx_settings.SetBoolProp(IMP_3DS_FILTER, val);
		else if (type == UMIOSetting::eImp3dsSmoothgroup)
			fbx_settings.SetBoolProp(IMP_3DS_SMOOTHGROUP, val);
		else if (type == UMIOSetting::eImpFbxModelCount)
			fbx_settings.SetBoolProp(IMP_FBX_MODEL_COUNT, val);
		else if (type == UMIOSetting::eImpFbxDeviceCount)
			fbx_settings.SetBoolProp(IMP_FBX_DEVICE_COUNT, val);
		else if (type == UMIOSetting::eImpFbxCharacterCount)
			fbx_settings.SetBoolProp(IMP_FBX_CHARACTER_COUNT, val);
		else if (type == UMIOSetting::eImpFbxActorCount)
			fbx_settings.SetBoolProp(IMP_FBX_ACTOR_COUNT, val);
		else if (type == UMIOSetting::eImpFbxConstraintCount)
			fbx_settings.SetBoolProp(IMP_FBX_CONSTRAINT_COUNT, val);
		else if (type == UMIOSetting::eImpFbxMediaCount)
			fbx_settings.SetBoolProp(IMP_FBX_MEDIA_COUNT, val);
		else if (type == UMIOSetting::eImpFbxTemplate)
			fbx_settings.SetBoolProp(IMP_FBX_TEMPLATE, val);
		else if (type == UMIOSetting::eImpFbxPivot)
			fbx_settings.SetBoolProp(IMP_FBX_PIVOT, val);
		else if (type == UMIOSetting::eImpFbxGlobalSettings)
			fbx_settings.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, val);
		else if (type == UMIOSetting::eImpFbxCharacter)
			fbx_settings.SetBoolProp(IMP_FBX_CHARACTER, val);
		else if (type == UMIOSetting::eImpFbxConstraint)
			fbx_settings.SetBoolProp(IMP_FBX_CONSTRAINT, val);
		else if (type == UMIOSetting::eImpFbxMergeLayerAndTimewarp)
			fbx_settings.SetBoolProp(IMP_FBX_MERGE_LAYER_AND_TIMEWARP, val);
		else if (type == UMIOSetting::eImpFbxGobo)
			fbx_settings.SetBoolProp(IMP_FBX_GOBO, val);
		else if (type == UMIOSetting::eImpFbxShape)
			fbx_settings.SetBoolProp(IMP_FBX_SHAPE, val);
		else if (type == UMIOSetting::eImpFbxLink)
			fbx_settings.SetBoolProp(IMP_FBX_LINK, val);
		else if (type == UMIOSetting::eImpFbxMaterial)
			fbx_settings.SetBoolProp(IMP_FBX_MATERIAL, val);
		else if (type == UMIOSetting::eImpFbxTexture)
			fbx_settings.SetBoolProp(IMP_FBX_TEXTURE, val);
		else if (type == UMIOSetting::eImpFbxModel)
			fbx_settings.SetBoolProp(IMP_FBX_MODEL, val);
		else if (type == UMIOSetting::eImpFbxAnimation)
			fbx_settings.SetBoolProp(IMP_FBX_ANIMATION, val);
		else if (type == UMIOSetting::eImpFbxPassword)
			fbx_settings.SetBoolProp(IMP_FBX_PASSWORD, val);
		else if (type == UMIOSetting::eImpFbxPasswordEnable)
			fbx_settings.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, val);
		else if (type == UMIOSetting::eImpFbxCurrentTakeName)
			fbx_settings.SetBoolProp(IMP_FBX_CURRENT_TAKE_NAME, val);
	}
	UMIOSetting::UMImpSettingMap::const_iterator bt = setting.bl_imp_prop_map().begin();
	for (; bt != setting.bl_imp_prop_map().end(); ++bt)
	{
		UMIOSetting::EUMImpSettingType type = bt->first;
		bool val = bt->second;

		if (type == UMIOSetting::eUMImpTriangulate)
			set_triangulate(val);
		else if (type == UMIOSetting::eUMImpNurbs)
			set_load_nurbs(val);
		else if (type == UMIOSetting::eUMImpPatch)
			set_load_patch(val);
	}
}

/**
 * set axis type
 */
void UMFbxLoadImpl::set_axis_type(FbxScene* fbx_scene, const UMIOSetting& setting)
{
	if (!fbx_scene) return;
	const UMIOSetting::EFbxAxisType type = setting.axis_type();

	FbxAxisSystem::EPreDefinedAxisSystem axis_system = FbxAxisSystem::eOpenGL;
	if (type == UMIOSetting::eFbxAxisMayaZUp) 
		axis_system = FbxAxisSystem::eMayaZUp;
	else if (type == UMIOSetting::eFbxAxisMayaYUp) 
		axis_system = FbxAxisSystem::eMayaYUp;
	else if (type == UMIOSetting::eFbxAxisMax) 
		axis_system = FbxAxisSystem::eMax;
	else if (type == UMIOSetting::eFbxAxisMotionBuilder) 
		axis_system = FbxAxisSystem::eMotionBuilder;
	else if (type == UMIOSetting::eFbxAxisOpenGL) 
		axis_system = FbxAxisSystem::eOpenGL;
	else if (type == UMIOSetting::eFbxAxisDirectX) 
		axis_system = FbxAxisSystem::eDirectX;
	else if (type == UMIOSetting::eFbxAxisLightWave) 
		axis_system = FbxAxisSystem::eLightwave;

	FbxAxisSystem fbx_axis_system(axis_system);
	fbx_axis_system.ConvertScene(fbx_scene);
}

/**
 * set system unit type to scene
 */
void UMFbxLoadImpl::set_system_unit_type(FbxScene* fbx_scene, const UMIOSetting& setting)
{
	if (!fbx_scene) return;
	const UMIOSetting::EFbxSystemUnitType unit_type = setting.system_unit_type();
	FbxSystemUnit fbx_system_unit;
	if (unit_type == UMIOSetting::eFbxSystemUnitMM)
		fbx_system_unit = FbxSystemUnit::mm;
	if (unit_type == UMIOSetting::eFbxSystemUnitDM)
		fbx_system_unit = FbxSystemUnit::dm;
	if (unit_type == UMIOSetting::eFbxSystemUnitCM)
		fbx_system_unit = FbxSystemUnit::cm;
	if (unit_type == UMIOSetting::eFbxSystemUnitM)
		fbx_system_unit = FbxSystemUnit::m;
	if (unit_type == UMIOSetting::eFbxSystemUnitKM)
		fbx_system_unit = FbxSystemUnit::km;
	if (unit_type == UMIOSetting::eFbxSystemUnitInch)
		fbx_system_unit = FbxSystemUnit::Inch;
	if (unit_type == UMIOSetting::eFbxSystemUnitFoot)
		fbx_system_unit = FbxSystemUnit::Foot;
	if (unit_type == UMIOSetting::eFbxSystemUnitMile)
		fbx_system_unit = FbxSystemUnit::Mile;
	if (unit_type == UMIOSetting::eFbxSystemUnitYard)
		fbx_system_unit = FbxSystemUnit::Yard;

	fbx_system_unit.ConvertScene(fbx_scene);
}

/**
 * assign imported textures to UMMaterial.
 */
bool UMFbxLoadImpl::assign_textures(UMObjectPtr object, UMMaterial& material, FbxProperty& fbx_property)
{
	const int layered_texture_count = fbx_property.GetSrcObjectCount<FbxLayeredTexture>();
	if (layered_texture_count > 0)
	{
		printf("has layered texture");

		for (int i = 0; i < layered_texture_count; ++i)
		{
			FbxLayeredTexture* fbx_layered_texture = fbx_property.GetSrcObject<FbxLayeredTexture>(i);
			if (!fbx_layered_texture) continue;

			UMMaterial::TextureList texture_list;

			const int texture_count = fbx_layered_texture->GetSrcObjectCount<FbxTexture>();
			for (int k = 0; k < texture_count; ++k)
			{
				FbxTexture* fbx_texture = fbx_layered_texture->GetSrcObject<FbxTexture>(k);
				if (!fbx_texture) continue;
				
				FbxFileTexture *fbx_file_texture = FbxCast<FbxFileTexture>(fbx_texture);
				if (!fbx_file_texture) continue;

				UMTexture texture;
				texture.set_name(std::string(fbx_texture->GetName()));
				texture.set_relative_file_name(std::string(fbx_file_texture->GetRelativeFileName()));
				texture.set_file_name(std::string(fbx_file_texture->GetFileName()));

				FbxLayeredTexture::EBlendMode blend_mode;
				if (fbx_layered_texture->GetTextureBlendMode(k, blend_mode))
				{
					texture.set_blend_mode(static_cast<int>(blend_mode));
				}
				double alpha = 0.0;
				if (fbx_layered_texture->GetTextureAlpha(k, alpha))
				{
					texture.set_alpha(static_cast<int>(alpha));
				}

				texture_list.push_back(texture);
			}
			material.mutable_layered_texture_list().push_back(texture_list);
		}
	}
	else
	{
		const int texture_count = fbx_property.GetSrcObjectCount<FbxTexture>();
		for (int k = 0; k < texture_count; ++k)
		{
			FbxTexture* fbx_texture = fbx_property.GetSrcObject<FbxTexture>();
			if (!fbx_texture) continue;

			FbxFileTexture *fbx_file_texture = FbxCast<FbxFileTexture>(fbx_texture);
			if (!fbx_file_texture) continue;

			UMTexture texture;
			texture.set_name(std::string(fbx_texture->GetName()));
			texture.set_relative_file_name(std::string(fbx_file_texture->GetRelativeFileName()));
			texture.set_file_name(std::string(fbx_file_texture->GetFileName()));

			material.mutable_texture_list().push_back(texture);
		}
	}
	return true;
}

/**
 * assign imported materials to UMMesh.
 * @param node mesh's node
 * TODO: assing materials to other
 */
bool UMFbxLoadImpl::assign_materials(UMObjectPtr object, UMMesh& mesh, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	const int material_count = node->GetMaterialCount();
	for (int i = 0; i < material_count; ++i)
	{
		if (FbxSurfaceMaterial* fbx_material = node->GetMaterial(i))
		{
			UMMaterial material;
			FbxClassId class_id = fbx_material->GetClassId();

			// shading
			if (class_id.Is(FbxSurfacePhong::ClassId))
			{
				// Phong
				FbxSurfacePhong * phong = static_cast<FbxSurfacePhong *>(fbx_material);
				if (!phong) continue;

				// Ambient Color
				{
					material.mutable_ambient()[0] = phong->Ambient.Get()[0];
					material.mutable_ambient()[1] = phong->Ambient.Get()[1];
					material.mutable_ambient()[2] = phong->Ambient.Get()[2];
				}

				// AmbientFactor
				{
					material.set_ambient_factor(phong->AmbientFactor.Get());
				}
				
				// Diffuse Color
				{
					material.mutable_diffuse()[0] = phong->Diffuse.Get()[0];
					material.mutable_diffuse()[1] = phong->Diffuse.Get()[1];
					material.mutable_diffuse()[2] = phong->Diffuse.Get()[2];
				}
				
				// DiffuseFactor
				{
					material.set_diffuse_factor(phong->DiffuseFactor.Get());
				}

				// Specular Color
				{
					material.mutable_specular()[0] = phong->Specular.Get()[0];
					material.mutable_specular()[1] = phong->Specular.Get()[1];
					material.mutable_specular()[2] = phong->Specular.Get()[2];
				}
				
				// SpecularFactor
				{
					material.set_specular_factor(phong->SpecularFactor.Get());
				}

				// Emissive Color
				{
					material.mutable_emissive()[0] = phong->Emissive.Get()[0];
					material.mutable_emissive()[1] = phong->Emissive.Get()[1];
					material.mutable_emissive()[2] = phong->Emissive.Get()[2];
				}

				// EmissiveFactor
				{
					material.set_emissive_factor(phong->EmissiveFactor.Get());
				}

				// TransparentColor
				{
					material.mutable_transparent()[0] = phong->TransparentColor.Get()[0];
					material.mutable_transparent()[1] = phong->TransparentColor.Get()[1];
					material.mutable_transparent()[2] = phong->TransparentColor.Get()[2];
				}

				// TransparencyFactor
				{
					material.set_transparency_factor(phong->TransparencyFactor.Get());
				}

				// Reflection
				{
					material.mutable_refrection()[0] = phong->Reflection.Get()[0];
					material.mutable_refrection()[1] = phong->Reflection.Get()[1];
					material.mutable_refrection()[2] = phong->Reflection.Get()[2];
				}

				// ReflectionFactor
				{
					material.set_reflection_factor(phong->ReflectionFactor.Get());
				}

				// Shininess
				{
					material.set_shininess(phong->Shininess.Get());
				}
			}
			else if(class_id.Is(FbxSurfaceLambert::ClassId) )
			{
				// Phong
				FbxSurfaceLambert * lambert = static_cast<FbxSurfaceLambert *>(fbx_material);
				if (!lambert) continue;
				
				// Ambient Color
				{
					material.mutable_ambient()[0] = lambert->Ambient.Get()[0];
					material.mutable_ambient()[1] = lambert->Ambient.Get()[1];
					material.mutable_ambient()[2] = lambert->Ambient.Get()[2];
				}

				// AmbientFactor
				{
					material.set_ambient_factor(lambert->AmbientFactor.Get());
				}
				
				// Diffuse Color
				{
					material.mutable_diffuse()[0] = lambert->Diffuse.Get()[0];
					material.mutable_diffuse()[1] = lambert->Diffuse.Get()[1];
					material.mutable_diffuse()[2] = lambert->Diffuse.Get()[2];
				}

				// DiffuseFactor
				{
					material.set_diffuse_factor(lambert->DiffuseFactor.Get());
				}
				
				// Emissive Color
				{
					material.mutable_emissive()[0] = lambert->Emissive.Get()[0];
					material.mutable_emissive()[1] = lambert->Emissive.Get()[1];
					material.mutable_emissive()[2] = lambert->Emissive.Get()[2];
				}

				// EmissiveFactor
				{
					material.set_emissive_factor(lambert->EmissiveFactor.Get());
				}
				
				// TransparentColor
				{
					material.mutable_transparent()[0] = lambert->TransparentColor.Get()[0];
					material.mutable_transparent()[1] = lambert->TransparentColor.Get()[1];
					material.mutable_transparent()[2] = lambert->TransparentColor.Get()[2];
				}

				// TransparencyFactor
				{
					material.set_transparency_factor(lambert->TransparencyFactor.Get());
				}

				// Bump
				{
					material.mutable_bump()[0] = lambert->Bump.Get()[0];
					material.mutable_bump()[1] = lambert->Bump.Get()[1];
					material.mutable_bump()[2] = lambert->Bump.Get()[2];
				}

				// BumpFactor
				{
					material.set_bump_factor(lambert->BumpFactor.Get());
				}

				// VectorDisplacementColor
				{
					material.mutable_vector_displacement()[0] = lambert->VectorDisplacementColor.Get()[0];
					material.mutable_vector_displacement()[1] = lambert->VectorDisplacementColor.Get()[1];
					material.mutable_vector_displacement()[2] = lambert->VectorDisplacementColor.Get()[2];
				}

				// VectorDisplacementFactor
				{
					material.set_vector_displacement_factor(lambert->VectorDisplacementFactor.Get());
				}
			}

			// texture
			FbxProperty fbx_property = fbx_material->GetFirstProperty();
			for (; fbx_property.IsValid(); fbx_property = fbx_material->GetNextProperty(fbx_property))
			{
				FbxString fbx_property_name = fbx_property.GetName();
				if (fbx_property_name == FbxSurfaceMaterial::sDiffuse
					|| fbx_property_name == FbxSurfaceMaterial::sDiffuseFactor
					|| fbx_property_name == FbxSurfaceMaterial::sEmissive
					|| fbx_property_name == FbxSurfaceMaterial::sEmissiveFactor
					|| fbx_property_name == FbxSurfaceMaterial::sAmbient
					|| fbx_property_name == FbxSurfaceMaterial::sAmbientFactor
					|| fbx_property_name == FbxSurfaceMaterial::sSpecular
					|| fbx_property_name == FbxSurfaceMaterial::sSpecularFactor
					|| fbx_property_name == FbxSurfaceMaterial::sShininess
					|| fbx_property_name == FbxSurfaceMaterial::sBump
					|| fbx_property_name == FbxSurfaceMaterial::sNormalMap
					|| fbx_property_name == FbxSurfaceMaterial::sTransparentColor
					|| fbx_property_name == FbxSurfaceMaterial::sTransparencyFactor
					|| fbx_property_name == FbxSurfaceMaterial::sReflection
					|| fbx_property_name == FbxSurfaceMaterial::sReflectionFactor)
				{
					assign_textures(object, material, fbx_property);
				}
			}
			mesh.mutable_material_list().push_back(material);
		}
	}
	return true;
}

/**
 * assign imported skins to UMObject.
 * @param node mesh's node
 */
bool UMFbxLoadImpl::assign_skins(UMObjectPtr object, UMMesh& mesh, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	FbxMesh* fbx_mesh = node->GetMesh();
	if (!fbx_mesh) return false;

	const int skin_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int i = 0; i < skin_count; ++i)
	{
		if (FbxSkin* fbx_skin = FbxCast<FbxSkin>(fbx_mesh->GetDeformer(i, FbxDeformer::eSkin)))
		{
			UMSkin skin;
			skin.set_name(std::string(fbx_skin->GetInitialName()));
			FbxGeometry* geometry = fbx_skin->GetGeometry();
			if (geometry == fbx_mesh)
			{
				skin.set_geometry_node_id(static_cast<int>(node->GetUniqueID()));
			}

			const int cluster_count = fbx_skin->GetClusterCount();
			for (int k = 0; k < cluster_count; ++k)
			{
				if (FbxCluster* fbx_cluster = fbx_skin->GetCluster(k))
				{
					UMCluster cluster;
					cluster.set_name(std::string(fbx_cluster->GetInitialName()));
					// link
					cluster.set_link_mode(static_cast<int>(fbx_cluster->GetLinkMode()));
					if (FbxNode* fbx_node = fbx_cluster->GetLink())
					{
						cluster.set_link_node_id(static_cast<int>(fbx_node->GetUniqueID()));
					}
					// vertex index
					const int vertex_index_size = fbx_cluster->GetControlPointIndicesCount();
					for (int n = 0; n < vertex_index_size; ++n)
					{
						int index = fbx_cluster->GetControlPointIndices()[n];
						cluster.mutable_index_list().push_back(index);
					}
					// weight
					for (int n = 0; n < vertex_index_size; ++n)
					{
						double weight = fbx_cluster->GetControlPointWeights()[n];
						cluster.mutable_weight_list().push_back(weight);
					}

					skin.mutable_cluster_list().push_back(cluster);
				}
			}
			mesh.mutable_skin_list().push_back(skin);
		}
	}

	return true;
}

/**
 * assign imported blend shapes to UMObject.
 * @param node mesh's node
 */
bool UMFbxLoadImpl::assign_blend_shapes(UMObjectPtr object, UMMesh& mesh, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	FbxMesh* fbx_mesh = node->GetMesh();
	if (!fbx_mesh) return false;

	const int blend_shape_count = fbx_mesh->GetDeformerCount(FbxDeformer::eBlendShape);
	for (int i = 0; i < blend_shape_count; ++i)
	{
		if (FbxBlendShape* fbx_blend_shape = FbxCast<FbxBlendShape>(fbx_mesh->GetDeformer(i, FbxDeformer::eBlendShape)))
		{
			UMBlendShape blend_shape;
			blend_shape.set_name(std::string(fbx_blend_shape->GetInitialName()));
			FbxGeometry* geometry = fbx_blend_shape->GetGeometry();
			if (geometry == fbx_mesh)
			{
				blend_shape.set_geometry_node_id(static_cast<int>(node->GetUniqueID()));
			}

			const int shape_channel_count = fbx_blend_shape->GetBlendShapeChannelCount();
			for (int k = 0; k < shape_channel_count; ++k)
			{
				if (FbxBlendShapeChannel* fbx_shape_channel = fbx_blend_shape->GetBlendShapeChannel(k))
				{
					UMBlendShapeChannel shape_channel;
					const int shape_count = fbx_shape_channel->GetTargetShapeCount();
					for (int n = 0; n < shape_count; ++n)
					{
						if (FbxShape* fbx_shape = fbx_shape_channel->GetTargetShape(n))
						{
							UMShape shape;

							shape.set_name(fbx_shape->GetName());

							FbxGeometry* geometry = fbx_shape->GetBaseGeometry();
							if (geometry == fbx_mesh)
							{
								shape.set_base_geometry_node_id(static_cast<int>(geometry->GetUniqueID()));
							}

							// vertex index
							const int vertex_indices_count = fbx_shape->GetControlPointIndicesCount();
							for (int m = 0; m < vertex_indices_count; ++m)
							{
								int index = fbx_shape->GetControlPointIndices()[m];
								shape.mutable_vertex_index_list().push_back(index);
							}

							// vertex
							const int vertex_count = fbx_shape->GetControlPointsCount();
							for (int m = 0; m < vertex_count; ++m)
							{
								FbxVector4 v = fbx_shape->GetControlPointAt(m);
								DoubleList vertex;
								vertex.push_back(v[0]);
								vertex.push_back(v[1]);
								vertex.push_back(v[2]);
								vertex.push_back(v[3]);
								shape.mutable_vertex_list().push_back(vertex);
							}

							// normal index
							FbxLayerElementArrayTemplate<int>* fbx_normals_indices = NULL;
							fbx_shape->GetNormalsIndices(&fbx_normals_indices);
							if (fbx_normals_indices)
							{
								const int normal_indices_count = fbx_normals_indices->GetCount();
								for (int m = 0; m < normal_indices_count; ++m)
								{
									int index = fbx_normals_indices->GetAt(m);
									shape.mutable_normal_index_list().push_back(index);
								}
							}

							// normal
							FbxLayerElementArrayTemplate<FbxVector4>* fbx_normals = NULL;
							fbx_shape->GetNormals(&fbx_normals);
							if (fbx_normals)
							{
								const int normal_count = fbx_normals->GetCount();
								for (int m = 0; m < normal_count; ++m)
								{
									FbxVector4 v = fbx_normals->GetAt(m);
									DoubleList normal;
									normal.push_back(v[0]);
									normal.push_back(v[1]);
									normal.push_back(v[2]);
									normal.push_back(v[3]);
									shape.mutable_normal_list().push_back(normal);
								}
							}

							// full weight list
							double full_weight = fbx_shape_channel->GetTargetShapeFullWeights()[n];
							shape_channel.mutable_full_weight_list().push_back(full_weight);
							
							shape_channel.mutable_target_shape_list().push_back(shape);
						}
					}

					// deform percent
					double percent = fbx_shape_channel->DeformPercent.Get();
					shape_channel.set_deform_percent(percent);

					blend_shape.mutable_blend_shape_channel_list().push_back(shape_channel);
				}
			}
			mesh.mutable_blend_shape_list().push_back(blend_shape);
		}
	}
	return true;
}

/**
 * assing normals to UMMesh
 */
bool UMFbxLoadImpl::assign_normals(
	UMMesh& mesh, 
	FbxMesh* fbx_mesh, 
	FbxLayer* layer,
	int polygon_index,
	int point_index,
	int polygon_vertex_index,
	int control_point_index
	)
{
	if (!fbx_mesh) return false;
	if (!layer) return false;

	if (FbxLayerElementNormal* fbx_normals = layer->GetNormals())
	{
		FbxLayerElement::EMappingMode mode = fbx_normals->GetMappingMode();
		FbxLayerElement::EReferenceMode ref_mode = fbx_normals->GetReferenceMode();
		umio::DoubleList normal;
		normal.resize(3);

		FbxVector4 n;
		if (mode == FbxLayerElement::eByControlPoint)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				n = fbx_normals->GetDirectArray().GetAt(control_point_index);
			}
		}
		else if (mode == FbxLayerElement::eByPolygonVertex)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				n = fbx_normals->GetDirectArray().GetAt(polygon_vertex_index);
			}
			if (ref_mode == FbxLayerElement::eIndexToDirect)
			{
				//if (fbx_mesh->GetPolygonVertexNormal(polygon_index, point_index, normal))
				int index = fbx_normals->GetIndexArray().GetAt(polygon_vertex_index);
				n = fbx_normals->GetDirectArray().GetAt(index);
			}
		}
		normal[0] = n[0];
		normal[1] = n[1];
		normal[2] = n[2];
		
		if (mesh.layered_normal_list().size() == 1 && mesh.normal_list().empty())
		{
			normal_layer_to_index_map_[layer] = 0;
		}

		if (normal_layer_to_index_map_.find(layer) == normal_layer_to_index_map_.end())
		{
			int layer_index = mesh.add_normal_layer();
			normal_layer_to_index_map_[layer] = layer_index;
			mesh.mutable_normal_list(layer_index).push_back(normal);
		}
		else
		{
			mesh.mutable_normal_list(normal_layer_to_index_map_[layer]).push_back(normal);
		}
		return true;
	}
	return false;
}

/**
 * assing uvs to UMMesh
 */
bool UMFbxLoadImpl::assign_uvs(
	UMMesh& mesh, 
	FbxMesh* fbx_mesh, 
	FbxLayer* layer,
	int polygon_index,
	int point_index,
	int polygon_vertex_index,
	int control_point_index
	)
{
	if (!fbx_mesh) return false;
	if (!layer) return false;

	// http://community.softimage.com/forum/autodesk-fbx/fbx-sdk/texture-lookups-for-uv-coordinates/
	if (FbxLayerElementUV* fbx_uv = layer->GetUVs())
	{
		FbxLayerElement::EMappingMode mode = fbx_uv->GetMappingMode();
		FbxLayerElement::EReferenceMode ref_mode = fbx_uv->GetReferenceMode();
		umio::DoubleList uv;
		uv.resize(2);

		FbxVector2 v;
		if (mode == FbxLayerElement::eByControlPoint)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				v = fbx_uv->GetDirectArray().GetAt(control_point_index);
			}
			else if (ref_mode == FbxLayerElement::eIndexToDirect)
			{
				int index = fbx_uv->GetIndexArray().GetAt(control_point_index);
				v = fbx_uv->GetDirectArray().GetAt(index);
			}
		}
		else if (mode == FbxLayerElement::eByPolygonVertex)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				v = fbx_uv->GetDirectArray().GetAt(polygon_vertex_index);
			}
			else if (ref_mode == FbxLayerElement::eIndexToDirect)
			{
				int index = fbx_mesh->GetTextureUVIndex(polygon_index, point_index);
				v = fbx_uv->GetDirectArray().GetAt(index);
			}
		}
		uv[0] = v[0];
		uv[1] = v[1];

		if (mesh.layered_uv_list().size() == 1 && mesh.uv_list().empty())
		{
			uv_layer_to_index_map_[layer] = 0;
		}

		if (uv_layer_to_index_map_.find(layer) == uv_layer_to_index_map_.end())
		{
			int layer_index = mesh.add_uv_layer();
			uv_layer_to_index_map_[layer] = layer_index;
			mesh.mutable_uv_list(layer_index).push_back(uv);
		}
		else
		{
			mesh.mutable_uv_list(uv_layer_to_index_map_[layer]).push_back(uv);
		}
		return true;
	}
	return false;
}

/**
 * assing vertex colors to UMMesh
 */
bool UMFbxLoadImpl::assign_vertex_colors(
	UMMesh& mesh, 
	FbxMesh* fbx_mesh, 
	FbxLayer* layer,
	int polygon_index,
	int point_index,
	int polygon_vertex_index,
	int control_point_index
	)
{
	if (!fbx_mesh) return false;
	if (!layer) return false;
	
	if (FbxLayerElementVertexColor* fbx_vertex_color = layer->GetVertexColors())
	{
		FbxLayerElement::EMappingMode mode = fbx_vertex_color->GetMappingMode();
		FbxLayerElement::EReferenceMode ref_mode = fbx_vertex_color->GetReferenceMode();
		umio::DoubleList vertex_color;
		vertex_color.resize(4);

		FbxColor v;
		if (mode == FbxLayerElement::eByControlPoint)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				v = fbx_vertex_color->GetDirectArray().GetAt(control_point_index);
			}
			else if (ref_mode == FbxLayerElement::eIndexToDirect)
			{
				int index = fbx_vertex_color->GetIndexArray().GetAt(control_point_index);
				v = fbx_vertex_color->GetDirectArray().GetAt(index);
			}
		}
		else if (mode == FbxLayerElement::eByPolygonVertex)
		{
			if (ref_mode == FbxLayerElement::eDirect)
			{
				v = fbx_vertex_color->GetDirectArray().GetAt(polygon_vertex_index);
			}
			else if (ref_mode == FbxLayerElement::eIndexToDirect)
			{
				int index = fbx_vertex_color->GetIndexArray().GetAt(polygon_vertex_index);
				v = fbx_vertex_color->GetDirectArray().GetAt(index);
			}
		}
		vertex_color[0] = v[0];
		vertex_color[1] = v[1];
		vertex_color[2] = v[2];
		vertex_color[3] = v[3];
		
		if (mesh.layered_vertex_color_list().size() == 1 && mesh.vertex_color_list().empty())
		{
			vertex_color_layer_to_index_map_[layer] = 0;
		}

		if (vertex_color_layer_to_index_map_.find(layer) == vertex_color_layer_to_index_map_.end())
		{
			int layer_index = mesh.add_vertex_color_layer();
			vertex_color_layer_to_index_map_[layer] = layer_index;
			mesh.mutable_vertex_color_list(layer_index).push_back(vertex_color);
		}
		else
		{
			mesh.mutable_vertex_color_list(vertex_color_layer_to_index_map_[layer]).push_back(vertex_color);
		}
		return true;
	}
	return false;
}

/**
 * assign imported mesh to UMObject.
 */
bool UMFbxLoadImpl::assign_mesh(UMObjectPtr object, FbxNode* node)
{
	if (!object) false;
	if (!node) return false;

	FbxMesh* fbx_mesh = node->GetMesh();
	if (!fbx_mesh) return false;
	
	FbxMesh* fbx_mesh_no_triangulated = fbx_mesh;
	if (is_triangulate())
	{
		FbxGeometryConverter converter(manager());
		FbxNodeAttribute* attr = converter.Triangulate(fbx_mesh, false);
		if (fbx_mesh = FbxCast<FbxMesh>(attr))
		{
			node->SetNodeAttribute(attr);
		}
		else
		{
			return false;
		}
	}

	const int polygon_count = fbx_mesh->GetPolygonCount();
	FbxVector4* control_points = fbx_mesh->GetControlPoints();
	const int control_point_count = fbx_mesh->GetControlPointsCount();
	
	// create mesh
	UMMesh mesh;

	// vertex counter
	int polygon_vertex_index = 0;

	if (polygon_count > 0 || control_point_count > 0)
	{
		// materials
		assign_materials(object, mesh, node);
		
		// vertex
		for (int i = 0; i < control_point_count; ++i)
		{
			umio::DoubleList vertex;
			vertex.push_back(control_points[i][0]);
			vertex.push_back(control_points[i][1]);
			vertex.push_back(control_points[i][2]);
			mesh.mutable_vertex_list().push_back(vertex);
		}

		// polygon
		for (int i = 0; i < polygon_count; ++i)
		{
			umio::IntList vertex_index;
			
			for (int n = 0, nsize = fbx_mesh->GetLayerCount(); n < nsize; ++n)
			{
				// material index
				if (FbxLayerElementMaterial* fbx_materials = fbx_mesh->GetLayer(n)->GetMaterials())
				{
					FbxLayerElement::EMappingMode mode = fbx_materials->GetMappingMode();
					FbxLayerElement::EReferenceMode ref_mode = fbx_materials->GetReferenceMode();
						
					if (mode == FbxLayerElement::eByPolygon)
					{
						if (ref_mode == FbxLayerElement::eIndexToDirect)
						{
							int material_index = fbx_materials->GetIndexArray().GetAt(i);
							mesh.mutable_material_index().push_back(material_index);
						}
					}
				}
			}

			const int polygon_size = fbx_mesh->GetPolygonSize(i);
			for (int k = 0; k < polygon_size; ++k, ++polygon_vertex_index)
			{
				const int control_point_index = fbx_mesh->GetPolygonVertex(i, k);
				FbxVector4 point = control_points[control_point_index];

				//vertex_index
				vertex_index.push_back(control_point_index);
				for (int n = 0, nsize = fbx_mesh->GetLayerCount(); n < nsize; ++n)
				{
					if (FbxLayer* layer = fbx_mesh->GetLayer(n))
					{
						// normals
						assign_normals(mesh, fbx_mesh, layer, i, k, polygon_vertex_index, control_point_index);

						// uvs
						assign_uvs(mesh, fbx_mesh, layer, i, k, polygon_vertex_index, control_point_index);

						// vertex colors
						assign_vertex_colors(mesh, fbx_mesh, layer, i, k, polygon_vertex_index, control_point_index);
					}
				}
			}
			mesh.mutable_vertex_index_list().push_back(vertex_index);
		}
		// blend shape
		assign_blend_shapes(object, mesh, node);
		// skin
		assign_skins(object, mesh, node);
	}
	
	if (is_triangulate())
	{
		node->SetNodeAttribute(fbx_mesh_no_triangulated);
	}

	object->mutable_mesh_map()[static_cast<int>(node->GetUniqueID())] = mesh;
	assign_one_node(
		object, 
		&object->mutable_mesh_map()[static_cast<int>(node->GetUniqueID())],
		node);

	return true;
}

/**
 * assign imported nurbs to UMObject as mesh.
 */
bool UMFbxLoadImpl::assign_nurbs(UMObjectPtr object, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	FbxNurbs* fbx_nurbs = node->GetNurbs();
	if (!fbx_nurbs) return false;
	
	FbxGeometryConverter converter(manager());
	if (FbxMesh* fbx_mesh = FbxCast<FbxMesh>(converter.Triangulate(fbx_nurbs, false)))
	{
		node->SetNodeAttribute(fbx_mesh);
		return assign_mesh(object, node);
	}
	return false;
}

/**
 * assign imported patch to UMObject as mesh.
 */
bool UMFbxLoadImpl::assign_patch(UMObjectPtr object, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	FbxPatch* fbx_patch = node->GetPatch();
	if (!fbx_patch) return false;
	
	FbxGeometryConverter converter(manager());
	if (FbxMesh* fbx_mesh = FbxCast<FbxMesh>(converter.Triangulate(fbx_patch, false)))
	{
		node->SetNodeAttribute(fbx_mesh);
		return assign_mesh(object, node);
	}
	return false;
}

/**
 * assign imported skeleton to UMObject.
 */
bool UMFbxLoadImpl::assing_skeleton(UMObjectPtr object, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;

	FbxSkeleton *fbx_skeleton = FbxCast<FbxSkeleton>(node->GetNodeAttribute());
	if (fbx_skeleton)
	{
		int id = static_cast<int>(node->GetUniqueID());
		IntList& skeleton_ids = mutable_skeleton_id_list();
		if (std::find(skeleton_ids.begin(), skeleton_ids.end(), id) == skeleton_ids.end())
		{
			skeleton_ids.push_back(id);

			UMSkeleton skeleton;

			FbxSkeleton::EType type = fbx_skeleton->GetSkeletonType();
			skeleton.set_type(static_cast<int>(type));
			if (type == FbxSkeleton::eLimb)
			{
				skeleton.set_limb_length(fbx_skeleton->LimbLength.Get());
			}
			else if (type == FbxSkeleton::eLimbNode)
			{
				skeleton.set_size(fbx_skeleton->Size.Get());
			}
			else if (type == FbxSkeleton::eRoot)
			{
				skeleton.set_size(fbx_skeleton->Size.Get());
			}

			object->mutable_skeleton_map()[static_cast<int>(node->GetUniqueID())] = skeleton;
			assign_one_node(
				object, 
				&object->mutable_skeleton_map()[static_cast<int>(node->GetUniqueID())], 
				node);
			return true;
		}
	}
	return false;
}


/**
 * assign imported camera to UMObject.
 */
bool UMFbxLoadImpl::assign_camera(UMObjectPtr object, FbxNode* node)
{
	if (!object) return false;
	if (!node) return false;
	
	FbxCamera *fbx_camera = FbxCast<FbxCamera>(node->GetNodeAttribute());
	if (fbx_camera)
	{
		UMCamera camera;
		
		camera.mutable_position()[0] = fbx_camera->Position.Get()[0];
		camera.mutable_position()[1] = fbx_camera->Position.Get()[1];
		camera.mutable_position()[2] = fbx_camera->Position.Get()[2];
		camera.mutable_up_vector()[0] = fbx_camera->UpVector.Get()[0];
		camera.mutable_up_vector()[1] = fbx_camera->UpVector.Get()[1];
		camera.mutable_up_vector()[2] = fbx_camera->UpVector.Get()[2];
		camera.mutable_interest_position()[0] = fbx_camera->InterestPosition.Get()[0];
		camera.mutable_interest_position()[1] = fbx_camera->InterestPosition.Get()[1];
		camera.mutable_interest_position()[2] = fbx_camera->InterestPosition.Get()[2];
		camera.set_roll(fbx_camera->Roll.Get());
		camera.set_optical_center_x(fbx_camera->OpticalCenterX.Get());
		camera.set_optical_center_y(fbx_camera->OpticalCenterY.Get());
		camera.mutable_background_color()[0] = fbx_camera->BackgroundColor.Get()[0];
		camera.mutable_background_color()[1] = fbx_camera->BackgroundColor.Get()[1];
		camera.mutable_background_color()[2] = fbx_camera->BackgroundColor.Get()[2];
		camera.set_turn_table(fbx_camera->TurnTable.Get());
		camera.set_display_turn_table_icon(fbx_camera->DisplayTurnTableIcon.Get());
		camera.set_use_motion_blur(fbx_camera->UseMotionBlur.Get());
		camera.set_use_real_time_motion_blur(fbx_camera->UseRealTimeMotionBlur.Get());
		camera.set_motion_blur_intensity(fbx_camera->MotionBlurIntensity.Get());
		camera.set_aspect_ratio_mode(fbx_camera->AspectRatioMode.Get());
		camera.set_aspect_width(fbx_camera->AspectWidth.Get());
		camera.set_aspect_height(fbx_camera->AspectHeight.Get());
		camera.set_pixel_aspect_ration(fbx_camera->PixelAspectRatio.Get());
		camera.set_aperture_mode(fbx_camera->ApertureMode.Get());
		camera.set_gate_fit(fbx_camera->GateFit.Get());
		camera.set_field_of_view(fbx_camera->FieldOfView.Get());
		camera.set_field_of_view_x(fbx_camera->FieldOfView.Get());
		camera.set_field_of_view_y(fbx_camera->FieldOfViewX.Get());
		camera.set_focal_length(fbx_camera->FocalLength.Get());
		camera.set_camera_format(fbx_camera->CameraFormat.Get());
		camera.set_use_frame_color(fbx_camera->UseFrameColor.Get());
		camera.mutable_frame_color()[0] = fbx_camera->FrameColor.Get()[0];
		camera.mutable_frame_color()[1] = fbx_camera->FrameColor.Get()[1];
		camera.mutable_frame_color()[2] = fbx_camera->FrameColor.Get()[2];
		camera.set_show_name(fbx_camera->ShowName.Get());
		camera.set_show_info_on_moving(fbx_camera->ShowInfoOnMoving.Get());
		camera.set_show_grid(fbx_camera->ShowGrid.Get());
		camera.set_show_optical_center(fbx_camera->ShowOpticalCenter.Get());
		camera.set_show_azimut(fbx_camera->ShowAzimut.Get());
		camera.set_show_time_code(fbx_camera->ShowTimeCode.Get());
		camera.set_show_audio(fbx_camera->ShowAudio.Get());
		camera.mutable_audio_color()[0] = fbx_camera->AudioColor.Get()[0];
		camera.mutable_audio_color()[1] = fbx_camera->AudioColor.Get()[1];
		camera.mutable_audio_color()[2] = fbx_camera->AudioColor.Get()[2];
		camera.set_near_plane(fbx_camera->NearPlane.Get());
		camera.set_far_plane(fbx_camera->FarPlane.Get());
		camera.set_auto_compute_clip_planes(fbx_camera->AutoComputeClipPlanes.Get());
		camera.set_film_width(fbx_camera->FilmWidth.Get());
		camera.set_film_height(fbx_camera->FilmHeight.Get());
		camera.set_film_aspect_ration(fbx_camera->FilmAspectRatio.Get());
		camera.set_film_squeeze_ration(fbx_camera->FilmSqueezeRatio.Get());
		camera.set_film_format(fbx_camera->FilmFormat.Get());
		camera.set_film_offset_x(fbx_camera->FilmOffsetX.Get());
		camera.set_film_offset_y(fbx_camera->FilmOffsetY.Get());
		camera.set_pre_scale(fbx_camera->PreScale.Get());
		camera.set_film_translate_x(fbx_camera->FilmTranslateX.Get());
		camera.set_film_translate_y(fbx_camera->FilmTranslateY.Get());
		camera.set_film_roll_pivot_x(fbx_camera->FilmRollPivotX.Get());
		camera.set_film_roll_pivot_y(fbx_camera->FilmRollPivotY.Get());
		camera.set_film_roll_value(fbx_camera->FilmRollValue.Get());
		camera.set_film_roll_order(fbx_camera->FilmRollOrder.Get());
		camera.set_view_camera_to_look_at(fbx_camera->ViewCameraToLookAt.Get());
		camera.set_view_frustum_near_far_plane(fbx_camera->ViewFrustumNearFarPlane.Get());
		camera.set_view_frustum_back_plane_mode(fbx_camera->ViewFrustumBackPlaneMode.Get());
		camera.set_back_plane_distance(fbx_camera->BackPlaneDistance.Get());
		camera.set_back_plane_distance_mode(fbx_camera->BackPlaneDistanceMode.Get());
		camera.set_view_frustum_front_plane_mode(fbx_camera->ViewFrustumFrontPlaneMode.Get());
		camera.set_front_plane_distance(fbx_camera->FrontPlaneDistance.Get());
		camera.set_front_plane_distance_mode(fbx_camera->FrontPlaneDistanceMode.Get());
		camera.set_lock_mode(fbx_camera->LockMode.Get());
		camera.set_lock_interest_navigation(fbx_camera->LockInterestNavigation.Get());
		camera.set_back_plate_fit_image(fbx_camera->BackPlateFitImage.Get());
		camera.set_back_plate_crop(fbx_camera->BackPlateCrop.Get());
		camera.set_back_plate_center(fbx_camera->BackPlateCenter.Get());
		camera.set_back_plate_keep_ration(fbx_camera->BackPlateKeepRatio.Get());
		camera.set_background_alpha_treshold(fbx_camera->BackgroundAlphaTreshold.Get());
		camera.set_back_plane_offset_x(fbx_camera->BackPlaneOffsetX.Get());
		camera.set_back_plane_offset_y(fbx_camera->BackPlaneOffsetY.Get());
		camera.set_back_plane_rotation(fbx_camera->BackPlaneRotation.Get());
		camera.set_back_plane_scale_x(fbx_camera->BackPlaneScaleX.Get());
		camera.set_back_plane_scale_y(fbx_camera->BackPlaneScaleY.Get());
		camera.set_show_backplate(fbx_camera->ShowBackplate.Get());
		camera.set_front_plate_fit_image(fbx_camera->FrontPlateFitImage.Get());
		camera.set_front_plate_crop(fbx_camera->FrontPlateCrop.Get());
		camera.set_front_plate_center(fbx_camera->FrontPlateCenter.Get());
		camera.set_front_plate_keep_ratio(fbx_camera->FrontPlateKeepRatio.Get());
		camera.set_show_frontplate(fbx_camera->ShowFrontplate.Get());
		camera.set_front_plane_offset_x(fbx_camera->FrontPlaneOffsetX.Get());
		camera.set_front_plane_offset_y(fbx_camera->FrontPlaneOffsetY.Get());
		camera.set_front_plane_rotation(fbx_camera->FrontPlaneRotation.Get());
		camera.set_front_plane_scale_x(fbx_camera->FrontPlaneScaleX.Get());
		camera.set_front_plane_scale_y(fbx_camera->FrontPlaneScaleY.Get());
		camera.set_foreground_opacity(fbx_camera->ForegroundOpacity.Get());
		camera.set_display_safe_area(fbx_camera->DisplaySafeArea.Get());
		camera.set_display_safe_area_on_render(fbx_camera->DisplaySafeAreaOnRender.Get());
		camera.set_safe_area_display_style(fbx_camera->SafeAreaDisplayStyle.Get());
		camera.set_safe_area_display_ratio(fbx_camera->SafeAreaAspectRatio.Get());
		camera.set_use_2d_magnifier_zoom(fbx_camera->Use2DMagnifierZoom.Get());
		camera.set__2d_magnifier_zoom(fbx_camera->_2DMagnifierZoom.Get());
		camera.set__2d_magnifier_x(fbx_camera->_2DMagnifierX.Get());
		camera.set__2d_magnifier_y(fbx_camera->_2DMagnifierY.Get());
		camera.set_projection_type(fbx_camera->ProjectionType.Get());
		camera.set_ortho_zoom(fbx_camera->OrthoZoom.Get());
		camera.set_use_real_time_dof_and_aa(fbx_camera->UseRealTimeDOFAndAA.Get());
		camera.set_use_depth_of_field(fbx_camera->UseDepthOfField.Get());
		camera.set_focus_source(fbx_camera->FocusSource.Get());
		camera.set_focus_angle(fbx_camera->FocusAngle.Get());
		camera.set_focus_distance(fbx_camera->FocusDistance.Get());
		camera.set_use_antialiasing(fbx_camera->UseAntialiasing.Get());
		camera.set_antialiasing_intensity(fbx_camera->AntialiasingIntensity.Get());
		camera.set_antialiasing_method(fbx_camera->AntialiasingMethod.Get());
		camera.set_use_accumulation_buffer(fbx_camera->UseAccumulationBuffer.Get());
		camera.set_frame_sample_count(fbx_camera->FrameSamplingCount.Get());
		camera.set_frame_sampling_type(fbx_camera->FrameSamplingType.Get());
		
		object->mutable_camera_map()[static_cast<int>(node->GetUniqueID())] = camera;
		assign_one_node(
			object, 
			&object->mutable_camera_map()[static_cast<int>(node->GetUniqueID())], 
			node);
		return true;
	}
	return false;
}

/**
 * convert FbxNode to UMNode.
 */
bool UMFbxLoadImpl::assign_one_node(UMObjectPtr object, UMNode* dst_node, FbxNode* node)
{
	if (!dst_node) return false;
	if (!node) return false;
	
	const int id = static_cast<int>(node->GetUniqueID());
	if (object->id_to_node_map().find(id) != object->id_to_node_map().end())
	{
		return false;
	}

	dst_node->set_id(id);
	dst_node->set_name(std::string(node->GetName()));
	
	object->mutable_id_to_node_map()[id] = dst_node;

	// local values
	FbxDouble3 trans = node->LclTranslation.Get();
	dst_node->mutable_local_translation()[0] = trans[0];
	dst_node->mutable_local_translation()[1] = trans[1];
	dst_node->mutable_local_translation()[2] = trans[2];
			
	FbxDouble3 rot = node->LclRotation.Get();
	dst_node->mutable_local_rotation()[0] = rot[0];
	dst_node->mutable_local_rotation()[1] = rot[1];
	dst_node->mutable_local_rotation()[2] = rot[2];

	FbxDouble3 scale = node->LclScaling.Get();
	dst_node->mutable_local_scaling()[0] = scale[0];
	dst_node->mutable_local_scaling()[1] = scale[1];
	dst_node->mutable_local_scaling()[2] = scale[2];

	// pivot values
	FbxVector4 rotation_offset = node->GetRotationOffset(FbxNode::eSourcePivot);
	dst_node->mutable_rotation_offset()[0] = rotation_offset[0];
	dst_node->mutable_rotation_offset()[1] = rotation_offset[1];
	dst_node->mutable_rotation_offset()[2] = rotation_offset[2];
	dst_node->mutable_rotation_offset()[3] = rotation_offset[3];

	FbxVector4 rotation_pivot = node->GetRotationPivot(FbxNode::eSourcePivot);
	dst_node->mutable_rotation_pivot()[0] = rotation_pivot[0];
	dst_node->mutable_rotation_pivot()[1] = rotation_pivot[1];
	dst_node->mutable_rotation_pivot()[2] = rotation_pivot[2];
	dst_node->mutable_rotation_pivot()[3] = rotation_pivot[3];

	FbxVector4 pre_rotation = node->GetPreRotation(FbxNode::eSourcePivot);
	dst_node->mutable_pre_rotation()[0] = pre_rotation[0];
	dst_node->mutable_pre_rotation()[1] = pre_rotation[1];
	dst_node->mutable_pre_rotation()[2] = pre_rotation[2];
	dst_node->mutable_pre_rotation()[3] = pre_rotation[3];

	FbxVector4 post_rotation = node->GetPostRotation(FbxNode::eSourcePivot);
	dst_node->mutable_post_rotation()[0] = post_rotation[0];
	dst_node->mutable_post_rotation()[1] = post_rotation[1];
	dst_node->mutable_post_rotation()[2] = post_rotation[2];
	dst_node->mutable_post_rotation()[3] = post_rotation[3];

	FbxVector4 scaling_offset = node->GetScalingOffset(FbxNode::eSourcePivot);
	dst_node->mutable_scaling_offset()[0] = scaling_offset[0];
	dst_node->mutable_scaling_offset()[1] = scaling_offset[1];
	dst_node->mutable_scaling_offset()[2] = scaling_offset[2];
	dst_node->mutable_scaling_offset()[3] = scaling_offset[3];

	FbxVector4 scaling_pivot = node->GetScalingPivot(FbxNode::eSourcePivot);
	dst_node->mutable_scaling_pivot()[0] = scaling_pivot[0];
	dst_node->mutable_scaling_pivot()[1] = scaling_pivot[1];
	dst_node->mutable_scaling_pivot()[2] = scaling_pivot[2];
	dst_node->mutable_scaling_pivot()[3] = scaling_pivot[3];

	FbxVector4 geometric_translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	dst_node->mutable_geometric_translation()[0] = geometric_translation[0];
	dst_node->mutable_geometric_translation()[1] = geometric_translation[1];
	dst_node->mutable_geometric_translation()[2] = geometric_translation[2];
	dst_node->mutable_geometric_translation()[3] = geometric_translation[3];

	FbxVector4 geometric_rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	dst_node->mutable_geometric_rotation()[0] = geometric_rotation[0];
	dst_node->mutable_geometric_rotation()[1] = geometric_rotation[1];
	dst_node->mutable_geometric_rotation()[2] = geometric_rotation[2];
	dst_node->mutable_geometric_rotation()[3] = geometric_rotation[3];

	FbxVector4 geometric_scaling = node->GetGeometricScaling(FbxNode::eSourcePivot);
	dst_node->mutable_geometric_scaling()[0] = geometric_scaling[0];
	dst_node->mutable_geometric_scaling()[1] = geometric_scaling[1];
	dst_node->mutable_geometric_scaling()[2] = geometric_scaling[2];
	dst_node->mutable_geometric_scaling()[3] = geometric_scaling[3];
	
	// set parent
	if (FbxNode* parent = node->GetParent())
	{
		dst_node->set_parent_id(static_cast<int>(parent->GetUniqueID()));
	}
	
	FbxScene* scene = current_scene();
	if (scene && scene->GetAnimationEvaluator())
	{
		// set local tranform
		matrix_to_UMMat44d(
			dst_node->mutable_local_transform(),
			scene->GetAnimationEvaluator()->GetNodeLocalTransform(node));

		// set global transform
		matrix_to_UMMat44d(
			dst_node->mutable_global_transform(),
			scene->GetAnimationEvaluator()->GetNodeGlobalTransform(node));
	}

	return true;
}

/**
 * assign all node to UMObject.
 */
bool UMFbxLoadImpl::assign_all_nodes(UMObjectPtr object, FbxNode* node)
{
	if (!node) return false;
	
	if (FbxNodeAttribute *attr = node->GetNodeAttribute())
	{
		// load by type
		FbxNodeAttribute::EType type = attr->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			assign_mesh(object, node);
		}
		else if (type == FbxNodeAttribute::eSkeleton)
		{
			assing_skeleton(object, node);
		}
		else if (type == FbxNodeAttribute::eCamera)
		{
			assign_camera(object, node);
		}
		else if (is_load_nurbs() && type == FbxNodeAttribute::eNurbs)
		{
			assign_nurbs(object, node);
		}
		else if (is_load_patch() && type == FbxNodeAttribute::ePatch)
		{
			assign_patch(object, node);
		}
		else
		{
			UMNode bl_node;
			object->mutable_other_node_map()[static_cast<int>(node->GetUniqueID())] = bl_node;
			assign_one_node(
				object, 
				&object->mutable_other_node_map()[static_cast<int>(node->GetUniqueID())], 
				node);
		}
	}
	
	for (int i = 0, size = node->GetChildCount(); i < size;++i)
	{
		assign_all_nodes(object, node->GetChild(i));
	}
	return true;
}

/**
 * assign all poses in scene to UMObject.
 */
bool UMFbxLoadImpl::assing_all_poses(UMObjectPtr object)
{
	FbxScene* scene = current_scene();
	if (!scene) return false;
	
	const int pose_count = scene->GetPoseCount();
	for (int i = 0; i < pose_count; ++i)
	{
		if (FbxPose* fbx_pose = scene->GetPose(i))
		{
			UMPose pose;
			// flag
			pose.set_bind_pose(fbx_pose->IsBindPose());
			pose.set_rest_pose(fbx_pose->IsRestPose());

			const int node_count = fbx_pose->GetCount();
			for (int k = 0; k < node_count; ++k)
			{
				// related node
				if (FbxNode* node = fbx_pose->GetNode(k))
				{
					pose.mutable_node_id_list().push_back(static_cast<int>(node->GetUniqueID()));
				}
				
				// matrix
				UMMat44d mat;
				matrix_to_UMMat44d(mat, fbx_pose->GetMatrix(k));
				pose.mutable_matrix_list().push_back(mat);

				// is local
				pose.mutable_matrix_is_local_list().push_back(fbx_pose->IsLocalMatrix(k));
			}
			object->mutable_pose_list().push_back(pose);
		}
	}

	return true;
}

/**
 * create object from fbx's manager, scene, importer
 */
UMObjectPtr UMFbxLoadImpl::create_object(FbxScene* scene)
{
	FbxManager* manager = this->manager();
	FbxImporter* importer = this->importer();
	if (!manager) return UMObjectPtr();
	if (!importer) return UMObjectPtr();

	if (!scene) return UMObjectPtr();
	current_scene_ = scene;
	
	// create object
	UMObjectPtr object = UMObjectPtr(new UMObject);
	if (!object) return UMObjectPtr();

	// assign all node
	assign_all_nodes(object, scene->GetRootNode());
	
	// assign poses
	assing_all_poses(object);

	// bind, and return
	if (UMObject::re_bind_all_nodes(object)) {
		return object;
	}

	return UMObjectPtr(); 
}

/**
 * assign imported animation curve to UMAnimationCurve
 */
bool UMFbxLoadImpl::assign_animation_curve(UMAnimationPtr animation, UMAnimationCurve& curve, FbxAnimCurve* fbx_curve)
{
	if (!animation) return false;
	if (!fbx_curve) return false;

	const int key_count = fbx_curve->KeyGetCount();
	for (int i = 0; i < key_count; ++i)
	{
		UMAnimationCurveKey key;
		FbxAnimCurveKey fbx_key = fbx_curve->KeyGet(i);
		
		key.set_time(fbx_key.GetTime().GetMilliSeconds());
		key.set_value(fbx_key.GetValue());

		key.set_break(fbx_key.GetBreak());
		key.set_constant_mode(fbx_key.GetConstantMode());
		key.set_interpolation_type(fbx_key.GetInterpolation());
		key.set_tangent_mode(fbx_key.GetTangentMode());
		key.set_tangent_visibility(fbx_key.GetTangentVisibility());
		key.set_velocity_mode(fbx_key.GetTangentVelocityMode());
		key.set_weighted_mode(fbx_key.GetTangentWeightMode());

		curve.mutable_key_map()[key.time()] = key;
	}

	return true;
}

/**
 * assign imported animation curve to UMAnimationCurveStack
 */
bool UMFbxLoadImpl::assign_animation_curves(
	UMAnimationPtr animation, 
	UMAnimationCurveStack& curve_stack,
	FbxAnimLayer* fbx_layer,
	FbxNode* node)
{
	// local trans x
	if (FbxAnimCurve* fbx_curve = node->LclTranslation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_X))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalTranslationX] = curve;
		}
	}
	// local trans y
	if (FbxAnimCurve* fbx_curve = node->LclTranslation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Y))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalTranslationY] = curve;
		}
	}
	// local trans z
	if (FbxAnimCurve* fbx_curve = node->LclTranslation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Z))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalTranslationZ] = curve;
		}
	}
	// local rot x
	if (FbxAnimCurve* fbx_curve = node->LclRotation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_X))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalRotationX] = curve;
		}
	}
	// local rot y
	if (FbxAnimCurve* fbx_curve = node->LclRotation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Y))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalRotationY] = curve;
		}
	}
	// local rot z
	if (FbxAnimCurve* fbx_curve = node->LclRotation.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Z))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalRotationZ] = curve;
		}
	}
	// local scale x
	if (FbxAnimCurve* fbx_curve = node->LclScaling.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_X))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalScalingX] = curve;
		}
	}
	// local scale y
	if (FbxAnimCurve* fbx_curve = node->LclScaling.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Y))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalScalingY] = curve;
		}
	}
	// local scale z
	if (FbxAnimCurve* fbx_curve = node->LclScaling.GetCurve(fbx_layer, FBXSDK_CURVENODE_COMPONENT_Z))
	{
		UMAnimationCurve curve;
		if (assign_animation_curve(animation, curve, fbx_curve))
		{
			curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLocalScalingZ] = curve;
		}
	}
	
	if (FbxNodeAttribute* attribute = node->GetNodeAttribute())
	{
		// color red
		if (FbxAnimCurve* fbx_curve = attribute->Color.GetCurve(fbx_layer, FBXSDK_CURVENODE_COLOR_RED))
		{
			UMAnimationCurve curve;
			if (assign_animation_curve(animation, curve, fbx_curve))
			{
				curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eColorRed] = curve;
			}
		}
		// color green
		if (FbxAnimCurve* fbx_curve = attribute->Color.GetCurve(fbx_layer, FBXSDK_CURVENODE_COLOR_GREEN))
		{
			UMAnimationCurve curve;
			if (assign_animation_curve(animation, curve, fbx_curve))
			{
				curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eColorGreen] = curve;
			}
		}
		// color blue
		if (FbxAnimCurve* fbx_curve = attribute->Color.GetCurve(fbx_layer, FBXSDK_CURVENODE_COLOR_BLUE))
		{
			UMAnimationCurve curve;
			if (assign_animation_curve(animation, curve, fbx_curve))
			{
				curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eColorBlue] = curve;
			}
		}

		// light
		if (FbxLight* light = node->GetLight())
		{
			// intensity
			if (FbxAnimCurve* fbx_curve = light->Intensity.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLightIntensity] = curve;
				}
			}
			// outer angle
			if (FbxAnimCurve* fbx_curve = light->OuterAngle.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLightOuterAngle] = curve;
				}
			}
			// fog
			if (FbxAnimCurve* fbx_curve = light->Fog.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eLightFog] = curve;
				}
			}
		}
		// camera
		if (FbxCamera* camera = node->GetCamera())
		{
			// fov
			if (FbxAnimCurve* fbx_curve = camera->FieldOfView.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eFieldOfView] = curve;
				}
			}
			// fov x
			if (FbxAnimCurve* fbx_curve = camera->FieldOfViewX.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eFieldOfViewX] = curve;
				}
			}
			// fov y
			if (FbxAnimCurve* fbx_curve = camera->FieldOfViewY.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eFieldOfViewY] = curve;
				}
			}
			// optical center x
			if (FbxAnimCurve* fbx_curve = camera->OpticalCenterX.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eOpticalCenterX] = curve;
				}
			}
			// optical center y
			if (FbxAnimCurve* fbx_curve = camera->OpticalCenterY.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eOpticalCenterY] = curve;
				}
			}
			// roll
			if (FbxAnimCurve* fbx_curve = camera->Roll.GetCurve(fbx_layer))
			{
				UMAnimationCurve curve;
				if (assign_animation_curve(animation, curve, fbx_curve))
				{
					curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eRoll] = curve;
				}
			}
		}
		// shape
		if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			attribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			attribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			if (FbxGeometry* geometry = FbxCast<FbxGeometry>(attribute))
			{
				const int deformer_count = geometry->GetDeformerCount(FbxDeformer::eBlendShape);
				for (int i = 0; i < deformer_count; ++i)
				{
					if (FbxBlendShape* blend_shape = FbxCast<FbxBlendShape>(geometry->GetDeformer(i, FbxDeformer::eBlendShape)))
					{
						const int channel_count = blend_shape->GetBlendShapeChannelCount();
						for (int k = 0; k < channel_count; ++k)
						{
							if (FbxBlendShapeChannel* channel = blend_shape->GetBlendShapeChannel(k))
							{
								// TODO save name
								const char* name = channel->GetName();
								if (FbxAnimCurve* fbx_curve = geometry->GetShapeChannel(i, k, fbx_layer))
								{
									UMAnimationCurve curve;
									if (assign_animation_curve(animation, curve, fbx_curve))
									{
										curve_stack.mutable_curve_map()[UMAnimationCurveKeyTypes::eBlendShapeValue] = curve;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// TODO  curves specific to properties

	return true;
}

/**
 * assign imported animation curve stacks to UMAnimationLayer
 */
bool UMFbxLoadImpl::assign_animation_curve_stacks(
	UMAnimationPtr animation, 
	UMAnimationLayer& layer, 
	FbxAnimLayer* fbx_layer, 
	FbxNode* node)
{
	if (!animation) return false;
	if (!fbx_layer) return false;
	if (!node) return false;

	UMAnimationCurveStack curve_stack;
	assign_animation_curves(animation, curve_stack, fbx_layer, node);
	layer.mutable_curve_stack_map()[static_cast<int>(node->GetUniqueID())] = curve_stack;

	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		assign_animation_curve_stacks(animation, layer, fbx_layer, node->GetChild(i));
	}

	return true;
}

/**
 * assign imported animation stacks to UMAnimationStack
 */
bool UMFbxLoadImpl::assign_animation_layers(UMAnimationPtr animation, UMAnimationStack& stack, FbxAnimStack* fbx_stack)
{
	if (!animation) return false;
	if (!fbx_stack) return false;

	const int layer_count = fbx_stack->GetMemberCount<FbxAnimLayer>();
	for (int k = 0; k < layer_count; ++k)
	{
		if (FbxAnimLayer* fbx_layer = fbx_stack->GetMember<FbxAnimLayer>(k))
		{
			UMAnimationLayer layer;

			layer.set_blend_mode(fbx_layer->BlendMode.Get());
			layer.set_lock(fbx_layer->Lock.Get());
			layer.set_mute(fbx_layer->Mute.Get());
			layer.set_rotation_accumulation_mode(fbx_layer->RotationAccumulationMode.Get());
			layer.set_scale_accumulation_mode(fbx_layer->ScaleAccumulationMode.Get());
			layer.set_solo(fbx_layer->Solo.Get());
			layer.set_weight(fbx_layer->Weight.Get());
			layer.mutable_color()[0] = fbx_layer->Color.Get()[0];
			layer.mutable_color()[1] = fbx_layer->Color.Get()[1];
			layer.mutable_color()[2] = fbx_layer->Color.Get()[2];

			assign_animation_curve_stacks(animation, layer, fbx_layer, current_scene()->GetRootNode());

			stack.mutable_layer_list().push_back(layer);
		}
	}
	return true;
}

/**
 * assign all animation stack to UMAnimation.
 */
bool UMFbxLoadImpl::assign_all_animation_stacks(UMAnimationPtr animation)
{
	FbxScene* scene = current_scene();
	if (!scene) return false;
	
	const int stack_count = scene->GetSrcObjectCount<FbxAnimStack>();
	for (int i = 0; i < stack_count; ++i)
	{
		if (FbxAnimStack* fbx_stack = scene->GetSrcObject<FbxAnimStack>(i))
		{
			UMAnimationStack stack;
			stack.set_name(fbx_stack->GetName());

			// assign animation layers
			assign_animation_layers(animation, stack, fbx_stack);
			
			animation->mutable_animation_stack_list().push_back(stack);
		}
	}
	return true;
}

/**
 * create animation from fbx's manager, scene, importer, and UMObject
 */
UMAnimationPtr UMFbxLoadImpl::create_animation(FbxScene* scene, UMObjectPtr object)
{
	FbxManager* manager = this->manager();
	FbxImporter* importer = this->importer();
	if (!manager) return UMAnimationPtr();
	if (!importer) return UMAnimationPtr();

	if (!scene) return UMAnimationPtr();
	current_scene_ = scene;
	
	// create animation
	UMAnimationPtr animation = UMAnimationPtr(new UMAnimation);
	if (!animation) return UMAnimationPtr();

	// assign all animation stack
	assign_all_animation_stacks(animation);
	
	return UMAnimationPtr(); 
}

/**
 * load fbx file to UMObject impl
 */
UMObjectPtr UMFbxLoadImpl::load(std::string path, const UMIOSetting& setting)
{
	// Create a manager
	manager_ = FbxManager::Create();
	if (!manager_) return UMObjectPtr();
	FbxManager* manager = manager_;

	// init manager
	FbxIOSettings * ios = FbxIOSettings::Create(manager, IOSROOT );
	manager->SetIOSettings(ios);
	
	// fbx sdk version
	{
		int sdk_major;
		int sdk_minor;
		int sdk_revision;
		FbxManager::GetFileFormatVersion(sdk_major, sdk_minor, sdk_revision);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["sdk_major"] = sdk_major;
//		UM["sdk_minor"] = sdk_minor;
//		UM["sdk_revision"] = sdk_revision;
//		printfm("FBX version number for this FBX SDK is ", UM.sdk_major, UM.sdk_minor, UM.sdk_revision);
//#endif // WITH_PYTHON
	}

	FbxScene* scene = FbxScene::Create(manager,"");
	if (!scene) {
		return UMObjectPtr();
	}
		
	// Create an importer.
	importer_ = FbxImporter::Create(manager,"");
	if (!importer_) {
		return UMObjectPtr();
	}
	FbxImporter* importer = importer_;
	
	FbxIOSettings& fbx_settings = (*manager->GetIOSettings());
	set_load_setting(fbx_settings, setting);
	set_system_unit_type(scene, setting);
	set_axis_type(scene, setting);

	// init importer
	if (!importer->Initialize(path.c_str(), -1, manager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		return UMObjectPtr();
	}

	// file version
	{
		int file_major;
		int file_minor;
		int file_revision;
		importer->GetFileVersion(file_major, file_minor, file_revision);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["file_major"] = file_major;
//		UM["file_minor"] = file_minor;
//		UM["file_revision"] = file_revision;
//		printfm("FBX version number for file number is ", UM.file_major, UM.file_minor, UM.file_revision);
//#endif // WITH_PYTHON
	}

	// Import the scene.
	if (!importer->Import(scene)) {
		printf("import failed.\n");
		return UMObjectPtr();
	}
	
	printf("import sccess!\n");

	UMObjectPtr object = create_object(scene);

	return object;
}

/*---------------------------------------------------------------------------*
 * UMFbxSaveImpl
 *---------------------------------------------------------------------------*/

/**
 * save setting
 */
void UMFbxSaveImpl::set_save_setting(FbxIOSettings& fbx_settings, const UMIOSetting& setting)
{
	UMIOSetting::ExpSettingMap::const_iterator it = setting.exp_prop_map().begin();
	for (; it != setting.exp_prop_map().end(); ++it)
	{
		UMIOSetting::EExpSettingType type = it->first;
		bool val = it->second;

		if (type == UMIOSetting::eExpColladaTriangulate) 
			fbx_settings.SetBoolProp(EXP_COLLADA_TRIANGULATE, val);
		else if (type == UMIOSetting::eExpColladaSingleMatrix) 
			fbx_settings.SetBoolProp(EXP_COLLADA_SINGLEMATRIX, val);
		else if (type == UMIOSetting::eExpColladaFrameRate)
			fbx_settings.SetBoolProp(EXP_COLLADA_FRAME_RATE, val);
		else if (type == UMIOSetting::eExpDxfTriangulate)
			fbx_settings.SetBoolProp(EXP_DXF_TRIANGULATE, val);
		else if (type == UMIOSetting::eExpDxfDeformation)
			fbx_settings.SetBoolProp(EXP_DXF_DEFORMATION, val);
		else if (type == UMIOSetting::eExpObjTriangulate)
			fbx_settings.SetBoolProp(EXP_OBJ_TRIANGULATE, val);
		else if (type == UMIOSetting::eExpObjDeformation)
			fbx_settings.SetBoolProp(EXP_OBJ_DEFORMATION, val);
		else if (type == UMIOSetting::eExp3dsReferenceNode)
			fbx_settings.SetBoolProp(EXP_3DS_REFERENCENODE, val);
		else if (type == UMIOSetting::eExp3dsTexture)
			fbx_settings.SetBoolProp(EXP_3DS_TEXTURE, val);
		else if (type == UMIOSetting::eExp3dsMaterial)
			fbx_settings.SetBoolProp(EXP_3DS_MATERIAL, val);
		else if (type == UMIOSetting::eExp3dsAnimation)
			fbx_settings.SetBoolProp(EXP_3DS_ANIMATION, val);
		else if (type == UMIOSetting::eExp3dsMesh)
			fbx_settings.SetBoolProp(EXP_3DS_MESH, val);
		else if (type == UMIOSetting::eExp3dsLight)
			fbx_settings.SetBoolProp(EXP_3DS_LIGHT, val);
		else if (type == UMIOSetting::eExp3dsCamera)
			fbx_settings.SetBoolProp(EXP_3DS_CAMERA, val);
		else if (type == UMIOSetting::eExp3dsAmbientLight)
			fbx_settings.SetBoolProp(EXP_3DS_AMBIENT_LIGHT, val);
		else if (type == UMIOSetting::eExp3dsRescaling)
			fbx_settings.SetBoolProp(EXP_3DS_RESCALING, val);
		else if (type == UMIOSetting::eExp3dsTexuvbypoly)
			fbx_settings.SetBoolProp(EXP_3DS_TEXUVBYPOLY, val);
		else if (type == UMIOSetting::eExpFbxTemplate)
			fbx_settings.SetBoolProp(EXP_FBX_TEMPLATE, val);
		else if (type == UMIOSetting::eExpFbxPivot)
			fbx_settings.SetBoolProp(EXP_FBX_PIVOT, val);
		else if (type == UMIOSetting::eExpFbxGlobalSettings)
			fbx_settings.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, val);
		else if (type == UMIOSetting::eExpFbxCharacter)
			fbx_settings.SetBoolProp(EXP_FBX_CHARACTER, val);
		else if (type == UMIOSetting::eExpFbxConstraint)
			fbx_settings.SetBoolProp(EXP_FBX_CONSTRAINT, val);
		else if (type == UMIOSetting::eExpFbxGobo)
			fbx_settings.SetBoolProp(EXP_FBX_GOBO, val);
		else if (type == UMIOSetting::eExpFbxShape)
			fbx_settings.SetBoolProp(EXP_FBX_SHAPE, val);
		else if (type == UMIOSetting::eExpFbxMaterial)
			fbx_settings.SetBoolProp(EXP_FBX_MATERIAL, val);
		else if (type == UMIOSetting::eExpFbxTexture)
			fbx_settings.SetBoolProp(EXP_FBX_TEXTURE, val);
		else if (type == UMIOSetting::eExpFbxModel)
			fbx_settings.SetBoolProp(EXP_FBX_MODEL, val);
		else if (type == UMIOSetting::eExpFbxAnimation)
			fbx_settings.SetBoolProp(EXP_FBX_ANIMATION, val);
		else if (type == UMIOSetting::eExpFbxEmbedded)
			fbx_settings.SetBoolProp(EXP_FBX_EMBEDDED, val);
		else if (type == UMIOSetting::eExpFbxPassword)
			fbx_settings.SetBoolProp(EXP_FBX_PASSWORD, val);
		else if (type == UMIOSetting::eExpFbxPasswordEnable)
			fbx_settings.SetBoolProp(EXP_FBX_PASSWORD_ENABLE, val);
		else if (type == UMIOSetting::eExpFbxCollapseExternals)
			fbx_settings.SetBoolProp(EXP_FBX_COLLAPSE_EXTERNALS, val);
		else if (type == UMIOSetting::eExpFbxCompressArrays)
			fbx_settings.SetBoolProp(EXP_FBX_COMPRESS_ARRAYS, val);
		else if (type == UMIOSetting::eExpFbxCompressLevel)
			fbx_settings.SetBoolProp(EXP_FBX_COMPRESS_LEVEL, val);
		else if (type == UMIOSetting::eExpFbxCompressMinsize)
			fbx_settings.SetBoolProp(EXP_FBX_COMPRESS_MINSIZE, val);
		else if (type == UMIOSetting::eExpFbxEmbeddedPropertiesSkipExpFbx)
			fbx_settings.SetBoolProp(EXP_FBX_EMBEDDED_PROPERTIES_SKIP, val);
		else if (type == UMIOSetting::eExpFbxExportFileVersion)
			fbx_settings.SetBoolProp(EXP_FBX_EXPORT_FILE_VERSION, val);
	}
	UMIOSetting::UMExpSettingMap::const_iterator bt = setting.bl_exp_prop_map().begin();
	for (; bt != setting.bl_exp_prop_map().end(); ++bt)
	{
		UMIOSetting::EUMExpSettingType type = bt->first;
		bool val = bt->second;

		if (type == UMIOSetting::eUMExpText)
			set_save_text(val);
		if (type == UMIOSetting::eUMExpOldFBX)
			set_old_fbx(val);
	}
}
 
/**
 * set axis type
 */
void UMFbxSaveImpl::set_axis_type(FbxScene* fbx_scene, const UMIOSetting& setting)
{
	if (!fbx_scene) return;
	const UMIOSetting::EFbxAxisType type = setting.axis_type();

	FbxAxisSystem::EPreDefinedAxisSystem axis_system = FbxAxisSystem::eOpenGL;
	if (type == UMIOSetting::eFbxAxisMayaZUp) 
		axis_system = FbxAxisSystem::eMayaZUp;
	else if (type == UMIOSetting::eFbxAxisMayaYUp) 
		axis_system = FbxAxisSystem::eMayaYUp;
	else if (type == UMIOSetting::eFbxAxisMax) 
		axis_system = FbxAxisSystem::eMax;
	else if (type == UMIOSetting::eFbxAxisMotionBuilder) 
		axis_system = FbxAxisSystem::eMotionBuilder;
	else if (type == UMIOSetting::eFbxAxisOpenGL) 
		axis_system = FbxAxisSystem::eOpenGL;
	else if (type == UMIOSetting::eFbxAxisDirectX) 
		axis_system = FbxAxisSystem::eDirectX;
	else if (type == UMIOSetting::eFbxAxisLightWave) 
		axis_system = FbxAxisSystem::eLightwave;

	FbxAxisSystem fbx_axis_system(axis_system);
	fbx_axis_system.ConvertScene(fbx_scene);
}

/**
 * set system unit type to scene
 */
void UMFbxSaveImpl::set_system_unit_type(FbxScene* fbx_scene, const UMIOSetting& setting)
{
	if (!fbx_scene) return;
	const UMIOSetting::EFbxSystemUnitType unit_type = setting.system_unit_type();
	FbxSystemUnit fbx_system_unit;
	if (unit_type == UMIOSetting::eFbxSystemUnitMM)
		fbx_system_unit = FbxSystemUnit::mm;
	if (unit_type == UMIOSetting::eFbxSystemUnitDM)
		fbx_system_unit = FbxSystemUnit::dm;
	if (unit_type == UMIOSetting::eFbxSystemUnitCM)
		fbx_system_unit = FbxSystemUnit::cm;
	if (unit_type == UMIOSetting::eFbxSystemUnitM)
		fbx_system_unit = FbxSystemUnit::m;
	if (unit_type == UMIOSetting::eFbxSystemUnitKM)
		fbx_system_unit = FbxSystemUnit::km;
	if (unit_type == UMIOSetting::eFbxSystemUnitInch)
		fbx_system_unit = FbxSystemUnit::Inch;
	if (unit_type == UMIOSetting::eFbxSystemUnitFoot)
		fbx_system_unit = FbxSystemUnit::Foot;
	if (unit_type == UMIOSetting::eFbxSystemUnitMile)
		fbx_system_unit = FbxSystemUnit::Mile;
	if (unit_type == UMIOSetting::eFbxSystemUnitYard)
		fbx_system_unit = FbxSystemUnit::Yard;

	fbx_system_unit.ConvertScene(fbx_scene);
}

/**
 * export skeleton to scene
 */
bool UMFbxSaveImpl::export_skeleton(FbxScene* scene, UMObjectPtr object)
{
	if (!scene) return false;
	if (!object) return false;

	// create all skeleton
	std::map<int, FbxNode*> fbx_skeleton_node_map;
	{
		UMSkeleton::IDToSkeletonMap::const_iterator it = object->skeleton_map().begin();
		for (; it != object->skeleton_map().end(); ++it)
		{
			UMSkeleton skeleton = it->second;
			FbxSkeleton* fbx_skeleton = FbxSkeleton::Create(manager(), skeleton.name().c_str());
			if (!fbx_skeleton) continue;
			FbxNode* fbx_skeleton_node = FbxNode::Create(manager(), skeleton.name().c_str());
			if (!fbx_skeleton_node) continue;

			fbx_skeleton_node->SetNodeAttribute(fbx_skeleton);

			const int type = skeleton.type();
			if (type == FbxSkeleton::eLimb)
			{
				fbx_skeleton->LimbLength.Set(FbxDouble(skeleton.limb_length()));
			}
			else if (type == FbxSkeleton::eLimbNode)
			{
				fbx_skeleton->Size.Set(FbxDouble(skeleton.size()));
			}
			else if (type == FbxSkeleton::eRoot)
			{
				fbx_skeleton->Size.Set(FbxDouble(skeleton.size()));
			}
			fbx_skeleton->SetSkeletonType(static_cast<FbxSkeleton::EType>(type));

			fbx_skeleton_node->LclTranslation.Set(FbxDouble3(
				skeleton.local_translation().at(0), 
				skeleton.local_translation().at(1), 
				skeleton.local_translation().at(2)));
		
			fbx_skeleton_node->LclRotation.Set(FbxDouble3(
				skeleton.local_rotation().at(0), 
				skeleton.local_rotation().at(1), 
				skeleton.local_rotation().at(2)));

			fbx_skeleton_node->LclScaling.Set(FbxDouble3(
				skeleton.local_scaling().at(0), 
				skeleton.local_scaling().at(1), 
				skeleton.local_scaling().at(2)));
				
			{
				fbx_skeleton_node->RotationOffset.Set(FbxDouble3(
					skeleton.rotation_offset().at(0), 
					skeleton.rotation_offset().at(1), 
					skeleton.rotation_offset().at(2)));
			}
			
			{
				fbx_skeleton_node->RotationPivot.Set(FbxDouble3(
					skeleton.rotation_pivot().at(0), 
					skeleton.rotation_pivot().at(1), 
					skeleton.rotation_pivot().at(2)));
			}
			
			{
					/*
				fbx_skeleton_node->RotationActive.Set(true);
				fbx_skeleton_node->PreRotation.Set(FbxDouble3(
					skeleton.pre_rotation().at(0), 
					skeleton.pre_rotation().at(1), 
					skeleton.pre_rotation().at(2)));
					
				fbx_skeleton_node->SetPreRotation(
					FbxNode::eDestinationPivot, 
					FbxDouble3(
					skeleton.pre_rotation().at(0), 
					skeleton.pre_rotation().at(1), 
					skeleton.pre_rotation().at(2)));
					*/
					
				fbx_skeleton_node->SetRotationActive(true);

				fbx_skeleton_node->SetPivotState(
					FbxNode::eDestinationPivot, 
					FbxNode::ePivotActive);

				fbx_skeleton_node->SetRotationOrder(
					FbxNode::eDestinationPivot, 
					eEulerXYZ);

				fbx_skeleton_node->SetPreRotation(
					FbxNode::eDestinationPivot, 
					FbxVector4(
					skeleton.pre_rotation().at(0), 
					skeleton.pre_rotation().at(1), 
					skeleton.pre_rotation().at(2), 
					0.0));

				FbxVector4 pre_rotation = fbx_skeleton_node->GetPreRotation(FbxNode::eDestinationPivot);
				
#ifdef WITH_PYTHON
		//boost::python::object UM = boost::python::import("UM").attr("__dict__");
		//UM["pre_rotation_x"] = pre_rotation[0];
		//UM["pre_rotation_y"] = pre_rotation[1];
		//UM["pre_rotation_z"] = pre_rotation[2];
		//printfm("pre rot", UM.pre_rotation_x, UM.pre_rotation_y, UM.pre_rotation_z);
#endif
			}
			
			{
				fbx_skeleton_node->PostRotation.Set(FbxDouble3(
					skeleton.post_rotation().at(0), 
					skeleton.post_rotation().at(1), 
					skeleton.post_rotation().at(2)));
			}
			
			{
				fbx_skeleton_node->ScalingOffset.Set(FbxDouble3(
					skeleton.scaling_offset().at(0), 
					skeleton.scaling_offset().at(1), 
					skeleton.scaling_offset().at(2)));
			}
			
			{
				fbx_skeleton_node->ScalingPivot.Set(FbxDouble3(
					skeleton.scaling_pivot().at(0), 
					skeleton.scaling_pivot().at(1), 
					skeleton.scaling_pivot().at(2)));
			}
			
			{
				fbx_skeleton_node->GeometricTranslation.Set(FbxDouble3(
					skeleton.geometric_translation().at(0), 
					skeleton.geometric_translation().at(1), 
					skeleton.geometric_translation().at(2)));
			}
			
			{
				fbx_skeleton_node->GeometricRotation.Set(FbxDouble3(
					skeleton.geometric_rotation().at(0), 
					skeleton.geometric_rotation().at(1), 
					skeleton.geometric_rotation().at(2)));
			}
			
			{
				fbx_skeleton_node->GeometricScaling.Set(FbxDouble3(
					skeleton.geometric_scaling().at(0), 
					skeleton.geometric_scaling().at(1), 
					skeleton.geometric_scaling().at(2)));
			}

			fbx_skeleton->LimbLength.Set(FbxDouble(
				skeleton.limb_length()));
	
			fbx_skeleton_node_map[skeleton.id()] = fbx_skeleton_node;
		}
	}

	// export skeleton to scene
	{
		UMSkeleton::IDToSkeletonMap::const_iterator it = object->skeleton_map().begin();
		for (; it != object->skeleton_map().end(); ++it)
		{
			UMSkeleton skeleton = it->second;
			FbxNode* skeleton_node = fbx_skeleton_node_map[skeleton.id()];

			// find root node
			int parent_id = static_cast<int>(skeleton.parent_id());
			if (fbx_skeleton_node_map.find(parent_id) == fbx_skeleton_node_map.end())
			{
				scene->GetRootNode()->AddChild(skeleton_node);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["root_skeleton_name"] = skeleton.name().c_str();
//		printfm("root node found", UM.root_skeleton_name);
//#endif
			}
			else
			{
				FbxNode* parent_node = fbx_skeleton_node_map[parent_id];
				parent_node->AddChild(skeleton_node);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["parent_skeleton_name"] = parent_node->GetName();
//		UM["skeleton_name"] = skeleton.name().c_str();
//		UM["parent_id"] = parent_id;
//		printfm("parent found", UM.parent_skeleton_name, UM.skeleton_name, UM.parent_id);
//#endif
			}
		}
	}
	return true;
}

/**
 * export material to mesh
 */
bool UMFbxSaveImpl::export_materials(FbxNode* fbx_node, const UMMesh& mesh)
{
	if (!fbx_node) return false;

	const int material_count = static_cast<int>(mesh.material_list().size());
	if (!mesh.material_list().empty())
	{
		for (int i = 0; i < material_count; ++i)
		{
			const UMMaterial& material = mesh.material_list().at(i);
			if (FbxSurfacePhong* phong = FbxSurfacePhong::Create(manager(), material.name().c_str()))
			{
				{
					FbxDouble3 diffuse(
						material.diffuse().at(0),
						material.diffuse().at(1),
						material.diffuse().at(2));
					phong->Diffuse.Set(diffuse);
				}
				phong->DiffuseFactor.Set(material.diffuse_factor());

				{
					FbxDouble3 specular(
						material.specular().at(0),
						material.specular().at(1),
						material.specular().at(2));
					phong->Specular.Set(specular);
				}
				phong->SpecularFactor.Set(material.specular_factor());

				{
					FbxDouble3 ambient(
						material.ambient().at(0),
						material.ambient().at(1),
						material.ambient().at(2));
					phong->Ambient.Set(ambient);
				}
				phong->AmbientFactor.Set(material.ambient_factor());

				{
					FbxDouble3 emissive(
						material.emissive().at(0),
						material.emissive().at(1),
						material.emissive().at(2));
					phong->Emissive.Set(emissive);
				}
				phong->EmissiveFactor.Set(material.emissive_factor());
				
				{
					FbxDouble3 transparent(
						material.transparent().at(0),
						material.transparent().at(1),
						material.transparent().at(2));
					phong->TransparentColor.Set(transparent);
				}
				phong->TransparencyFactor.Set(material.transparency_factor());
				
				{
					FbxDouble3 refrection(
						material.refrection().at(0),
						material.refrection().at(1),
						material.refrection().at(2));
					phong->Reflection.Set(refrection);
				}
				phong->ReflectionFactor.Set(material.reflection_factor());

				{
					FbxDouble3 bump(
						material.bump().at(0),
						material.bump().at(1),
						material.bump().at(2));
					phong->Bump.Set(bump);
				}
				phong->BumpFactor.Set(material.bump_factor());
				
				{
					FbxDouble3 vector_displacement(
						material.vector_displacement().at(0),
						material.vector_displacement().at(1),
						material.vector_displacement().at(2));
					phong->VectorDisplacementColor.Set(vector_displacement);
				}
				phong->VectorDisplacementFactor.Set(material.vector_displacement_factor());
				
				phong->Shininess.Set(material.shininess());
				

				// texture
				UMMaterial::TextureList::const_iterator it = material.texture_list().begin();
				for (; it != material.texture_list().end(); ++it)
				{
					const UMTexture& texture = (*it);
					if (FbxFileTexture* fbx_texture = FbxFileTexture::Create(manager(), ""))
					{
						fbx_texture->SetFileName(texture.file_name().c_str());
						fbx_texture->SetTextureUse(FbxTexture::eStandard);
						fbx_texture->SetMappingType(FbxTexture::eUV);
						fbx_texture->SetMaterialUse(FbxFileTexture::eModelMaterial);
						fbx_texture->SetSwapUV(false);
						fbx_texture->SetTranslation(0.0, 0.0);
						fbx_texture->SetScale(1.0, 1.0);
						fbx_texture->SetRotation(0.0, 0.0);
						phong->Diffuse.ConnectSrcObject(fbx_texture);
					}
				}

				fbx_node->AddMaterial(phong);
			}
		}
	}
	return true;
}

/**
 * export bind pose to scene
 */
bool UMFbxSaveImpl::export_bind_pose(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh)
{
	if (!scene) return false;
	if (!object) return false;
	if (!fbx_node) return false;

	FbxMesh *fbx_mesh = FbxCast<FbxMesh>(fbx_node->GetNodeAttribute());
	if (!fbx_mesh) return false;

	if (FbxPose* pose = FbxPose::Create(scene, ""))
	{
		pose->SetIsBindPose(true);

		const int skin_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int i = 0; i < skin_count; ++i)
		{
			if (FbxSkin* fbx_skin = FbxCast<FbxSkin>(fbx_mesh->GetDeformer(i, FbxDeformer::eSkin)))
			{
				const int cluster_count = fbx_skin->GetClusterCount();
				for (int k = 0; k < cluster_count; ++k)
				{
					if (FbxCluster* cluster = fbx_skin->GetCluster(k))
					{
						if (FbxNode* node = cluster->GetLink())
						{
							if (FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator())
							{
								const FbxAMatrix transform = evaluator->GetNodeGlobalTransform(node);
								pose->Add(node, transform, false, true);
							}
						}
					}
				}
			}
		}

		scene->AddPose(pose);
	}

	return true;
}

/**
 * export skin to scene
 */
bool UMFbxSaveImpl::export_skins(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh)
{
	if (!scene) return false;
	if (!object) return false;
	if (!fbx_node) return false;
	
	FbxMesh *fbx_mesh = FbxCast<FbxMesh>(fbx_node->GetNodeAttribute());
	if (!fbx_mesh) return false;

	// create all skin
	const int skin_count = static_cast<int>(mesh.skin_list().size());
	for (int i = 0; i < skin_count; ++i)
	{
		const UMSkin& skin = mesh.skin_list().at(i);
		if (FbxSkin *fbx_skin = FbxSkin::Create(scene, skin.name().c_str()))
		{
			const int cluster_count = static_cast<int>(skin.cluster_list().size());
			for (int k = 0; k < cluster_count; ++k)
			{
				const UMCluster& cluster = skin.cluster_list().at(k);
				if (FbxCluster* fbx_cluster = FbxCluster::Create(manager(), cluster.name().c_str()))
				{
					// link mode
					int link_mode = cluster.link_mode();
					if (link_mode >= 0) 
					{
						fbx_cluster->SetLinkMode(static_cast<FbxCluster::ELinkMode>(link_mode));
					}
					// link
					if (const UMNode* link_node = cluster.link_node()) 
					{
						if (FbxNode* fbx_link_node = scene->FindNodeByName(link_node->name().c_str()))
						{
							fbx_cluster->SetLink(fbx_link_node);
							// transform
							if (FbxAnimEvaluator* evaluator = scene->GetAnimationEvaluator())
							{
								const FbxAMatrix transform = evaluator->GetNodeGlobalTransform(fbx_node);
								fbx_cluster->SetTransformMatrix(transform);
								const FbxAMatrix transform_link = evaluator->GetNodeGlobalTransform(fbx_link_node);
								fbx_cluster->SetTransformLinkMatrix(transform_link);
							}
						}
					}

					// index, weight
					const int index_size = static_cast<int>(cluster.index_list().size());
					if (index_size == cluster.weight_list().size())
					{
						for (int n = 0; n < index_size; ++n)
						{
							int index = cluster.index_list().at(n);
							double weight = cluster.weight_list().at(n);
							fbx_cluster->AddControlPointIndex(index, weight);
						}
					}
					
					fbx_skin->AddCluster(fbx_cluster);
				}
			}
			fbx_mesh->AddDeformer(fbx_skin);
		}
	}
	return true;
}

/**
 * export shape to fbx shape
 */
bool UMFbxSaveImpl::export_shape(FbxShape* fbx_shape, const UMShape& shape)
{
	fbx_shape->SetName(shape.name().c_str());

	// vertex index
	const int vertex_index_count = static_cast<int>(shape.vertex_index_list().size());
	for (int i = 0; i < vertex_index_count; ++i)
	{
		int index = shape.vertex_index_list().at(i);
		fbx_shape->AddControlPointIndex(index);
	}

	// vertex
	const int vertex_count = static_cast<int>(shape.vertex_list().size());
	fbx_shape->InitControlPoints(vertex_count);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["export_shape_verts"] = vertex_count;
//		printfm("export_shape_verts:", UM.export_shape_verts);
//#endif // WITH_PYTHON
	for (int i = 0; i < vertex_count; ++i)
	{
		const DoubleList& vertex = shape.vertex_list().at(i);
		if (static_cast<int>(vertex.size()) < 3) continue;
		FbxVector4 v(
			vertex.at(0), 
			vertex.at(1), 
			vertex.at(2), 
			1.0);
		fbx_shape->SetControlPointAt(v, i);
	}

	// normal
	const int normal_count = static_cast<int>(shape.normal_list().size());
	const int normal_index_count = static_cast<int>(shape.normal_index_list().size());
	if (normal_count > 0)
	{
		if (FbxGeometryElementNormal* fbx_normal = fbx_shape->CreateElementNormal())
		{
			if (normal_index_count > 0)
			{
				fbx_normal->SetMappingMode(FbxGeometryElement::eByControlPoint);
				fbx_normal->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
				for (int i = 0; i < normal_index_count; ++i)
				{
					int index = shape.normal_index_list().at(i);
					fbx_normal->GetIndexArray().Add(index);
				}
			}
			else
			{
				fbx_normal->SetMappingMode(FbxGeometryElement::eByControlPoint);
				fbx_normal->SetReferenceMode(FbxGeometryElement::eDirect);
			}
			
			for (int i = 0; i < normal_count; ++i)
			{
				const DoubleList& normal = shape.normal_list().at(i);
				FbxVector4 n(
					normal.at(0), 
					normal.at(1), 
					normal.at(2),
					1.0);
				fbx_normal->GetDirectArray().Add(n);
			}
		}
	}

	return true;
}

/**
 * export blend shape to scene
 */
bool UMFbxSaveImpl::export_blend_shapes(FbxScene* scene, UMObjectPtr object, FbxNode* fbx_node, const UMMesh& mesh)
{
	if (!scene) return false;
	if (!object) return false;
	if (!fbx_node) return false;
	
	FbxMesh *fbx_mesh = FbxCast<FbxMesh>(fbx_node->GetNodeAttribute());
	if (!fbx_mesh) return false;

	// create all blend shapes
	const int blend_shape_count = static_cast<int>(mesh.blend_shape_list().size());
	for (int i = 0; i < blend_shape_count; ++i)
	{
		const UMBlendShape& blend_shape = mesh.blend_shape_list().at(i);
		if (FbxBlendShape* fbx_blend_shape = FbxBlendShape::Create(scene, blend_shape.name().c_str()))
		{
			printf("add blend shape");
			fbx_mesh->AddDeformer(fbx_blend_shape);

			const int channel_count = static_cast<int>(blend_shape.blend_shape_channel_list().size());
			for (int k = 0; k < channel_count; ++k)
			{
				const UMBlendShapeChannel& channel = blend_shape.blend_shape_channel_list().at(k);
				if (FbxBlendShapeChannel* fbx_channel = FbxBlendShapeChannel::Create(scene, "channel"))
				{
					//fbx_channel->SetBlendShapeDeformer(fbx_blend_shape);
					printf("add blend shape channel");
					fbx_blend_shape->AddBlendShapeChannel(fbx_channel);

					const int shape_count = static_cast<int>(channel.target_shape_list().size());
					for (int n = 0; n < shape_count; ++n)
					{
						const UMShape& shape = channel.target_shape_list().at(n);
						if (FbxShape* fbx_shape = FbxShape::Create(manager(), shape.name().c_str()))
						{
							export_shape(fbx_shape, shape);
							fbx_channel->AddTargetShape(fbx_shape);
							//fbx_mesh->AddShape(i, k, fbx_shape, 0.0);

							int shapes = fbx_mesh->GetShapeCount();
							printf("add shape");
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["shape_count"] = shapes;
//		printfm("current shape count", UM.shape_count);
//#endif
						}
					}
				}
				if (is_old_fbx())
				{
					break;
				}
			}
		}
	}
	return true;
}

/**
 * export mesh to scene
 */
bool UMFbxSaveImpl::export_mesh(FbxScene* scene, UMObjectPtr object)
{
	if (!scene) return false;
	if (!object) return false;

	// create all mesh
	std::map<int, FbxNode*> fbx_mesh_node_map;
	{
		UMMesh::IDToMeshMap::const_iterator it = object->mesh_map().begin();
		for (; it != object->mesh_map().end(); ++it)
		{
			const UMMesh& mesh = it->second;
			FbxMesh * fbx_mesh = FbxMesh::Create(manager(), mesh.name().c_str());
			if (!fbx_mesh) continue;
			FbxNode* fbx_mesh_node = FbxNode::Create(manager(), mesh.name().c_str());
			if (!fbx_mesh_node) continue;

			fbx_mesh_node->SetNodeAttribute(fbx_mesh);
			
			// vertex
			const int vertex_count = static_cast<int>(mesh.vertex_list().size());
			{
				fbx_mesh->InitControlPoints(vertex_count);
				for (int i = 0; i < vertex_count; ++i)
				{
					const DoubleList& vertex = mesh.vertex_list().at(i);
					if (static_cast<int>(vertex.size()) < 3) continue;
					FbxVector4 vertex_point(vertex.at(0), vertex.at(1), vertex.at(2), 1.0);
					fbx_mesh->SetControlPointAt(vertex_point, i);
				}
			}
			
			// vertex index
			const int vertex_index_count = static_cast<int>(mesh.vertex_index_list().size());
			if (!mesh.vertex_index_list().empty())
			{
				for (int i = 0; i < vertex_index_count; ++i)
				{
					const IntList& index_list = mesh.vertex_index_list().at(i);
					fbx_mesh->BeginPolygon(-1, -1, -1, false);
					IntList::const_iterator index_itr = index_list.begin();
					for (; index_itr != index_list.end(); ++index_itr)
					{
						fbx_mesh->AddPolygon((*index_itr));
					}
					fbx_mesh->EndPolygon();
				}
			}

			// material
			export_materials(fbx_mesh_node, mesh);
			
			FbxLayer* layer = fbx_mesh->GetLayer(0);
			if (!layer) {
				int index = fbx_mesh->CreateLayer();
				layer = fbx_mesh->GetLayer(index);
			}
			if (!layer) continue;

			// normal
			const int normal_count = static_cast<int>(mesh.normal_list().size());
			if (!mesh.normal_list().empty())
			{
				if (FbxLayerElementNormal* fbx_normal = FbxLayerElementNormal::Create(fbx_mesh, ""))
				{
					if (normal_count > vertex_count)
					{
						fbx_normal->SetMappingMode(FbxLayerElement::eByPolygonVertex);
					}
					else
					{
						fbx_normal->SetMappingMode(FbxLayerElement::eByControlPoint);
					}
					fbx_normal->SetReferenceMode(FbxLayerElement::eDirect);
					for (int i = 0; i < normal_count; ++i)
					{
						const DoubleList& normal = mesh.normal_list().at(i);
						if (static_cast<int>(normal.size()) < 3) continue;
						FbxVector4 normal_point(normal.at(0), normal.at(1), normal.at(2), 1.0);
						fbx_normal->GetDirectArray().Add(normal_point);
					}
					layer->SetNormals(fbx_normal);
				}
			}

			// uv
			const int uv_count = static_cast<int>(mesh.uv_list().size());
			if (!mesh.uv_list().empty())
			{
				if (FbxLayerElementUV* fbx_uv = FbxLayerElementUV::Create(fbx_mesh, ""))
				{
					fbx_uv->SetMappingMode(FbxLayerElement::eByPolygonVertex);
					fbx_uv->SetReferenceMode(FbxLayerElement::eIndexToDirect);
					for (int i = 0; i < uv_count; ++i)
					{
						const DoubleList& uv = mesh.uv_list().at(i);
						if (static_cast<int>(uv.size()) < 2) continue;
						FbxVector2 fbx_vertex_uv(uv.at(0), uv.at(1));
						fbx_uv->GetDirectArray().Add(fbx_vertex_uv);
						fbx_uv->GetIndexArray().Add(i);
					}
					layer->SetUVs(fbx_uv);
				}
			}

			// material index
			const int material_index_count = static_cast<int>(mesh.material_index_list().size());
			if (!mesh.material_index_list().empty())
			{
				if (FbxLayerElementMaterial* fbx_material = FbxLayerElementMaterial::Create(fbx_mesh, ""))
				{
					fbx_material->SetMappingMode(FbxLayerElement::eByPolygon);
					fbx_material->SetReferenceMode(FbxLayerElement::eIndexToDirect);
					for (int i = 0; i < material_index_count; ++i)
					{
						int material_index = mesh.material_index_list().at(i);
						fbx_material->GetIndexArray().Add(material_index);
					}
					layer->SetMaterials(fbx_material);
				}
			}
			
			// node
			{
				fbx_mesh_node->LclTranslation.Set(FbxDouble3(
					mesh.local_translation().at(0), 
					mesh.local_translation().at(1), 
					mesh.local_translation().at(2)));
			}
		
			{
				fbx_mesh_node->LclRotation.Set(FbxDouble3(
					mesh.local_rotation().at(0), 
					mesh.local_rotation().at(1), 
					mesh.local_rotation().at(2)));
			}
			
			{
				fbx_mesh_node->LclScaling.Set(FbxDouble3(
					mesh.local_scaling().at(0), 
					mesh.local_scaling().at(1), 
					mesh.local_scaling().at(2)));
			}
			
			fbx_mesh_node_map[mesh.id()] = fbx_mesh_node;
		}
	}
	
	// export mesh to scene
	{
		UMMesh::IDToMeshMap::const_iterator it = object->mesh_map().begin();
		for (; it != object->mesh_map().end(); ++it)
		{
			const UMMesh& mesh = it->second;
			FbxNode* mesh_node = fbx_mesh_node_map[mesh.id()];
			
			// find root node
			int parent_id = mesh.parent_id();
			if (fbx_mesh_node_map.find(parent_id) == fbx_mesh_node_map.end())
			{
				scene->GetRootNode()->AddChild(mesh_node);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["root_mesh_name"] = mesh.name().c_str();
//		printfm("root mesh node found", UM.root_mesh_name);
//#endif
			}
			else
			{
				FbxNode* parent_node = fbx_mesh_node_map[parent_id];
				parent_node->AddChild(mesh_node);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["parent_mesh_name"] = parent_node->GetName();
//		UM["mesh_name"] = mesh.name().c_str();
//		UM["parent_id"] = parent_id;
//		printfm("parent found", UM.parent_mesh_name, UM.mesh_name, UM.parent_id);
//#endif
			}
		}
	}
	
	// export mesh blendshape
	{
		UMMesh::IDToMeshMap::const_iterator it = object->mesh_map().begin();
		for (; it != object->mesh_map().end(); ++it)
		{
			const UMMesh& mesh = it->second;
			FbxNode* mesh_node = fbx_mesh_node_map[mesh.id()];
			export_blend_shapes(scene, object, mesh_node, mesh);
		}
	}

	// export mesh skin
	{
		UMMesh::IDToMeshMap::const_iterator it = object->mesh_map().begin();
		for (; it != object->mesh_map().end(); ++it)
		{
			const UMMesh& mesh = it->second;
			FbxNode* mesh_node = fbx_mesh_node_map[mesh.id()];
			if (export_skins(scene, object, mesh_node, mesh))
			{
				//export_bind_pose(scene, object, mesh_node, mesh);
			}
		}
	}

	return true;
}

/**
 * create scene from UMObject
 */
FbxScene* UMFbxSaveImpl::create_scene(UMObjectPtr object)
{
	FbxScene* scene = FbxScene::Create(manager(),"");
	if (!scene) return NULL;

	FbxDocumentInfo* document_info = FbxDocumentInfo::Create(manager(),"SceneInfo");
	if (!document_info) return NULL;

	// TODO
	document_info->mTitle = "title";
	document_info->mSubject = "subject";
	document_info->mAuthor = "author";
	document_info->mRevision = "revision";
	document_info->mKeywords = "keyword";
	document_info->mComment = "comment";
	scene->SetSceneInfo(document_info);

	if (!export_skeleton(scene, object))
	{
		printf("export skeleton failed");
	}
	if (!export_mesh(scene, object))
	{
		printf("export mesh failed");
	}

	return scene;
}

/**
 * save fbx file to UMObject
 */
bool UMFbxSaveImpl::save(std::string path, UMObjectPtr object, const UMIOSetting& setting)
{
	if (!object) return false;

	// Create a manager
	manager_ = FbxManager::Create();
	if (!manager_) return false;
	FbxManager* manager = manager_;

	// init manager
	FbxIOSettings * ios = FbxIOSettings::Create(manager, IOSROOT );
	manager->SetIOSettings(ios);
	
	// fbx sdk version
	{
		int sdk_major;
		int sdk_minor;
		int sdk_revision;
		FbxManager::GetFileFormatVersion(sdk_major, sdk_minor, sdk_revision);
//#ifdef WITH_PYTHON
//		boost::python::object UM = boost::python::import("UM").attr("__dict__");
//		UM["sdk_major"] = sdk_major;
//		UM["sdk_minor"] = sdk_minor;
//		UM["sdk_revision"] = sdk_revision;
//		printfm("FBX version number for this FBX SDK is ", UM.sdk_major, UM.sdk_minor, UM.sdk_revision);
//#endif // WITH_PYTHON
	}
		
	// Create an exporter.
	exporter_ = FbxExporter::Create(manager,"");
	if (!exporter_) {
		printf("FbxExporter::Create failed.\n");
		return false;
	}
	FbxExporter* exporter = exporter_;
	
	FbxIOSettings& fbx_settings = (*manager->GetIOSettings());
	set_save_setting(fbx_settings, setting);
	
//	for (int i = 0; i < manager->GetIOPluginRegistry()->GetWriterFormatCount(); ++i)
//	{
//		if (manager->GetIOPluginRegistry()->WriterIsFBX(i))
//		{
//			FbxString desc = manager->GetIOPluginRegistry()->GetWriterFormatDescription(i);
//		}
//	}
	
	// init exporter
	if (is_save_text())
	{
		if (is_old_fbx())
		{
			// old fbx, text
			if (!exporter->Initialize(path.c_str(), 4, manager->GetIOSettings())) {
				printf("Call to FbxExporter::Initialize() failed.\n");
				return false;
			}
		}
		else
		{
			// newest fbx, text
			if (!exporter->Initialize(path.c_str(), 1, manager->GetIOSettings())) {
				printf("Call to FbxExporter::Initialize() failed.\n");
				return false;
			}
		}
	}
	else
	{
		if (is_old_fbx())
		{
			// old fbx, binary
			if (!exporter->Initialize(path.c_str(), 3, manager->GetIOSettings())) {
				printf("Call to FbxExporter::Initialize() failed.\n");
				return false;
			}
		}
		else
		{
			// newest fbx, binary
			if (!exporter->Initialize(path.c_str(), 0, manager->GetIOSettings())) {
				printf("Call to FbxExporter::Initialize() failed.\n");
				return false;
			}
		}
	}

	FbxScene* scene = create_scene(object);
	set_system_unit_type(scene, setting);
	set_axis_type(scene, setting);
	if (!scene) {
		printf("create_scene failed.\n");
		return false;
	}

	// Import the scene.
	if (!exporter->Export(scene)) {
		printf("export failed.\n");
		return false;
	}

	printf("export success.\n");

	return true;
}


/*---------------------------------------------------------------------------*
 * UMFbx
 *---------------------------------------------------------------------------*/

/**
 * load fbx file to UMObject
 */
UMObjectPtr UMFbx::load(std::string path, const UMIOSetting& setting)
{
	UMFbxLoadImpl impl;
	return impl.load(path, setting);
}
/**
 * save fbx file
 */
bool UMFbx::save(std::string path, UMObjectPtr object, const UMIOSetting& setting)
{
	if (!UMObject::re_bind_all_nodes(object)) {
		printf("warning: bind node failed. check node's id, parent_id.");
	}
	UMFbxSaveImpl impl;
	return impl.save(path, object, setting);
}

} // namespace umio

#endif //WITH_FBX2015
