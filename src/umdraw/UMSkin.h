/**
 * @file UMSkin.h
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
#include <vector>

namespace umdraw
{

class UMSkin;
typedef std::vector<UMSkin> UMSkinList;

class UMNode;
typedef std::shared_ptr<UMNode> UMNodePtr;

/**
 * skin
 */
class UMSkin
{
public:
	UMSkin() : link_node_id_(-1) {}
	~UMSkin() {}
	
	typedef std::vector<int> IndexList;
	typedef std::vector<double> WeightList;

	const IndexList& index_list() const { return index_list_; }
	IndexList& mutable_index_list() { return index_list_; }

	const WeightList weight_list() const { return weight_list_; }
	WeightList& mutable_weight_list() { return weight_list_; }

	int link_node_id() const { return link_node_id_; }
	void set_link_node_id(int link_node_id) { link_node_id_ = link_node_id; }

	UMNodePtr link_node() { return link_node_; }
	void set_link_node(UMNodePtr node) { link_node_ = node; }

private:
	int link_node_id_;
	IndexList index_list_;
	WeightList weight_list_;
	UMNodePtr link_node_;
};

}
