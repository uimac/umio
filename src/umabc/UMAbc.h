/**
 * @file UMAbc.h
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
#include <string>
#include <vector>
#include <functional>
#include "UMMacro.h"
#include "UMAbcSetting.h"
#include "UMAbcCallback.h"

/// uimac alembic library
namespace umabc
{
	
class UMAbcScene;
typedef std::shared_ptr<UMAbcScene> UMAbcScenePtr;
typedef std::vector<UMAbcScenePtr> UMAbcSceneList;

class UMAbc;
typedef std::shared_ptr<UMAbc> UMAbcPtr;
typedef std::vector<UMAbcPtr> UMAbcList;

class UMAbc
{
	DISALLOW_COPY_AND_ASSIGN(UMAbc);
public:
	enum DrawType {
		eSoftware,
		eOpenGL,
		eDirectX,
	};
	
	UMAbc();
	~UMAbc();
	
	/**
	 * create instance
	 */
	static UMAbcPtr create(DrawType type);
	
	/**
	 * initialize
	 */
	bool init(UMAbcSceneList& scene_list);
	
	/**
	 * initialize
	 */
	virtual bool init(UMAbcScenePtr scene);

	/**
	 * release all scenes. call this function before delete.
	 */
	virtual bool dispose();

	/**
	 * tick forward
	 */
	virtual bool update(unsigned long time);
	
	/**
	 * refresh frame
	 */
	virtual bool draw();

	/**
	 * create view/depth
	 */
	virtual bool clear();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

} // umabc
