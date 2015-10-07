/**
 * @file UMAbcNurbsPatch.h
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

namespace Alembic {
	namespace Abc {
		namespace v7 {
			template <class SCHEMA>
			class ISchemaObject;
		}
	}
	namespace AbcGeom {
		namespace v7 {
			class INuPatchSchema;
			typedef Alembic::Abc::v7::ISchemaObject<INuPatchSchema> INuPatch;
		}
	}
}

namespace umabc
{
typedef std::shared_ptr<Alembic::AbcGeom::v7::INuPatch> INuPatchPtr;
	
class UMAbcNurbsPatch;
typedef std::shared_ptr<UMAbcNurbsPatch> UMAbcNurbsPatchPtr;
typedef std::weak_ptr<UMAbcNurbsPatch> UMAbcNurbsPatchWeakPtr;

class UMAbcNurbsPatch : public UMAbcObject
{
	DISALLOW_COPY_AND_ASSIGN(UMAbcNurbsPatch);
public:

	/**
	 * crate instance
	 */
	static UMAbcNurbsPatchPtr create(INuPatchPtr patch);

	~UMAbcNurbsPatch();

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
	 * update patch all
	 */
	void update_patch_all();

protected:
	UMAbcNurbsPatch(INuPatchPtr patch);

	virtual UMAbcObjectPtr self_reference();

private:
	class Impl;
	std::unique_ptr <Impl> impl_;
};

}
