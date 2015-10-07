/**
 * @file UMNode.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"
#include "UMMathTypes.h"
#include "UMVector.h"
#include "UMMatrix.h"
#include "UMShaderEntry.h"
#include <vector>

namespace umdraw
{

class UMNode;
typedef std::shared_ptr<UMNode> UMNodePtr;
typedef std::weak_ptr<UMNode> UMNodeWeakPtr;
typedef std::vector<UMNodePtr> UMNodeList;

/**
 * a node
 */
class UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMNode);
public:
	UMNode() {
		static unsigned int counter = 0;
		id_ = ++counter;
		node_color_ = UMVec4d(0.5, 0.5, 0.5, 1.0);
	}
	virtual ~UMNode() {}
	
	// getter
	unsigned int id() const { return id_; }
	const umstring& name() const { return name_; }
	const UMMat44d& local_transform() const { return local_transform_; }
	const UMMat44d& global_transform() const { return global_transform_; }
	const UMMat44d& initial_local_transform() const { return initial_local_transform_; }
	const UMMat44d& initial_global_transform() const { return initial_global_transform_; }
	bool is_valid_shader_entry() const { return shader_entry_ ? true : false; }
	UMShaderEntryPtr shader_entry() const { return shader_entry_; }
	const UMNodeList& children() const { return children_; }
	UMNodePtr parent() { return parent_.lock(); }
	const UMVec4d& node_color() const { return node_color_; }

	// setter
	void set_name(const umstring& name) { name_ = name; }
	UMMat44d& mutable_local_transform() { return local_transform_; }
	UMMat44d& mutable_global_transform() { return global_transform_; }
	UMMat44d& mutable_initial_local_transform() { return initial_local_transform_; }
	UMMat44d& mutable_initial_global_transform() { return initial_global_transform_; }
	UMNodeList& mutable_children() { return children_; }
	void set_parent(UMNodePtr parent) { parent_ = parent; }
	void set_node_color(const UMVec4d& color) { node_color_ = color; }

	void set_shader_entry(UMShaderEntryPtr shader_entry) { shader_entry_ = shader_entry; }
	
private:
	unsigned int id_;
	umstring name_;

	// evaluated transform
	UMMat44d local_transform_;
	UMMat44d global_transform_;
	UMMat44d initial_local_transform_;
	UMMat44d initial_global_transform_;

	UMShaderEntryPtr shader_entry_;
	UMVec4d node_color_;

	UMNodeWeakPtr parent_;
	UMNodeList children_;
};

}
