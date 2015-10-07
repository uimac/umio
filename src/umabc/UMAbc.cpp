/**
 * @file UMAbc.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */

#include "UMAbc.h"

#include <memory>
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

#include "UMAbcScene.h"
#ifdef WITH_OPENGL
	#include "UMOpenGLAbcScene.h"
#endif
#ifdef WITH_DIRECTX
	#include "UMDirectX11AbcScene.h"
#endif

namespace umabc
{

/**
 * UMAbc implementation
 */
class UMAbc::Impl
{
	DISALLOW_COPY_AND_ASSIGN(Impl);
public:
	Impl() {}
	~Impl() {}
	
	bool init(UMAbcSceneList& scene_list)
	{
		drawer_list_.clear();
		const int scene_size = static_cast<int>(scene_list.size());
		for (int i = 0; i < scene_size; ++i)
		{
			UMAbcScenePtr scene = scene_list.at(i);
			if (draw_type_ == UMAbc::eDirectX)
			{
#ifdef WITH_DIRECTX
				UMAbcPtr drawer = UMDirectX11AbcScene::create();
				drawer->init(scene);
				drawer_list_.push_back(drawer);
#endif // WITH_DIRECTX
			}
			else if (draw_type_ == UMAbc::eOpenGL)
			{
#ifdef WITH_OPENGL
				UMAbcPtr drawer = UMOpenGLAbcScene::create();
				drawer->init(scene);
				drawer_list_.push_back(drawer);
#endif // WITH_OPENGL
			}
		}
		return true;
	}
	
	bool init(UMAbcScenePtr scene)
	{
		drawer_list_.clear();
		if (draw_type_ == UMAbc::eDirectX)
		{
#ifdef WITH_DIRECTX
			UMAbcPtr drawer = UMDirectX11AbcScene::create();
			drawer->init(scene);
			drawer_list_.push_back(drawer);
#endif // WITH_DIRECTX
		}
		else if (draw_type_ == UMAbc::eOpenGL)
		{
#ifdef WITH_OPENGL
			UMAbcPtr drawer = UMOpenGLAbcScene::create();
			drawer->init(scene);
			drawer_list_.push_back(drawer);
#endif // WITH_OPENGL
		}
		return true;
	}

	bool dispose()
	{
		UMAbcList::const_iterator it = drawer_list_.begin();
		for (int scene_index = 0; it != drawer_list_.end(); ++it, ++scene_index)
		{
			UMAbcPtr drawer = *it;
			drawer->dispose();
		}
		return true;
	}

	bool draw()
	{
		UMAbcList::const_iterator it = drawer_list_.begin();
		for (int scene_index = 0; it != drawer_list_.end(); ++it, ++scene_index)
		{
			UMAbcPtr drawer = *it;
			drawer->draw();
		}
		return true;
	}

	bool update(unsigned long time)
	{
		UMAbcList::const_iterator it = drawer_list_.begin();
		for (int scene_index = 0; it != drawer_list_.end(); ++it, ++scene_index)
		{
			UMAbcPtr drawer = *it;
			drawer->update(time);
		}
		return true;
	}

	bool clear() 
	{
		UMAbcList::const_iterator it = drawer_list_.begin();
		for (int scene_index = 0; it != drawer_list_.end(); ++it, ++scene_index)
		{
			UMAbcPtr drawer = *it;
			drawer->clear();
		}
		return false;
	}

	DrawType draw_type_;
private:
	double get_minimum_time();
	UMAbcSceneList scene_list_;
	UMAbcList drawer_list_;
};

/**
 * create instance
 */
UMAbcPtr UMAbc::create(DrawType type)
{
#ifdef WITH_DIRECTX
	if (type == eDirectX)
	{
		UMAbcPtr wrapper = std::make_shared<UMAbc>();
		wrapper->impl_->draw_type_ = type;
		return wrapper;
	}
#endif // WITH_DIRECTX

#ifdef WITH_OPENGL
	if (type == eOpenGL)
	{
		UMAbcPtr wrapper = std::make_shared<UMAbc>();
		wrapper->impl_->draw_type_ = type;
		return wrapper;
	}
#endif // WITH_OPENGL
	return UMAbcPtr();
}

/**
 * constructor
 */
UMAbc::UMAbc()
	: impl_(new UMAbc::Impl)
{
}

/**
 * destructor
 */
UMAbc::~UMAbc()
{
}

/**
 * release all scenes. call this function before delete.
 */
bool UMAbc::dispose()
{
	return impl_->dispose();
}

/**
 * initialize
 */
bool UMAbc::init(UMAbcSceneList& scene_list)
{
	return impl_->init(scene_list);
}

/**
 * initialize
 */
bool UMAbc::init(UMAbcScenePtr scene)
{
	return impl_->init(scene);
}
	
/**
 * tick forward
 */
bool UMAbc::update(unsigned long time)
{
	return impl_->update(time);
}
	
/**
 * refresh frame
 */
bool UMAbc::draw()
{
	return impl_->draw();
}

/**
 * create view/depth
 */
bool UMAbc::clear()
{
	return impl_->clear();
}

} // umabc
