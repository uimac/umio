/**
 *
 * main.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#if !defined(WITH_EMSCRIPTEN)

#include <stdio.h>
#include <iostream>
#include <string>

#include "UMIO.h"
#include "UMObject.h"
#include "UMAnimation.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#include <unistd.h>
#else
#include <windows.h>
#include <tchar.h>
#endif // __APPLE__

// run and convert
int main(int argc, char **argv) 
{
	char* srcpath = NULL;
	char* dstpath = NULL;
	char* iosettingpath = NULL;
	
	// input
	for (int i = 0; i < argc; ++i)
	{
		char* str = argv[i];
		//std::cout << str << std::endl;
		if (i == 1) srcpath = str;
		if (i == 2) dstpath = str;
		if (i == 3) iosettingpath = str;
		if (i >= 3) break;
	}
	
	if (!srcpath || !dstpath)
	{
		std::cout << "this is 3d data converter." << std::endl;
#ifdef WITH_BOOST_SERIALIZATION
		std::cout << "convert (.fbx) to (.bos) or (.bos) to (.fbx)." << std::endl;
#endif //WITH_BOOST_SERIALIZATION
		std::cout << "usage: ./UM.app absolute_src_filepath absolute_dst_filepath absolute_iosetting_filepath" << std::endl;
		std::cout << " " << std::endl;

#ifndef __APPLE__
	#ifdef WITH_FBX2013
			std::wstring fbx_license(
				_T("This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2012 Autodesk, Inc. All rights, reserved. ")
				_T("Such code is provided “as is” and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including ")
				_T("without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights. ")
				_T("In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, ")
				_T("or consequential damages (including, but not limited to, procurement of substitute goods or services; ")
				_T("loss of use, data, or profits; or business interruption) however caused and on any theory of liability, ")
				_T("whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of such code."));

			const HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD numWritten = 0;
			::WriteConsoleW(stdOut, fbx_license.c_str(), static_cast<DWORD>(fbx_license.size()), &numWritten, NULL);

	#endif // WITH_FBX2013
	#ifdef WITH_FBX2011
			std::wstring fbx_license(
				_T("This software contains Autodesk® FBX® code developed by Autodesk, Inc. Copyright 2010 Autodesk, Inc. All rights, reserved. ")
				_T("Such code is provided “as is” and Autodesk, Inc. disclaims any and all warranties, whether express or implied, including ")
				_T("without limitation the implied warranties of merchantability, fitness for a particular purpose or non-infringement of third party rights. ")
				_T("In no event shall Autodesk, Inc. be liable for any direct, indirect, incidental, special, exemplary, ")
				_T("or consequential damages (including, but not limited to, procurement of substitute goods or services; ")
				_T("loss of use, data, or profits; or business interruption) however caused and on any theory of liability, ")
				_T("whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of such code."));
		
			const HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
			DWORD numWritten = 0;
			::WriteConsoleW(stdOut, fbx_license.c_str(), static_cast<DWORD>(fbx_license.size()), &numWritten, NULL);

	#endif //WITH_FBX2011
#endif // __APPLE__
		return 0;
	}
	
	// path is exist?
#ifdef __APPLE__
	int src_is_exist = access(srcpath, R_OK);
	if (src_is_exist < 0) {
		std::cout << "src is not exist or access error." << std::endl;
		std::cout << "usage: ./UM.app absolute_src_path absolute_dst_path" << std::endl;
		return 0;
	}
#else
	//std::cout << "sorry, mac only." << std::endl;
	//return 0;
#endif
	
	std::string srcpath_str(srcpath);
	std::string dstpath_str(dstpath);
	std::string iosettingpath_str;
	if (iosettingpath)
	{
		iosettingpath_str = std::string(iosettingpath);
	}
	bool is_convert_bos_to_fbx = false;
	
	// find bos extension
	std::string::size_type ext_pos = srcpath_str.find_last_of(".");
	if (ext_pos == std::string::npos) {
		std::cout << "input file has not extension." << std::endl;
		return 0;
	}
	if (srcpath_str.substr(ext_pos + 1) == "bos")
	{
		is_convert_bos_to_fbx = true;
	}
	
	umio::UMIO io;
	umio::UMIOSetting setting;

	if (is_convert_bos_to_fbx)
	{
		// convert bos to fbx or other format 
		setting.set_bl_imp_bool_prop(umio::UMIOSetting::eUMImpFBX, false);
		setting.set_bl_exp_bool_prop(umio::UMIOSetting::eUMExpFBX, true);
	}
	else
	{
		// convert fbx or other format to bos
		setting.set_bl_imp_bool_prop(umio::UMIOSetting::eUMImpFBX, true);
		setting.set_bl_exp_bool_prop(umio::UMIOSetting::eUMExpFBX, false);
	}

	// if exists iosettingfile, use it
	if (!iosettingpath_str.empty())
	{
		io.load_setting(iosettingpath_str, setting);
	}
	
	// convert
	if (umio::UMObjectPtr obj = io.load(srcpath_str, setting))
	{
		if (io.save(dstpath_str, obj, setting))
		{
			std::cout << "convert success" << std::endl;
		}
	}

	std::cout << "success." << std::endl;
	return 0;
}

#endif // !defined(WITH_EMSCRIPTEN)
