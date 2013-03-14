/**
 *
 * UMObject.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#pragma once

#include "UMListTypes.h"

#include <map>
#include <vector>
#include <list>

#ifdef WITH_PYTHON
	#include <boost/shared_ptr.hpp>
#endif

#ifdef WITH_BOOST_SERIALIZATION
	#include <boost/serialization/serialization.hpp>
	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/map.hpp>
	#include <boost/serialization/string.hpp>
	#include <boost/serialization/array.hpp>
#endif

namespace UM
{
	
class UMObject;
#ifdef WITH_PYTHON
	typedef boost::shared_ptr<UMObject> UMObjectPtr;
#else
	typedef std::shared_ptr<UMObject> UMObjectPtr;
#endif // WITH_PYTHON

/*---------------------------------------------------------------------------*
 * UMNode
 *---------------------------------------------------------------------------*/
class UMNode {
public:
	UMNode() :
		parent_(NULL), parent_id_(-1), id_(-1) {}
	virtual ~UMNode() {}
	
	typedef std::map<int, UMNode> IDToNodeMap;
	typedef std::vector<UMNode> NodeList;
	typedef std::vector<UMNode*> NodePointerList;
	
	bool operator==(const UMNode& node)
	{
		return (id() == node.id());
	}

	// getter
	inline int id() const { return id_; }
	inline int parent_id() const { return parent_id_; }
	inline const std::string& name() const { return name_; }
	inline const UMNode* parent() const { return parent_; }
	inline const NodePointerList children() const { return children_; }
	inline const Array4d& local_translation() const { return local_translation_; }
	inline const Array4d& local_rotation() const { return local_rotation_; }
	inline const Array4d& local_scaling() const { return local_scaling_; }
	inline const Array4d& rotation_offset() const { return rotation_offset_; }
	inline const Array4d& rotation_pivot() const { return rotation_pivot_; }
	inline const Array4d& pre_rotation() const { return pre_rotation_; }
	inline const Array4d& post_rotation() const { return post_rotation_; }
	inline const Array4d& scaling_offset() const { return scaling_offset_; }
	inline const Array4d& scaling_pivot() const { return scaling_pivot_; }
	inline const Array4d& geometric_translation() const { return geometric_translation_; }
	inline const Array4d& geometric_rotation() const { return geometric_rotation_; }
	inline const Array4d& geometric_scaling() const { return geometric_scaling_; }
	inline const Array44d& local_transform() const { return local_transform_; }
	inline const Array44d& global_transform() const { return global_transform_; }

	// setter
	inline void set_id(int id) { id_ = id; }
	inline void set_parent_id(int parent_id) { parent_id_ = parent_id; }
	inline void set_name(const std::string& name) { name_ = name; }
	inline void set_parent(UMNode* parent) { parent_ = parent; }
	inline NodePointerList mutable_children() { return children_; }
	inline Array4d& mutable_local_translation() { return local_translation_; }
	inline Array4d& mutable_local_rotation() { return local_rotation_; }
	inline Array4d& mutable_local_scaling() { return local_scaling_; }
	inline Array4d& mutable_rotation_offset() { return rotation_offset_; }
	inline Array4d& mutable_rotation_pivot() { return rotation_pivot_; }
	inline Array4d& mutable_pre_rotation() { return pre_rotation_; }
	inline Array4d& mutable_post_rotation() { return post_rotation_; }
	inline Array4d& mutable_scaling_offset() { return scaling_offset_; }
	inline Array4d& mutable_scaling_pivot() { return scaling_pivot_; }
	inline Array4d& mutable_geometric_translation() { return geometric_translation_; }
	inline Array4d& mutable_geometric_rotation() { return geometric_rotation_; }
	inline Array4d& mutable_geometric_scaling() { return geometric_scaling_; }
	inline Array44d& mutable_local_transform() { return local_transform_; }
	inline Array44d& mutable_global_transform() { return global_transform_; }
	
	// external use
	void set_local_translation(const Array4d& trans) { local_translation_ = trans; }
	void set_local_rotation(const Array4d& rot) { local_rotation_ = rot; }
	void set_local_scaling(const Array4d& scale) { local_scaling_ = scale; }
	void set_rotation_offset(const Array4d& offset) { rotation_offset_ = offset; }
	void set_rotation_pivot(const Array4d& pivot) { rotation_pivot_ = pivot; }
	void set_pre_rotation(const Array4d& rot) { pre_rotation_ = rot; }
	void set_post_rotation(const Array4d& rot) { post_rotation_ = rot; }
	void set_scaling_offset(const Array4d& offset) { scaling_offset_ = offset; }
	void set_scaling_pivot(const Array4d& pivot) { scaling_pivot_ = pivot; }
	void set_geometric_translation(const Array4d& trans) { geometric_translation_ = trans; }
	void set_geometric_rotation(const Array4d& rot) { geometric_rotation_ = rot; }
	void set_geometric_scaling(const Array4d& scale) { geometric_scaling_ = scale; }
	
private:
	// boost serialization
	#include "UMNodeBos.h"

	UMNode* parent_;
	NodePointerList children_;

	int parent_id_;
	int id_;
	std::string name_;

	// local
	Array4d local_translation_;
	Array4d local_rotation_;
	Array4d local_scaling_;

	// pivot
	Array4d rotation_offset_;
	Array4d rotation_pivot_;
	Array4d pre_rotation_;
	Array4d post_rotation_;
	Array4d scaling_offset_;
	Array4d scaling_pivot_;
	Array4d geometric_translation_;
	Array4d geometric_rotation_;
	Array4d geometric_scaling_;
	
	// evaluated transform
	Array44d local_transform_;
	Array44d global_transform_;
};

/*---------------------------------------------------------------------------*
 * UMCameraTypes
 *---------------------------------------------------------------------------*/
class UMCameraTypes
{
public:
	enum EProjectionType
	{
		ePerspective,
		eOrthogonal
	};

	enum EFormat
	{
		eCustomFormat,
		eD1NTSC,
		eNTSC,
		ePAL,
		eD1PAL,
		eHD,
		e640x480,
		e320x200,
		e320x240,
		e128x128,
		eFullscreen
	};

	enum EAspectRatioMode
	{
		eWindowSize,
		eFixedRatio,
		eFixedResolution,
		eFixedWidth,
		eFixedHeight
	};

	enum EApertureFormat{
		eCustomAperture,
		e16mmTheatrical,
		eSuper16mm,
		e35mmAcademy,
		e35mmTVProjection,
		e35mmFullAperture,
		e35mm185Projection,
		e35mmAnamorphic,
		e70mmProjection,
		eVistaVision,
		eDynaVision,
		eIMAX
	};

	enum EApertureMode
	{
		eHorizAndVert,
		eHorizontal,
		eVertical,
		eFocalLength
	};

