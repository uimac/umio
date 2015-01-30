/**
 *
 * @file UMFbx.h
 *
 * @author tori31001 at gmail.com
 * 
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#pragma once

#include "UMObject.h"
#include "UMAnimation.h"
#include "UMIOSetting.h"

#include <string>

namespace umio
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


} // namespace umio
