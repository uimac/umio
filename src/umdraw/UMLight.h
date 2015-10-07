/**
 * @file UMLight.h
 * a light
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
#include "UMMathTypes.h"
#include "UMVector.h"
#include "UMNode.h"

namespace umdraw
{

class UMLight;
typedef std::shared_ptr<UMLight> UMLightPtr;
typedef std::weak_ptr<UMLight> UMLightWeakPtr;
typedef std::vector< UMLightPtr > UMLightList;

/**
 * a light
 */
class UMLight : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMLight);
public:
	UMLight() {}

	/**
	 * @param [in] position light position
	 */
	UMLight(const UMVec3d& position) :
		position_(position),
		color_(13) {}
	
	virtual ~UMLight() {}
	
	/**
	 * get node
	 */
	UMNodePtr node() { return node_; }
	
	/**
	 * get node
	 */
	UMNodePtr node() const { return node_; }

	/**
	 * get position
	 */
	UMVec3d position() const { return position_; }
	
	/**
	 * set position
	 * @param [in] position source position
	 */
	void set_position(const UMVec3d& position) { position_ = position; }

	/** 
	 * get color
	 */
	UMVec3d color() const { return color_; }

	/**
	 * set color
	 * @param [in] color light color
	 */
	void set_color(const UMVec3d& color) { color_ = color; }

private:
	UMVec3d position_;
	UMVec3d color_;
	
	UMNodePtr node_;
};

} // umdraw
