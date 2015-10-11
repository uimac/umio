/**
 * @file UMAbcCamera.h
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
			class ICameraSchema;
			typedef Alembic::Abc::v7::ISchemaObject<ICameraSchema> ICamera;
		}
	}
}

namespace umdraw
{

class UMCamera;
typedef std::shared_ptr<UMCamera> UMCameraPtr;

class UMOpenGLCamera;
typedef std::shared_ptr<UMOpenGLCamera> UMOpenGLCameraPtr;

class UMDirectX11Camera;
typedef std::shared_ptr<UMDirectX11Camera> UMDirectX11CameraPtr;
} // umdraw

namespace umabc
{
typedef std::shared_ptr<Alembic::AbcGeom::v7::ICamera> ICameraPtr;
	
class UMAbcCamera;
typedef std::shared_ptr<UMAbcCamera> UMAbcCameraPtr;
typedef std::weak_ptr<UMAbcCamera> UMAbcCameraWeakPtr;

class UMAbcCamera : public UMAbcObject
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcCamera);
public:

	/**
	 * crate instance
	 */
	static UMAbcCameraPtr create(ICameraPtr camera);

	~UMAbcCamera();

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
	 * get umcamera
	 */
	umdraw::UMCameraPtr umcamera() const;
	
protected:
	UMAbcCamera(ICameraPtr camera);
	
	virtual UMAbcObjectPtr self_reference();

private:
	class Impl;
	std::unique_ptr<Impl> impl_;
};

}