	enum EGateFit
	{
		eFitNone,
		eFitVertical,
		eFitHorizontal,
		eFitFill,
		eFitOverscan,
		eFitStretch
	};

	enum EFilmRollOrder
	{
		eRotateFirst,
		eTranslateFirst
	};

	enum EPlateDrawingMode
	{
		ePlateBackground,
		ePlateForeground,
		ePlateBackAndFront
	};
	
	enum EFrontBackPlaneDistanceMode
	{
		eRelativeToInterest,
		eRelativeToCamera
	};
	
	enum EFrontBackPlaneDisplayMode
	{
		ePlanesDisabled,
		ePlanesAlways,
		ePlanesWhenMedia
	};
	
	enum ESafeAreaStyle
	{
		eSafeAreaRound,
		eSafeAreaSquare
	};

	enum ERenderOptionsUsageTime
	{
		eInteractive,
		eOnDemand
	};

	enum EAntialiasingMethod
	{
		eAAOversampling,
		eAAHardware
	};

	enum ESamplingType
	{
		eSamplingUniform,
		eSamplingStochastic
	};

	enum EFocusDistanceSource
	{
		eFocusSrcCameraInterest,
		eFocusSpecificDistance
	};

};

/*---------------------------------------------------------------------------*
 * UMCamera
 *---------------------------------------------------------------------------*/
class UMCamera : public UMNode {
public:
	typedef std::map<int, UMCamera> IDToCameraMap;

	UMCamera() {}
	virtual ~UMCamera() {}

	///
	/// getter
	/// 
	inline const Array4d& position() const { return position_; }
	inline const Array4d& up_vector() const { return up_vector_; }
	inline const Array4d& interest_position() const { return interest_position_; }
	inline double roll() const { return roll_; }
	inline double optical_center_x() const { return optical_center_x_; }
	inline double optical_center_y() const { return optical_center_y_; }
	inline const Array4d& background_color() const { return background_color_; }
	inline double turn_table() const { return turn_table_; }
	inline bool display_turn_table_icon() const { return display_turn_table_icon_; }
	inline bool use_motion_blur() const { return use_motion_blur_; }
	inline bool use_real_time_motion_blur() const { return use_real_time_motion_blur_; }
	inline double motion_blur_intensity() const { return motion_blur_intensity_; }
	inline int aspect_ratio_mode() const { return aspect_ratio_mode_; }
	inline double aspect_width() const { return aspect_width_; }
	inline double aspect_height() const { return aspect_height_; }
	inline double pixel_aspect_ration() const { return pixel_aspect_ration_; }
	inline int aperture_mode() const { return aperture_mode_; }
	inline int gate_fit() const { return gate_fit_; }
	inline double field_of_view() const { return field_of_view_; }
	inline double field_of_view_x() const { return field_of_view_x_; }
	inline double field_of_view_y() const { return field_of_view_y_; }
	inline double focal_length() const { return focal_length_; }
	inline int camera_format() const { return camera_format_; }
	inline bool use_frame_color() const { return use_frame_color_; }
	inline const Array4d& frame_color() const { return frame_color_; }
	inline bool show_name() const { return show_name_; }
	inline bool show_info_on_moving() const { return show_info_on_moving_; }
	inline bool show_grid() const { return show_grid_; }
	inline bool show_optical_center() const { return show_optical_center_; }
	inline bool show_azimut() const { return show_azimut_; }
	inline bool show_time_code() const { return show_time_code_; }
	inline bool show_audio() const { return show_audio_; }
	inline const Array4d& audio_color() const { return audio_color_; }
	inline double near_plane() const { return near_plane_; }
	inline double far_plane() const { return far_plane_; }
	inline bool auto_compute_clip_planes() const { return auto_compute_clip_planes_; }
	inline double film_width() const { return film_width_; }
	inline double film_height() const { return film_height_; }
	inline double film_aspect_ration() const { return film_aspect_ration_; }
	inline double film_squeeze_ration() const { return film_squeeze_ration_; }
	inline int film_format() const { return film_format_; }
	inline double film_offset_x() const { return film_offset_x_; }
	inline double film_offset_y() const { return film_offset_y_; }
	inline double pre_scale() const { return pre_scale_; }
	inline double film_translate_x() const { return film_translate_x_; }
	inline double film_translate_y() const { return film_translate_y_; }
	inline double film_roll_pivot_x() const { return film_roll_pivot_x_; }
	inline double film_roll_pivot_y() const { return film_roll_pivot_y_; }
	inline double film_roll_value() const { return film_roll_value_; }
	inline int film_roll_order() const { return film_roll_order_; }
	inline bool view_camera_to_look_at() const { return view_camera_to_look_at_; }
	inline bool view_frustum_near_far_plane() const { return view_frustum_near_far_plane_; }
	inline int view_frustum_back_plane_mode() const { return view_frustum_back_plane_mode_; }
	inline double back_plane_distance() const { return back_plane_distance_; }
	inline int back_plane_distance_mode() const { return back_plane_distance_mode_; }
	inline int view_frustum_front_plane_mode() const { return view_frustum_front_plane_mode_; }
	inline double front_plane_distance() const { return front_plane_distance_; }
	inline int front_plane_distance_mode() const { return front_plane_distance_mode_; }
	inline bool lock_mode() const { return lock_mode_; }
	inline bool lock_interest_navigation() const { return lock_interest_navigation_; }
	inline bool back_plate_fit_image() const { return back_plate_fit_image_; }
	inline bool back_plate_crop() const { return back_plate_crop_; }
	inline bool back_plate_center() const { return back_plate_center_; }
	inline bool back_plate_keep_ration() const { return back_plate_keep_ration_; }
	inline double background_alpha_treshold() const { return background_alpha_treshold_; }
	inline double back_plane_offset_x() const { return back_plane_offset_x_; }
	inline double back_plane_offset_y() const { return back_plane_offset_y_; }
	inline double back_plane_rotation() const { return back_plane_rotation_; }
	inline double back_plane_scale_x() const { return back_plane_scale_x_; }
	inline double back_plane_scale_y() const { return back_plane_scale_y_; }
	inline bool show_backplate() const { return show_backplate_; }
	inline bool front_plate_fit_image() const { return front_plate_fit_image_; }
	inline bool front_plate_crop() const { return front_plate_crop_; }
	inline bool front_plate_center() const { return front_plate_center_; }
	inline bool front_plate_keep_ratio() const { return front_plate_keep_ratio_; }
	inline bool show_frontplate() const { return show_frontplate_; }
	inline double front_plane_offset_x() const { return front_plane_offset_x_; }
	inline double front_plane_offset_y() const { return front_plane_offset_y_; }
	inline double front_plane_rotation() const { return front_plane_rotation_; }
	inline double front_plane_scale_x() const { return front_plane_scale_x_; }
	inline double front_plane_scale_y() const { return front_plane_scale_y_; }
	inline double foreground_opacity() const { return foreground_opacity_; }
	inline bool display_safe_area() const { return display_safe_area_; }
	inline bool display_safe_area_on_render() const { return display_safe_area_on_render_; }
	inline int safe_area_display_style() const { return safe_area_display_style_; }
	inline double safe_area_display_ratio() const { return safe_area_display_ratio_; }
	inline bool use_2d_magnifier_zoom() const { return use_2d_magnifier_zoom_; }
	inline double _2d_magnifier_zoom() const { return _2d_magnifier_zoom_; }
	inline double _2d_magnifier_x() const { return _2d_magnifier_x_; }
	inline double _2d_magnifier_y() const { return _2d_magnifier_y_; }
	inline int projection_type() const { return projection_type_; }
	inline double ortho_zoom() const { return ortho_zoom_; }
	inline bool use_real_time_dof_and_aa() const { return use_real_time_dof_and_aa_; }
	inline bool use_depth_of_field() const { return use_depth_of_field_; }
	inline int focus_source() const { return focus_source_; }
	inline double focus_angle() const { return focus_angle_; }
	inline double focus_distance() const { return focus_distance_; }
	inline bool use_antialiasing() const { return use_antialiasing_; }
	inline double antialiasing_intensity() const { return antialiasing_intensity_; }
	inline int antialiasing_method() const { return antialiasing_method_; }
	inline bool use_accumulation_buffer() const { return use_accumulation_buffer_; }
	inline int frame_sample_count() const { return frame_sample_count_; }
	inline int frame_sampling_type() const { return frame_sampling_type_; }

