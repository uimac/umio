/**
 * @file UMMeshGroup.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <vector>

#include "UMMacro.h"
#include "UMVector.h"
#include "UMMesh.h"
#include "UMNode.h"

namespace umdraw
{

class UMMeshGroup;
typedef std::shared_ptr<UMMeshGroup> UMMeshGroupPtr;
typedef std::weak_ptr<UMMeshGroup> UMMeshGroupWeakPtr;

typedef std::vector<UMMeshGroupPtr> UMMeshGroupList;

/**
 * a mesh group
 */
class UMMeshGroup
{
	DISALLOW_COPY_AND_ASSIGN(UMMeshGroup);
public:
	UMMeshGroup() {}
	~UMMeshGroup() {}
	
	/**
	 * get node
	 */
	UMNodePtr node() { return node_; }
	
	/**
	 * get node
	 */
	UMNodePtr node() const { return node_; }

	/**
	 * get mesh list
	 */
	const UMMeshList& mesh_list() const { return mesh_list_; }
	
	/**
	 * get mesh list
	 */
	UMMeshList& mutable_mesh_list() { return mesh_list_; }

private:
	UMNodePtr node_;
	UMMeshList mesh_list_;
};

} //umdraw
