/**
 *
 * UMIO.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#include "UMIO.h"
#include "UMObject.h"
#include "UMFbx.h"

#include <string>
#include <map>
#include <iostream>
#include <fstream>


#ifdef WITH_BOOST_SERIALIZATION
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#endif

namespace UM
{

/**
 * load file
 */
UMObjectPtr UMIO::load(std::string path, const UMIOSetting& setting)
{
	bool is_fbx_load = false;
	UMIOSetting::UMImpSettingMap::const_iterator bt = setting.bl_imp_prop_map().begin();
	for (; bt != setting.bl_imp_prop_map().end(); ++bt)
	{
		UMIOSetting::EUMImpSettingType type = bt->first;
		bool val = bt->second;

		if (type == UMIOSetting::eUMImpFBX)
		{
			is_fbx_load = val;
		}
	}
	
	// load by fbx sdk
	if (is_fbx_load)
	{
		UMFbx fbx;
		return fbx.load(path, setting);
	}
	
#ifdef WITH_BOOST_SERIALIZATION
	// load bos
	try {
		// convert bos to fbx or other format 
		std::ifstream file(path, std::ios::in | std::ios::binary);
		boost::archive::binary_iarchive ia(file);
		
		UM::UMObjectPtr obj = UM::UMObject::create_object();
		
		ia >> (*obj);
		
		file.close();

		if (UMObject::re_bind_all_nodes(obj))
		{
			return obj;
		}
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return UMObjectPtr();
	}
#endif // WITH_BOOST_SERIALIZATION

	return UMObjectPtr();
}

/**
 * save file
 */
bool UMIO::save(std::string path, UMObjectPtr object, const UMIOSetting& setting)
{
	bool is_fbx_save = false;
	UMIOSetting::UMExpSettingMap::const_iterator bt = setting.bl_exp_prop_map().begin();
	for (; bt != setting.bl_exp_prop_map().end(); ++bt)
	{
		UMIOSetting::EUMExpSettingType type = bt->first;
		bool val = bt->second;

		if (type == UMIOSetting::eUMExpFBX)
		{
			is_fbx_save = val;
		}
	}
	
	// save by fbx sdk
	if (is_fbx_save)
	{
		UMFbx fbx;
		return fbx.save(path, object, setting);
	}
	
#ifdef WITH_BOOST_SERIALIZATION
	// save bos
	try {
		// convert fbx or other format to bos
		std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
		boost::archive::binary_oarchive oa(file);

		oa << (*object);

		file.flush();
		file.close();

		return true;
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return false;
	}
#endif // WITH_BOOST_SERIALIZATION

	return false;
}

/**
 * load settings
 */
bool UMIO::load_setting(std::string path, UMIOSetting& setting)
{
#ifdef WITH_BOOST_SERIALIZATION
	// load bos
	try {
		// convert bos to fbx or other format 
		std::ifstream file(path, std::ios::in | std::ios::binary);
		boost::archive::binary_iarchive ia(file);
		
		ia >> (setting);
		
		file.close();

		return true;
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return UMObjectPtr();
	}
#endif // WITH_BOOST_SERIALIZATION
	return false;
}
	
/**
 * save settings
 */
bool UMIO::save_setting(std::string path, const UMIOSetting& setting)
{
#ifdef WITH_BOOST_SERIALIZATION
	// save bos
	try {
		// convert fbx or other format to bos
		std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
		boost::archive::binary_oarchive oa(file);

		oa << setting;
		
		file.flush();
		file.close();

		return true;
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return false;
	}
#endif // WITH_BOOST_SERIALIZATION
	return false;
}

} // namespace UM