	///
	/// setter
	///
	inline Array4d& mutable_position() { return position_; }
	inline Array4d& mutable_up_vector() { return up_vector_; }
	inline Array4d& mutable_interest_position() { return interest_position_; }
	inline void set_roll(double val) { roll_ = val; }
	inline void set_optical_center_x(double val) { optical_center_x_ = val; }
	inline void set_optical_center_y(double val) { optical_center_y_ = val; }
	inline Array4d& mutable_background_color() { return background_color_; }
	inline void set_turn_table(double val) { turn_table_ = val; }
	inline void set_display_turn_table_icon(bool val) { display_turn_table_icon_ = val; }
	inline void set_use_motion_blur(bool val) { use_motion_blur_ = val; }
	inline void set_use_real_time_motion_blur(bool val) { use_real_time_motion_blur_ = val; }
	inline void set_motion_blur_intensity(double val) { motion_blur_intensity_ = val; }
	inline void set_aspect_ratio_mode(int val) { aspect_ratio_mode_ = val; }
	inline void set_aspect_width(double val) { aspect_width_ = val; }
	inline void set_aspect_height(double val) { aspect_height_ = val; }
	inline void set_pixel_aspect_ration(double val) { pixel_aspect_ration_ = val; }
	inline void set_aperture_mode(int val) { aperture_mode_ = val; }
	inline void set_gate_fit(int val) { gate_fit_ = val; }
	inline void set_field_of_view(double val) { field_of_view_ = val; }
	inline void set_field_of_view_x(double val) { field_of_view_x_ = val; }
	inline void set_field_of_view_y(double val) { field_of_view_y_ = val; }
	inline void set_focal_length(double val) { focal_length_ = val; }
	inline void set_camera_format(int val) { camera_format_ = val; }
	inline void set_use_frame_color(bool val) { use_frame_color_ = val; }
	inline Array4d& mutable_frame_color() { return frame_color_; }
	inline void set_show_name(bool val) { show_name_ = val; }
	inline void set_show_info_on_moving(bool val) { show_info_on_moving_ = val; }
	inline void set_show_grid(bool val) { show_grid_ = val; }
	inline void set_show_optical_center(bool val) { show_optical_center_ = val; }
	inline void set_show_azimut(bool val) { show_azimut_ = val; }
	inline void set_show_time_code(bool val) { show_time_code_ = val; }
	inline void set_show_audio(bool val) { show_audio_ = val; }
	inline Array4d& mutable_audio_color() { return audio_color_; }
	inline void set_near_plane(double val) { near_plane_ = val; }
	inline void set_far_plane(double val) { far_plane_ = val; }
	inline void set_auto_compute_clip_planes(bool val) { auto_compute_clip_planes_ = val; }
	inline void set_film_width(double val) { film_width_ = val; }
	inline void set_film_height(double val) { film_height_ = val; }
	inline void set_film_aspect_ration(double val) { film_aspect_ration_ = val; }
	inline void set_film_squeeze_ration(double val) { film_squeeze_ration_ = val; }
	inline void set_film_format(int val) { film_format_ = val; }
	inline void set_film_offset_x(double val) { film_offset_x_ = val; }
	inline void set_film_offset_y(double val) { film_offset_y_ = val; }
	inline void set_pre_scale(double val) { pre_scale_ = val; }
	inline void set_film_translate_x(double val) { film_translate_x_ = val; }
	inline void set_film_translate_y(double val) { film_translate_y_ = val; }
	inline void set_film_roll_pivot_x(double val) { film_roll_pivot_x_ = val; }
	inline void set_film_roll_pivot_y(double val) { film_roll_pivot_y_ = val; }
	inline void set_film_roll_value(double val) { film_roll_value_ = val; }
	inline void set_film_roll_order(int val) { film_roll_order_ = val; }
	inline void set_view_camera_to_look_at(bool val) { view_camera_to_look_at_ = val; }
	inline void set_view_frustum_near_far_plane(bool val) { view_frustum_near_far_plane_ = val; }
	inline void set_view_frustum_back_plane_mode(int val) { view_frustum_back_plane_mode_ = val; }
	inline void set_back_plane_distance(double val) { back_plane_distance_ = val; }
	inline void set_back_plane_distance_mode(int val) { back_plane_distance_mode_ = val; }
	inline void set_view_frustum_front_plane_mode(int val) { view_frustum_front_plane_mode_ = val; }
	inline void set_front_plane_distance(double val) { front_plane_distance_ = val; }
	inline void set_front_plane_distance_mode(int val) { front_plane_distance_mode_ = val; }
	inline void set_lock_mode(bool val) { lock_mode_ = val; }
	inline void set_lock_interest_navigation(bool val) { lock_interest_navigation_ = val; }
	inline void set_back_plate_fit_image(bool val) { back_plate_fit_image_ = val; }
	inline void set_back_plate_crop(bool val) { back_plate_crop_ = val; }
	inline void set_back_plate_center(bool val) { back_plate_center_ = val; }
	inline void set_back_plate_keep_ration(bool val) { back_plate_keep_ration_ = val; }
	inline void set_background_alpha_treshold(double val) { background_alpha_treshold_ = val; }
	inline void set_back_plane_offset_x(double val) { back_plane_offset_x_ = val; }
	inline void set_back_plane_offset_y(double val) { back_plane_offset_y_ = val; }
	inline void set_back_plane_rotation(double val) { back_plane_rotation_ = val; }
	inline void set_back_plane_scale_x(double val) { back_plane_scale_x_ = val; }
	inline void set_back_plane_scale_y(double val) { back_plane_scale_y_ = val; }
	inline void set_show_backplate(bool val) { show_backplate_ = val; }
	inline void set_front_plate_fit_image(bool val) { front_plate_fit_image_ = val; }
	inline void set_front_plate_crop(bool val) { front_plate_crop_ = val; }
	inline void set_front_plate_center(bool val) { front_plate_center_ = val; }
	inline void set_front_plate_keep_ratio(bool val) { front_plate_keep_ratio_ = val; }
	inline void set_show_frontplate(bool val) { show_frontplate_ = val; }
	inline void set_front_plane_offset_x(double val) { front_plane_offset_x_ = val; }
	inline void set_front_plane_offset_y(double val) { front_plane_offset_y_ = val; }
	inline void set_front_plane_rotation(double val) { front_plane_rotation_ = val; }
	inline void set_front_plane_scale_x(double val) { front_plane_scale_x_ = val; }
	inline void set_front_plane_scale_y(double val) { front_plane_scale_y_ = val; }
	inline void set_foreground_opacity(double val) { foreground_opacity_ = val; }
	inline void set_display_safe_area(bool val) { display_safe_area_ = val; }
	inline void set_display_safe_area_on_render(bool val) { display_safe_area_on_render_ = val; }
	inline void set_safe_area_display_style(int val) { safe_area_display_style_ = val; }
	inline void set_safe_area_display_ratio(double val) { safe_area_display_ratio_ = val; }
	inline void set_use_2d_magnifier_zoom(bool val) { use_2d_magnifier_zoom_ = val; }
	inline void set__2d_magnifier_zoom(double val) { _2d_magnifier_zoom_ = val; }
	inline void set__2d_magnifier_x(double val) { _2d_magnifier_x_ = val; }
	inline void set__2d_magnifier_y(double val) { _2d_magnifier_y_ = val; }
	inline void set_projection_type(int val) { projection_type_ = val; }
	inline void set_ortho_zoom(double val) { ortho_zoom_ = val; }
	inline void set_use_real_time_dof_and_aa(bool val) { use_real_time_dof_and_aa_ = val; }
	inline void set_use_depth_of_field(bool val) { use_depth_of_field_ = val; }
	inline void set_focus_source(int val) { focus_source_ = val; }
	inline void set_focus_angle(double val) { focus_angle_ = val; }
	inline void set_focus_distance(double val) { focus_distance_ = val; }
	inline void set_use_antialiasing(bool val) { use_antialiasing_ = val; }
	inline void set_antialiasing_intensity(double val) { antialiasing_intensity_ = val; }
	inline void set_antialiasing_method(int val) { antialiasing_method_ = val; }
	inline void set_use_accumulation_buffer(bool val) { use_accumulation_buffer_ = val; }
	inline void set_frame_sample_count(int val) { frame_sample_count_ = val; }
	inline void set_frame_sampling_type(int val) { frame_sampling_type_ = val; }

