/**
 * @file UMCamera.cpp
 * a Camera
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMCamera.h"

#include "UMMathTypes.h"
#include "UMMath.h"
#include "UMVector.h"
#include "UMMatrix.h"
#include "UMBox.h"

namespace umdraw
{
	
UMCamera::~UMCamera()
{
}

void UMCamera::init(bool is_ortho, int width, int height)
{
	view_matrix_.identity();
	projection_matrix_.identity();
	view_projection_matrix_.identity();
	generate_ray_x_scale_ = UMVec3d(0);
	generate_ray_y_scale_ = UMVec3d(0);
	generate_ray_adder_ = UMVec3d(0);
	
	is_ortho_ = is_ortho;
	aspect_ = (static_cast<double>(width) / static_cast<double>(height));
	fov_y_ = 45.0;
	near_ = 1.0;
	far_ = 1000.0;
	position_ = UMVec3d(0,  15, 50);
	target_ = UMVec3d(0, 15, 0);
	up_ = UMVec3d(0, 1, 0);
	rotation_ = UMVec3d(0.0, 0.0, 0.0);
	theta_ = 0.0;
	phi_ = 0.0;
	inverted_width_ = 1.0 / static_cast<double>(width);
	inverted_height_ = 1.0 / static_cast<double>(height);
	
	if (is_ortho)
	{
		umbase::um_matrix_ortho_rh(view_matrix_, 
			static_cast<double>(width), 
			static_cast<double>(height), 
			near_, 
			far_);
	}
	else
	{
		umbase::um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
		//um_matrix_perspective_rh(projection_matrix_, (double)width, (double)height, near_, far_);
		umbase::um_matrix_perspective_fov_rh(projection_matrix_, umbase::um_to_radian(fov_y_), aspect_, near_, far_);
	}

	update();
}

/**
 * update
 */
void UMCamera::update()
{
	view_projection_matrix_ = view_matrix() * projection_matrix();
}

/**
 * rotate camera
 */
