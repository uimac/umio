/**
 * @file UMAbcCurve.h
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
#include "UMMacro.h"
#include "UMAbcObject.h"

namespace Alembic
{
	namespace Abc {
		namespace v7 {
			template <class SCHEMA>
			class ISchemaObject;
		}
	}
	namespace AbcGeom {
		namespace v7 {
			class ICurvesSchema;
			typedef Alembic::Abc::v7::ISchemaObject<ICurvesSchema> ICurves;
		}
	}
}

namespace umabc
{
typedef std::shared_ptr<Alembic::AbcGeom::v7::ICurves> ICurvesPtr;
	
class UMAbcCurve;
typedef std::shared_ptr<UMAbcCurve> UMAbcCurvePtr;
typedef std::weak_ptr<UMAbcCurve> UMAbcCurveWeakPtr;

class UMAbcCurve : public UMAbcObject
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcCurve);
public:

	/**
	 * crate instance
	 */
	static UMAbcCurvePtr create(ICurvesPtr curves);

	~UMAbcCurve();

	/**
	 * initialize
	 * @param [in] recursive do children recursively
	 * @retval succsess or fail
	 */
	virtual bool init(bool recursive, UMAbcObjectPtr parent);
	
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
	
	/**
	 * update curve all
	 */
	void update_curve_all();

protected:
	UMAbcCurve(ICurvesPtr curves);
	
	virtual UMAbcObjectPtr self_reference();

private:
	class Impl;
	std::unique_ptr <Impl> impl_;
};

}
