/**
 * @file UMCurve.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <vector>
#include <memory>
#include "UMMacro.h"
#include "UMMathTypes.h"
#include "UMVector.h"
#include "UMMatrix.h"
#include "UMNode.h"
#include "UMMaterial.h"

namespace umdraw
{

class UMCurve;
typedef std::shared_ptr<UMCurve> UMCurvePtr;
typedef std::vector<UMCurvePtr> UMCurveList;

/**
 * curves
 */
class UMCurve
{
	DISALLOW_COPY_AND_ASSIGN(UMCurve);

public:

	struct Curve
	{
		Curve(const UMVec3d& _p0, const UMVec3d& _p1, const UMVec3d& _p2, const UMVec3d& _p3)
			: p0(_p0) , p1(_p1), p2(_p2), p3(_p3) {}
		UMVec3d p0, p1, p2, p3;
	};
	typedef std::vector<Curve> CurveList;

	/**
	 * calculate bezier curve
	 * @param curve curve
	 * @param t bezier parameter 0.0 ~ 1.0
	 */
	static UMVec3d calculate(const Curve& curve, double t);

	UMCurve() : is_changed_(false) {}

	UMCurve(const CurveList& curve_list)
		: curve_list_(curve_list)
		, is_changed_(false)
	{}

	/// get curve list
	const CurveList& curve_list() const { return curve_list_; }

	/// get curve list
	CurveList& mutable_curve_list() { return curve_list_; }

	/**
	* get material list
	*/
	const UMMaterialList& material_list() const { return material_list_; }

	/**
	* get material list
	*/
	UMMaterialList& mutable_material_list() { return material_list_; }

	/**
	* is changed or not
	*/
	bool is_changed() const { return is_changed_; }

	/**
	* is changed or not
	*/
	void set_changed(bool is_changed) { is_changed_ = is_changed; }


private:
	CurveList curve_list_;
	UMMaterialList material_list_;
	bool is_changed_;
};

} // umdraw