void UMCamera::rotate(double mx, double my)
{
	if (is_ortho_) return;
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;

	rotation_.y -= umbase::um_to_radian(mx);
	rotation_.x -= umbase::um_to_radian(my);

	const double cx = cos(rotation_.x);
	if (cx > -FLT_EPSILON &&  cx < FLT_EPSILON) {
		rotation_.x -= umbase::um_sign(rotation_.x) * 0.01;
	}

	const double radius = (target_ - position_).length();
	const UMMat44d rot = umbase::um_euler_to_matrix_xyz(rotation_);
	if (cx < 0.0)
	{
		up_ = UMVec3d(0.0, -1.0, 0.0);
	}
	else
	{
		up_ = UMVec3d(0.0, 1.0, 0.0);
	}
	position_ = rot * UMVec3d(0.0, 0.0, radius) + target_;
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

/**
 * zoom camera
 */
void UMCamera::zoom(double mx, double my)
{
	if (is_ortho_) return;
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;

	fov_y_ -= my * 0.2;

	if (fov_y_ > 175) {
		fov_y_ = 175;
	}
	if (fov_y_ < 5) {
		fov_y_ = 5;
	}

	umbase::um_matrix_perspective_fov_rh(projection_matrix_, umbase::um_to_radian(fov_y_), aspect_, near_, far_);
}


void UMCamera::update_from_node()
{
	UMMat44d transform = global_transform();

	UMVec3d x (transform.m[0][0], transform.m[0][1], transform.m[0][2]);
	UMVec3d y (transform.m[1][0], transform.m[1][1], transform.m[1][2]);
	UMVec3d z (transform.m[2][0], transform.m[2][1], transform.m[2][2]);
	position_.x = transform.m[3][0];
	position_.y = transform.m[3][1];
	position_.z = transform.m[3][2];
	
	up_ = -x.cross(z);
	target_ = position_ - z * 100;

	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

/**
 * dolly camera
 * @param [in] mx x-axis mouse move
 * @param [in] my y-axis mouse move
 */
void UMCamera::dolly(double mx, double my)
{
	if (is_ortho_) return;
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;

	const double dolly_value = -my * 0.2;
	const double radius = (target_ - position_).length() + dolly_value;
	if (radius < 1) return;

	const UMMat44d rot = umbase::um_euler_to_matrix_xyz(rotation_);
	const double cx = cos(rotation_.x);
	if (cx < 0)
	{
		up_ = UMVec3d(0.0, -1.0, 0.0);
	}
	else
	{
		up_ = UMVec3d(0.0, 1.0, 0.0);
	}
	position_ = rot * UMVec3d(0.0, 0.0, radius) + target_;
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

/**
 * move camera
 */
void UMCamera::pan(double mx, double my)
{
	if (is_ortho_) return;
	if (fabs(mx) <= 2 && fabs(my) <= 2) return;
	
	double pan_value = 0.2;

	const UMMat44d rot = umbase::um_euler_to_matrix_xyz(rotation_);
	UMVec3d mhorizon = rot * UMVec3d(mx * pan_value, 0.0, 0.0);
	UMVec3d mvertial = rot * UMVec3d(0.0, -my * pan_value, 0.0);
	position_ += mvertial + mhorizon;
	target_ += mvertial + mhorizon;
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

/**
* fit camera
*/
void UMCamera::fit(UMBoxPtr box)
{
	const UMVec3d& min = box->minimum();
	const UMVec3d& max = box->maximum();
	const UMVec3d& center = box->center();
	const UMMat44d rot = umbase::um_euler_to_matrix_xyz(rotation_);

	position_ += (center - target_);
	target_ = center;

	UMVec3d verts[] = {
		UMVec3d(min.x, min.y, min.z),
		UMVec3d(min.x, min.y, max.z),
		UMVec3d(min.x, max.y, max.z),
		UMVec3d(min.x, max.y, min.z),
		UMVec3d(max.x, max.y, max.z),
		UMVec3d(max.x, max.y, min.z),
		UMVec3d(max.x, min.y, min.z),
		UMVec3d(max.x, min.y, max.z)
	};
	const UMMat44d m = view_projection_matrix();
	UMVec3d frustumN[] = {
		//UMVec3d(-m[0][3], -m[1][3], -m[2][3]).normalized(), //near
		//UMVec3d(-m[0][3] + m[0][2], -m[1][3] + m[1][2], -m[2][3] + m[2][2]).normalized(), //far
		UMVec3d(-m[0][3] - m[0][0], -m[1][3] - m[1][0], -m[2][3] - m[2][0]).normalized(), // left
		UMVec3d(-m[0][3] + m[0][0], -m[1][3] + m[1][0], -m[2][3] + m[2][0]).normalized(), // right
		UMVec3d(-m[0][3] + m[0][1], -m[1][3] + m[1][1], -m[2][3] + m[2][1]).normalized(), // top
		UMVec3d(-m[0][3] - m[0][1], -m[1][3] - m[1][1], -m[2][3] - m[2][1]).normalized(), // bottom
	};

	UMMat44d newview;
	const double radius = (target_ - position_).length();
	position_ = rot * UMVec3d(0.0, 0.0, radius) + target_;
	double radius2 = (target_ - position_).length();
	um_matrix_look_at_rh(newview, position_, target_, up_);

	double radfovy = (umbase::um_to_radian(fov_y_) * 0.5);
	double radfovy2 = (umbase::um_to_radian(90 - fov_y_ * 0.5));
	double radfovy3 = (umbase::um_to_radian(fov_x()) * 0.5);
	double radfovy4 = (umbase::um_to_radian(90 - fov_x() * 0.5));
	double back = 0.0;
	for (int i = 0; i < sizeof(verts) / sizeof(verts[0]); ++i)
	{
		UMVec3d& v = verts[i];
		double mindist = 0.0;// std::numeric_limits<double>::max();
		UMVec3d n;
		bool isLR = true;
		for (int k = 0; k < sizeof(frustumN) / sizeof(frustumN[0]); ++k) {
			double dist = (v - position_).dot(frustumN[k]);
			double dist2 = (v - position_).length();
			if (dist > 0) {
				if (mindist < fabs(dist)) {
					mindist = dist;
					n = frustumN[k];
					if (k > 1) {
						isLR = false;
					}
				}
			}
		}
		UMVec3d p = v - n * mindist;
		double L = (p - v).length();
		double backlen = (p - v).length() / (sin(radfovy2) * tan(radfovy));
		if (isLR) {
			backlen = (p - v).length() / (sin(radfovy4) * tan(radfovy3));
		}
		if (backlen > back) {
			back = backlen;
		}
	}
	position_ = target_ + (position_ - target_).normalized() * (radius2 + back);
	um_matrix_look_at_rh(view_matrix_, position_, target_, up_);
}

} // umdraw
