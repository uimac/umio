/**
 * @file UMLine.h
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

class UMLine;
typedef std::shared_ptr<UMLine> UMLinePtr;
typedef std::vector<UMLinePtr> UMLineList;

/**
 * lines
 */
class UMLine : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMLine);
public:
	struct Line
	{
		Line(const UMVec3d& _p0, const UMVec3d& _p1) 
			: p0(_p0)
			, p1(_p1) {}
		UMVec3d p0;
		UMVec3d p1;
	};
	//typedef std::pair<UMVec3d, UMVec3d> Line;
	typedef std::vector<Line> LineList;

	UMLine() : is_changed_(false) {}

	UMLine(const LineList& line_list) 
		: line_list_(line_list)
		, is_changed_(false)
	{}

	~UMLine() {}

	/// get line list
	const LineList& line_list() const { return line_list_; }

	/// get line list
	LineList& mutable_line_list() { return line_list_; }
	
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
	LineList line_list_;
	UMMaterialList material_list_;
	bool is_changed_;
};

}
