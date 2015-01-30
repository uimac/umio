/**
 *
 * @file UMIO.h
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
#include "UMIOSetting.h"

#include <string>

/// Uimac model io library
namespace umio
{

class UMIO
{
public:
	UMIO(){}

	~UMIO(){}
	
	/**
	 * load 3d file to UMObject
	 */
	UMObjectPtr load(std::string path, const UMIOSetting& setting);
	
	/**
	 * save 3d file
	 */
	bool save(std::string path, UMObjectPtr object, const UMIOSetting& setting);
	
	/**
	 * load bos from memory
	 */
	UMObjectPtr load_from_memory(const std::string& src, const UMIOSetting& setting);

	/**
	 * save bos to memory
	 */
	char* save_to_memory(int& dst_size, UMObjectPtr object, const UMIOSetting& setting);

	/**
	 * load settings
	 */
	bool load_setting(std::string path, UMIOSetting& setting);
	
	/**
	 * save settings
	 */
	bool save_setting(std::string path, const UMIOSetting& setting);
};


} // namespace umio