	//
	// external use
	//
	void set_position(const Array4d& val) { position_ = val; }
	void set_up_vector(const Array4d& val) { up_vector_ = val; }
	void set_interest_position(const Array4d& val) { interest_position_ = val; }
	void set_background_color(const Array4d& val) { background_color_ = val; }
	void set_frame_color(const Array4d& val) { frame_color_ = val; }
	void set_audio_color(const Array4d& val) { audio_color_ = val; }
	
private:
	// boost serialization
	#include "UMCameraBos.h"

	/// geometrical
	Array4d position_;
	Array4d up_vector_;
	Array4d interest_position_;
	double roll_;
	double optical_center_x_;
	double optical_center_y_;
	Array4d background_color_;
	double turn_table_;
	bool display_turn_table_icon_;
	
	/// motion blur
	bool use_motion_blur_;
	bool use_real_time_motion_blur_;
	double motion_blur_intensity_;

	/// optical
	//EAspectRatioMode
	int aspect_ratio_mode_;
	double aspect_width_;
	double aspect_height_;
	double pixel_aspect_ration_;
	//EApertureMode
	int aperture_mode_;
	//EGateFit
	int gate_fit_;
	double field_of_view_;
	double field_of_view_x_;
	double field_of_view_y_;
	double focal_length_;
	//EFormat
	int camera_format_;
	
	///Frame
	bool use_frame_color_;
	Array4d frame_color_;

	///on screen display
	bool show_name_;
	bool show_info_on_moving_;
	bool show_grid_;
	bool show_optical_center_;
	bool show_azimut_;
	bool show_time_code_;
	bool show_audio_;
	Array4d audio_color_;

	/// clipping planes
	double near_plane_;
	double far_plane_;
	bool auto_compute_clip_planes_;

	/// camera film setting
	double film_width_;
	double film_height_;
	double film_aspect_ration_;
	double film_squeeze_ration_;
	//EApertureFormat
	int film_format_;
	double film_offset_x_;
	double film_offset_y_;
	double pre_scale_;
	double film_translate_x_;
	double film_translate_y_;
	double film_roll_pivot_x_;
	double film_roll_pivot_y_;
	double film_roll_value_;
	//EFilmRollOrder
	int film_roll_order_;

	/// camera view widget option
	bool view_camera_to_look_at_;
	bool view_frustum_near_far_plane_;
	//EFrontBackPlaneDisplayMode
	int view_frustum_back_plane_mode_;
	double back_plane_distance_;
	//EFrontBackPlaneDistanceMode
	int back_plane_distance_mode_;
	//EFrontBackPlaneDisplayMode
	int view_frustum_front_plane_mode_;
	double front_plane_distance_;
	//EFrontBackPlaneDistanceMode
	int front_plane_distance_mode_;

	/// camera lock mode
	bool lock_mode_;
	bool lock_interest_navigation_;

	/// background image display options
	bool back_plate_fit_image_;
	bool back_plate_crop_;
	bool back_plate_center_;
	bool back_plate_keep_ration_;
	double background_alpha_treshold_;
	double back_plane_offset_x_;
	double back_plane_offset_y_;
	double back_plane_rotation_;
	double back_plane_scale_x_;
	double back_plane_scale_y_;
	bool show_backplate_;

	// TODO: FbxPropertyT<FbxReference> BackgroundTexture;

