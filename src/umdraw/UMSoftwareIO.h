/**
 * @file UMSoftwareIO.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMMacro.h"

namespace umio
{
	class UMObject;
	typedef std::shared_ptr<UMObject> UMObjectPtr;
};

namespace umdraw
{
	
class UMMesh;
typedef std::shared_ptr<UMMesh> UMMeshPtr;
typedef std::vector<UMMeshPtr> UMMeshList;

class UMCamera;
typedef std::shared_ptr<UMCamera> UMCameraPtr;
typedef std::vector<UMCameraPtr> UMCameraList;

/**
 * model in/out
 */
class UMSoftwareIO
{
	DISALLOW_COPY_AND_ASSIGN(UMSoftwareIO);

public:
	
	/** 
	 * import umdraw mesh list
	 * @param [out] dst destination mesh list
	 * @param [in] src source object
	 * @param [in] absolute_file_path file path for texture loading
	 */
	static bool import_mesh_list(
		UMMeshList& dst, 
		const umio::UMObjectPtr src,
		const umstring& absolute_file_path);
	
	/** 
	 * import umdraw node list
	 * @param [out] dst destination mesh list
	 * @param [out] mesh list for node bind
	 * @param [in] src source object
	 */
	static bool import_node_list(
		UMNodeList& dst,
		UMMeshList& mesh_list, 
		const umio::UMObjectPtr src);

	/** 
	 * import umdraw camera list
	 * @param [out] dst destination mesh list
	 * @param [in] src source object
	 * @param [in] initial_width camera initial width
	 * @param [in] initial_height camera initial height
	 */
	static bool import_camera_list(
		UMCameraList& dst,
		const umio::UMObjectPtr src,
		int initial_width,
		int initial_height);

	/**
	 * export mesh list
	 * @param [out] dst destination 
	 */
	static bool export_mesh_list(
		umio::UMObjectPtr dst,
		const UMMeshList& src);

	static bool convert_node_to_octahedron(
		std::vector<UMVec3d>& dst_octahedron,
		std::vector<UMVec3d>& dst_triangles,
		UMNodePtr src);

};

} // umdraw
