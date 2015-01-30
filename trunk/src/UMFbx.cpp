/**
 *
 * @file UMFbx.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#if  !defined (WITH_FBX2011) && !defined (WITH_FBX2013) && !defined(WITH_FBX2014) && !defined(WITH_FBX2015)

#include "UMFbx.h"
#include "UMObject.h"

namespace umio
{

UMObjectPtr UMFbx::load(std::string path, const UMIOSetting& setting) { return UMObjectPtr(); }

bool UMFbx::save(std::string path, UMObjectPtr object, const UMIOSetting& setting) { return false; }

} // namespace umio

#endif // !defined (WITH_FBX2011) && !defined (WITH_FBX2013)