	/// foreground image display options
	bool front_plate_fit_image_;
	bool front_plate_crop_;
	bool front_plate_center_;
	bool front_plate_keep_ratio_;
	bool show_frontplate_;
	double front_plane_offset_x_;
	double front_plane_offset_y_;
	double front_plane_rotation_;
	double front_plane_scale_x_;
	double front_plane_scale_y_;
	// TODO: FbxPropertyT<FbxReference> ForegroundTexture;
	double foreground_opacity_;

	/// safe area
	bool display_safe_area_;
	bool display_safe_area_on_render_;
	//ESafeAreaStyle
	int safe_area_display_style_;
	double safe_area_display_ratio_;

	/// 2d magnifier
	bool use_2d_magnifier_zoom_;
	double _2d_magnifier_zoom_;
	double _2d_magnifier_x_;
	double _2d_magnifier_y_;

	/// projection type
	//EProjectionType
	int projection_type_;
	double ortho_zoom_;

	/// depth of field & anti aliasing
	bool use_real_time_dof_and_aa_;
	bool use_depth_of_field_;
	//EFocusDistanceSource
	int focus_source_;
	double focus_angle_;
	double focus_distance_;
	bool use_antialiasing_;
	double antialiasing_intensity_;
	//EAntialiasingMethod
	int antialiasing_method_;

	///accumulation buffer
	bool use_accumulation_buffer_;
	int frame_sample_count_;
	//ESamplingType
	int frame_sampling_type_;
};

/*---------------------------------------------------------------------------*
 * UMSkeleton
 *---------------------------------------------------------------------------*/
class UMSkeleton : public UMNode {
public:

	enum EType
	{
		eRoot,
		eLimb,
		eLimbNode,
		eEffector
	};

	UMSkeleton() : 
		type_(-1), limb_length_(0), size_(0) {}
	virtual ~UMSkeleton() {}
	
	typedef std::map<int, UMSkeleton> IDToSkeletonMap;
	typedef std::vector<UMSkeleton> SkeletonList;
	
	bool operator==(const UMSkeleton& skeleton)
	{
		return (id() == skeleton.id());
	}
	
	// getter
	inline int type() const { return type_; }
	inline const double limb_length() const { return limb_length_; }
	inline const double size() const { return size_; }
	
	// setter
	inline void set_type(int type) { type_ = type; }
	inline void set_limb_length(double value) { limb_length_ = value; }
	inline void set_size(double value) { size_ = value; }
	

private:
	// boost serialization
	#include "UMSkeletonBos.h"

	//	FbxSkeleton::EType or -1
	int type_;
	double limb_length_;
	double size_;
};

/*---------------------------------------------------------------------------*
 * UMPose
 *---------------------------------------------------------------------------*/
class UMPose {
public:
	typedef std::vector<UMPose> PoseList;

	UMPose() : bind_pose_(false), rest_pose_(false) {}
	virtual ~UMPose() {}

	// getter
	inline bool bind_pose() const { return bind_pose_; }
	inline bool rest_pose() const { return rest_pose_; }
	inline const IntList& node_id_list() const { return node_id_list_; }
	inline const UMNode::NodePointerList& node_list() const { return node_list_; }
	inline const Array44dList& matrix_list() const { return matrix_list_; }
	inline const BoolList& matrix_is_local_list() const { return matrix_is_local_list_; }

	// setter
	inline void set_bind_pose(bool bind) { bind_pose_ = bind; }
	inline void set_rest_pose(bool rest) { rest_pose_ = rest; }
	inline IntList& mutable_node_id_list() { return node_id_list_; }
	inline UMNode::NodePointerList& mutable_node_list() { return node_list_; }
	inline Array44dList& mutable_matrix_list() { return matrix_list_; }
	inline BoolList& mutable_matrix_is_local_list() { return matrix_is_local_list_; }
	
private:
	// boost serialization
	#include "UMPoseBos.h"
	
	bool bind_pose_;
	bool rest_pose_;
	IntList node_id_list_;
	BoolList matrix_is_local_list_;
	UMNode::NodePointerList node_list_;
	Array44dList matrix_list_;		
};

/*---------------------------------------------------------------------------*
 * UMTexture
 *---------------------------------------------------------------------------*/
class UMTexture {
public:
	UMTexture() : blend_mode_(-1), alpha_(1.0) {}
	virtual ~UMTexture() {}

	enum EBlendMode
	{
		eTranslucent,
		eAdditive,
		eModulate,
		eModulate2,
		eOver,
		eNormal,
		eDissolve,
		eDarken,
		eColorBurn,
		eLinearBurn,
		eDarkerColor,
		eLighten, 
		eScreen,
		eColorDodge,
		eLinearDodge,
		eLighterColor,
		eSoftLight, 
		eHardLight, 
		eVividLight,
		eLinearLight,
		ePinLight,
		eHardMix, 
		eDifference,
		eExclusion, 
		eSubtract,
		eDivide,
		eHue, 
		eSaturation,
		eColor, 
		eLuminosity,
		eOverlay,
		eBlendModeCount,
	};

	// getter
	inline const std::string& name() const { return name_; }
	inline const std::string& file_name() const { return file_name_; }
	inline const std::string& relative_file_name() const { return relative_file_name_; }
	inline const int blend_mode() const { return blend_mode_; }
	inline const double alpha() const { return alpha_; }

	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline void set_file_name(const std::string& file_name) { file_name_ = file_name; }
	inline void set_relative_file_name(const std::string& relative_file_name) { relative_file_name_ = relative_file_name; }
	inline void set_blend_mode(int blend_mode) { blend_mode_ = blend_mode; }
	inline void set_alpha(double alpha) { alpha_ = alpha; }
	
private:
	// boost serialization
	#include "UMTextureBos.h"

	std::string name_;
	std::string file_name_;
	std::string relative_file_name_;

	//	FbxLayeredTexture::EBlendMode or -1
	int blend_mode_;
	double alpha_;
};

/*---------------------------------------------------------------------------*
 * UMCluster
 *---------------------------------------------------------------------------*/
class UMCluster {
public:

	typedef std::vector<UMCluster> ClusterList;
	
	enum ELinkMode
	{
		eNormalize,
		eAdditive,
		eTotalOne,
	};

	UMCluster() :
		link_node_(NULL), link_node_id_(-1), link_mode_(-1) {}
	virtual ~UMCluster() {}

	// getter
	inline const std::string& name() const { return name_; }
	inline const IntList& index_list() const { return index_list_; }
	inline const DoubleList& weight_list() const { return weight_list_; }
	inline int link_mode() const { return link_mode_; }
	inline const UMNode* link_node() const { return link_node_; }
	inline int link_node_id() const { return link_node_id_; }
	
	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline IntList& mutable_index_list() { return index_list_; }
	inline DoubleList& mutable_weight_list() { return weight_list_; }
	inline void set_link_mode(int link_mode) { link_mode_ = link_mode; }
	inline void set_link_node(UMNode* link_node) { link_node_ = link_node; }
	inline void set_link_node_id(int link_node_id) { link_node_id_ = link_node_id; }
	
