/**
 * @file UMScene.h
 * 3D scene including many objects.
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <vector>
#include <string>
#include <bitset>
#include "UMVector.h"
#include "UMCamera.h"
#include "UMLight.h"
#include "UMMeshGroup.h"
#include "UMNode.h"

namespace umbase
{
class UMEvent;
typedef std::shared_ptr<UMEvent> UMEventPtr;
} // umbase

/// umdraw
namespace umdraw
{

class UMScene;
typedef std::shared_ptr<UMScene> UMScenePtr;
typedef std::weak_ptr<UMScene> UMSceneWeakPtr;

class UMBvh;
typedef std::shared_ptr<UMBvh> UMBvhPtr;

class UMLine;
typedef std::shared_ptr<UMLine> UMLinePtr;
typedef std::vector<UMLinePtr> UMLineList;

class UMCurve;
typedef std::shared_ptr<UMCurve> UMCurvePtr;
typedef std::vector<UMCurvePtr> UMCurveList;

/**
 * 3D scene including many objects, lights, cameras, ...
 */
class UMScene : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMScene);

public:
	enum VisibleType
	{
		eMesh,
		eNode,
		eLine,
		eCurve,
		ePoint,
		eCamera,
		eLight,
		eTemporaryLine,
		eTemporaryMesh
	};

	UMScene() { init(1280, 720); }
	UMScene(int width, int height) { init(width, height); }
	~UMScene() {}

	/** 
	 * init scene
	 */
	void init(int width, int height);
	
	/**
	 * get width
	 */
	int width() const { return width_; }
	
	/**
	 * get height
	 */
	int height() const { return height_; }

	/**
	 * clear all data
	 */
	void clear_geometry();

	/**
	 * get camera
	 */
	UMCameraPtr camera();

	/**
	 * set camera
	 */
	void set_current_camera(UMCameraPtr camera);

	/**
	 * get camera list
	 */
	const UMCameraList& camera_list() const { return camera_list_; }

	/**
	 * get camera list
	 */
	UMCameraList& mutable_camera_list() { return camera_list_; }

	/**
	 * get mesh group list
	 */
	const UMMeshGroupList& mesh_group_list() const { return mesh_group_list_; }
	
	/**
	 * get mesh group list
	 */
	UMMeshGroupList& mutable_mesh_group_list() { return mesh_group_list_; }
	
	/**
	 * get line list
	 */
	const UMLineList& line_list() const { return line_list_; }
	
	/**
	 * get line list
	 */
	UMLineList& mutable_line_list() { return line_list_; }

	/**
	* get curve list
	*/
	const UMCurveList& curve_list() const { return curve_list_; }

	/**
	* get curve list
	*/
	UMCurveList& mutable_curve_list() { return curve_list_; }

	/**
	 * get light list
	 */
	const UMLightList& light_list() const { return light_list_; }

	/**
	 * get light list
	 */
	UMLightList& mutable_light_list() { return light_list_; }

	/**
	 * get node list
	 */
	const UMNodeList& node_list() const { return node_list_; }

	/**
	 * get node list
	 */
	UMNodeList& mutable_node_list() { return node_list_; }
	
	/**
	 * get background color
	 */
	UMVec3d background_color() const { return UMVec3d(0.1, 0.1, 0.1); }
	
	/**
	 * get background image
	 */
	void set_background_image(umimage::UMImagePtr image);

	/**
	 * get foreground image
	 */ 
	void set_foreground_image(umimage::UMImagePtr image);

	/**
	 * get background image
	 */
	umimage::UMImagePtr background_image() { return background_image_; }

	/**
	 * get foreground image
	 */ 
	umimage::UMImagePtr foreground_image() { return foreground_image_; }

	/**
	 * load model data
	 * @param [in] file_path source file path
	 */
	bool load(const umstring& file_path);
	
	/**
	 * load model data from memory
	 * @param [in] src string as char array
	 */
	bool load_from_memory(const std::string& src);

	/**
	* save model data
	* @param [in] file_path dst file path
	*/
	bool save(const umstring& file_path) const;

	/**
	 * get total polygons
	 */
	size_t total_polygon_size() const;

	/** 
	 * deformation setting
	 */
	void set_enable_deform(bool enable) { is_enable_deform_ = enable; }
	
	/** 
	 * deformation is enable or not
	 */
	bool is_enable_deform() const { return is_enable_deform_; }
	
	/** 
	 *  get visibility
	 */
	bool is_visible(VisibleType type) const { return visibility_.test(type); }
		
	/** 
	 *  set visibility
	 */
	void set_visible(VisibleType type, bool visible);
	
	/**
	 * get temporary line list
	 */
	const UMLineList& temporary_line_list() const { return temporary_line_list_; }
	
	/**
	 * get temporary line list
	 */
	UMLineList& mutable_temporary_line_list() { return temporary_line_list_; }
	
	/**
	 * get temporary mesh list
	 */
	const UMMeshList& temporary_mesh_list() const { return temporary_mesh_list_; }
	
	/**
	 * get temporary mesh list
	 */
	UMMeshList& mutable_temporary_mesh_list() { return temporary_mesh_list_; }

	void resize(int width, int height);

private:
	std::bitset<32> visibility_;
	int width_;
	int height_;
	bool is_enable_deform_;

	UMCameraList camera_list_;
	UMLightList light_list_;

	UMNodeList node_list_;
	UMMeshGroupList mesh_group_list_;
	UMLineList line_list_;
	UMCurveList curve_list_;
	umimage::UMImagePtr background_image_;
	umimage::UMImagePtr foreground_image_;

	UMLineList temporary_line_list_;
	UMMeshList temporary_mesh_list_;
};

} // umdraw
