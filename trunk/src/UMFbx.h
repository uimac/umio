/**
 *
 * UMFbx.h
 *
 * @author tori31001 at gmail.com
 * 
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#pragma once

#include "UMObject.h"
#include "UMAnimation.h"
#include "UMIOSetting.h"

#include <string>

namespace UM
{

class UMFbx
{
public:
	UMFbx(){}

	~UMFbx(){}
	
	/**
	 * load fbx file to UMObject
	 */
	UMObjectPtr load(std::string path, const UMIOSetting& setting);
	
	/**
	 * save fbx file
	 */
	bool save(std::string path, UMObjectPtr object, const UMIOSetting& setting);
};


} // namespace UM