	// external use
	void add_index(int index) { mutable_index_list().push_back(index); }
	void add_weight(double weight) { mutable_weight_list().push_back(weight); }
	
private:
	// boost serialization
	#include "UMClusterBos.h"

	std::string name_;
	IntList index_list_;
	DoubleList weight_list_;
	// FbxCluster::ELinkMode or -1
	int link_mode_;
	UMNode* link_node_;
	int link_node_id_;
};

/*---------------------------------------------------------------------------*
 * UMDeformer
 *---------------------------------------------------------------------------*/
class UMDeformer {
public:
	enum EDeformerType
	{
		eUnknown,
		eSkin,
		eBlendShape,
		eVertexCache,
	};

	UMDeformer() {}

	explicit UMDeformer(int deformerType) : 
		geometry_node_(NULL), 
		geometry_node_id_(-1), 
		deformer_type_(deformerType) {}

	virtual ~UMDeformer() {}

	// getter
	inline const UMNode* geometry_node() const { return geometry_node_; }
	inline int geometry_node_id() const { return geometry_node_id_; }
	inline int deformer_type() const { return deformer_type_; }

	// setter
	inline void set_geometry_node(UMNode* geometry_node) { geometry_node_ = geometry_node; }
	inline void set_geometry_node_id(int geometry_id) { geometry_node_id_ = geometry_id; }
	
protected:
	// boost serialization
	#include "UMDeformerBos.h"

	UMNode* geometry_node_;
	int geometry_node_id_;
	int deformer_type_;
};

/*---------------------------------------------------------------------------*
 * UMShape
 *---------------------------------------------------------------------------*/
class UMShape {
public:
	typedef std::vector<UMShape> ShapeList;

	UMShape() : base_geometry_node_(NULL), base_geometry_node_id_(-1) {}
	virtual ~UMShape() {}

	// getter
	inline const std::string& name() const { return name_; }
	inline const UMNode* base_geometry_node() const { return base_geometry_node_; }
	inline int base_geometry_node_id() const { return base_geometry_node_id_; }

	inline const IntList& vertex_index_list() const { return vertex_index_list_; }
	inline const IntList& normal_index_list() const { return normal_index_list_; }
	inline const DoubleListVec& vertex_list() const { return vertex_list_; }
	inline const DoubleListVec& normal_list() const { return normal_list_; }
	
	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline void set_base_geometry_node(UMNode* geometry_node) { base_geometry_node_ = geometry_node; }
	inline void set_base_geometry_node_id(int geometry_id) { base_geometry_node_id_ = geometry_id; }
	
	inline IntList& mutable_vertex_index_list() { return vertex_index_list_; }
	inline IntList& mutable_normal_index_list() { return normal_index_list_; }
	inline DoubleListVec& mutable_vertex_list() { return vertex_list_; }
	inline DoubleListVec& mutable_normal_list() { return normal_list_; }

	// external use
	void add_vertex_index(int vertex_index) { mutable_vertex_index_list().push_back(vertex_index); }
	void add_normal_index(int normal_index) { mutable_normal_index_list().push_back(normal_index); }
	void add_vertex(const DoubleList& vertex) { mutable_vertex_list().push_back(vertex); }
	void add_normal(const DoubleList& normal) { mutable_normal_list().push_back(normal); }
	
private:
	// boost serialization
	#include "UMShapeBos.h"

	std::string name_;
	UMNode* base_geometry_node_;
	int base_geometry_node_id_;
	IntList vertex_index_list_;
	IntList normal_index_list_;
	DoubleListVec vertex_list_;
	DoubleListVec normal_list_;
};

/*---------------------------------------------------------------------------*
 * UMBlendShapeChannel
 *---------------------------------------------------------------------------*/
class UMBlendShapeChannel {
public:
	typedef std::vector<UMBlendShapeChannel> ChannelList;

	UMBlendShapeChannel() : deform_percent_(0.0) {}
	virtual ~UMBlendShapeChannel() {}

	// getter
	inline const UMShape::ShapeList& target_shape_list() const { return target_shape_list_; }
	inline const DoubleList& full_weight_list() const { return full_weight_list_; }
	inline double deform_percent() const { return deform_percent_; }

	// setter
	inline UMShape::ShapeList& mutable_target_shape_list() { return target_shape_list_; }
	inline DoubleList& mutable_full_weight_list() { return full_weight_list_; }
	inline double mutable_deform_percent() { return deform_percent_; }

	// external use
	void add_target_shape(const UMShape& shape) { target_shape_list_.push_back(shape); }
	void add_full_weight(double weight) { full_weight_list_.push_back(weight); }
	void set_deform_percent(double percent) { deform_percent_ = percent; }
	
private:
	// boost serialization
	#include "UMBlendShapeChannelBos.h"

	UMShape::ShapeList target_shape_list_;
	DoubleList full_weight_list_;
	double deform_percent_;
};

/*---------------------------------------------------------------------------*
 * UMBlendShape
 *---------------------------------------------------------------------------*/
class UMBlendShape : public UMDeformer {
public:
	typedef std::vector<UMBlendShape> BlendShapeList;

	UMBlendShape() : UMDeformer(eBlendShape) {}
	virtual ~UMBlendShape() {}

	// getter
	inline const std::string& name() const { return name_; }
	inline const UMBlendShapeChannel::ChannelList& blend_shape_channel_list() const { return blend_shape_channel_list_; }

	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline UMBlendShapeChannel::ChannelList& mutable_blend_shape_channel_list() { return blend_shape_channel_list_; }

	// external use
	void add_blend_shape_channel(const UMBlendShapeChannel& channel) { blend_shape_channel_list_.push_back(channel); }

private:
	// boost serialization
	#include "UMBlendShapeBos.h"

	std::string name_;
	UMBlendShapeChannel::ChannelList blend_shape_channel_list_;
};

/*---------------------------------------------------------------------------*
 * UMSkin
 *---------------------------------------------------------------------------*/
class UMSkin : public UMDeformer {
public:
	typedef std::vector<UMSkin> SkinList;
	
	UMSkin() : UMDeformer(eSkin) {}
	virtual ~UMSkin() {}

	// getter
	inline const std::string& name() const { return name_; }
	inline const UMCluster::ClusterList& cluster_list() const { return cluster_list_; }
	
	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline UMCluster::ClusterList& mutable_cluster_list() { return cluster_list_; }
	
	// external use
	void add_cluster(const UMCluster& cluster) { mutable_cluster_list().push_back(cluster); }
	
private:
	// boost serialization
	#include "UMSkinBos.h"

