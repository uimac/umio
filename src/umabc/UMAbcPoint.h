/**
 * @file UMAbcPoint.h
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
#include <algorithm>

#include "UMMacro.h"
#include "UMAbcObject.h"

namespace Alembic {
	namespace Abc {
		namespace v7 {
			template <class SCHEMA>
			class ISchemaObject;
		}
	}
	namespace AbcGeom {
		namespace v7 {
			class IPointsSchema;
			typedef Alembic::Abc::v7::ISchemaObject<IPointsSchema> IPoints;
		}
	}
}

namespace umabc
{
typedef std::shared_ptr<Alembic::AbcGeom::v7::IPoints> IPointsPtr;
	
class UMAbcPoint;
typedef std::shared_ptr<UMAbcPoint> UMAbcPointPtr;
typedef std::weak_ptr<UMAbcPoint> UMAbcPointWeakPtr;

class UMOpenGLAbcPoint;
typedef std::shared_ptr<UMOpenGLAbcPoint> UMOpenGLAbcPointPtr;

class UMAbcPoint : public UMAbcObject
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcPoint);
public:

	/**
	 * crate instance
	 */
	static UMAbcPointPtr create(IPointsPtr points);

	~UMAbcPoint();

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

	///**
	// * draw
	// * @param [in] recursive do children recursively
	// */
	//virtual void draw(bool recursive, UMAbc::DrawType type);
	
	/** 
	 * update point all
	 */
	void update_point_all();
	
	/**
	 * get opengl point
	 */
	UMOpenGLAbcPointPtr opengl_point();

	///**
	// * get directx point
	// */
	//UMDirectX11AbcPointPtr directx_point() { return directx_point_; }

protected:
	UMAbcPoint(IPointsPtr points);

	virtual UMAbcObjectPtr self_reference();

private:
	class Impl;
	std::unique_ptr <Impl> impl_;
};

}
