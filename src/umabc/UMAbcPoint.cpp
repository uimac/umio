/**
 * @file UMAbcPoint.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

#include "UMAbcPoint.h"

#include "UMAbcConvert.h"

#ifdef WITH_DIRECTX
	#include "UMDirectX11.h"
#endif

#ifdef WITH_OPENGL
	#include "UMOpenGLAbcPoint.h"
#endif

namespace umabc
{
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;
	
	class UMAbcPoint::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(IPointsPtr points)
			: UMAbcObject(points)
			, points_(points)
		{}

		~Impl() {}

		virtual UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}

		/**
		* initialize
		* @param [in] recursive do children recursively
		* @retval succsess or fail
		*/
		virtual bool init(bool recursive);

		/**
		* set current time
		* @param [in] time time
		* @param [in] recursive do children recursively
		*/
		virtual void set_current_time(unsigned long time, bool recursive);

		/**
		* update box
		* @param [in] recursive do children recursively
		*/
		virtual void update_box(bool recursive);

		///**
		//* draw
		//* @param [in] recursive do children recursively
		//*/
		//virtual void draw(bool recursive, UMAbc::DrawType type);

		/**
		* update point all
		*/
		void update_point_all();

		/**
		* get opengl point
		*/
		UMOpenGLAbcPointPtr opengl_point() { return opengl_point_; }

		UMAbcPointWeakPtr self_reference_;
	private:
		/**
		* update points
		*/
		void update_point();

		/**
		* update normal
		*/
		void update_normal();

		/**
		* update color
		*/
		void update_color();

		IPointsPtr points_;
		Alembic::AbcGeom::IPointsSchema::Sample sample_;

		Alembic::AbcGeom::IBox3dProperty initial_bounds_prop_;
		Alembic::AbcGeom::IC3fArrayProperty color_prop_;
		Alembic::AbcGeom::IN3fArrayProperty normal_prop_;

		Alembic::AbcGeom::P3fArraySamplePtr positions_;
		Alembic::AbcGeom::C3fArraySamplePtr colors_;
		Alembic::AbcGeom::N3fArraySamplePtr normals_;

		UMOpenGLAbcPointPtr opengl_point_;
	};

/**
 * create
 */
UMAbcPointPtr UMAbcPoint::create(IPointsPtr points)
{
	UMAbcPointPtr instance = UMAbcPointPtr(new UMAbcPoint(points));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcPoint::UMAbcPoint(IPointsPtr points)
	: UMAbcObject(points)
	, impl_(new UMAbcPoint::Impl(points))
{}

UMAbcPoint::~UMAbcPoint()
{
}

/**
 * initialize
 */
bool UMAbcPoint::Impl::init(bool recursive)
{
	if (!is_valid()) return false;
	
	// try to create colors.
	IPointsSchema &schema = points_->getSchema();
	if (const PropertyHeader *head = schema.getPropertyHeader("Cs"))
	{
		if (IC3fArrayProperty::matches(*head))
		{
			color_prop_ = IC3fArrayProperty(schema, "Cs");
		}
	}
	// try to create normals.
	if (const PropertyHeader *head = schema.getPropertyHeader("N"))
	{
		normal_prop_ = IN3fArrayProperty(schema, "N");
	}

	const size_t num_samples = schema.getNumSamples();
	if (num_samples > 0)
	{
		// get sample
		schema.get(sample_);
		
		// if not consistant, we get time
		if (!schema.isConstant())
		{
			TimeSamplingPtr time = schema.getTimeSampling();
			set_min_time(static_cast<unsigned long>(time->getSampleTime(0)*1000));
			set_max_time(static_cast<unsigned long>(time->getSampleTime(num_samples - 1) * 1000));
		}
	}
	
#ifdef WITH_OPENGL
	if (UMAbcPointPtr self = self_reference_.lock())
	{
		opengl_point_ = UMOpenGLAbcPointPtr(new UMOpenGLAbcPoint(self));
	}
#endif // WITH_OPENGL
	return true;
}

/**
 * set_current_time
 */
void UMAbcPoint::Impl::set_current_time(unsigned long time, bool recursive)
{
	if (!is_valid()) return;
	update_point_all();
}

/**
 * update box
 */
void UMAbcPoint::Impl::update_box(bool recursive)
{
	mutable_box().init();

	if (initial_bounds_prop_ && initial_bounds_prop_.getNumSamples() > 0)
	{
		ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);

		mutable_box() = 
			UMAbcConvert::imath_box_to_um(
				initial_bounds_prop_.getValue(selector));
	}

	if (mutable_box().is_empty() && positions_)
	{
		size_t point_count = positions_->size();
		for (size_t i = 0; i < point_count; ++i)
		{
			const Imath::V3f& p = (*positions_)[i];
			mutable_box().extend(UMAbcConvert::imath_vec_to_um(p));
		}
	}
}

/** 
 * update color
 */
void UMAbcPoint::Impl::update_color()
{
	if (color_prop_)
	{
		ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
		colors_ = color_prop_.getValue(selector);
	}
}

/** 
 * update normal
 */
void UMAbcPoint::Impl::update_normal()
{
	if (normal_prop_)
	{
		ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
		normals_ = normal_prop_.getValue(selector);
	}
}

void UMAbcPoint::Impl::update_point()
{
	if (!is_valid()) return;
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
	IPointsSchema::Sample sample;
	points_->getSchema().get(sample, selector);
	positions_ = sample.getPositions();
	
#ifdef WITH_OPENGL
	if (opengl_point_ && !umdraw::UMDirectX11::current_device_pointer())
	{
		opengl_point_->update_vertex(positions_);
	}
#endif // WITH_OPENGL

}

/** 
 * update point all
 */
void UMAbcPoint::Impl::update_point_all()
{
	if (!is_valid()) return;
	update_point();
	update_color();
	update_normal();
}

//void UMAbcPoint::Impl::draw(bool recursive, UMAbc::DrawType type)
//{
//	if (!is_valid()) return;
//	if (!positions_) return;
//	
//#ifdef WITH_OPENGL
//	if (type == UMAbc::eOpenGL && !umdraw::UMDirectX11::current_device_pointer())
//	{
//		if (opengl_point_)
//		{
//			opengl_point_->draw();
//		}
//	}
//#endif // WITH_OPENGL
//}
/**
* initialize
* @param [in] recursive do children recursively
* @retval succsess or fail
*/
bool UMAbcPoint::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
* set current time
* @param [in] time time
* @param [in] recursive do children recursively
*/
void UMAbcPoint::set_current_time(unsigned long time, bool recursive)
{
	if (!impl_->is_valid()) return;
	UMAbcObject::set_current_time(time, recursive);
	impl_->set_current_time(time, recursive);
}

/**
* update box
* @param [in] recursive do children recursively
*/
void UMAbcPoint::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

///**
//* draw
//* @param [in] recursive do children recursively
//*/
//void UMAbcPoint::draw(bool recursive, UMAbc::DrawType type)
//{
//	impl_->draw(recursive, type);
//	UMAbcObject::draw(recursive, type);
//}

/**
* update point all
*/
void UMAbcPoint::update_point_all()
{
	impl_->update_point_all();
}

/**
* get opengl point
*/
UMOpenGLAbcPointPtr UMAbcPoint::opengl_point()
{
	return impl_->opengl_point();
}

UMAbcObjectPtr UMAbcPoint::self_reference()
{
	return impl_->self_reference();
}

}