	std::string name_;
	UMCluster::ClusterList cluster_list_;
		
};

/*---------------------------------------------------------------------------*
 * UMMaterial
 *---------------------------------------------------------------------------*/
class UMMaterial {
public:
	typedef std::vector<UMMaterial> MaterialList;
	typedef std::vector<UMTexture> TextureList;
	typedef std::vector<TextureList> LayeredTextureList;

	UMMaterial() : 
		shininess_(0.0), 
		transparency_factor_(1.0),
		reflection_factor_(0.0),
		diffuse_factor_(0.0), 
		specular_factor_(0.0),
		emissive_factor_(0.0),
		ambient_factor_(0.0),
		bump_factor_(0.0),
		vector_displacement_factor_(0.0)
		{}

	virtual ~UMMaterial() {}

	// getter
	inline const std::string& name() const { return name_; }
	inline const Array4d& ambient() const { return ambient_; }
	inline const Array4d& diffuse() const { return diffuse_; }
	inline const Array4d& specular() const { return specular_; }
	inline const Array4d& emissive() const { return emissive_; }
	inline const Array4d& refrection() const { return refrection_; }
	inline const Array4d& transparent() const { return transparent_; }
	inline const Array4d& bump() const { return bump_; }
	inline const Array4d& vector_displacement() const { return vector_displacement_; }
	inline const double transparency_factor() const { return transparency_factor_; }
	inline const double shininess() const { return shininess_; }
	inline const double reflection_factor() const { return reflection_factor_; }
	inline const double diffuse_factor() const { return diffuse_factor_; }
	inline const double specular_factor() const { return specular_factor_; }
	inline const double emissive_factor() const { return emissive_factor_; }
	inline const double ambient_factor() const { return ambient_factor_; }
	inline const double bump_factor() const { return bump_factor_; }
	inline const double vector_displacement_factor() const { return vector_displacement_factor_; }
	inline const TextureList& texture_list() const { return texture_list_; }
	inline const LayeredTextureList& layered_texture_list() const { return layered_texture_list_; }
	
	// setter
	inline void set_name(const std::string& name) { name_ = name; }
	inline Array4d& mutable_ambient() { return ambient_; }
	inline Array4d& mutable_diffuse() { return diffuse_; }
	inline Array4d& mutable_specular() { return specular_; }
	inline Array4d& mutable_emissive() { return emissive_; }
	inline Array4d& mutable_refrection() { return refrection_; }
	inline Array4d& mutable_transparent() { return transparent_; }
	inline Array4d& mutable_bump() { return bump_; }
	inline Array4d& mutable_vector_displacement() { return vector_displacement_; }
	inline void set_transparency_factor(double value) { transparency_factor_ = value; }
	inline void set_shininess(double value) { shininess_ = value; }
	inline void set_reflection_factor(double value) { reflection_factor_ = value; }
	inline void set_diffuse_factor(double value) { diffuse_factor_ = value; }
	inline void set_specular_factor(double value) { specular_factor_ = value; }
	inline void set_emissive_factor(double value) { emissive_factor_ = value; }
	inline void set_ambient_factor(double value) { ambient_factor_ = value; }
	inline void set_bump_factor(double value) { bump_factor_ = value; }
	inline void set_vector_displacement_factor(double value) { vector_displacement_factor_ = value; }
	inline TextureList& mutable_texture_list() { return texture_list_; }
	inline LayeredTextureList& mutable_layered_texture_list() { return layered_texture_list_; }
	
	// external use
	void set_ambient(const Array4d& ambient) { ambient_ = ambient; }
	void set_diffuse(const Array4d& diffuse) { diffuse_ = diffuse; }
	void set_specular(const Array4d& specular) { specular_ = specular; }
	void set_emissive(const Array4d& emissive) { emissive_ = emissive; }
	void set_refrection(const Array4d& refrection) { refrection_ = refrection; }
	void set_transparent(const Array4d& transparent) { transparent_ = transparent; }
	void set_bump(const Array4d& bump) { bump_ = bump; }
	void set_vector_displacement(const Array4d& vector_displacement) { vector_displacement_ = vector_displacement; }
	void add_texture(const UMTexture& texture) { mutable_texture_list().push_back(texture); }
	void add_layered_texture(const TextureList& texture_list) { mutable_layered_texture_list().push_back(texture_list); }
	
private:
	// boost serialization
	#include "UMMaterialBos.h"

	std::string name_;
	
	Array4d ambient_;
	Array4d diffuse_;
	Array4d specular_;
	Array4d emissive_;
	Array4d refrection_;
	Array4d transparent_;
	Array4d bump_;
	Array4d vector_displacement_;
	double shininess_;
	double transparency_factor_;
	double reflection_factor_;
	double diffuse_factor_;
	double specular_factor_;
	double emissive_factor_;
	double ambient_factor_;
	double bump_factor_;
	double vector_displacement_factor_;

	TextureList texture_list_;
	LayeredTextureList layered_texture_list_;
};

/*---------------------------------------------------------------------------*
 * UMMesh
 *---------------------------------------------------------------------------*/
class UMMesh : public UMNode {
public:
	typedef std::map<int, UMMesh> IDToMeshMap;
	typedef std::vector<UMMesh> MeshList;

	UMMesh() {
		DoubleListVec normal_list_;
		DoubleListVec uv_list_;
		DoubleListVec vertex_color_list_;
		layered_uv_list_.push_back(uv_list_);
		layered_normal_list_.push_back(normal_list_);
		layered_vertex_color_list_.push_back(vertex_color_list_);
	}
	virtual ~UMMesh() {}
	
	bool operator==(const UMMesh& mesh)
	{
		return (id() == mesh.id());
	}
	
	// getter
	inline const IntListVec& vertex_index_list() const { return vertex_index_list_; }
	inline const DoubleListVec& vertex_list() const { return vertex_list_; }
	inline const IntList& material_index_list() const { return material_index_; }
	inline const UMMaterial::MaterialList& material_list() const { return material_list_; }
	inline const UMSkin::SkinList& skin_list() const { return skin_list_; }
	inline const UMBlendShape::BlendShapeList& blend_shape_list() const { return blend_shape_list_; }
	inline const DoubleListVec& normal_list(int layer = 0) const { return layered_normal_list_.at(layer); }
	inline const DoubleListVec& vertex_color_list(int layer = 0) const { return layered_vertex_color_list_.at(layer); }
	inline const DoubleListVec& uv_list(int layer = 0) const { return layered_uv_list_.at(layer); }
	
	inline const LayeredDoubleListVec& layered_uv_list() const { return layered_uv_list_; }
	inline const LayeredDoubleListVec& layered_normal_list() const { return layered_normal_list_; }
	inline const LayeredDoubleListVec& layered_vertex_color_list() const { return layered_vertex_color_list_; }

