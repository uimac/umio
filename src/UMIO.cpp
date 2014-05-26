/**
 *
 * @file UMIO.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#include "UMIO.h"
#include "UMObject.h"

#if defined(WITH_FBX2014) || defined(WITH_FBX2013) || defined(WITH_FBX2011)
#define WITH_FBX
#endif

#ifdef WITH_FBX
#include "UMFbx.h"
#endif

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef WITH_BOOST_SERIALIZATION
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#endif

#ifdef WITH_MSGPACK
#include <msgpack.hpp>
#endif

namespace umio
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
	
#ifdef WITH_FBX
	// load by fbx sdk
	if (is_fbx_load)
	{
		UMFbx fbx;
		if (UMObjectPtr obj = fbx.load(path, setting))
		{
			return obj;
		}
	}
#endif
	
#ifdef WITH_BOOST_SERIALIZATION
	// load bos
	try {
		// convert bos to fbx or other format 
		std::ifstream file(path, std::ios::in | std::ios::binary);
		boost::archive::binary_iarchive ia(file);
		
		umio::UMObjectPtr obj = umio::UMObject::create_object();
		
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


#ifdef WITH_MSGPACK
	// load msgpack msg
	try {
		// convert msg to fbx or other format 
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::istreambuf_iterator<char> first(file);
		std::istreambuf_iterator<char> last;
		const std::string data(first, last);
			
		msgpack::zone zone;
		msgpack::object msg_obj;
		msgpack::unpack(data.data(), data.size(), NULL, &zone, &msg_obj);
			
		umio::UMObjectPtr obj = umio::UMObject::create_object();
		msg_obj.convert(&(*obj));
		return obj;
	}
	catch (msgpack::unpack_error& ) {
		//std::cout << "msg unpack failed" << std::endl;
		return UMObjectPtr();
	}
	catch (msgpack::type_error& ) {
		//std::cout << "msg type error" << std::endl;
		return UMObjectPtr();
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return UMObjectPtr();
	}
#endif // WITH_MSGPACK

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
	
#ifdef WITH_FBX
	// save by fbx sdk
	if (is_fbx_save)
	{
		UMFbx fbx;
		return fbx.save(path, object, setting);
	}
#endif
	
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
	
#ifdef WITH_MSGPACK
	// save msgpack msg
	try {
		// convert fbx or other format to msg
		std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
		msgpack::pack(&file, *object);
		if (object) {
			return true;
		}
				
		/* // for debug
		msgpack::sbuffer sbuf;
		msgpack::pack(sbuf, *obj);
				 
		msgpack::zone zone;
		msgpack::object msg_obj;
		msgpack::unpack(sbuf.data(), sbuf.size(), NULL, &zone, &msg_obj);
		UM::UMObjectPtr new_obj = UM::UMObject::create_object();
		msg_obj.convert(&(*new_obj));
		new_obj = new_obj;
			*/
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return false;
	}
#endif // WITH_MSGPACK

	return false;
}

/**
 * load file
 */
UMObjectPtr UMIO::load_bos_from_memory(const std::string& src, const UMIOSetting& setting)
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
	
#ifdef WITH_BOOST_SERIALIZATION
	// load bos
	try {
		std::istringstream stream(src);

		// convert bos to fbx or other format 
		boost::archive::binary_iarchive ia(stream);
		
		umio::UMObjectPtr obj = umio::UMObject::create_object();
		
		ia >> (*obj);
		
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
	
#ifdef WITH_MSGPACK
	// load msgpack msg
	try {
		// convert msg to fbx or other format 
		std::istringstream stream(src);
		std::istreambuf_iterator<char> first(stream);
		std::istreambuf_iterator<char> last;
		const std::string data(first, last);
			
		msgpack::zone zone;
		msgpack::object msg_obj;
		msgpack::unpack(data.data(), data.size(), NULL, &zone, &msg_obj);
			
		umio::UMObjectPtr obj = umio::UMObject::create_object();
		msg_obj.convert(&(*obj));
		return obj;
	}
	catch (msgpack::unpack_error& ) {
		//std::cout << "msg unpack failed" << std::endl;
		return UMObjectPtr();
	}
	catch (msgpack::type_error& ) {
		//std::cout << "msg type error" << std::endl;
		return UMObjectPtr();
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return UMObjectPtr();
	}
#endif // WITH_MSGPACK

	return UMObjectPtr();
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


#ifdef WITH_MSGPACK
	// load msgpack msg
	try {
		// convert msg to fbx or other format 
		std::ifstream file(path, std::ios::in | std::ios::binary);
		std::istreambuf_iterator<char> first(file);
		std::istreambuf_iterator<char> last;
		const std::string data(first, last);
			
		msgpack::zone zone;
		msgpack::object msg_obj;
		msgpack::unpack(data.data(), data.size(), NULL, &zone, &msg_obj);
			
		msg_obj.convert(&setting);
		return true;
	}
	catch (msgpack::unpack_error& ) {
		//std::cout << "msg unpack failed" << std::endl;
		return false;
	}
	catch (msgpack::type_error& ) {
		//std::cout << "msg type error" << std::endl;
		return false;
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return false;
	}
#endif // WITH_MSGPACK

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
	
#ifdef WITH_MSGPACK
	// save msgpack msg
	try {
		// convert fbx or other format to msg
		std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::trunc);
		msgpack::pack(&file, setting);
		return true;
	}
	catch (...) {
		//std::cout << "unknown excaption" << std::endl;
		return false;
	}
#endif // WITH_MSGPACK

	return false;
}

} // namespace umio
