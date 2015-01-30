/**
 *
 * @file UMVector4Msg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	MSGPACK_DEFINE(
				x,
				y,
				z,
				w);
#endif //WITH_MSGPACK
	