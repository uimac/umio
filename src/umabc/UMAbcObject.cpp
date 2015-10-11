/**
 * @file UMAbcObject.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <memory>
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/Abc/IObject.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcMaterial/All.h>
#include <Alembic/AbcCoreFactory/All.h>
#include "UMAbcObject.h"
#include "UMAbcMesh.h"
#include "UMAbcXform.h"
#include "UMAbcPoint.h"
#include "UMAbcCurve.h"
#include "UMAbcNurbsPatch.h"
#include "UMAbcCamera.h"
#include "UMObject.h"

namespace umabc
{

	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

	class UMAbcObject::Impl : public umio::UMNode
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(IObjectPtr object)
			: object_(object)
			, min_time_(0)
			, max_time_(0)
			, current_time_(0)
		{
		}

		~Impl(){}

		/**
		* initialize
		* @param [in] recursive do children recursively
		*/
		bool init(bool recursive, UMAbcObjectPtr parent);

		/**
		* get minumum time
		*/
		unsigned long min_time() const { return min_time_; }

		/**
		* get maximum time
		*/
		unsigned long max_time() const { return max_time_; }

		/**
		* set current time
		*/
		void set_current_time(unsigned long time, bool recursive);

		/**
		* get current time
		*/
		double current_time() const { return static_cast<double>(current_time_ / 1000.0); }
		/**
		* get current time
		*/
		unsigned int current_time_ms() const { return current_time_; }

		/**
		* is valid or not
		*/
		bool is_valid() const { return object_ ? object_->valid() : false; }

		/**
		* is visible or not
		*/
		bool is_visible() const;

		/**
		* get bounding box
		*/
		const UMBox& box() const { return box_; }

		/**
		* get no inherit bounding box
		*/
		const UMBox& no_inherit_box() const { return no_inherit_box_; }

		/**
		* update box
		* @param [in] recursive do children recursively
		*/
		void update_box(bool recursive);

		///**
		//* draw
		//*/
		//void draw(bool recursive, UMAbc::DrawType type);

		/**
		* get children
		*/
		const UMAbcObjectList& children() const { return children_; }

		/**
		* get children
		*/
		UMAbcObjectList& mutable_children() { return children_; }

		/**
		* get parent
		*/
		UMAbcObjectPtr parent() { return parent_object_.lock(); }

		void set_parent_object(UMAbcObjectPtr parent)
		{
			parent_object_ = parent;
		}

		/**
		* get name
		*/
		const std::string& name() const { return name_; }

		void set_min_time(unsigned long time) { min_time_ = time; }

		void set_max_time(unsigned long time) { max_time_ = time; }

		/**
		* get bounding box
		*/
		UMBox& mutable_box() { return box_; }

		/**
		* get no inherit bounding box
		*/
		UMBox& mutable_no_inherit_box() { return no_inherit_box_; }

		UMAbcObjectPtr self_reference() { return self_reference_.lock(); }
		
		UMAbcObjectWeakPtr self_reference_;

	protected:
		IObjectPtr object_;

		std::string name_;

		unsigned long min_time_;
		unsigned long max_time_;
		unsigned long current_time_;

		UMBox box_;
		UMBox no_inherit_box_;
		UMAbcObjectList children_;
		UMAbcObjectWeakPtr parent_object_;
	};

/**
 * initialize
 */
