/**
 * @file UMAbcScene.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMAbcScene.h"

#include <memory>
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

#include "UMAbcObject.h"
#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMMaterial.h"
#include "UMAbcSoftwareIO.h"
#include "UMAbcMesh.h"
#include "UMAbcPoint.h"
#include "UMAbcCamera.h"

#include "UMScene.h"
#include "UMCamera.h"

namespace umabc
{
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;
	using namespace umdraw;

class UMAbcScene::SceneImpl
{
	DISALLOW_COPY_AND_ASSIGN(SceneImpl);
public:

	SceneImpl(UMAbcObjectPtr root)
		: is_cw_(true)
		, object_(root)
		, pre_time_(-1)
		{}
	~SceneImpl() {}

	/**
	 * initialize
	 */
	bool init()
	{
		unsigned long current = object_->current_time_ms();
		if (object_->init(true, UMAbcObjectPtr()))
		{
			if (!is_cw_)
			{
				UMAbcSoftwareIO::convert_abc_mesh_ccw_to_cw(object_);
			}
			unsigned long min_time_ = object_->min_time();
			unsigned long max_time_ = object_->max_time();

			// set initial time
			if (min_time_ <= max_time_)
			{
				object_->set_current_time(min_time_, true);
			}
			else
			{
				object_->set_current_time(0, true);
			}

			// calculate bounding box
			object_->update_box(true);
			object_->set_current_time(current, true);
		}
		return true;
	}
	
	bool update(unsigned long time)
	{
		if (is_constant()) return true;
		if (pre_time_ == time) return true;
		if (time < min_time() || time > max_time())
		{
			return false;
		}
		if (object_)
		{
			object_->set_current_time(time, true);
		}
		pre_time_ = time;
		return true;
	}

	///** 
	// * refresh scene
	// */
	//bool draw()
	//{
	//	return true;
	//}

	bool clear() 
	{
		return true;
	}

	/**
	 * get minimum time
	 */
	double min_time() const
	{
		if (object_)
		{
			return object_->min_time();
		}
		return 0.0;
	}
	
	/**
	 * get maximum time
	 */
	double max_time() const
	{
		if (object_)
		{
			return object_->max_time();
		}
		return 0.0;
	}

	/**
	 * set camera
	 */
	void set_umdraw_scene(umdraw::UMScenePtr scene)
	{
		scene_ = scene;
	}

	/**
	 * get name list
	 */
	void object_name_list(std::vector<std::string>& name_list)
	{
		if (object_)
		{
			name_list.push_back(object_->name());
			get_name_list_recursive(name_list, object_);
		}
	}

	umdraw::UMMaterialMap& material_map() { return material_map_; }
	
	umdraw::UMScenePtr umdraw_scene() { return scene_.lock(); }

	umdraw::UMCameraPtr umdraw_camera(const umstring& name)
	{
		return find_camera_recursive(name, object_);
	}

	UMAbcObjectPtr root_object() const { return object_; }
	
	/**
	 * get total polygons
	 */
	size_t total_polygon_size() const
	{
		size_t total_size = 0;
		total_polygon_size_recursive(total_size, root_object());
		return total_size;
	}

private:
	UMAbcObjectPtr object_;
	umdraw::UMSceneWeakPtr scene_;
	umdraw::UMMaterialMap material_map_;
	bool is_cw_;
	unsigned long pre_time_;

	/**
	 * is constant
	 */
	bool is_constant() const { return min_time() >= max_time(); }

	void get_name_list_recursive(std::vector<std::string>& name_list, UMAbcObjectPtr object)
	{
		for (UMAbcObjectList::const_iterator it = object_->children().begin();
			it != object_->children().end();
			++it)
		{
			name_list.push_back((*it)->name());
		}
	}
	
	void total_polygon_size_recursive(size_t& dst_size, UMAbcObjectPtr object) const
	{
		if (!object) return;
		if (UMAbcMeshPtr mesh = std::dynamic_pointer_cast<UMAbcMesh>(object))
		{
			dst_size += mesh->triangle_index().size();
		}
		for (UMAbcObjectList::const_iterator it = object->children().begin();
			it != object->children().end();
			++it)
		{
			total_polygon_size_recursive(dst_size, *it);
		}
	}

	umdraw::UMCameraPtr find_camera_recursive(const umstring& name, UMAbcObjectPtr object)
	{
		if (!object) {
			return umdraw::UMCameraPtr();
		}
		if (UMAbcCameraPtr camera = std::dynamic_pointer_cast<UMAbcCamera>(object))
		{
			return camera->umcamera();
		}
		for (UMAbcObjectList::const_iterator it = object->children().begin();
			it != object->children().end();
			++it)
		{
			if (umdraw::UMCameraPtr camera = find_camera_recursive(name, *it))
			{
				return camera;
			}
		}
		return umdraw::UMCameraPtr();
	}
};

/**
 * constructor
 */
UMAbcScene::UMAbcScene(UMAbcObjectPtr root)
	: impl_(new UMAbcScene::SceneImpl(root))
{
}

/**
 * destructor
 */
UMAbcScene::~UMAbcScene()
{
}

/**
 * get minimum time
 */
double UMAbcScene::min_time() const
{
	return impl_->min_time();
}
	
/**
 * get maximum time
 */
double UMAbcScene::max_time() const
{
	return impl_->max_time();
}

/** 
 * set scene
 */
void UMAbcScene::set_umdraw_scene(umdraw::UMScenePtr scene)
{
	impl_->set_umdraw_scene(scene);
}

/**
 * initialize
 */
bool UMAbcScene::init()
{
	return impl_->init();
}

/**
 * release all resources. call this function before delete.
 */
bool UMAbcScene::dispose()
{
	impl_ = SceneImplPtr();
	return true;
}

///** 
// * refresh scene
// */
//bool UMAbcScene::draw()
//{
//	return impl_->draw();
//}

/** 
 * update scene
 */
bool UMAbcScene::update(unsigned long time)
{
	return impl_->update(time);
}

/** 
 * clear scene
 */
bool UMAbcScene::clear()
{
	return impl_->clear();
}

/**
 * get name list
 */
std::vector<std::string> UMAbcScene::object_name_list()
{
	std::vector<std::string> name_list;
	impl_->object_name_list(name_list);
	return name_list;
}

/** 
 * get material map
 */
umdraw::UMMaterialMap& UMAbcScene::material_map()
{
	return impl_->material_map();
}

/** 
 * get scene
 */
umdraw::UMScenePtr UMAbcScene::umdraw_scene()
{
	return impl_->umdraw_scene();
}

/**
 * get camera
 */
umdraw::UMCameraPtr UMAbcScene::umdraw_camera(const umstring& name)
{
	return impl_->umdraw_camera(name);
}

/**
 * get root object
 */
UMAbcObjectPtr UMAbcScene::root_object()
{
	return impl_->root_object();
}

/**
 * get total polygons
 */
size_t UMAbcScene::total_polygon_size() const
{
	return impl_->total_polygon_size();
}

} // umabc
