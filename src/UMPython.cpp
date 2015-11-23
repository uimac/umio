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
#ifdef WITH_PYTHON

#include <stdio.h>
#include <iostream>
#include <string>

#include "UMIO.h"
#include "UMObject.h"
#include "UMAnimation.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>);
namespace umio
{

// for UMIO::load_from_memory
//static umio::UMObjectPtr load_from_memory_wrap(
//	UMIO& self, 
//	pybind11::object memory_view)
//{
//	PyObject* py_obj = memory_view.ptr();
//	Py_buffer py_buffer = {};
//	PyObject_GetBuffer(py_obj, &py_buffer, PyBUF_SIMPLE);
//	std::string buffer((char*)py_buffer.buf, py_buffer.len);
//	return self.load_from_memory(buffer, umio::UMIOSetting());
//}
//
// for UMIO::save_to_memory
//static pybind11::object save_to_memory_wrap(
//	UMIO& self, 
//	UMObjectPtr object, 
//	const UMIOSetting& setting)
//{
//	int buffer_size = 0;
//	char* buffer = self.save_to_memory(buffer_size, object, setting);
//	if (buffer && buffer_size > 0)
//	{
//		pybind11::object memory_view(pybind11::handle<>(PyMemoryView_FromMemory(buffer, buffer_size, PyBUF_READ)));
//		return memory_view;
//	}
//	return boost::python::object();
//}

UMObjectPtr create_object() {
	return UMObjectPtr(new UMObject());
}

PYBIND11_PLUGIN(UMIO)
{
	using namespace pybind11;

	pybind11::module m("UMIO", "UMIO module");
	
	class_<UMIOSetting> iosetting(m, "UMIOSetting");
		// setter
	iosetting.def(init<>())
		.def("set_exp_bool_prop", &UMIOSetting::set_exp_bool_prop)
		.def("set_imp_bool_prop", &UMIOSetting::set_imp_bool_prop)
		.def("set_bl_exp_bool_prop", &UMIOSetting::set_bl_exp_bool_prop)
		.def("set_bl_imp_bool_prop", &UMIOSetting::set_bl_imp_bool_prop)
		.def("set_axis_type", &UMIOSetting::set_axis_type)
		.def("set_system_unit_type", &UMIOSetting::set_system_unit_type)
		;

	enum_<UMIOSetting::EExpSettingType>(iosetting, "ExpSettingType")
		.value("ExpColladaTriangulate", UMIOSetting::eExpColladaTriangulate)
		.value("ExpColladaSingleMatrix", UMIOSetting::eExpColladaSingleMatrix)
		.value("ExpColladaFrameRate", UMIOSetting::eExpColladaFrameRate)
		.value("ExpDxfTriangulate", UMIOSetting::eExpDxfTriangulate)
		.value("ExpDxfDeformation", UMIOSetting::eExpDxfDeformation)
		.value("ExpObjTriangulate", UMIOSetting::eExpObjTriangulate)
		.value("ExpObjDeformation", UMIOSetting::eExpObjDeformation)
		.value("Exp3dsReferenceNode", UMIOSetting::eExp3dsReferenceNode)
		.value("Exp3dsTexture", UMIOSetting::eExp3dsTexture)
		.value("Exp3dsMaterial", UMIOSetting::eExp3dsMaterial)
		.value("Exp3dsAnimation", UMIOSetting::eExp3dsAnimation)
		.value("Exp3dsMesh", UMIOSetting::eExp3dsMesh)
		.value("Exp3dsLight", UMIOSetting::eExp3dsLight)
		.value("Exp3dsCamera", UMIOSetting::eExp3dsCamera)
		.value("Exp3dsAmbientLight", UMIOSetting::eExp3dsAmbientLight)
		.value("Exp3dsRescaling", UMIOSetting::eExp3dsRescaling)
		.value("Exp3dsTexuvbypoly", UMIOSetting::eExp3dsTexuvbypoly)
		.value("ExpFbxTemplate", UMIOSetting::eExpFbxTemplate)
		.value("ExpFbxPivot", UMIOSetting::eExpFbxPivot)
		.value("ExpFbxGlobalSettings", UMIOSetting::eExpFbxGlobalSettings)
		.value("ExpFbxCharacter", UMIOSetting::eExpFbxCharacter)
		.value("ExpFbxConstraint", UMIOSetting::eExpFbxConstraint)
		.value("ExpFbxGobo", UMIOSetting::eExpFbxGobo)
		.value("ExpFbxShape", UMIOSetting::eExpFbxShape)
		.value("ExpFbxMaterial", UMIOSetting::eExpFbxMaterial)
		.value("ExpFbxTexture", UMIOSetting::eExpFbxTexture)
		.value("ExpFbxModel", UMIOSetting::eExpFbxModel)
		.value("ExpFbxAnimation", UMIOSetting::eExpFbxAnimation)
		.value("ExpFbxEmbedded", UMIOSetting::eExpFbxEmbedded)
		.value("ExpFbxPassword", UMIOSetting::eExpFbxPassword)
		.value("ExpFbxPasswordEnable", UMIOSetting::eExpFbxPasswordEnable)
		.value("ExpFbxCollapseExternals", UMIOSetting::eExpFbxCollapseExternals)
		.value("ExpFbxCompressArrays", UMIOSetting::eExpFbxCompressArrays)
		.value("ExpFbxCompressLevel", UMIOSetting::eExpFbxCompressLevel)
		.value("ExpFbxCompressMinsize", UMIOSetting::eExpFbxCompressMinsize)
		.value("ExpFbxEmbeddedPropertiesSkipExpFbx", UMIOSetting::eExpFbxEmbeddedPropertiesSkipExpFbx)
		.value("ExpFbxExportFileVersion", UMIOSetting::eExpFbxExportFileVersion)
		.export_values()
		;

	enum_<UMIOSetting::EImpSettingType>(iosetting, "ImpSettingType")
		.value("ImpDxfWeldVertices", UMIOSetting::eImpDxfWeldVertices)
		.value("ImpDxfObjectDerivation", UMIOSetting::eImpDxfObjectDerivation)
		.value("ImpDxfReferenceNode", UMIOSetting::eImpDxfReferenceNode)
		.value("ImpObjReferenceNode", UMIOSetting::eImpObjReferenceNode)
		.value("Imp3dsReferenceNode", UMIOSetting::eImp3dsReferenceNode)
		.value("Imp3dsTexture", UMIOSetting::eImp3dsTexture)
		.value("Imp3dsMaterial", UMIOSetting::eImp3dsMaterial)
		.value("Imp3dsAnimation", UMIOSetting::eImp3dsAnimation)
		.value("Imp3dsMesh", UMIOSetting::eImp3dsMesh)
		.value("Imp3dsLight", UMIOSetting::eImp3dsLight)
		.value("Imp3dsCamera", UMIOSetting::eImp3dsCamera)
		.value("Imp3dsAmbientLight", UMIOSetting::eImp3dsAmbientLight)
		.value("Imp3dsRescaling", UMIOSetting::eImp3dsRescaling)
		.value("Imp3dsFilter", UMIOSetting::eImp3dsFilter)
		.value("Imp3dsSmoothgroup", UMIOSetting::eImp3dsSmoothgroup)
		.value("ImpFbxModelCount", UMIOSetting::eImpFbxModelCount)
		.value("ImpFbxDeviceCount", UMIOSetting::eImpFbxDeviceCount)
		.value("ImpFbxCharacterCount", UMIOSetting::eImpFbxCharacterCount)
		.value("ImpFbxActorCount", UMIOSetting::eImpFbxActorCount)
		.value("ImpFbxConstraintCount", UMIOSetting::eImpFbxConstraintCount)
		.value("ImpFbxMediaCount", UMIOSetting::eImpFbxMediaCount)
		.value("ImpFbxTemplate", UMIOSetting::eImpFbxTemplate)
		.value("ImpFbxPivot", UMIOSetting::eImpFbxPivot)
		.value("ImpFbxGlobalSettings", UMIOSetting::eImpFbxGlobalSettings)
		.value("ImpFbxCharacter", UMIOSetting::eImpFbxCharacter)
		.value("ImpFbxConstraint", UMIOSetting::eImpFbxConstraint)
		.value("ImpFbxMergeLayerAndTimewarp", UMIOSetting::eImpFbxMergeLayerAndTimewarp)
		.value("ImpFbxGobo", UMIOSetting::eImpFbxGobo)
		.value("ImpFbxShape", UMIOSetting::eImpFbxShape)
		.value("ImpFbxLink", UMIOSetting::eImpFbxLink)
		.value("ImpFbxMaterial", UMIOSetting::eImpFbxMaterial)
		.value("ImpFbxTexture", UMIOSetting::eImpFbxTexture)
		.value("ImpFbxModel", UMIOSetting::eImpFbxModel)
		.value("ImpFbxAnimation", UMIOSetting::eImpFbxAnimation)
		.value("ImpFbxPassword", UMIOSetting::eImpFbxPassword)
		.value("ImpFbxPasswordEnable", UMIOSetting::eImpFbxPasswordEnable)
		.value("ImpFbxCurrentTakeName", UMIOSetting::eImpFbxCurrentTakeName)
		.export_values()
		;

	enum_<UMIOSetting::EUMExpSettingType>(iosetting, "UMExpSettingType")
		.value("UMExpFBX", UMIOSetting::eUMExpFBX)
		.value("UMExpText", UMIOSetting::eUMExpText)
		.value("UMExpOldFBX", UMIOSetting::eUMExpOldFBX)
		.export_values()
		;

	enum_<UMIOSetting::EUMImpSettingType>(iosetting, "UMImpSettingType")
		.value("UMImpFBX", UMIOSetting::eUMImpFBX)
		.value("UMImpTriangulate", UMIOSetting::eUMImpTriangulate)
		.value("UMImpNurbs", UMIOSetting::eUMImpNurbs)
		.value("UMImpPatch", UMIOSetting::eUMImpPatch)
		.export_values()
		;

	enum_<UMIOSetting::EFbxAxisType>(iosetting, "AxisType")
		.value("AxisMayaZUp", UMIOSetting::eFbxAxisMayaZUp)
		.value("AxisMayaYUp", UMIOSetting::eFbxAxisMayaYUp)
		.value("AxisMax", UMIOSetting::eFbxAxisMax)
		.value("AxisMotionBuilder", UMIOSetting::eFbxAxisMotionBuilder)
		.value("AxisOpenGL", UMIOSetting::eFbxAxisOpenGL)
		.value("AxisDirectX", UMIOSetting::eFbxAxisDirectX)
		.value("AxisLightWave", UMIOSetting::eFbxAxisLightWave)
		.export_values()
		;

	enum_<UMIOSetting::EFbxSystemUnitType>(iosetting, "SystemUnitType")
		.value("SystemUnitMM", UMIOSetting::eFbxSystemUnitMM)
		.value("SystemUnitDM", UMIOSetting::eFbxSystemUnitDM)
		.value("SystemUnitCM", UMIOSetting::eFbxSystemUnitCM)
		.value("SystemUnitM", UMIOSetting::eFbxSystemUnitM)
		.value("SystemUnitKM", UMIOSetting::eFbxSystemUnitKM)
		.value("SystemUnitInch", UMIOSetting::eFbxSystemUnitInch)
		.value("SystemUnitFoot", UMIOSetting::eFbxSystemUnitFoot)
		.value("SystemUnitMile", UMIOSetting::eFbxSystemUnitMile)
		.value("SystemUnitYard", UMIOSetting::eFbxSystemUnitYard)
		.export_values()
		;
	
	//
	// UMVec4d
	//
	class_<UMVec4d> vec4(m, "UMVec4d");
	// TODO
	//vec4.def_readwrite("x", &UMVec4d::x)
	//	.def_readwrite("y", &UMVec4d::y)
	//	.def_readwrite("z", &UMVec4d::z)
	//	.def_readwrite("w", &UMVec4d::w)
	//	;
	
	//
	// UMMat44d
	//
	class_<UMMat44d> mat44d(m, "UMMat44d");

	mat44d.def(init<>())
		.def("set", &UMMat44d::set)
		.def("get", &UMMat44d::get)
		;

	//
	// UMNode
	//
	class_<UMNode> umnode(m, "UMNode");
		
		// getter
	umnode.def(init<>())
		.def("id", &UMNode::id)
		.def("parent_id", &UMNode::parent_id)
		.def("parent", &UMNode::parent, return_value_policy::reference)
		.def("name", &UMNode::name)
		.def("local_translation", &UMNode::local_translation)
		.def("local_rotation", &UMNode::local_rotation)
		.def("local_scaling", &UMNode::local_scaling)
		.def("rotation_offset", &UMNode::rotation_offset)
		.def("rotation_pivot", &UMNode::rotation_pivot)
		.def("pre_rotation", &UMNode::pre_rotation)
		.def("post_rotation", &UMNode::post_rotation)
		.def("scaling_offset", &UMNode::scaling_offset)
		.def("scaling_pivot", &UMNode::scaling_pivot)
		.def("geometric_translation", &UMNode::geometric_translation)
		.def("geometric_rotation", &UMNode::geometric_rotation)
		.def("geometric_scaling", &UMNode::geometric_scaling)
		.def("local_transform", &UMNode::local_transform)
		.def("global_transform", &UMNode::global_transform)
		// setter
		.def("set_id", &UMNode::set_id)
		.def("set_parent_id", &UMNode::set_parent_id)
		.def("set_name", &UMNode::set_name)
		.def("set_local_translation", &UMNode::set_local_translation)
		.def("set_local_rotation", &UMNode::set_local_rotation)
		.def("set_local_scaling", &UMNode::set_local_scaling)
		.def("set_rotation_offset", &UMNode::set_rotation_offset)
		.def("set_rotation_pivot", &UMNode::set_rotation_pivot)
		.def("set_pre_rotation", &UMNode::set_pre_rotation)
		.def("set_post_rotation", &UMNode::set_post_rotation)
		.def("set_scaling_offset", &UMNode::set_scaling_offset)
		.def("set_scaling_pivot", &UMNode::set_scaling_pivot)
		.def("set_geometric_translation", &UMNode::set_geometric_translation)
		.def("set_geometric_rotation", &UMNode::set_geometric_rotation)
		.def("set_geometric_scaling", &UMNode::set_geometric_scaling)
		.def("set_local_transform", &UMNode::set_local_transform)
		.def("set_global_transform", &UMNode::set_global_transform)
		;

	//
	// UMCamera
	//
	{
		class_<UMCamera> camera(m, "UMCamera", umnode);
			// getter
		camera.def(init<>())
			.def("position", &UMCamera::position)
			.def("up_vector", &UMCamera::up_vector)
			.def("interest_position", &UMCamera::interest_position)
			.def("roll", &UMCamera::roll)
			.def("optical_center_x", &UMCamera::optical_center_x)
			.def("optical_center_y", &UMCamera::optical_center_y)
			.def("background_color", &UMCamera::background_color)
			.def("turn_table", &UMCamera::turn_table)
			.def("display_turn_table_icon", &UMCamera::display_turn_table_icon)
			.def("use_motion_blur", &UMCamera::use_motion_blur)
			.def("use_real_time_motion_blur", &UMCamera::use_real_time_motion_blur)
			.def("motion_blur_intensity", &UMCamera::motion_blur_intensity)
			.def("aspect_ratio_mode", &UMCamera::aspect_ratio_mode)
			.def("aspect_width", &UMCamera::aspect_width)
			.def("aspect_height", &UMCamera::aspect_height)
			.def("pixel_aspect_ration", &UMCamera::pixel_aspect_ration)
			.def("aperture_mode", &UMCamera::aperture_mode)
			.def("gate_fit", &UMCamera::gate_fit)
			.def("field_of_view", &UMCamera::field_of_view)
			.def("field_of_view_x", &UMCamera::field_of_view_x)
			.def("field_of_view_y", &UMCamera::field_of_view_y)
			.def("focal_length", &UMCamera::focal_length)
			.def("camera_format", &UMCamera::camera_format)
			.def("use_frame_color", &UMCamera::use_frame_color)
			.def("frame_color", &UMCamera::frame_color)
			.def("show_name", &UMCamera::show_name)
			.def("show_info_on_moving", &UMCamera::show_info_on_moving)
			.def("show_grid", &UMCamera::show_grid)
			.def("show_optical_center", &UMCamera::show_optical_center)
			.def("show_azimut", &UMCamera::show_azimut)
			.def("show_time_code", &UMCamera::show_time_code)
			.def("show_audio", &UMCamera::show_audio)
			.def("audio_color", &UMCamera::audio_color)
			.def("near_plane", &UMCamera::near_plane)
			.def("far_plane", &UMCamera::far_plane)
			.def("auto_compute_clip_planes", &UMCamera::auto_compute_clip_planes)
			.def("film_width", &UMCamera::film_width)
			.def("film_height", &UMCamera::film_height)
			.def("film_aspect_ration", &UMCamera::film_aspect_ration)
			.def("film_squeeze_ration", &UMCamera::film_squeeze_ration)
			.def("film_format", &UMCamera::film_format)
			.def("film_offset_x", &UMCamera::film_offset_x)
			.def("film_offset_y", &UMCamera::film_offset_y)
			.def("pre_scale", &UMCamera::pre_scale)
			.def("film_translate_x", &UMCamera::film_translate_x)
			.def("film_translate_y", &UMCamera::film_translate_y)
			.def("film_roll_pivot_x", &UMCamera::film_roll_pivot_x)
			.def("film_roll_pivot_y", &UMCamera::film_roll_pivot_y)
			.def("film_roll_value", &UMCamera::film_roll_value)
			.def("film_roll_order", &UMCamera::film_roll_order)
			.def("view_camera_to_look_at", &UMCamera::view_camera_to_look_at)
			.def("view_frustum_near_far_plane", &UMCamera::view_frustum_near_far_plane)
			.def("view_frustum_back_plane_mode", &UMCamera::view_frustum_back_plane_mode)
			.def("back_plane_distance", &UMCamera::back_plane_distance)
			.def("back_plane_distance_mode", &UMCamera::back_plane_distance_mode)
			.def("view_frustum_front_plane_mode", &UMCamera::view_frustum_front_plane_mode)
			.def("front_plane_distance", &UMCamera::front_plane_distance)
			.def("front_plane_distance_mode", &UMCamera::front_plane_distance_mode)
			.def("lock_mode", &UMCamera::lock_mode)
			.def("lock_interest_navigation", &UMCamera::lock_interest_navigation)
			.def("back_plate_fit_image", &UMCamera::back_plate_fit_image)
			.def("back_plate_crop", &UMCamera::back_plate_crop)
			.def("back_plate_center", &UMCamera::back_plate_center)
			.def("back_plate_keep_ration", &UMCamera::back_plate_keep_ration)
			.def("background_alpha_treshold", &UMCamera::background_alpha_treshold)
			.def("back_plane_offset_x", &UMCamera::back_plane_offset_x)
			.def("back_plane_offset_y", &UMCamera::back_plane_offset_y)
			.def("back_plane_rotation", &UMCamera::back_plane_rotation)
			.def("back_plane_scale_x", &UMCamera::back_plane_scale_x)
			.def("back_plane_scale_y", &UMCamera::back_plane_scale_y)
			.def("show_backplate", &UMCamera::show_backplate)
			.def("front_plate_fit_image", &UMCamera::front_plate_fit_image)
			.def("front_plate_crop", &UMCamera::front_plate_crop)
			.def("front_plate_center", &UMCamera::front_plate_center)
			.def("front_plate_keep_ratio", &UMCamera::front_plate_keep_ratio)
			.def("show_frontplate", &UMCamera::show_frontplate)
			.def("front_plane_offset_x", &UMCamera::front_plane_offset_x)
			.def("front_plane_offset_y", &UMCamera::front_plane_offset_y)
			.def("front_plane_rotation", &UMCamera::front_plane_rotation)
			.def("front_plane_scale_x", &UMCamera::front_plane_scale_x)
			.def("front_plane_scale_y", &UMCamera::front_plane_scale_y)
			.def("foreground_opacity", &UMCamera::foreground_opacity)
			.def("display_safe_area", &UMCamera::display_safe_area)
			.def("display_safe_area_on_render", &UMCamera::display_safe_area_on_render)
			.def("safe_area_display_style", &UMCamera::safe_area_display_style)
			.def("safe_area_display_ratio", &UMCamera::safe_area_display_ratio)
			.def("use_2d_magnifier_zoom", &UMCamera::use_2d_magnifier_zoom)
			.def("_2d_magnifier_zoom", &UMCamera::_2d_magnifier_zoom)
			.def("_2d_magnifier_x", &UMCamera::_2d_magnifier_x)
			.def("_2d_magnifier_y", &UMCamera::_2d_magnifier_y)
			.def("projection_type", &UMCamera::projection_type)
			.def("ortho_zoom", &UMCamera::ortho_zoom)
			.def("use_real_time_dof_and_aa", &UMCamera::use_real_time_dof_and_aa)
			.def("use_depth_of_field", &UMCamera::use_depth_of_field)
			.def("focus_source", &UMCamera::focus_source)
			.def("focus_angle", &UMCamera::focus_angle)
			.def("focus_distance", &UMCamera::focus_distance)
			.def("use_antialiasing", &UMCamera::use_antialiasing)
			.def("antialiasing_intensity", &UMCamera::antialiasing_intensity)
			.def("antialiasing_method", &UMCamera::antialiasing_method)
			.def("use_accumulation_buffer", &UMCamera::use_accumulation_buffer)
			.def("frame_sample_count", &UMCamera::frame_sample_count)
			.def("frame_sampling_type", &UMCamera::frame_sampling_type)

			// setter
			.def("set_position", &UMCamera::set_position)
			.def("set_up_vector", &UMCamera::set_up_vector)
			.def("set_interest_position", &UMCamera::set_interest_position)
			.def("set_roll", &UMCamera::set_roll)
			.def("set_optical_center_x", &UMCamera::set_optical_center_x)
			.def("set_optical_center_y", &UMCamera::set_optical_center_y)
			.def("set_background_color", &UMCamera::set_background_color)
			.def("set_turn_table", &UMCamera::set_turn_table)
			.def("set_display_turn_table_icon", &UMCamera::set_display_turn_table_icon)
			.def("set_use_motion_blur", &UMCamera::set_use_motion_blur)
			.def("set_use_real_time_motion_blur", &UMCamera::set_use_real_time_motion_blur)
			.def("set_motion_blur_intensity", &UMCamera::set_motion_blur_intensity)
			.def("set_aspect_ratio_mode", &UMCamera::set_aspect_ratio_mode)
			.def("set_aspect_width", &UMCamera::set_aspect_width)
			.def("set_aspect_height", &UMCamera::set_aspect_height)
			.def("set_pixel_aspect_ration", &UMCamera::set_pixel_aspect_ration)
			.def("set_aperture_mode", &UMCamera::set_aperture_mode)
			.def("set_gate_fit", &UMCamera::set_gate_fit)
			.def("set_field_of_view", &UMCamera::set_field_of_view)
			.def("set_field_of_view_x", &UMCamera::set_field_of_view_x)
			.def("set_field_of_view_y", &UMCamera::set_field_of_view_y)
			.def("set_focal_length", &UMCamera::set_focal_length)
			.def("set_camera_format", &UMCamera::set_camera_format)
			.def("set_use_frame_color", &UMCamera::set_use_frame_color)
			.def("set_frame_color", &UMCamera::set_frame_color)
			.def("set_show_name", &UMCamera::set_show_name)
			.def("set_show_info_on_moving", &UMCamera::set_show_info_on_moving)
			.def("set_show_grid", &UMCamera::set_show_grid)
			.def("set_show_optical_center", &UMCamera::set_show_optical_center)
			.def("set_show_azimut", &UMCamera::set_show_azimut)
			.def("set_show_time_code", &UMCamera::set_show_time_code)
			.def("set_show_audio", &UMCamera::set_show_audio)
			.def("set_audio_color", &UMCamera::set_audio_color)
			.def("set_near_plane", &UMCamera::set_near_plane)
			.def("set_far_plane", &UMCamera::set_far_plane)
			.def("set_auto_compute_clip_planes", &UMCamera::set_auto_compute_clip_planes)
			.def("set_film_width", &UMCamera::set_film_width)
			.def("set_film_height", &UMCamera::set_film_height)
			.def("set_film_aspect_ration", &UMCamera::set_film_aspect_ration)
			.def("set_film_squeeze_ration", &UMCamera::set_film_squeeze_ration)
			.def("set_film_format", &UMCamera::set_film_format)
			.def("set_film_offset_x", &UMCamera::set_film_offset_x)
			.def("set_film_offset_y", &UMCamera::set_film_offset_y)
			.def("set_pre_scale", &UMCamera::set_pre_scale)
			.def("set_film_translate_x", &UMCamera::set_film_translate_x)
			.def("set_film_translate_y", &UMCamera::set_film_translate_y)
			.def("set_film_roll_pivot_x", &UMCamera::set_film_roll_pivot_x)
			.def("set_film_roll_pivot_y", &UMCamera::set_film_roll_pivot_y)
			.def("set_film_roll_value", &UMCamera::set_film_roll_value)
			.def("set_film_roll_order", &UMCamera::set_film_roll_order)
			.def("set_view_camera_to_look_at", &UMCamera::set_view_camera_to_look_at)
			.def("set_view_frustum_near_far_plane", &UMCamera::set_view_frustum_near_far_plane)
			.def("set_view_frustum_back_plane_mode", &UMCamera::set_view_frustum_back_plane_mode)
			.def("set_back_plane_distance", &UMCamera::set_back_plane_distance)
			.def("set_back_plane_distance_mode", &UMCamera::set_back_plane_distance_mode)
			.def("set_view_frustum_front_plane_mode", &UMCamera::set_view_frustum_front_plane_mode)
			.def("set_front_plane_distance", &UMCamera::set_front_plane_distance)
			.def("set_front_plane_distance_mode", &UMCamera::set_front_plane_distance_mode)
			.def("set_lock_mode", &UMCamera::set_lock_mode)
			.def("set_lock_interest_navigation", &UMCamera::set_lock_interest_navigation)
			.def("set_back_plate_fit_image", &UMCamera::set_back_plate_fit_image)
			.def("set_back_plate_crop", &UMCamera::set_back_plate_crop)
			.def("set_back_plate_center", &UMCamera::set_back_plate_center)
			.def("set_back_plate_keep_ration", &UMCamera::set_back_plate_keep_ration)
			.def("set_background_alpha_treshold", &UMCamera::set_background_alpha_treshold)
			.def("set_back_plane_offset_x", &UMCamera::set_back_plane_offset_x)
			.def("set_back_plane_offset_y", &UMCamera::set_back_plane_offset_y)
			.def("set_back_plane_rotation", &UMCamera::set_back_plane_rotation)
			.def("set_back_plane_scale_x", &UMCamera::set_back_plane_scale_x)
			.def("set_back_plane_scale_y", &UMCamera::set_back_plane_scale_y)
			.def("set_show_backplate", &UMCamera::set_show_backplate)
			.def("set_front_plate_fit_image", &UMCamera::set_front_plate_fit_image)
			.def("set_front_plate_crop", &UMCamera::set_front_plate_crop)
			.def("set_front_plate_center", &UMCamera::set_front_plate_center)
			.def("set_front_plate_keep_ratio", &UMCamera::set_front_plate_keep_ratio)
			.def("set_show_frontplate", &UMCamera::set_show_frontplate)
			.def("set_front_plane_offset_x", &UMCamera::set_front_plane_offset_x)
			.def("set_front_plane_offset_y", &UMCamera::set_front_plane_offset_y)
			.def("set_front_plane_rotation", &UMCamera::set_front_plane_rotation)
			.def("set_front_plane_scale_x", &UMCamera::set_front_plane_scale_x)
			.def("set_front_plane_scale_y", &UMCamera::set_front_plane_scale_y)
			.def("set_foreground_opacity", &UMCamera::set_foreground_opacity)
			.def("set_display_safe_area", &UMCamera::set_display_safe_area)
			.def("set_display_safe_area_on_render", &UMCamera::set_display_safe_area_on_render)
			.def("set_safe_area_display_style", &UMCamera::set_safe_area_display_style)
			.def("set_safe_area_display_ratio", &UMCamera::set_safe_area_display_ratio)
			.def("set_use_2d_magnifier_zoom", &UMCamera::set_use_2d_magnifier_zoom)
			.def("set__2d_magnifier_zoom", &UMCamera::set__2d_magnifier_zoom)
			.def("set__2d_magnifier_x", &UMCamera::set__2d_magnifier_x)
			.def("set__2d_magnifier_y", &UMCamera::set__2d_magnifier_y)
			.def("set_projection_type", &UMCamera::set_projection_type)
			.def("set_ortho_zoom", &UMCamera::set_ortho_zoom)
			.def("set_use_real_time_dof_and_aa", &UMCamera::set_use_real_time_dof_and_aa)
			.def("set_use_depth_of_field", &UMCamera::set_use_depth_of_field)
			.def("set_focus_source", &UMCamera::set_focus_source)
			.def("set_focus_angle", &UMCamera::set_focus_angle)
			.def("set_focus_distance", &UMCamera::set_focus_distance)
			.def("set_use_antialiasing", &UMCamera::set_use_antialiasing)
			.def("set_antialiasing_intensity", &UMCamera::set_antialiasing_intensity)
			.def("set_antialiasing_method", &UMCamera::set_antialiasing_method)
			.def("set_use_accumulation_buffer", &UMCamera::set_use_accumulation_buffer)
			.def("set_frame_sample_count", &UMCamera::set_frame_sample_count)
			.def("set_frame_sampling_type", &UMCamera::set_frame_sampling_type)
			;
	}

	{
		class_<UMCameraTypes> cameratypes(m, "UMCameraTypes");

		enum_<UMCameraTypes::EProjectionType>(cameratypes, "ProjectionType")
			.value("Perspective", UMCameraTypes::ePerspective)
			.value("Orthogonal", UMCameraTypes::eOrthogonal)
			.export_values()
			;
		
		enum_<UMCameraTypes::EFormat>(cameratypes, "Format")
			.value("CustomFormat", UMCameraTypes::eCustomFormat)
			.value("D1NTSC", UMCameraTypes::eD1NTSC)
			.value("NTSC", UMCameraTypes::eNTSC)
			.value("PAL", UMCameraTypes::ePAL)
			.value("D1PAL", UMCameraTypes::eD1PAL)
			.value("HD", UMCameraTypes::eHD)
			.value("640x480", UMCameraTypes::e640x480)
			.value("320x200", UMCameraTypes::e320x200)
			.value("320x240", UMCameraTypes::e320x240)
			.value("128x128", UMCameraTypes::e128x128)
			.value("Fullscreen", UMCameraTypes::eFullscreen)
			.export_values()
			;
		
		enum_<UMCameraTypes::EAspectRatioMode>(cameratypes, "AspectRatioMode")
			.value("WindowSize", UMCameraTypes::eWindowSize)
			.value("FixedRatio", UMCameraTypes::eFixedRatio)
			.value("FixedResolution", UMCameraTypes::eFixedResolution)
			.value("FixedWidth", UMCameraTypes::eFixedWidth)
			.value("FixedHeight", UMCameraTypes::eFixedHeight)
			.export_values()
			;
		
		enum_<UMCameraTypes::EApertureFormat>(cameratypes, "ApertureFormat")
			.value("CustomAperture", UMCameraTypes::eCustomAperture)
			.value("16mmTheatrical", UMCameraTypes::e16mmTheatrical)
			.value("Super16mm", UMCameraTypes::eSuper16mm)
			.value("35mmAcademy", UMCameraTypes::e35mmAcademy)
			.value("35mmTVProjection", UMCameraTypes::e35mmTVProjection)
			.value("35mmFullAperture", UMCameraTypes::e35mmFullAperture)
			.value("35mm185Projection", UMCameraTypes::e35mm185Projection)
			.value("35mmAnamorphic", UMCameraTypes::e35mmAnamorphic)
			.value("70mmProjection", UMCameraTypes::e70mmProjection)
			.value("VistaVision", UMCameraTypes::eVistaVision)
			.value("DynaVision", UMCameraTypes::eDynaVision)
			.value("IMAX", UMCameraTypes::eIMAX)
			.export_values()
			;

		enum_<UMCameraTypes::EApertureMode>(cameratypes, "ApertureMode")
			.value("HorizAndVert", UMCameraTypes::eHorizAndVert)
			.value("Horizontal", UMCameraTypes::eHorizontal)
			.value("Vertical", UMCameraTypes::eVertical)
			.value("FocalLength", UMCameraTypes::eFocalLength)
			.export_values()
			;

		enum_<UMCameraTypes::EGateFit>(cameratypes, "GateFit")
			.value("FitNone", UMCameraTypes::eFitNone)
			.value("FitVertical", UMCameraTypes::eFitVertical)
			.value("FitHorizontal", UMCameraTypes::eFitHorizontal)
			.value("FitFill", UMCameraTypes::eFitFill)
			.value("FitOverscan", UMCameraTypes::eFitOverscan)
			.value("FitStretch", UMCameraTypes::eFitStretch)
			.export_values()
			;

		enum_<UMCameraTypes::EFilmRollOrder>(cameratypes, "FilmRollOrder")
			.value("RotateFirst", UMCameraTypes::eRotateFirst)
			.value("TranslateFirst", UMCameraTypes::eTranslateFirst)
			.export_values()
			;
		
		enum_<UMCameraTypes::EPlateDrawingMode>(cameratypes, "PlateDrawingMode")
			.value("PlateBackground", UMCameraTypes::ePlateBackground)
			.value("PlateForeground", UMCameraTypes::ePlateForeground)
			.value("PlateBackAndFront", UMCameraTypes::ePlateBackAndFront)
			.export_values()
			;

		enum_<UMCameraTypes::EFrontBackPlaneDistanceMode>(cameratypes, "FrontBackPlaneDistanceMode")
			.value("RelativeToInterest", UMCameraTypes::eRelativeToInterest)
			.value("RelativeToCamera", UMCameraTypes::eRelativeToCamera)
			.export_values()
			;

		enum_<UMCameraTypes::EFrontBackPlaneDisplayMode>(cameratypes, "FrontBackPlaneDisplayMode")
			.value("PlanesDisabled", UMCameraTypes::ePlanesDisabled)
			.value("PlanesAlways", UMCameraTypes::ePlanesAlways)
			.value("PlanesWhenMedia", UMCameraTypes::ePlanesWhenMedia)
			.export_values()
			;

		enum_<UMCameraTypes::ESafeAreaStyle>(cameratypes, "SafeAreaStyle")
			.value("SafeAreaRound", UMCameraTypes::eSafeAreaRound)
			.value("SafeAreaSquare", UMCameraTypes::eSafeAreaSquare)
			.export_values()
			;

		enum_<UMCameraTypes::ERenderOptionsUsageTime>(cameratypes, "RenderOptionsUsageTime")
			.value("Interactive", UMCameraTypes::eInteractive)
			.value("OnDemand", UMCameraTypes::eOnDemand)
			.export_values()
			;

		enum_<UMCameraTypes::EAntialiasingMethod>(cameratypes, "AntialiasingMethod")
			.value("AAOversampling", UMCameraTypes::eAAOversampling)
			.value("AAHardware", UMCameraTypes::eAAHardware)
			.export_values()
			;

		enum_<UMCameraTypes::ESamplingType>(cameratypes, "SamplingType")
			.value("SamplingUniform", UMCameraTypes::eSamplingUniform)
			.value("SamplingStochastic", UMCameraTypes::eSamplingStochastic)
			.export_values()
			;

		enum_<UMCameraTypes::EFocusDistanceSource>(cameratypes, "FocusDistanceSource")
			.value("FocusSrcCameraInterest", UMCameraTypes::eFocusSrcCameraInterest)
			.value("FocusSpecificDistance", UMCameraTypes::eFocusSpecificDistance)
			.export_values()
			;
	}

	//
	// UMSkeleton
	//
	{
		class_<UMSkeleton> skeleton(m, "UMSkeleton", umnode);
			// getter
		skeleton.def(init<>())
			.def("type", &UMSkeleton::type)
			.def("limb_length", &UMSkeleton::limb_length)
			.def("size", &UMSkeleton::size)
			// setter
			.def("set_type", &UMSkeleton::set_type)
			.def("set_limb_length", &UMSkeleton::set_limb_length)
			.def("set_size", &UMSkeleton::set_size)
			;

		enum_<UMSkeleton::EType>(skeleton, "SkeletonType")
			.value("Root", UMSkeleton::eRoot)
			.value("Limb", UMSkeleton::eLimb)
			.value("LimbNode", UMSkeleton::eLimbNode)
			.value("Effector", UMSkeleton::eEffector)
			.export_values()
			;
	}
	
	// UMTexture
	{
		class_<UMTexture> texture(m, "UMTexture");
			// getter
		texture.def(init<>())
			.def("name", &UMTexture::name)
			.def("file_name", &UMTexture::file_name)
			.def("relative_file_name", &UMTexture::relative_file_name)
			.def("blend_mode", &UMTexture::blend_mode)
			.def("alpha", &UMTexture::alpha)
			// setter
			.def("set_name", &UMTexture::set_name)
			.def("set_file_name", &UMTexture::set_file_name)
			.def("set_relative_file_name", &UMTexture::set_relative_file_name)
			.def("set_blend_mode", &UMTexture::set_blend_mode)
			.def("set_alpha", &UMTexture::set_alpha)
			;

			enum_<UMTexture::EBlendMode>(texture, "BlendMode")
			.value("Translucent", UMTexture::eTranslucent)
			.value("Additive", UMTexture::eAdditive)
			.value("Modulate", UMTexture::eModulate)
			.value("Modulate2", UMTexture::eModulate2)
			.value("Over", UMTexture::eOver)
			.value("Normal", UMTexture::eNormal)
			.value("Dissolve", UMTexture::eDissolve)
			.value("Darken", UMTexture::eDarken)
			.value("ColorBurn", UMTexture::eColorBurn)
			.value("LinearBurn", UMTexture::eLinearBurn)
			.value("DarkerColor", UMTexture::eDarkerColor)
			.value("Lighten", UMTexture::eLighten)
			.value("Screen", UMTexture::eScreen)
			.value("ColorDodge", UMTexture::eColorDodge)
			.value("LinearDodge", UMTexture::eLinearDodge)
			.value("LighterColor", UMTexture::eLighterColor)
			.value("SoftLight", UMTexture::eSoftLight)
			.value("HardLight", UMTexture::eHardLight)
			.value("VividLight", UMTexture::eVividLight)
			.value("LinearLight", UMTexture::eLinearLight)
			.value("PinLight", UMTexture::ePinLight)
			.value("HardMix", UMTexture::eHardMix)
			.value("Difference", UMTexture::eDifference)
			.value("Exclusion", UMTexture::eExclusion)
			.value("Subtract", UMTexture::eSubtract)
			.value("Divide", UMTexture::eDivide)
			.value("Hue", UMTexture::eHue)
			.value("Saturation", UMTexture::eSaturation)
			.value("Color", UMTexture::eColor)
			.value("Luminosity", UMTexture::eLuminosity)
			.value("Overlay", UMTexture::eOverlay)
			.value("BlendModeCount", UMTexture::eBlendModeCount)
			.export_values()
			;
	}

	//
	// UMMaterial
	//
	class_<UMMaterial> material(m, "UMMaterial");
		// getter
	material.def(init<>())
		.def("name", &UMMaterial::name)
		.def("ambient", &UMMaterial::ambient)
		.def("ambient_factor", &UMMaterial::ambient_factor)
		.def("diffuse", &UMMaterial::diffuse)
		.def("diffuse_factor", &UMMaterial::diffuse_factor)
		.def("specular", &UMMaterial::specular)
		.def("specular_factor", &UMMaterial::specular_factor)
		.def("emissive", &UMMaterial::emissive)
		.def("emissive_factor", &UMMaterial::emissive_factor)
		.def("transparent", &UMMaterial::transparent)
		.def("transparency_factor", &UMMaterial::transparency_factor)
		.def("refrection", &UMMaterial::refrection)
		.def("reflection_factor", &UMMaterial::reflection_factor)
		.def("bump", &UMMaterial::bump)
		.def("bump_factor", &UMMaterial::bump_factor)
		.def("vector_displacement", &UMMaterial::vector_displacement)
		.def("vector_displacement_factor", &UMMaterial::vector_displacement_factor)
		.def("shininess", &UMMaterial::shininess)
		.def("texture_list", &UMMaterial::texture_list)
		.def("layered_texture_list", &UMMaterial::layered_texture_list)
		// setter
		.def("set_name", &UMMaterial::set_name)
		.def("set_ambient", &UMMaterial::set_ambient)
		.def("set_ambient_factor", &UMMaterial::set_ambient_factor)
		.def("set_diffuse", &UMMaterial::set_diffuse)
		.def("set_diffuse_factor", &UMMaterial::set_diffuse_factor)
		.def("set_specular", &UMMaterial::set_specular)
		.def("set_specular_factor", &UMMaterial::set_specular_factor)
		.def("set_emissive", &UMMaterial::set_emissive)
		.def("set_emissive_factor", &UMMaterial::set_emissive_factor)
		.def("set_transparency", &UMMaterial::set_transparent)
		.def("set_transparency_factor", &UMMaterial::set_transparency_factor)
		.def("set_refrection", &UMMaterial::set_refrection)
		.def("set_reflection_factor", &UMMaterial::set_reflection_factor)
		.def("set_bump", &UMMaterial::set_bump)
		.def("set_bump_factor", &UMMaterial::set_bump_factor)
		.def("set_vector_displacement", &UMMaterial::set_vector_displacement)
		.def("set_vector_displacement_factor", &UMMaterial::set_vector_displacement_factor)
		.def("set_shininess", &UMMaterial::set_shininess)
		.def("add_texture", &UMMaterial::add_texture)
		.def("add_layered_texture", &UMMaterial::add_layered_texture)
		;
	
	// UMCluster
	{
		class_<UMCluster> cluster(m, "UMCluster");
			// getter
		cluster.def(init<>())
			.def("name", &UMCluster::name)
			.def("index_list", &UMCluster::index_list)
			.def("weight_list", &UMCluster::weight_list)
			.def("link_mode", &UMCluster::link_mode)
			.def("link_node", &UMCluster::link_node, return_value_policy::reference)
			.def("link_node_id", &UMCluster::link_node_id)
			// setter
			.def("set_name", &UMCluster::set_name)
			.def("set_link_mode", &UMCluster::set_link_mode)
			.def("set_link_node_id", &UMCluster::set_link_node_id)
			.def("add_weight", &UMCluster::add_weight)
			.def("add_index", &UMCluster::add_index)
			;

		enum_<UMCluster::ELinkMode>(cluster, "LinkMode")
			.value("Normalize", UMCluster::eNormalize)
			.value("Additive", UMCluster::eAdditive)
			.value("TotalOne", UMCluster::eTotalOne)
			.export_values()
			;
	}
	
	// UMDeformer
	class_<UMDeformer> umdeformer(m, "UMDeformer");
		// getter
	umdeformer.def(init<>())
		.def("geometry_node", &UMDeformer::geometry_node, return_value_policy::reference)
		.def("geometry_node_id", &UMDeformer::geometry_node_id)
		.def("deformer_type", &UMDeformer::deformer_type)
		// setter
		.def("set_geometry_node_id", &UMDeformer::set_geometry_node_id)
		;

		enum_<UMDeformer::EDeformerType>(umdeformer, "DeformerType")
			.value("Unknown", UMDeformer::eUnknown)
			.value("Skin", UMDeformer::eSkin)
			.value("BlendShape", UMDeformer::eBlendShape)
			.value("VertexCache", UMDeformer::eVertexCache)
			.export_values()
			;

	// UMShape
	class_<UMShape> shape(m, "UMShape");
	// getter
	shape.def(init<>())
		.def("name", &UMShape::name)
		.def("base_geometry_node", &UMShape::base_geometry_node, return_value_policy::reference)
		.def("base_geometry_node_id", &UMShape::base_geometry_node_id)
		.def("vertex_index_list", &UMShape::vertex_index_list)
		.def("normal_index_list", &UMShape::normal_index_list)
		.def("vertex_list", &UMShape::vertex_list)
		.def("normal_list", &UMShape::normal_list)
		// setter
		.def("set_name", &UMShape::set_name)
		.def("add_vertex_index", &UMShape::add_vertex_index)
		.def("add_normal_index", &UMShape::add_normal_index)
		.def("add_vertex", &UMShape::add_vertex)
		.def("add_normal", &UMShape::add_normal)
		.def("set_base_geometry_node_id", &UMShape::set_base_geometry_node_id)
		;
		
	// UMBlendShapeChannel
	class_<UMBlendShapeChannel> shapechannel(m, "UMBlendShapeChannel");
		// getter
		shapechannel.def(init<>())
		.def("target_shape_list", &UMBlendShapeChannel::target_shape_list)
		.def("full_weight_list", &UMBlendShapeChannel::full_weight_list)
		.def("deform_percent", &UMBlendShapeChannel::deform_percent)
		// setter
		.def("add_target_shape", &UMBlendShapeChannel::add_target_shape)
		.def("add_full_weight", &UMBlendShapeChannel::add_full_weight)
		.def("set_deform_percent", &UMBlendShapeChannel::set_deform_percent)
		;

	// UMBlendShape
		class_<UMBlendShape> blendshape(m, "UMBlendShape");
		// getter
		blendshape.def(init<>())
		.def("name", &UMBlendShape::name)
		.def("blend_shape_channel_list", &UMBlendShape::blend_shape_channel_list)
		// setter
		.def("set_name", &UMBlendShape::set_name)
		.def("add_blend_shape_channel", &UMBlendShape::add_blend_shape_channel)
		;
		
	// UMSkin
	class_<UMSkin> skin(m, "UMSkin", umdeformer);
		// getter
		skin.def(init<>())
		.def("name", &UMSkin::name)
		.def("cluster_list", &UMSkin::cluster_list)
		// setter
		.def("add_cluster", &UMSkin::add_cluster)
		.def("set_name", &UMSkin::set_name)
		;
		
	//
	// UMMesh
	//
	class_<UMMesh> mesh(m, "UMMesh", umnode);
		// getter
	mesh.def(init<>())
		.def("vertex_index_list", &UMMesh::vertex_index_list)
		.def("vertex_list", &UMMesh::vertex_list)
		.def("normal_list", &UMMesh::normal_list)
		.def("vertex_color_list", &UMMesh::vertex_color_list)
		.def("uv_list", &UMMesh::uv_list)
		.def("material_index_list", &UMMesh::material_index_list)
		.def("material_list", &UMMesh::material_list)
		.def("skin_list", &UMMesh::skin_list)
		.def("blend_shape_list", &UMMesh::blend_shape_list)
		.def("layered_uv_list", &UMMesh::layered_uv_list)
		.def("layered_normal_list", &UMMesh::layered_normal_list)
		.def("layered_vetex_color_list", &UMMesh::layered_vertex_color_list)
		// setter
		.def("add_vertex_index", &UMMesh::add_vertex_index)
		.def("add_vertex", &UMMesh::add_vertex)
		.def("add_normal", &UMMesh::add_normal)
		.def("add_vertex_color", &UMMesh::add_vertex_color)
		.def("add_uv", &UMMesh::add_uv)
		.def("add_material_index", &UMMesh::add_material_index)
		.def("add_material", &UMMesh::add_material)
		.def("add_skin", &UMMesh::add_skin)
		.def("add_blend_shape", &UMMesh::add_blend_shape)
		.def("add_uv_layer", &UMMesh::add_uv_layer)
		.def("add_normal_layer", &UMMesh::add_normal_layer)
		.def("add_vertex_color_layer", &UMMesh::add_vertex_color_layer)
		;
		
	//
	// UMObject
	//
	class_<UMObject, std::shared_ptr<UMObject> > object(m, "UMObject");

		// getter
		object.def(init<>())
		.def("mesh_list", &UMObject::mesh_map, return_value_policy::reference_internal)
		.def("skeleton_list", &UMObject::skeleton_map, return_value_policy::reference_internal)
		.def("camera_list", &UMObject::camera_map, return_value_policy::reference_internal)
		.def("other_node_list", &UMObject::other_node_map, return_value_policy::reference_internal)
		.def("next_id", &UMObject::next_id)
		// setter
		.def("add_mesh", &UMObject::add_mesh)
		.def("add_skeleton", &UMObject::add_skeleton)
		.def("add_camera", &UMObject::add_camera)
		.def("add_other_node", &UMObject::add_other_node)
		;
	
	//
	// UMAnimationCurveKeyTypes
	//
	{
		class_<UMAnimationCurveKeyTypes> curvekeytypes(m, "UMAnimationCurveKeyTypes");

		enum_<UMAnimationCurveKeyTypes::ECurveType>(curvekeytypes, "CurveType")
			.value("LocalTranslationX", UMAnimationCurveKeyTypes::eLocalTranslationX)
			.value("LocalTranslationY", UMAnimationCurveKeyTypes::eLocalTranslationY)
			.value("LocalTranslationZ", UMAnimationCurveKeyTypes::eLocalTranslationZ)
			.value("LocalRotationX", UMAnimationCurveKeyTypes::eLocalRotationX)
			.value("LocalRotationY", UMAnimationCurveKeyTypes::eLocalRotationY)
			.value("LocalRotationZ", UMAnimationCurveKeyTypes::eLocalRotationZ)
			.value("LocalScalingX", UMAnimationCurveKeyTypes::eLocalScalingX)
			.value("LocalScalingY", UMAnimationCurveKeyTypes::eLocalScalingY)
			.value("LocalScalingZ", UMAnimationCurveKeyTypes::eLocalScalingZ)
			.value("BlendShapeValue", UMAnimationCurveKeyTypes::eBlendShapeValue)
			.value("FieldOfView", UMAnimationCurveKeyTypes::eFieldOfView)
			.value("FieldOfViewX", UMAnimationCurveKeyTypes::eFieldOfViewX)
			.value("FieldOfViewY", UMAnimationCurveKeyTypes::eFieldOfViewY)
			.value("OpticalCenterX", UMAnimationCurveKeyTypes::eOpticalCenterX)
			.value("OpticalCenterY", UMAnimationCurveKeyTypes::eOpticalCenterY)
			.value("Roll", UMAnimationCurveKeyTypes::eRoll)
			.value("LightIntensity", UMAnimationCurveKeyTypes::eLightIntensity)
			.value("LightOuterAngle", UMAnimationCurveKeyTypes::eLightOuterAngle)
			.value("LightFog", UMAnimationCurveKeyTypes::eLightFog)
			.value("ColorRed", UMAnimationCurveKeyTypes::eColorRed)
			.value("ColorGreen", UMAnimationCurveKeyTypes::eColorGreen)
			.value("ColorBlue", UMAnimationCurveKeyTypes::eColorBlue)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::ETangentMode>(curvekeytypes, "TangentMode")
			.value("TangentAuto", UMAnimationCurveKeyTypes::eTangentAuto)
			.value("TangentTCB", UMAnimationCurveKeyTypes::eTangentTCB)
			.value("TangentUser", UMAnimationCurveKeyTypes::eTangentUser)
			.value("TangentGenericBreak", UMAnimationCurveKeyTypes::eTangentGenericBreak)
			.value("TangentBreak", UMAnimationCurveKeyTypes::eTangentBreak)
			.value("TangentAutoBreak", UMAnimationCurveKeyTypes::eTangentAutoBreak)
			.value("TangentGenericClamp", UMAnimationCurveKeyTypes::eTangentGenericClamp)
			.value("TangentGenericTimeIndependent", UMAnimationCurveKeyTypes::eTangentGenericTimeIndependent)
			.value("TangentGenericClampProgressive", UMAnimationCurveKeyTypes::eTangentGenericClampProgressive)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EInterpolationType>(curvekeytypes, "InterpolationType")
			.value("InterpolationConstant", UMAnimationCurveKeyTypes::eInterpolationConstant)
			.value("InterpolationLinear", UMAnimationCurveKeyTypes::eInterpolationLinear)
			.value("InterpolationCubic", UMAnimationCurveKeyTypes::eInterpolationCubic)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EWeightedMode>(curvekeytypes, "WeightedMode")
			.value("WeightedNone", UMAnimationCurveKeyTypes::eWeightedNone)
			.value("WeightedRight", UMAnimationCurveKeyTypes::eWeightedRight)
			.value("WeightedNextLeft", UMAnimationCurveKeyTypes::eWeightedNextLeft)
			.value("WeightedAll", UMAnimationCurveKeyTypes::eWeightedAll)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EConstantMode>(curvekeytypes, "ConstantMode")
			.value("ConstantStandard", UMAnimationCurveKeyTypes::eConstantStandard)
			.value("ConstantNext", UMAnimationCurveKeyTypes::eConstantNext)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EVelocityMode>(curvekeytypes, "VelocityMode")
			.value("VelocityNone", UMAnimationCurveKeyTypes::eVelocityNone)
			.value("VelocityRight", UMAnimationCurveKeyTypes::eVelocityRight)
			.value("VelocityNextLeft", UMAnimationCurveKeyTypes::eVelocityNextLeft)
			.value("VelocityAll", UMAnimationCurveKeyTypes::eVelocityAll)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EDataIndex>(curvekeytypes, "DataIndex")
			.value("RightSlope", UMAnimationCurveKeyTypes::eRightSlope)
			.value("NextLeftSlope", UMAnimationCurveKeyTypes::eNextLeftSlope)
			.value("Weights", UMAnimationCurveKeyTypes::eWeights)
			.value("RightWeight", UMAnimationCurveKeyTypes::eRightWeight)
			.value("NextLeftWeight", UMAnimationCurveKeyTypes::eNextLeftWeight)
			.value("Velocity", UMAnimationCurveKeyTypes::eVelocity)
			.value("RightVelocity", UMAnimationCurveKeyTypes::eRightVelocity)
			.value("NextLeftVelocity", UMAnimationCurveKeyTypes::eNextLeftVelocity)
			.value("TCBTension", UMAnimationCurveKeyTypes::eTCBTension)
			.value("TCBContinuity", UMAnimationCurveKeyTypes::eTCBContinuity)
			.value("TCBBias", UMAnimationCurveKeyTypes::eTCBBias)
			.export_values()
			;
	}

	//
	// UMAnimationCurveKey
	//
	class_<UMAnimationCurveKey> curvekey(m, "UMAnimationCurveKey");
	curvekey.def(init<>())
		.def("time", &UMAnimationCurveKey::time)
		.def("data_map", &UMAnimationCurveKey::data_map, return_value_policy::reference_internal)
		.def("interpolation_type", &UMAnimationCurveKey::interpolation_type)
		.def("tangent_mode", &UMAnimationCurveKey::tangent_mode)
		.def("weighted_mode", &UMAnimationCurveKey::weighted_mode)
		.def("velocity_mode", &UMAnimationCurveKey::velocity_mode)
		.def("constant_mode", &UMAnimationCurveKey::constant_mode)
		.def("tangent_visibility", &UMAnimationCurveKey::tangent_visibility)
		.def("is_break", &UMAnimationCurveKey::is_break)
		;
	
	//
	// UMAnimationCurve
	//
	class_<UMAnimationCurve>(m, "UMAnimationCurve")
		.def("key_map", &UMAnimationCurve::key_map, return_value_policy::reference_internal)
		;
	
	//
	// UMAnimationCurveStack
	//
	class_<UMAnimationCurveStack>(m, "UMAnimationCurveStack")
		.def("curve_map", &UMAnimationCurveStack::curve_map, return_value_policy::reference_internal)
		;
	
	//
	// UMAnimationLayer
	//
	class_<UMAnimationLayer> animationlayer(m, "UMAnimationLayer");
	animationlayer.def(init<>())
		.def("weight", &UMAnimationLayer::weight)
		.def("mute", &UMAnimationLayer::mute)
		.def("solo", &UMAnimationLayer::solo)
		.def("lock", &UMAnimationLayer::lock)
		.def("color", &UMAnimationLayer::color)
		.def("blend_mode", &UMAnimationLayer::blend_mode)
		.def("rotation_accumulation_mode", &UMAnimationLayer::rotation_accumulation_mode)
		.def("scale_accumulation_mode", &UMAnimationLayer::scale_accumulation_mode)
		.def("curve_stack_map", &UMAnimationLayer::curve_stack_map, return_value_policy::reference_internal)
		;
	
	//
	// UMAnimationStack
	//
	class_<UMAnimationStack> animationstack(m, "UMAnimationStack");
		animationstack.def(init<>())
		.def("name", &UMAnimationStack::name)
		.def("description", &UMAnimationStack::description)
		.def("local_start", &UMAnimationStack::local_start)
		.def("local_stop", &UMAnimationStack::local_stop)
		.def("reference_start", &UMAnimationStack::reference_start)
		.def("reference_stop", &UMAnimationStack::reference_stop)
		.def("layer_list", &UMAnimationStack::layer_list)
		;
	
	//
	// UMAnimation
	//
	class_<UMAnimation> animation(m, "UMAnimation");
		animation.def(init<>())
		.def("animation_stack_list", &UMAnimation::animation_stack_list, return_value_policy::reference_internal)
		;

	//
	// UMIO
	//
	class_<UMIO> umio(m, "UMIO");
		umio.def(init<>())
		.def("load", &UMIO::load)
		.def("save", &UMIO::save)
		//.def("load_from_memory", &load_from_memory_wrap, (arg("self"), arg("buffer")))
		//		.def("save_to_memory", &save_to_memory_wrap, (arg("self")))
		.def("load_setting", &UMIO::load_setting)
		.def("save_setting", &UMIO::save_setting)
		.def("create_object", &UMIO::create_object)
		;

	return m.ptr();
}

} // namespace umio

#endif //WITH_PYTHON