bool UMAbcObject::Impl::init(bool recursive, UMAbcObjectPtr parent)
{
	if (!is_valid()) return false;
	if (!recursive) return true;

	name_ = object_->getName();

	if (!children_.empty())
	{
		children_.clear();
	}

	const size_t child_count = object_->getNumChildren();
	for (size_t i = 0; i < child_count; ++i)
	{
		const ObjectHeader &ohead = object_->getChildHeader(i);
		UMAbcObjectPtr child;
		if (IPolyMesh::matches(ohead))
		{
			IPolyMeshPtr mesh(new IPolyMesh(*object_, ohead.getName()));
			child = UMAbcMesh::create(mesh);
			UMAbcObjectPtr ref = parent;
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		else if (IPoints::matches(ohead))
		{
			IPointsPtr points(new IPoints(*object_, ohead.getName()));
			child = UMAbcPoint::create(points);
			UMAbcObjectPtr ref = parent;
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		else if (ICurves::matches(ohead))
		{
			ICurvesPtr curves(new ICurves(*object_, ohead.getName()));
			child = UMAbcCurve::create(curves);
			UMAbcObjectPtr ref = parent;
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		else if (INuPatch::matches(ohead))
		{
			INuPatchPtr patch(new INuPatch(*object_, ohead.getName()));
			child = UMAbcNurbsPatch::create(patch);
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		else if (IXform::matches(ohead))
		{
			IXformPtr xform(new IXform(*object_, ohead.getName()));
			child = UMAbcXform::create(xform);
			UMAbcObjectPtr ref = parent;
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		else if (ICamera::matches(ohead))
		{
			ICameraPtr camera(new ICamera(*object_, ohead.getName()));
			child = UMAbcCamera::create(camera);
			UMAbcObjectPtr ref = parent;
			child->set_parent_object(parent);
			children_.push_back(child);
		}
		// recursive
		if (child && recursive)
		{
			child->init(recursive, child);
		}

		// set time
		if (child && child->is_valid())
		{
			if (parent) 
			{
				parent->set_min_time(std::min(parent->min_time(), child->min_time()));
				parent->set_max_time(std::max(parent->max_time(), child->max_time()));
			}
			else
			{
				set_min_time(std::min(min_time(), child->min_time()));
				set_max_time(std::max(max_time(), child->max_time()));
			}
		}
	}
	return true;
}

/**
 * set current time
 */
void UMAbcObject::Impl::set_current_time(unsigned long time, bool recursive)
{
	if (!object_) return;

	current_time_ = time;

	if (recursive)
	{
		UMAbcObjectList::iterator it = children_.begin();
		for (; it != children_.end(); ++it)
		{
			UMAbcObjectPtr child = *it;
			child->set_current_time(time, recursive);
		}
	}
}

/**
 * update box
 */
void UMAbcObject::Impl::update_box(bool recursive)
{
	box_.init();
	UMAbcObjectList::iterator it = children_.begin();
	for (; it != children_.end(); ++it)
	{
		UMAbcObjectPtr child = *it;
		if (recursive)
		{
			child->update_box(recursive);
		}
		box_.extend(child->box());
	}
}

/**
 * is visible
 */
bool UMAbcObject::Impl::is_visible() const
{
	Alembic::Abc::ICompoundProperty props = object_->getProperties();
	const Alembic::Abc::PropertyHeader* header = props.getPropertyHeader("visible");
	if (header)
	{
		Alembic::Abc::IScalarProperty visible(props, "visible");
		Alembic::Abc::ISampleSelector selector(current_time());
		Alembic::Abc::int8_t is_visible = 1;
		visible.get(reinterpret_cast<void*>(&is_visible), selector);
		return is_visible == 1;
	}
	return false;
}

///**
// * draw
// */
//void UMAbcObject::Impl::draw(bool recursive, UMAbc::DrawType type)
//{
//	if (!is_valid()) return;
//	//if (!is_visible()) return;
//	
//	UMAbcObjectList::const_iterator it = children_.begin();
//	for (; it != children_.end(); ++it)
//	{
//		UMAbcObjectPtr child = *it;
//		if (recursive)
//		{
//			child->draw(recursive, type);
//		}
//	}
//}

/**
 * crate instance
 */
UMAbcObjectPtr UMAbcObject::create(IObjectPtr object)
{
	UMAbcObjectPtr instance = UMAbcObjectPtr(new UMAbcObject(object));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcObject::UMAbcObject(IObjectPtr object)
	: impl_(new UMAbcObject::Impl(object))
{}

UMAbcObject::~UMAbcObject(){}


/**
* initialize
* @param [in] recursive do children recursively
*/
bool UMAbcObject::init(bool recursive, UMAbcObjectPtr parent)
{
	return impl_->init(recursive, parent);
}

/**
* get minumum time
*/
unsigned long UMAbcObject::min_time() const
{
	return impl_->min_time();
}

/**
* get maximum time
*/
unsigned long UMAbcObject::max_time() const
{
	return impl_->max_time();
}

/**
* set current time
*/
void UMAbcObject::set_current_time(unsigned long time, bool recursive)
{
	return impl_->set_current_time(time, recursive);
}

/**
* get current time
*/
double UMAbcObject::current_time() const
{
	return impl_->current_time();
}

/**
* get current time
*/
unsigned int UMAbcObject::current_time_ms() const
{
	return static_cast<unsigned int>(impl_->current_time());
}

/**
* is valid or not
*/
bool UMAbcObject::is_valid() const
{ 
	return impl_->is_valid();
}

/**
* is visible or not
*/
bool UMAbcObject::is_visible() const
{
	return impl_->is_visible();
}

/**
* get bounding box
*/
const UMBox& UMAbcObject::box() const
{
	return impl_->box();
}

/**
* get no inherit bounding box
*/
const UMBox& UMAbcObject::no_inherit_box() const
{
	return impl_->no_inherit_box();
}

/**
* update box
* @param [in] recursive do children recursively
*/
void UMAbcObject::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

///**
//* draw
//*/
//void UMAbcObject::draw(bool recursive, UMAbc::DrawType type)
//{
//	impl_->draw(recursive, type);
//}

/**
* get children
*/
const UMAbcObjectList& UMAbcObject::children() const
{
	return impl_->children();
}

/**
* get children
*/
UMAbcObjectList& UMAbcObject::mutable_children()
{
	return impl_->mutable_children();
}

/**
* get parent
*/
UMAbcObjectPtr UMAbcObject::parent()
{
	return impl_->parent();
}

/**
* get name
*/
const std::string& UMAbcObject::name() const
{
	return impl_->name();
}

/**
* get minumum time
*/
void UMAbcObject::set_min_time(unsigned long time)
{
	impl_->set_min_time(time);
}

/**
* get maximum time
*/
void UMAbcObject::set_max_time(unsigned long time)
{
	impl_->set_max_time(time);
}

/**
* get bounding box
*/
UMBox& UMAbcObject::mutable_box()
{
	return impl_->mutable_box();
}

/**
* get no inherit bounding box
*/
UMBox& UMAbcObject::mutable_no_inherit_box()
{
	return impl_->mutable_no_inherit_box();
}

UMAbcObjectPtr UMAbcObject::self_reference()
{
	return impl_->self_reference();
}

void UMAbcObject::set_parent_object(UMAbcObjectPtr parent)
{
	impl_->set_parent_object(parent);
}

} // umabc
