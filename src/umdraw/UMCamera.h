/**
 * @file UMCamera.h
 * a Camera
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
#include "UMMacro.h"
#include "UMMathTypes.h"
#include "UMMatrix.h"
#include "UMMath.h"
#include "UMNode.h"

namespace umdraw
{

class UMCamera;
typedef std::shared_ptr<UMCamera> UMCameraPtr;
typedef std::weak_ptr<UMCamera> UMCameraWeakPtr;
typedef std::vector<UMCameraPtr> UMCameraList;

/**
 * a camera
 */
class UMCamera : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMCamera);

public:
	explicit UMCamera(bool is_ortho, int width, int height) {
		init(is_ortho, width, height);
	}
	~UMCamera();

	/**
	 * initialize camera
	 */
	void init(bool is_ortho, int width, int height);

	/**
	 * is ortho
	 */
	bool is_ortho() const { return is_ortho_; }
	
	/**
	 * update camera matrix
	 */
	void update();

	/**
	 * update camera matrix from node
	 */
	void update_from_node();

	/**
	 * draw camera
	 */
	void draw() {}
	
	/**
	 * get node
	 */
	UMNodePtr node() { return node_; }
	
	/**
	 * get node
	 */
	UMNodePtr node() const { return node_; }

	/** 
	 * get view matrix
	 */
	const UMMat44d& view_matrix() const { return view_matrix_; }

	/** 
	 * set view matrix
	 */
	void set_view_matrix(const UMMat44d& mat) { view_matrix_ = mat; }
	
	/** 
	 * get projection matrix
	 */
	const UMMat44d& projection_matrix() const { return projection_matrix_; }

	/** 
	 * set projection matrix
	 */
	void set_projection_matrix(const UMMat44d& mat) { projection_matrix_ = mat; }
	
	/**
	 * get view projection matrix
	 */
	const UMMat44d& view_projection_matrix() const { return view_projection_matrix_; }

	/** 
	 * get aspect
	 */
	double aspect() const { return aspect_; }
	
	/**
	 * get vertical fov
	 */
	double fov_y() const { return fov_y_; }

	/**
	 * get horizontal fov
	 */
	double fov_x() const { return umbase::um_to_degree(2.0 * atan(tan( fov_y() / 2.0) *aspect())); }

	/**
	 * get near
	 */
	double near_plane() const { return near_; }

	/**
	 * get far
	 */
	double far_plane() const { return far_; }

	/**
	 * get positoin
	 */
	const UMVec3d& position() const { return position_; }

	/**
	 * set position
	 */
	void set_position(const UMVec3d& position) { position_ = position; }

	/**
	 * get target
	 */
	const UMVec3d& target() const { return target_; }

	/**
	 * get up
	 */
	const UMVec3d& up() const { return up_; }

	/**
	 * rotate camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void rotate(double mx, double my);

	/**
	 * zoom camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void zoom(double mx, double my);

	/**
	 * dolly camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void dolly(double mx, double my);
	
	/**
	 * pan camera
	 * @param [in] mx x-axis mouse move
	 * @param [in] my y-axis mouse move
	 */
	void pan(double mx, double my);

	/**
	 * fit camera
	 */
	void fit(UMBoxPtr bbox);

private:
	UMMat44d view_matrix_;
	UMMat44d projection_matrix_;
	UMMat44d view_projection_matrix_;
	UMVec3d rotation_;

	double aspect_;
	double fov_y_;
	double near_;
	double far_;
	UMVec3d position_;
	UMVec3d target_;
	UMVec3d up_;
		
	double theta_;
	double phi_;

	// for performance
	double inverted_width_;
	double inverted_height_;
	UMVec3d generate_ray_x_scale_;
	UMVec3d generate_ray_y_scale_;
	UMVec3d generate_ray_adder_;
	
	UMNodePtr node_;
	bool is_ortho_;
};

} // umdraw
