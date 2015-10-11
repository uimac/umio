/**
 * @file UMAbcScene.h
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
#include <map>
#include "UMMacro.h"
#include "UMAbcSetting.h"

namespace umdraw
{
	class UMScene;
	typedef std::shared_ptr<UMScene> UMScenePtr;
	typedef std::weak_ptr<UMScene> UMSceneWeakPtr;

	class UMMaterial;
	typedef std::shared_ptr<UMMaterial> UMMaterialPtr;
	typedef std::map<umstring, UMMaterialPtr> UMMaterialMap;

	class UMCamera;
	typedef std::shared_ptr<UMCamera> UMCameraPtr;

} // umdraw

/// uimac alembic library
namespace umabc
{
	
class UMAbcObject;
typedef std::shared_ptr<UMAbcObject> UMAbcObjectPtr;

class UMAbcScene;
typedef std::shared_ptr<UMAbcScene> UMAbcScenePtr;
typedef std::vector<UMAbcScenePtr> UMAbcSceneList;

class UMAbcScene //: public UMAbc
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcScene);
public:

	UMAbcScene(UMAbcObjectPtr root);
	~UMAbcScene();
	
	/**
	 * initialize
	 */
	virtual bool init();

	/**
	 * release all resources. call this function before delete.
	 */
	virtual bool dispose();
	
	///** 
	// * refresh scene
	// */
	//virtual bool draw();
	
	/**
	 * update scene
	 */
	virtual bool update(unsigned long time);
	
	/**
	 * clear frame
	 */
	virtual bool clear();

	/**
	 * get minimum time
	 */
	double min_time() const;
	
	/**
	 * get maximum time
	 */
	double max_time() const;
	
	/** 
	 * get scene
	 */
	umdraw::UMScenePtr umdraw_scene();

	/**
	 * get camera
	 */
	umdraw::UMCameraPtr umdraw_camera(const umstring& name);

	/** 
	 * set scene
	 */
	void set_umdraw_scene(umdraw::UMScenePtr scene);
	
	/**
	 * get name list
	 */
	std::vector<std::string> object_name_list();

	/**
	 * get material map
	 */
	umdraw::UMMaterialMap& material_map();

	/**
	 * get root object
	 */
	UMAbcObjectPtr root_object();
	
	/**
	 * get total polygons
	 */
	size_t total_polygon_size() const;

private:
	class SceneImpl;
	typedef std::unique_ptr<SceneImpl> SceneImplPtr;
	SceneImplPtr impl_;
};

} // umabc
