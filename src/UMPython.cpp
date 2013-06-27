/**
 *
 * @file main.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#ifdef WITH_PYTHON

#include <stdio.h>
#include <iostream>
#include <string>

#include "UMIO.h"
#include "UMObject.h"
#include "UMAnimation.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/class.hpp>
#include <boost/python/overloads.hpp>
#include <boost/format.hpp>

namespace umio
{

// refered to http://d.hatena.ne.jp/moriyoshi/20091214/1260779899
template<typename T_>
class vector_to_pylist_converter {
public:
	typedef T_ native_type;

	static PyObject* convert(native_type const& v) {
		namespace py = boost::python;
		py::list retval;
		BOOST_FOREACH(typename boost::range_value<native_type>::type i, v)
		{
			retval.append(py::object(i));
		}
		return py::incref(retval.ptr());
	}

	static void* convertible(PyObject* pyo) {
		if (!PySequence_Check(pyo))
			return 0;

		return pyo;
	}

	static void construct(PyObject* pyo, boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		namespace py = boost::python;
		native_type* storage = new(reinterpret_cast<py::converter::rvalue_from_python_storage<native_type>*>(data)->storage.bytes) native_type();
		py::ssize_t size = PySequence_Size(pyo);
		storage->resize(size);
		for (py::ssize_t i = 0; i < size; ++i) {
			storage->at(i) = (
				py::extract<typename boost::range_value<native_type>::type>(
					PySequence_GetItem(pyo, i)));
		}
		data->convertible = storage;
	}
};

template<typename T_>
class array_to_pylist_converter {
public:
	typedef T_ native_type;

	static PyObject* convert(native_type const& v) {
		namespace py = boost::python;
		py::list retval;
		BOOST_FOREACH(typename boost::range_value<native_type>::type i, v)
		{
			retval.append(py::object(i));
		}
		return py::incref(retval.ptr());
	}

	static void* convertible(PyObject* pyo) {
		if (!PySequence_Check(pyo))
			return 0;

		return pyo;
	}

	static void construct(PyObject* pyo, boost::python::converter::rvalue_from_python_stage1_data* data)
	{
		namespace py = boost::python;
		native_type* storage = new(reinterpret_cast<py::converter::rvalue_from_python_storage<native_type>*>(data)->storage.bytes) native_type;
		for (py::ssize_t i = 0, size = PySequence_Size(pyo); i < size; ++i) {
			storage->at(i)= (
				py::extract<typename boost::range_value<native_type>::type>(
					PySequence_GetItem(pyo, i)));
		}
		data->convertible = storage;
	}
};

// call function with default arguments
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(uv_list_with_default_arg, uv_list, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(normal_list_with_default_arg, normal_list, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(vertex_color_list_with_default_arg, vertex_color_list, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(add_normal_with_default_args, add_normal, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(add_uv_with_default_args, add_uv, 1, 2);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(add_vertex_color_with_default_args, add_vertex_color, 1, 2);


BOOST_PYTHON_MODULE(UM)
{
	using namespace boost::python;
	
	//
	to_python_converter<umio::IntList, vector_to_pylist_converter<umio::IntList> >();
	to_python_converter<umio::DoubleList, vector_to_pylist_converter<umio::DoubleList> >();
	to_python_converter<umio::IntListVec, vector_to_pylist_converter<umio::IntListVec> >();
	to_python_converter<umio::DoubleListVec, vector_to_pylist_converter<umio::DoubleListVec > >();
	to_python_converter<umio::LayeredDoubleListVec, vector_to_pylist_converter<umio::LayeredDoubleListVec > >();
	to_python_converter<umio::UMVec4d, vector_to_pylist_converter<umio::UMVec4d> >();
	to_python_converter<umio::UMMat44d, vector_to_pylist_converter<umio::UMMat44d> >();
	to_python_converter<umio::Mat44dList, vector_to_pylist_converter<umio::Mat44dList> >();

	converter::registry::push_back(
		&vector_to_pylist_converter<umio::IntList>::convertible,
		&vector_to_pylist_converter<umio::IntList>::construct,
		boost::python::type_id<umio::IntList>());
	
	converter::registry::push_back(
		&vector_to_pylist_converter<umio::IntListVec>::convertible,
		&vector_to_pylist_converter<umio::IntListVec>::construct,
		boost::python::type_id<umio::IntListVec>());
	
	converter::registry::push_back(
		&vector_to_pylist_converter<umio::DoubleList>::convertible,
		&vector_to_pylist_converter<umio::DoubleList>::construct,
		boost::python::type_id<umio::DoubleList>());
	
	converter::registry::push_back(
		&vector_to_pylist_converter<umio::DoubleListVec>::convertible,
		&vector_to_pylist_converter<umio::DoubleListVec>::construct,
		boost::python::type_id<umio::DoubleListVec>());
	
	converter::registry::push_back(
		&vector_to_pylist_converter<umio::LayeredDoubleListVec>::convertible,
		&vector_to_pylist_converter<umio::LayeredDoubleListVec>::construct,
		boost::python::type_id<umio::LayeredDoubleListVec>());
	
	converter::registry::push_back(
		&array_to_pylist_converter<umio::UMVec4d>::convertible,
		&array_to_pylist_converter<umio::UMVec4d>::construct,
		boost::python::type_id<umio::UMVec4d>());
	
	converter::registry::push_back(
		&array_to_pylist_converter<umio::UMMat44d>::convertible,
		&array_to_pylist_converter<umio::UMMat44d>::construct,
		boost::python::type_id<umio::UMMat44d>());
	
	converter::registry::push_back(
		&vector_to_pylist_converter<umio::Mat44dList>::convertible,
		&vector_to_pylist_converter<umio::Mat44dList>::construct,
		boost::python::type_id<umio::Mat44dList>());

	{
		scope setting_scope = class_<UMIOSetting>("UMIOSetting")
			// setter
			.def("set_exp_bool_prop", &UMIOSetting::set_exp_bool_prop)
			.def("set_imp_bool_prop", &UMIOSetting::set_imp_bool_prop)
			.def("set_bl_exp_bool_prop", &UMIOSetting::set_bl_exp_bool_prop)
			.def("set_bl_imp_bool_prop", &UMIOSetting::set_bl_imp_bool_prop)
			;

		enum_<UMIOSetting::EExpSettingType>("ExpSettingType")
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

		enum_<UMIOSetting::EImpSettingType>("ImpSettingType")
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

		enum_<UMIOSetting::EUMExpSettingType>("UMExpSettingType")
			.value("UMExpFBX", UMIOSetting::eUMExpFBX)
			.value("UMExpText", UMIOSetting::eUMExpText)
			.value("UMExpOldFBX", UMIOSetting::eUMExpOldFBX)
			.export_values()
			;

		enum_<UMIOSetting::EUMImpSettingType>("UMImpSettingType")
			.value("UMImpFBX", UMIOSetting::eUMImpFBX)
			.value("UMImpTriangulate", UMIOSetting::eUMImpTriangulate)
			.value("UMImpNurbs", UMIOSetting::eUMImpNurbs)
			.value("UMImpPatch", UMIOSetting::eUMImpPatch)
			.export_values()
			;
	}

	// UMNode
	//
	class_<UMNode>("UMNode")
		// getter
		.def("id", &UMNode::id)
		.def("parent_id", &UMNode::parent_id)
		.def("parent", &UMNode::parent, return_value_policy<reference_existing_object>())
		.def("name", &UMNode::name, return_value_policy<copy_const_reference>())
		.def("local_translation", &UMNode::local_translation, return_value_policy<copy_const_reference>())
		.def("local_rotation", &UMNode::local_rotation, return_value_policy<copy_const_reference>())
		.def("local_scaling", &UMNode::local_scaling, return_value_policy<copy_const_reference>())
		.def("rotation_offset", &UMNode::rotation_offset, return_value_policy<copy_const_reference>())
		.def("rotation_pivot", &UMNode::rotation_pivot, return_value_policy<copy_const_reference>())
		.def("pre_rotation", &UMNode::pre_rotation, return_value_policy<copy_const_reference>())
		.def("post_rotation", &UMNode::post_rotation, return_value_policy<copy_const_reference>())
		.def("scaling_offset", &UMNode::scaling_offset, return_value_policy<copy_const_reference>())
		.def("scaling_pivot", &UMNode::scaling_pivot, return_value_policy<copy_const_reference>())
		.def("geometric_translation", &UMNode::geometric_translation, return_value_policy<copy_const_reference>())
		.def("geometric_rotation", &UMNode::geometric_rotation, return_value_policy<copy_const_reference>())
		.def("geometric_scaling", &UMNode::geometric_scaling, return_value_policy<copy_const_reference>())
		.def("local_transform", &UMNode::local_transform, return_value_policy<copy_const_reference>())
		.def("global_transform", &UMNode::global_transform, return_value_policy<copy_const_reference>())
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
		;
		
	class_<std::vector<UMNode> >("NodeList")
		.def(vector_indexing_suite<std::vector<UMNode> >());

	//
	// UMCamera
	//
	{
		scope camera_scope = class_<UMCamera, bases<UMNode> >("UMCamera")
			// getter
			.def("position", &UMCamera::position, return_value_policy<copy_const_reference>())
			.def("up_vector", &UMCamera::up_vector, return_value_policy<copy_const_reference>())
			.def("interest_position", &UMCamera::interest_position, return_value_policy<copy_const_reference>())
			.def("roll", &UMCamera::roll)
			.def("optical_center_x", &UMCamera::optical_center_x)
			.def("optical_center_y", &UMCamera::optical_center_y)
			.def("background_color", &UMCamera::background_color, return_value_policy<copy_const_reference>())
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
			.def("frame_color", &UMCamera::frame_color, return_value_policy<copy_const_reference>())
			.def("show_name", &UMCamera::show_name)
			.def("show_info_on_moving", &UMCamera::show_info_on_moving)
			.def("show_grid", &UMCamera::show_grid)
			.def("show_optical_center", &UMCamera::show_optical_center)
			.def("show_azimut", &UMCamera::show_azimut)
			.def("show_time_code", &UMCamera::show_time_code)
			.def("show_audio", &UMCamera::show_audio)
			.def("audio_color", &UMCamera::audio_color, return_value_policy<copy_const_reference>())
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
	
	class_<std::map<int, UMCamera> >("CameraMap")
		.def(map_indexing_suite<std::map<int, UMCamera> >());

	{
		scope camera_types_scope = class_<UMCameraTypes>("UMCameraTypes");

		enum_<UMCameraTypes::EProjectionType>("ProjectionType")
			.value("Perspective", UMCameraTypes::ePerspective)
			.value("Orthogonal", UMCameraTypes::eOrthogonal)
			.export_values()
			;
		
		enum_<UMCameraTypes::EFormat>("Format")
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
		
		enum_<UMCameraTypes::EAspectRatioMode>("AspectRatioMode")
			.value("WindowSize", UMCameraTypes::eWindowSize)
			.value("FixedRatio", UMCameraTypes::eFixedRatio)
			.value("FixedResolution", UMCameraTypes::eFixedResolution)
			.value("FixedWidth", UMCameraTypes::eFixedWidth)
			.value("FixedHeight", UMCameraTypes::eFixedHeight)
			.export_values()
			;
		
		enum_<UMCameraTypes::EApertureFormat>("ApertureFormat")
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

		enum_<UMCameraTypes::EApertureMode>("ApertureMode")
			.value("HorizAndVert", UMCameraTypes::eHorizAndVert)
			.value("Horizontal", UMCameraTypes::eHorizontal)
			.value("Vertical", UMCameraTypes::eVertical)
			.value("FocalLength", UMCameraTypes::eFocalLength)
			.export_values()
			;

		enum_<UMCameraTypes::EGateFit>("GateFit")
			.value("FitNone", UMCameraTypes::eFitNone)
			.value("FitVertical", UMCameraTypes::eFitVertical)
			.value("FitHorizontal", UMCameraTypes::eFitHorizontal)
			.value("FitFill", UMCameraTypes::eFitFill)
			.value("FitOverscan", UMCameraTypes::eFitOverscan)
			.value("FitStretch", UMCameraTypes::eFitStretch)
			.export_values()
			;

		enum_<UMCameraTypes::EFilmRollOrder>("FilmRollOrder")
			.value("RotateFirst", UMCameraTypes::eRotateFirst)
			.value("TranslateFirst", UMCameraTypes::eTranslateFirst)
			.export_values()
			;
		
		enum_<UMCameraTypes::EPlateDrawingMode>("PlateDrawingMode")
			.value("PlateBackground", UMCameraTypes::ePlateBackground)
			.value("PlateForeground", UMCameraTypes::ePlateForeground)
			.value("PlateBackAndFront", UMCameraTypes::ePlateBackAndFront)
			.export_values()
			;

		enum_<UMCameraTypes::EFrontBackPlaneDistanceMode>("FrontBackPlaneDistanceMode")
			.value("RelativeToInterest", UMCameraTypes::eRelativeToInterest)
			.value("RelativeToCamera", UMCameraTypes::eRelativeToCamera)
			.export_values()
			;

		enum_<UMCameraTypes::EFrontBackPlaneDisplayMode>("FrontBackPlaneDisplayMode")
			.value("PlanesDisabled", UMCameraTypes::ePlanesDisabled)
			.value("PlanesAlways", UMCameraTypes::ePlanesAlways)
			.value("PlanesWhenMedia", UMCameraTypes::ePlanesWhenMedia)
			.export_values()
			;

		enum_<UMCameraTypes::ESafeAreaStyle>("SafeAreaStyle")
			.value("SafeAreaRound", UMCameraTypes::eSafeAreaRound)
			.value("SafeAreaSquare", UMCameraTypes::eSafeAreaSquare)
			.export_values()
			;

		enum_<UMCameraTypes::ERenderOptionsUsageTime>("RenderOptionsUsageTime")
			.value("Interactive", UMCameraTypes::eInteractive)
			.value("OnDemand", UMCameraTypes::eOnDemand)
			.export_values()
			;

		enum_<UMCameraTypes::EAntialiasingMethod>("AntialiasingMethod")
			.value("AAOversampling", UMCameraTypes::eAAOversampling)
			.value("AAHardware", UMCameraTypes::eAAHardware)
			.export_values()
			;

		enum_<UMCameraTypes::ESamplingType>("SamplingType")
			.value("SamplingUniform", UMCameraTypes::eSamplingUniform)
			.value("SamplingStochastic", UMCameraTypes::eSamplingStochastic)
			.export_values()
			;

		enum_<UMCameraTypes::EFocusDistanceSource>("FocusDistanceSource")
			.value("FocusSrcCameraInterest", UMCameraTypes::eFocusSrcCameraInterest)
			.value("FocusSpecificDistance", UMCameraTypes::eFocusSpecificDistance)
			.export_values()
			;
	}

	//
	// UMSkeleton
	//
	{
		scope skeleton_scope = class_<UMSkeleton, bases<UMNode> >("UMSkeleton")
			// getter
			.def("type", &UMSkeleton::type)
			.def("limb_length", &UMSkeleton::limb_length)
			.def("size", &UMSkeleton::size)
			// setter
			.def("set_type", &UMSkeleton::set_type)
			.def("set_limb_length", &UMSkeleton::set_limb_length)
			.def("set_size", &UMSkeleton::set_size)
			;

		enum_<UMSkeleton::EType>("SkeletonType")
			.value("Root", UMSkeleton::eRoot)
			.value("Limb", UMSkeleton::eLimb)
			.value("LimbNode", UMSkeleton::eLimbNode)
			.value("Effector", UMSkeleton::eEffector)
			.export_values()
			;
	}
	
	class_<std::map<int, UMSkeleton> >("SkeletonMap")
		.def(map_indexing_suite<std::map<int, UMSkeleton> >());

	
	// UMTexture
	//
	to_python_converter<UMMaterial::TextureList, vector_to_pylist_converter<UMMaterial::TextureList> >();
	to_python_converter<UMMaterial::LayeredTextureList, vector_to_pylist_converter<UMMaterial::LayeredTextureList> >();

	{
		scope texture_scope = class_<UMTexture>("UMTexture")
			// getter
			.def("name", &UMTexture::name, return_value_policy<copy_const_reference>())
			.def("file_name", &UMTexture::file_name, return_value_policy<copy_const_reference>())
			.def("relative_file_name", &UMTexture::relative_file_name, return_value_policy<copy_const_reference>())
			.def("blend_mode", &UMTexture::blend_mode)
			.def("alpha", &UMTexture::alpha)
			// setter
			.def("set_name", &UMTexture::set_name)
			.def("set_file_name", &UMTexture::set_file_name)
			.def("set_relative_file_name", &UMTexture::set_relative_file_name)
			.def("set_blend_mode", &UMTexture::set_blend_mode)
			.def("set_alpha", &UMTexture::set_alpha)
			;

		enum_<UMTexture::EBlendMode>("BlendMode")
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
	class_<UMMaterial>("UMMaterial")
		// getter
		.def("name", &UMMaterial::name, return_value_policy<copy_const_reference>())
		.def("ambient", &UMMaterial::ambient, return_value_policy<copy_const_reference>())
		.def("ambient_factor", &UMMaterial::ambient_factor)
		.def("diffuse", &UMMaterial::diffuse, return_value_policy<copy_const_reference>())
		.def("diffuse_factor", &UMMaterial::diffuse_factor)
		.def("specular", &UMMaterial::specular, return_value_policy<copy_const_reference>())
		.def("specular_factor", &UMMaterial::specular_factor)
		.def("emissive", &UMMaterial::emissive, return_value_policy<copy_const_reference>())
		.def("emissive_factor", &UMMaterial::emissive_factor)
		.def("transparent", &UMMaterial::transparent, return_value_policy<copy_const_reference>())
		.def("transparency_factor", &UMMaterial::transparency_factor)
		.def("refrection", &UMMaterial::refrection, return_value_policy<copy_const_reference>())
		.def("reflection_factor", &UMMaterial::reflection_factor)
		.def("bump", &UMMaterial::bump, return_value_policy<copy_const_reference>())
		.def("bump_factor", &UMMaterial::bump_factor)
		.def("vector_displacement", &UMMaterial::vector_displacement, return_value_policy<copy_const_reference>())
		.def("vector_displacement_factor", &UMMaterial::vector_displacement_factor)
		.def("shininess", &UMMaterial::shininess)
		.def("texture_list", &UMMaterial::texture_list, return_value_policy<copy_const_reference>())
		.def("layered_texture_list", &UMMaterial::layered_texture_list, return_value_policy<copy_const_reference>())
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
	
	to_python_converter<UMMaterial::MaterialList, vector_to_pylist_converter<UMMaterial::MaterialList> >();

	// UMCluster
	{
		scope cluster_scope = class_<UMCluster>("UMCluster")
			// getter
			.def("name", &UMCluster::name, return_value_policy<copy_const_reference>())
			.def("index_list", &UMCluster::index_list, return_value_policy<copy_const_reference>())
			.def("weight_list", &UMCluster::weight_list, return_value_policy<copy_const_reference>())
			.def("link_mode", &UMCluster::link_mode)
			.def("link_node", &UMCluster::link_node, return_value_policy<reference_existing_object>())
			.def("link_node_id", &UMCluster::link_node_id)
			// setter
			.def("set_name", &UMCluster::set_name)
			.def("set_link_mode", &UMCluster::set_link_mode)
			.def("set_link_node_id", &UMCluster::set_link_node_id)
			.def("add_weight", &UMCluster::add_weight)
			.def("add_index", &UMCluster::add_index)
			;

		enum_<UMCluster::ELinkMode>("LinkMode")
			.value("Normalize", UMCluster::eNormalize)
			.value("Additive", UMCluster::eAdditive)
			.value("TotalOne", UMCluster::eTotalOne)
			.export_values()
			;
	}
	
	to_python_converter<UMCluster::ClusterList, vector_to_pylist_converter<UMCluster::ClusterList> >();

	// UMDeformer
	{
		scope deformer_scope = class_<UMDeformer>("UMDeformer")
			// getter
			.def("geometry_node", &UMDeformer::geometry_node, return_value_policy<reference_existing_object>())
			.def("geometry_node_id", &UMDeformer::geometry_node_id)
			.def("deformer_type", &UMDeformer::deformer_type)
			// setter
			.def("set_geometry_node_id", &UMDeformer::set_geometry_node_id)
			;

		enum_<UMDeformer::EDeformerType>("DeformerType")
			.value("Unknown", UMDeformer::eUnknown)
			.value("Skin", UMDeformer::eSkin)
			.value("BlendShape", UMDeformer::eBlendShape)
			.value("VertexCache", UMDeformer::eVertexCache)
			.export_values()
			;
	}

	// UMShape
	class_<UMShape>("UMShape")
		// getter
		.def("name", &UMShape::name, return_value_policy<copy_const_reference>())
		.def("base_geometry_node", &UMShape::base_geometry_node, return_value_policy<reference_existing_object>())
		.def("base_geometry_node_id", &UMShape::base_geometry_node_id)
		.def("vertex_index_list", &UMShape::vertex_index_list, return_value_policy<copy_const_reference>())
		.def("normal_index_list", &UMShape::normal_index_list, return_value_policy<copy_const_reference>())
		.def("vertex_list", &UMShape::vertex_list, return_value_policy<copy_const_reference>())
		.def("normal_list", &UMShape::normal_list, return_value_policy<copy_const_reference>())
		// setter
		.def("set_name", &UMShape::set_name)
		.def("add_vertex_index", &UMShape::add_vertex_index)
		.def("add_normal_index", &UMShape::add_normal_index)
		.def("add_vertex", &UMShape::add_vertex)
		.def("add_normal", &UMShape::add_normal)
		.def("set_base_geometry_node_id", &UMShape::set_base_geometry_node_id)
		;
		
	to_python_converter<UMShape::ShapeList, vector_to_pylist_converter<UMShape::ShapeList> >();
	
	// UMBlendShapeChannel
	class_<UMBlendShapeChannel>("UMBlendShapeChannel")
		// getter
		.def("target_shape_list", &UMBlendShapeChannel::target_shape_list, return_value_policy<copy_const_reference>())
		.def("full_weight_list", &UMBlendShapeChannel::full_weight_list, return_value_policy<copy_const_reference>())
		.def("deform_percent", &UMBlendShapeChannel::deform_percent)
		// setter
		.def("add_target_shape", &UMBlendShapeChannel::add_target_shape)
		.def("add_full_weight", &UMBlendShapeChannel::add_full_weight)
		.def("set_deform_percent", &UMBlendShapeChannel::set_deform_percent)
		;

	to_python_converter<UMBlendShapeChannel::ChannelList, vector_to_pylist_converter<UMBlendShapeChannel::ChannelList> >();
	
	// UMBlendShape
	class_<UMBlendShape>("UMBlendShape")
		// getter
		.def("name", &UMBlendShape::name, return_value_policy<copy_const_reference>())
		.def("blend_shape_channel_list", &UMBlendShape::blend_shape_channel_list, return_value_policy<copy_const_reference>())
		// setter
		.def("set_name", &UMBlendShape::set_name)
		.def("add_blend_shape_channel", &UMBlendShape::add_blend_shape_channel)
		;
		
	to_python_converter<UMBlendShape::BlendShapeList, vector_to_pylist_converter<UMBlendShape::BlendShapeList> >();

	// UMSkin
	class_<UMSkin, bases<UMDeformer> >("UMSkin")
		// getter
		.def("name", &UMSkin::name, return_value_policy<copy_const_reference>())
		.def("cluster_list", &UMSkin::cluster_list, return_value_policy<copy_const_reference>())
		// setter
		.def("add_cluster", &UMSkin::add_cluster)
		.def("set_name", &UMSkin::set_name)
		;
		
	to_python_converter<UMSkin::SkinList, vector_to_pylist_converter<UMSkin::SkinList> >();

	//
	// UMMesh
	//
	class_<UMMesh, bases<UMNode> >("UMMesh")
		// getter
		.def("vertex_index_list", &UMMesh::vertex_index_list, return_value_policy<copy_const_reference>())
		.def("vertex_list", &UMMesh::vertex_list, return_value_policy<copy_const_reference>())
		.def("normal_list", &UMMesh::normal_list, normal_list_with_default_arg()[return_value_policy<copy_const_reference>()])
		.def("vertex_color_list", &UMMesh::vertex_color_list, vertex_color_list_with_default_arg()[return_value_policy<copy_const_reference>()])
		.def("uv_list", &UMMesh::uv_list, uv_list_with_default_arg()[return_value_policy<copy_const_reference>()])
		.def("material_index_list", &UMMesh::material_index_list, return_value_policy<copy_const_reference>())
		.def("material_list", &UMMesh::material_list, return_value_policy<copy_const_reference>())
		.def("skin_list", &UMMesh::skin_list, return_value_policy<copy_const_reference>())
		.def("blend_shape_list", &UMMesh::blend_shape_list, return_value_policy<copy_const_reference>())
		.def("layered_uv_list", &UMMesh::layered_uv_list, return_value_policy<copy_const_reference>())
		.def("layered_normal_list", &UMMesh::layered_normal_list, return_value_policy<copy_const_reference>())
		.def("layered_vetex_color_list", &UMMesh::layered_vertex_color_list, return_value_policy<copy_const_reference>())
		// setter
		.def("add_vertex_index", &UMMesh::add_vertex_index)
		.def("add_vertex", &UMMesh::add_vertex)
		.def("add_normal", &UMMesh::add_normal, add_normal_with_default_args())
		.def("add_vertex_color", &UMMesh::add_vertex_color, add_vertex_color_with_default_args())
		.def("add_uv", &UMMesh::add_uv, add_uv_with_default_args())
		.def("add_material_index", &UMMesh::add_material_index)
		.def("add_material", &UMMesh::add_material)
		.def("add_skin", &UMMesh::add_skin)
		.def("add_blend_shape", &UMMesh::add_blend_shape)
		.def("add_uv_layer", &UMMesh::add_uv_layer)
		.def("add_normal_layer", &UMMesh::add_normal_layer)
		.def("add_vertex_color_layer", &UMMesh::add_vertex_color_layer)
		;
		
	class_<std::map<int, UMMesh> >("MeshMap")
		.def(map_indexing_suite<std::map<int, UMMesh> >());

	//
	// UMObject
	//
	register_ptr_to_python< UMObjectPtr >();
	class_<UMObject>("UMObject")
		// getter
		.def("mesh_list", &UMObject::mesh_map, return_internal_reference<>())
		.def("skeleton_list", &UMObject::skeleton_map, return_internal_reference<>())
		.def("camera_list", &UMObject::camera_map, return_internal_reference<>())
		.def("other_node_list", &UMObject::other_node_map, return_internal_reference<>())
		.def("next_id", &UMObject::next_id)
		// setter
		.def("add_mesh", &UMObject::add_mesh)
		.def("add_skeleton", &UMObject::add_skeleton)
		.def("add_camera", &UMObject::add_camera)
		.def("add_other_node", &UMObject::add_other_node)
		.def("create_object", UMObject::create_object)
		;
	
	//
	// UMAnimationCurveKeyTypes
	//
	{
		scope curvekeytypes_scope = class_<UMAnimationCurveKeyTypes>("UMAnimationCurveKeyTypes");

		enum_<UMAnimationCurveKeyTypes::ECurveType>("CurveType")
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
		
		enum_<UMAnimationCurveKeyTypes::ETangentMode>("TangentMode")
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
		
		enum_<UMAnimationCurveKeyTypes::EInterpolationType>("InterpolationType")
			.value("InterpolationConstant", UMAnimationCurveKeyTypes::eInterpolationConstant)
			.value("InterpolationLinear", UMAnimationCurveKeyTypes::eInterpolationLinear)
			.value("InterpolationCubic", UMAnimationCurveKeyTypes::eInterpolationCubic)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EWeightedMode>("WeightedMode")
			.value("WeightedNone", UMAnimationCurveKeyTypes::eWeightedNone)
			.value("WeightedRight", UMAnimationCurveKeyTypes::eWeightedRight)
			.value("WeightedNextLeft", UMAnimationCurveKeyTypes::eWeightedNextLeft)
			.value("WeightedAll", UMAnimationCurveKeyTypes::eWeightedAll)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EConstantMode>("ConstantMode")
			.value("ConstantStandard", UMAnimationCurveKeyTypes::eConstantStandard)
			.value("ConstantNext", UMAnimationCurveKeyTypes::eConstantNext)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EVelocityMode>("VelocityMode")
			.value("VelocityNone", UMAnimationCurveKeyTypes::eVelocityNone)
			.value("VelocityRight", UMAnimationCurveKeyTypes::eVelocityRight)
			.value("VelocityNextLeft", UMAnimationCurveKeyTypes::eVelocityNextLeft)
			.value("VelocityAll", UMAnimationCurveKeyTypes::eVelocityAll)
			.export_values()
			;
		
		enum_<UMAnimationCurveKeyTypes::EDataIndex>("DataIndex")
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
	class_<UMAnimationCurveKey>("UMAnimationCurveKey")
		.def("time", &UMAnimationCurveKey::time)
		.def("data_map", &UMAnimationCurveKey::data_map, return_internal_reference<>())
		.def("interpolation_type", &UMAnimationCurveKey::interpolation_type)
		.def("tangent_mode", &UMAnimationCurveKey::tangent_mode)
		.def("weighted_mode", &UMAnimationCurveKey::weighted_mode)
		.def("velocity_mode", &UMAnimationCurveKey::velocity_mode)
		.def("constant_mode", &UMAnimationCurveKey::constant_mode)
		.def("tangent_visibility", &UMAnimationCurveKey::tangent_visibility)
		.def("is_break", &UMAnimationCurveKey::is_break)
		;
	
	class_<std::map<UMTime, UMAnimationCurveKey> >("KeyMap")
		.def(map_indexing_suite<std::map<UMTime, UMAnimationCurveKey> >());

	//
	// UMAnimationCurve
	//
	class_<UMAnimationCurve>("UMAnimationCurve")
		.def("key_map", &UMAnimationCurve::key_map, return_internal_reference<>())
		;
	
	class_<std::map<int, UMAnimationCurve> >("CurveMap")
		.def(map_indexing_suite<std::map<int, UMAnimationCurve> >());

	//
	// UMAnimationCurveStack
	//
	class_<UMAnimationCurveStack>("UMAnimationCurveStack")
		.def("curve_map", &UMAnimationCurveStack::curve_map, return_internal_reference<>())
		;
	
	
	class_<std::map<int, UMAnimationCurveStack> >("IDToCurveStackMap")
		.def(map_indexing_suite<std::map<int, UMAnimationCurveStack> >());

	//
	// UMAnimationLayer
	//
	class_<UMAnimationLayer>("UMAnimationLayer")
		.def("weight", &UMAnimationLayer::weight)
		.def("mute", &UMAnimationLayer::mute)
		.def("solo", &UMAnimationLayer::solo)
		.def("lock", &UMAnimationLayer::lock)
		.def("color", &UMAnimationLayer::color, return_value_policy<copy_const_reference>())
		.def("blend_mode", &UMAnimationLayer::blend_mode)
		.def("rotation_accumulation_mode", &UMAnimationLayer::rotation_accumulation_mode)
		.def("scale_accumulation_mode", &UMAnimationLayer::scale_accumulation_mode)
		.def("curve_stack_map", &UMAnimationLayer::curve_stack_map, return_internal_reference<>())
		;
	
	to_python_converter<UMAnimationLayer::LayerList, vector_to_pylist_converter<UMAnimationLayer::LayerList> >();

	//
	// UMAnimationStack
	//
	class_<UMAnimationStack>("UMAnimationStack")
		.def("name", &UMAnimationStack::name, return_value_policy<copy_const_reference>())
		.def("description", &UMAnimationStack::description, return_value_policy<copy_const_reference>())
		.def("local_start", &UMAnimationStack::local_start)
		.def("local_stop", &UMAnimationStack::local_stop)
		.def("reference_start", &UMAnimationStack::reference_start)
		.def("reference_stop", &UMAnimationStack::reference_stop)
		.def("layer_list", &UMAnimationStack::layer_list, return_value_policy<copy_const_reference>())
		;
	
	to_python_converter<UMAnimationStack::AnimationStackList, vector_to_pylist_converter<UMAnimationStack::AnimationStackList> >();

	//
	// UMAnimation
	//
	class_<UMAnimation>("UMAnimation")
		.def("animation_stack_list", &UMAnimation::animation_stack_list, return_internal_reference<>())
		;

	//
	// UMIO
	//
	class_<UMIO>("UMIO")
		.def("load", &UMIO::load)
		.def("save", &UMIO::save)
		.def("load_setting", &UMIO::load_setting)
		.def("save_setting", &UMIO::save_setting)
		;
}

} // namespace umio

#endif //WITH_PYTHON
