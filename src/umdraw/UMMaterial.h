/**
 * @file UMMaterial.h
 * a material
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
#include <string>
#include <map>
#include "UMMacro.h"
#include "UMMathTypes.h"
#include "UMMath.h"
#include "UMNode.h"

namespace umimage
{

	class UMImage;
	typedef std::shared_ptr<UMImage> UMImagePtr;

};

namespace umdraw
{

class UMMaterial;
typedef std::shared_ptr<UMMaterial> UMMaterialPtr;
typedef std::weak_ptr<UMMaterial> UMMaterialWeakPtr;
typedef std::vector<UMMaterialPtr> UMMaterialList;
typedef std::map<umstring, UMMaterialPtr> UMMaterialMap;

/**
 * a material
 */
class UMMaterial : public UMNode
{
	DISALLOW_COPY_AND_ASSIGN(UMMaterial);

public:
	typedef std::vector<umimage::UMImagePtr> TextureList;
	typedef std::vector<umstring> TexturePathList;

	UMMaterial() :
		polygon_count_(0),
		shininess_(0.0), 
		transparency_factor_(1.0),
		reflection_factor_(0.0),
		diffuse_factor_(0.0), 
		specular_factor_(0.0),
		emissive_factor_(0.0),
		ambient_factor_(0.0)
	{}

	~UMMaterial() {}
	
	static UMMaterialPtr default_material() { 
		UMMaterialPtr material(std::make_shared<UMMaterial>());
		material->set_diffuse(UMVec4d(0.7, 0.7, 0.7, 1.0));
		material->set_specular(UMVec4d(0.9, 0.9, 0.9, 1.0));
		material->set_ambient(UMVec4d(0.3, 0.3, 0.3, 1.0));
		return material;
	}

	const umstring& name() const { return name_; }
	const UMVec4d& ambient() const { return ambient_; }
	const UMVec4d& diffuse() const { return diffuse_; }
	const UMVec4d& specular() const { return specular_; }
	const UMVec4d& emissive() const { return emissive_; }
	const UMVec4d& refrection() const { return refrection_; }
	const UMVec4d& transparent() const { return transparent_; }
	const double transparency_factor() const { return transparency_factor_; }
	const double shininess() const { return shininess_; }
	const double reflection_factor() const { return reflection_factor_; }
	const double diffuse_factor() const { return diffuse_factor_; }
	const double specular_factor() const { return specular_factor_; }
	const double emissive_factor() const { return emissive_factor_; }
	const double ambient_factor() const { return ambient_factor_; }
	const TextureList& texture_list() const { return texture_list_; }

	void set_name(const umstring& name) { name_ = name; }
	void set_ambient(const UMVec4d& ambient) { ambient_ = ambient; }
	void set_diffuse(const UMVec4d& diffuse) { diffuse_ = diffuse; }
	void set_specular(const UMVec4d& specular) { specular_ = specular; }
	void set_emissive(const UMVec4d& emissive) { emissive_ = emissive; }
	void set_refrection(const UMVec4d& refrection) { refrection_ = refrection; }
	void set_transparent(const UMVec4d& transparent) { transparent_ = transparent; }
	void set_transparency_factor(double value) { transparency_factor_ = value; }
	void set_shininess(double value) { shininess_ = value; }
	void set_reflection_factor(double value) { reflection_factor_ = value; }
	void set_diffuse_factor(double value) { diffuse_factor_ = value; }
	void set_specular_factor(double value) { specular_factor_ = value; }
	void set_emissive_factor(double value) { emissive_factor_ = value; }
	void set_ambient_factor(double value) { ambient_factor_ = value; }
	TextureList& mutable_texture_list() { return texture_list_; }
	
	/**
	 * get texture path list
	 */
	const TexturePathList& texture_path_list() const { return texture_path_list_; }

	/**
	 * get texture path list
	 */
	TexturePathList& mutable_texture_path_list() { return texture_path_list_; }

	/**
	 * get polygon count
	 */
	int polygon_count() const { return polygon_count_; }

	/**
	 * set polygon count
	 */
	void set_polygon_count(int count) { polygon_count_ = count; }
private:
	umstring name_;
	UMVec4d ambient_;
	UMVec4d diffuse_;
	UMVec4d specular_;
	UMVec4d emissive_;
	UMVec4d refrection_;
	UMVec4d transparent_;
	double shininess_;
	double transparency_factor_;
	double reflection_factor_;
	double diffuse_factor_;
	double specular_factor_;
	double emissive_factor_;
	double ambient_factor_;

	TextureList texture_list_;
	TexturePathList texture_path_list_;

	int polygon_count_;

};

} // umdraw
