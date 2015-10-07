/**
 * @file UMScene.cpp
 * 3D scene including many objects.
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMScene.h"

#include "UMVector.h"
#include "UMMesh.h"

#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMIO.h"
#include "UMSoftwareIO.h"
#include "UMAny.h"

namespace umdraw
{
	using namespace umimage;

/** 
 * init scene
 */
void UMScene::init(int width, int height)
{
	width_ = width;
	height_ = height;
	is_enable_deform_ = true;
	visibility_ = 0xFFFFFFFF;

	light_list_.clear();
	temporary_line_list_.clear();
	temporary_mesh_list_.clear();
	mutable_camera_list().clear();
	mutable_camera_list().push_back(UMCameraPtr(new UMCamera(false, width, height)));
}

/**
 * load model data
 */
bool UMScene::load(const umstring& absolute_file_path)
{
	umio::UMIO io;
	umio::UMObjectPtr obj;

	umio::UMIOSetting setting = umio::UMIOSetting();
	setting.set_bl_imp_bool_prop(umio::UMIOSetting::eUMImpTriangulate, true);
	setting.set_system_unit_type(umio::UMIOSetting::eFbxSystemUnitM);
	obj = io.load(umbase::UMStringUtil::utf16_to_utf8(absolute_file_path), setting);
	if (!obj) return false;
	
	// import to umdraw
	UMMeshGroupPtr mesh_group(std::make_shared<UMMeshGroup>());
	{
		if (!UMSoftwareIO::import_mesh_list(
			mesh_group->mutable_mesh_list(), 
			obj,
			absolute_file_path))
		{
			return false;
		}
		mutable_mesh_group_list().push_back(mesh_group);
	}

	if (!UMSoftwareIO::import_node_list(node_list_, mesh_group->mutable_mesh_list(), obj))
	{
		node_list_.clear();
		return false;
	}

	return true;
}

/**
 * load model data from memory
 */
bool UMScene::load_from_memory(const std::string& src)
{
	umio::UMIO io;
	umio::UMObjectPtr obj;

	umio::UMIOSetting setting = umio::UMIOSetting();
	setting.set_bl_imp_bool_prop(umio::UMIOSetting::eUMImpTriangulate, true);
	setting.set_system_unit_type(umio::UMIOSetting::eFbxSystemUnitM);
	printf("loading model ...\n");
	obj = io.load_from_memory(src, setting);
	if (!obj) return false;
	printf("finish loading model ...\n");
	
	// import camera
	if (!UMSoftwareIO::import_camera_list(
		mutable_camera_list(),
		obj,
		width(),
		height()))
	{
		return false;
	}

	// import to umdraw
	UMMeshGroupPtr mesh_group(std::make_shared<UMMeshGroup>());
	{
		if (UMSoftwareIO::import_mesh_list(
			mesh_group->mutable_mesh_list(), 
			obj,
			umstring()))
		{
			mutable_mesh_group_list().push_back(mesh_group);
		}
	}

	if (!UMSoftwareIO::import_node_list(node_list_, mesh_group->mutable_mesh_list(), obj))
	{
		node_list_.clear();
		return false;
	}

	return true;
}

/**
* save model data
* @param [in] file_path dst file path
*/
bool UMScene::save(const umstring& file_path) const
{
	umio::UMIO io;
	umio::UMIOSetting setting = umio::UMIOSetting();
	umio::UMObjectPtr umobj = umio::UMObject::create_object();
	for (int i = 0, isize = static_cast<int>(mesh_group_list_.size()); i < isize; ++i)
	{
		UMMeshGroupPtr group = mesh_group_list_.at(i);
		UMSoftwareIO::export_mesh_list(umobj, group->mesh_list());
	}
	return io.save(umbase::UMStringUtil::utf16_to_utf8(file_path), umobj, setting);
}

/**
 * get total polygons
 */
size_t UMScene::total_polygon_size() const
{
	size_t polygons = 0;
	UMMeshGroupList::const_iterator it = mesh_group_list().begin();
	for (; it != mesh_group_list().end(); ++it)
	{
		UMMeshList::const_iterator mt = (*it)->mesh_list().begin();
		for (; mt != (*it)->mesh_list().end(); ++mt)
		{
			polygons += (*mt)->face_list().size();
		}
	}
	return polygons;
}

/**
 * set camera
 */
void UMScene::set_current_camera(UMCameraPtr camera)
{
	if (mutable_camera_list().empty())
	{
		mutable_camera_list().push_back(camera);
	}
	else
	{
		mutable_camera_list()[0] = camera;
	}
}

/**
 * get background image
 */
void UMScene::set_background_image(UMImagePtr image)
{
	background_image_ = image;
}

/**
 * get foreground image
 */ 
void UMScene::set_foreground_image(UMImagePtr image)
{
	foreground_image_ = image;
}

/**
 * get camera
 */
UMCameraPtr UMScene::camera()
{
	if (camera_list_.empty())
	{
		return UMCameraPtr();
	}
	return camera_list_[0];
}

/**
 * clear all data
 */
void UMScene::clear_geometry()
{
	mutable_mesh_group_list().clear();
	mutable_node_list().clear();
}

void UMScene::resize(int width, int height)
{
	if (!mutable_camera_list().empty())
	{
		mutable_camera_list().at(0)->init(false, width, height);
	}
	width_ = width;
	height_ = height;
}

/** 
 *  set visibility
 */
void UMScene::set_visible(VisibleType type, bool visible)
{
	if (visible)
	{ 
		visibility_.set(type);
	}
	else
	{
		visibility_.reset(type);
	}
}

} // umdraw
