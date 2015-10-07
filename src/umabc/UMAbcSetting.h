/**
 * @file UMAbcSetting.h
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
#include "UMMacro.h"

namespace umdraw
{
	class UMScene;
	typedef std::shared_ptr<UMScene> UMScenePtr;
	typedef std::weak_ptr<UMScene> UMSceneWeakPtr;
} // umdraw

/// uimac alembic library
namespace umabc
{

class UMAbcSetting
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcSetting);
public:

	UMAbcSetting() {}
	~UMAbcSetting() {}

	/**
	 * set reference scene
	 */
	void set_reference_scene(umdraw::UMScenePtr scene) { reference_scene_ = scene; }

	/**
	 * get refrerence scene
	 */
	umdraw::UMScenePtr reference_scene() const { return reference_scene_.lock(); }

private:
	umdraw::UMSceneWeakPtr reference_scene_;
};

} // umabc
