/**
 * @file UMPoint.h
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

namespace umdraw
{

class UMPoint;
typedef std::shared_ptr<UMPoint> UMPointPtr;
typedef std::vector<UMPointPtr> UMPointList;

/**
 * points
 */
class UMPoint : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMPoint);
public:
	UMPoint() {}
	~UMPoint() {}

	typedef std::vector<UMVec3d> PointList;

	/// get point list
	const PointList& point_list() const { return point_list_; }

	/// get point list
	PointList& mutable_point_list() { return point_list_; }
	
private:
	PointList point_list_;
};

}
