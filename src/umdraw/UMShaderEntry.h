/**
 * @file UMShaderEntry.h
 * shader entry
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <string>
#include <memory>
#include "UMMacro.h"

namespace umdraw
{

class UMShaderEntry;
typedef std::shared_ptr<UMShaderEntry> UMShaderEntryPtr;

/**
 * shader entry
 */
class UMShaderEntry
{
	DISALLOW_COPY_AND_ASSIGN(UMShaderEntry);
public:
	UMShaderEntry() {}
	~UMShaderEntry() {}

	static UMShaderEntry& instance() {
		static UMShaderEntry instance_;
		return instance_;
	}
	
	void set_gl_vertex_shader(const std::string& shader) { gl_vertex_shader_ = shader; }
	void set_gl_fragment_shader(const std::string& shader) { gl_fragment_shader_ = shader; }
	
	void set_gl_point_vertex_shader(const std::string& shader) { gl_point_vertex_shader_ = shader; }
	void set_gl_point_fragment_shader(const std::string& shader) { gl_point_fragment_shader_ = shader; }

	void set_gl_board_vertex_shader(const std::string& shader) { gl_board_vertex_shader_ = shader; }
	void set_gl_board_fragment_shader(const std::string& shader) { gl_board_fragment_shader_ = shader; }
	
	// for deferred
	void set_gl_board_light_pass_vertex_shader(const std::string& shader) { gl_board_light_pass_vertex_shader_ = shader; }
	void set_gl_board_light_pass_fragment_shader(const std::string& shader) { gl_board_light_pass_fragment_shader_ = shader; }
	void set_gl_vertex_geo_shader(const std::string& shader) { gl_vertex_geo_shader_ = shader; }
	void set_gl_fragment_geo_shader(const std::string& shader) { gl_fragment_geo_shader_ = shader; }
	
	void set_dx_vertex_shader(const std::string& shader) { dx_vertex_shader_ = shader; }
	void set_dx_pixel_shader(const std::string& shader) { dx_pixel_shader_ = shader; }
	void set_dx_include_shader(const std::string& shader) { dx_include_shader_ = shader; }

	const std::string& gl_vertex_shader() const { return gl_vertex_shader_; }
	const std::string& gl_fragment_shader() const { return gl_fragment_shader_; }
	const std::string& gl_point_vertex_shader() const { return gl_point_vertex_shader_; }
	const std::string& gl_point_fragment_shader() const { return gl_point_fragment_shader_; }
	const std::string& gl_board_vertex_shader() const { return gl_board_vertex_shader_; }
	const std::string& gl_board_fragment_shader() const { return gl_board_fragment_shader_; }

	// for deferred
	const std::string& gl_board_light_pass_vertex_shader() const { return gl_board_light_pass_vertex_shader_; }
	const std::string& gl_board_light_pass_fragment_shader() const { return gl_board_light_pass_fragment_shader_; }
	const std::string& gl_vertex_geo_shader() const { return gl_vertex_geo_shader_; }
	const std::string& gl_fragment_geo_shader() const { return gl_fragment_geo_shader_; }
	
	const std::string& dx_vertex_shader() const { return dx_vertex_shader_; }
	const std::string& dx_pixel_shader() const { return dx_pixel_shader_; }
	const std::string& dx_include_shader() const { return dx_include_shader_; }

private:
	std::string gl_vertex_shader_;
	std::string gl_fragment_shader_;
	std::string gl_point_vertex_shader_;
	std::string gl_point_fragment_shader_;
	std::string gl_board_vertex_shader_;
	std::string gl_board_fragment_shader_;
	std::string gl_board_light_pass_vertex_shader_;
	std::string gl_board_light_pass_fragment_shader_;
	std::string gl_vertex_geo_shader_;
	std::string gl_fragment_geo_shader_;
	std::string dx_vertex_shader_;
	std::string dx_pixel_shader_;
	std::string dx_include_shader_;
};

} // umdraw
