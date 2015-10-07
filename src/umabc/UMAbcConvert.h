/**
 * @file UMAbcConvert.h
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <memory>
#include <algorithm>

#include "UMMacro.h"
#include "UMMathTypes.h"
#include "UMMatrix.h"
#include "UMVector.h"
#include "UMBox.h"

namespace umabc
{

class UMAbcConvert
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcConvert);

public:

	/** 
	 * convert imath matrix to ummatrix
	 */
	static UMMat44d imath_mat_to_um(const Imath::M44d& m) {
		return UMMat44d(
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]);
	}
	
	/** 
	 * convert ummatrix to imath matrix
	 */
	static Imath::M44d um_mat_to_imath(const UMMat44d& m) {
		return Imath::M44d(
			m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
			m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
			m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
			m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]);
	}

	/** 
	 * convert imath vector to umvector
	 */
	template <typename T>
	static umbase::UMVec4d imath_vec_to_um(const Imath::Vec4<T>& v) {
		return umbase::UMVec4d(v.x, v.y, v.z, v.w);
	}
	
	/** 
	 * convert imath vector to umvector
	 */
	template <typename T>
	static umbase::UMVec3d imath_vec_to_um(const Imath::Vec3<T>& v) {
		return umbase::UMVec3d(v.x, v.y, v.z);
	}

	/** 
	 * convert imath box to umbox
	 */
	static umbase::UMBox imath_box_to_um(const Imath::Box3d& box) {
		return umbase::UMBox(
			imath_vec_to_um(box.min), 
			imath_vec_to_um(box.max));
	}
};

} // umabc
