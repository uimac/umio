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
#include "UMMaterial.h"

namespace umdraw
{
	class UMMesh;
	typedef std::shared_ptr<UMMesh> UMMeshPtr;
}

/// uimac alembic library
namespace umabc
{
	
class UMAbcScene;
typedef std::shared_ptr<UMAbcScene> UMAbcScenePtr;

class UMAbcObject;
typedef std::shared_ptr<UMAbcObject> UMAbcObjectPtr;

class UMAbcMesh;
typedef std::shared_ptr<UMAbcMesh> UMAbcMeshPtr;

class UMAbcSoftwareIO;
typedef std::shared_ptr<UMAbcSoftwareIO> UMAbcSoftwareIOPtr;

typedef std::vector<umdraw::UMMaterialMap> UMAbcSceneMaterialList;

class UMAbcSoftwareIO
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcSoftwareIO);
public:

	UMAbcSoftwareIO() {}
	~UMAbcSoftwareIO() {}
	
	/**
	 * release all scenes. call this function before delete.
	 */
	void dispose();

	/**
	 * load 3d file to UMAbcScene
	 */
	UMAbcScenePtr load(umstring path, const UMAbcSetting& setting);
	
	/**
	 * save 3d file
	 */
	bool save(umstring path, UMAbcScenePtr scene, const UMAbcSetting& setting);

	/**
	 * load settings
	 */
	bool load_setting(std::string path, UMAbcSetting& setting);
	
	/**
	 * save settings
	 */
	bool save_setting(std::string path, const UMAbcSetting& setting);

	/**
	* get loaded event
	*/
	//umbase::UMEventPtr loaded_event() { return loaded_event_; }

	/** 
	 * import umabc materials
	 * @param [out] dst destination material list
	 * @param [in] abc_mtl_filepath material file path
	 */
	static bool import_material_map(
		umdraw::UMMaterialMap& dst, 
		const umstring& abc_mtl_filepath);
	
	/**
	 * convert abc mesh to mesh
	 * @param [in] src source abc mesh
	 * @retval UMMeshPtr converted mesh or none
	 */
	static umdraw::UMMeshPtr convert_abc_mesh_to_mesh(
		umabc::UMAbcMeshPtr src);

	/**
	 * convert ccw(wrong) mesh to cw mesh
	 */
	static void convert_abc_mesh_ccw_to_cw(umabc::UMAbcObjectPtr src);

	/**
	 * assign default material to scene
	 */
	static void assign_default_material(UMAbcScenePtr scene);

private:
};

} // umabc
