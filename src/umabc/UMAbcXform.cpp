/**
 * @file UMAbcXform.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMAbcXform.h"
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include "UMAbcConvert.h"


namespace umabc
{
	
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

	class UMAbcXform::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(IXformPtr xform)
			: UMAbcObject(xform)
			, xform_(xform)
		{}

		~Impl() {}

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

		/**
		* draw
		* @param [in] recursive do children recursively
		*/
		virtual void draw(bool recursive, UMAbc::DrawType type);
		
		virtual UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}
		
		UMAbcXformWeakPtr self_reference_;

		/**
		* set minumum time
		*/
		void set_min_time(unsigned long time)
		{
			UMAbcObject::set_min_time(time);
		}

		/**
		* set maximum time
		*/
		void set_max_time(unsigned long time)
		{
			UMAbcObject::set_max_time(time);
		}

	private:
		IXformPtr xform_;

		UMMat44d static_matrix_;
		bool is_inherit_;
	};


/**
 * create
 */
UMAbcXformPtr UMAbcXform::create(IXformPtr xform)
{
	UMAbcXformPtr instance = UMAbcXformPtr(new UMAbcXform(xform));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcXform::UMAbcXform(IXformPtr xform)
	: impl_(new UMAbcXform::Impl(xform))
	, UMAbcObject(xform)
{}

UMAbcXform::~UMAbcXform() {}

/**
 * initialize
 */
bool UMAbcXform::Impl::init(bool recursive)
{
	if (!is_valid()) return false;
	
	mutable_local_transform().identity();
	static_matrix_.identity();

	if (xform_->getSchema().isConstant())
	{
		static_matrix_ = 
			UMAbcConvert::imath_mat_to_um(
				xform_->getSchema().getValue().getMatrix());
	}
	else
	{
		// not consistant.
		// we get time
		size_t num_samples = xform_->getSchema().getNumSamples();
		if (num_samples > 0)
		{
			TimeSamplingPtr time = xform_->getSchema().getTimeSampling();
			set_min_time(static_cast<unsigned long>(time->getSampleTime(0)*1000));
			set_max_time(static_cast<unsigned long>(time->getSampleTime(num_samples-1)*1000));
		}
	}
	return true;
}

/**
 * set current time
 */
void UMAbcXform::Impl::set_current_time(unsigned long time, bool recursive)
{
	if (!is_valid()) {
		mutable_local_transform().identity();
		return;
	}
	
	if (min_time() < time && time < max_time())
	{
		ISampleSelector selector(time / 1000.0, ISampleSelector::kNearIndex);
		is_inherit_ = xform_->getSchema().getInheritsXforms(selector);

		if (xform_->getSchema().isConstant())
		{
			mutable_local_transform() = static_matrix_;
		}
		else
		{
			mutable_local_transform() = 
				UMAbcConvert::imath_mat_to_um(
					xform_->getSchema().getValue(selector).getMatrix());
		}
	}
	UMAbcObject::set_current_time(time, false);
}

/**
 * update box
 */
void UMAbcXform::Impl::update_box(bool recursive)
{
	UMAbcObject::update_box(recursive);

	mutable_box().init();
	mutable_no_inherit_box().init();

	UMAbcObjectList::iterator it = mutable_children().begin();
	for (; it != mutable_children().end(); ++it)
	{
		UMAbcObjectPtr child = *it;
		if (!child->box().is_empty())
		{
			const UMBox& child_box = child->box();
			child_box.transformed(local_transform());
			mutable_global_transform() = mutable_global_transform() * local_transform();

			if (!child_box.is_empty())
			{
				if (is_inherit_)
				{
					mutable_box().extend(child_box);
				}
				else
				{
					mutable_no_inherit_box().extend(child_box);
				}
			}

			const UMBox& child_no_inherit_box = child->no_inherit_box();
			if (!child_no_inherit_box.is_empty())
			{
				mutable_no_inherit_box().extend(child_no_inherit_box);
			}
		}
	}
}

/**
 * refresh
 */
void UMAbcXform::Impl::draw(bool recursive, UMAbc::DrawType type)
{
}

/**
* initialize
* @param [in] recursive do children recursively
* @retval succsess or fail
*/
bool UMAbcXform::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
* set current time
* @param [in] time time
* @param [in] recursive do children recursively
*/
void UMAbcXform::set_current_time(unsigned long time, bool recursive)
{
	impl_->set_current_time(time, recursive);
	UMAbcObject::set_current_time(time, recursive);
}

/**
* update box
* @param [in] recursive do children recursively
*/
void UMAbcXform::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

/**
* draw
* @param [in] recursive do children recursively
*/
void UMAbcXform::draw(bool recursive, UMAbc::DrawType type)
{
	impl_->draw(recursive, type);
	UMAbcObject::draw(recursive, type);
}

UMAbcObjectPtr UMAbcXform::self_reference()
{
	return impl_->self_reference();
}

/**
* get minumum time
*/
unsigned long UMAbcXform::min_time() const
{
	return impl_->min_time();
}

/**
* get maximum time
*/
unsigned long UMAbcXform::max_time() const
{
	return impl_->max_time();
}

/**
* set minumum time
*/
void UMAbcXform::set_min_time(unsigned long time)
{
	impl_->set_min_time(time);
}

/**
* set maximum time
*/
void UMAbcXform::set_max_time(unsigned long time)
{
	impl_->set_max_time(time);
}

/**
* get current time
*/
double UMAbcXform::current_time() const
{
	return impl_->current_time();
}

} // umabc
