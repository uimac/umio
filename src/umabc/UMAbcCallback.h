/**
 * @file UMAbcCallback.h
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include <functional>
#include "UMAny.h"

/// uimac alembic library
namespace umabc
{
typedef std::function<void (int state, umbase::UMAny& parameter)> UMAbcDrawCallback;

enum UMDrawCallbackState 
{
	eDrawCallbackStateBegin,
	eDrawCallbackStateNormal,
	eDrawCallbackStateVertex,
	eDrawCallbackStateUV,
	eDrawCallbackStatePreDraw,
	eDrawCallbackStateSceneChanged,
	eDrawCallbackStateEnd,
};

} // umabc