	// setter
	inline IntListVec& mutable_vertex_index_list() { return vertex_index_list_; }
	inline DoubleListVec& mutable_vertex_list() { return vertex_list_; }
	inline IntList& mutable_material_index() { return material_index_; }
	inline UMMaterial::MaterialList& mutable_material_list() { return material_list_; }
	inline UMSkin::SkinList& mutable_skin_list() { return skin_list_; }
	inline UMBlendShape::BlendShapeList& mutable_blend_shape_list() { return blend_shape_list_; }
	inline DoubleListVec& mutable_normal_list(int layer = 0) { return layered_normal_list_.at(layer); }
	inline DoubleListVec& mutable_vertex_color_list(int layer = 0) { return layered_vertex_color_list_.at(layer); }
	inline DoubleListVec& mutable_uv_list(int layer = 0) { return layered_uv_list_.at(layer); }

	inline LayeredDoubleListVec& mutable_layered_uv_list() { return layered_uv_list_; }
	inline LayeredDoubleListVec& mutable_layered_normal_list() { return layered_normal_list_; }
	inline LayeredDoubleListVec& mutable_layered_vertex_color_list() { return layered_vertex_color_list_; }
	
	// external use
	void add_vertex_index(const IntList& vertex_index) { mutable_vertex_index_list().push_back(vertex_index); }
	void add_vertex(const DoubleList& vertex) { mutable_vertex_list().push_back(vertex); }
	void add_material_index(int index) { mutable_material_index().push_back(index);	}
	void add_material(const UMMaterial& material) { mutable_material_list().push_back(material); }
	void add_skin(const UMSkin& skin) {	mutable_skin_list().push_back(skin); }
	void add_blend_shape(const UMBlendShape& blend_shape) { mutable_blend_shape_list().push_back(blend_shape); }
	void add_vertex_color(const DoubleList& vertex_color, int layer = 0) { mutable_vertex_color_list(layer).push_back(vertex_color); }
	void add_normal(const DoubleList& normal, int layer = 0) {	mutable_normal_list(layer).push_back(normal); }
	void add_uv(const DoubleList& uv, int layer = 0) {	mutable_uv_list(layer).push_back(uv); }
	
	int add_uv_layer() { layered_uv_list_.push_back(DoubleListVec()); return static_cast<int>(layered_uv_list_.size()-1); }
	int add_normal_layer() { layered_normal_list_.push_back(DoubleListVec()); return static_cast<int>(layered_normal_list_.size()-1); }
	int add_vertex_color_layer() { layered_vertex_color_list_.push_back(DoubleListVec()); return static_cast<int>(layered_vertex_color_list_.size()-1); }
	
private:
	// boost serialization
	#include "UMMeshBos.h"

	IntListVec vertex_index_list_;
	DoubleListVec vertex_list_;
	IntList material_index_;
	UMMaterial::MaterialList material_list_;
	UMSkin::SkinList skin_list_;
	UMBlendShape::BlendShapeList blend_shape_list_;

	LayeredDoubleListVec layered_uv_list_;
	LayeredDoubleListVec layered_normal_list_;
	LayeredDoubleListVec layered_vertex_color_list_;
		
};

/*---------------------------------------------------------------------------*
 * UMObject
 *---------------------------------------------------------------------------*/
class UMObject {
public:	
	typedef std::map<int, UMNode* > IDToNodePointerMap;

	UMObject() {}
	virtual ~UMObject() {}

	static UMObjectPtr create_object() { return UMObjectPtr(new UMObject()); }

	// mesh
	inline const UMMesh::IDToMeshMap& mesh_map() const { return mesh_map_; }
	inline UMMesh::IDToMeshMap& mutable_mesh_map() { return mesh_map_; }

	// skeleton
	inline const UMSkeleton::IDToSkeletonMap& skeleton_map() const { return skeleton_map_; }
	inline UMSkeleton::IDToSkeletonMap& mutable_skeleton_map() { return skeleton_map_; }
	
	// camera
	inline const UMCamera::IDToCameraMap& camera_map() const { return camera_map_; }
	inline UMCamera::IDToCameraMap& mutable_camera_map() { return camera_map_; }
	
	// other node list
	inline const UMNode::IDToNodeMap& other_node_map() const { return other_node_map_; }
	inline UMNode::IDToNodeMap& mutable_other_node_map() { return other_node_map_; }

	// pose
	inline const UMPose::PoseList& pose_list() const { return pose_list_; }
	inline UMPose::PoseList& mutable_pose_list() { return pose_list_; }

	// id
	inline int next_id() { 
		if (!mutable_id_to_node_map().empty()) {
			return (*mutable_id_to_node_map().rbegin()).first + 1;
		} else {
			return 0;
		}
	}

	// setter (external use)
	void add_mesh(UMMesh& mesh) {
		if (mesh.id() < 0)	{
			mesh.set_id(next_id());
		}
		mutable_mesh_map()[mesh.id()] = mesh;
		mutable_id_to_node_map()[mesh.id()] = &mutable_mesh_map()[mesh.id()];
	}
	void add_skeleton(UMSkeleton& skeleton) {
		if (skeleton.id() < 0)	{
			skeleton.set_id(next_id());
		}
		mutable_skeleton_map()[skeleton.id()] = skeleton;
		mutable_id_to_node_map()[skeleton.id()] = &mutable_skeleton_map()[skeleton.id()];
	}
	void add_camera(UMCamera& camera) {
		if (camera.id() < 0)	{
			camera.set_id(next_id());
		}
		mutable_camera_map()[camera.id()] = camera;
		mutable_id_to_node_map()[camera.id()] = &mutable_skeleton_map()[camera.id()];
	}
	void add_other_node(UMNode& node) {
		if (node.id() < 0)	{
			node.set_id(next_id());
		}
		mutable_other_node_map()[node.id()] = node;
		mutable_id_to_node_map()[node.id()] = &mutable_other_node_map()[node.id()];
	}
	
	// internal use
	inline const IDToNodePointerMap& id_to_node_map() const { return id_to_node_map_; }
	inline IDToNodePointerMap& mutable_id_to_node_map() { return id_to_node_map_; }
	
	// internal use
	// re bind all node from node_id.
	// do after loading a serialized file
	static bool re_bind_all_nodes(UMObjectPtr object);

private:
	// boost serialization
	#include "UMObjectBos.h"

	UMMesh::IDToMeshMap mesh_map_;
	UMSkeleton::IDToSkeletonMap skeleton_map_;
	UMCamera::IDToCameraMap camera_map_;
	UMNode::IDToNodeMap other_node_map_;
	UMPose::PoseList pose_list_;

	IDToNodePointerMap id_to_node_map_;

};

} // namespace UM
