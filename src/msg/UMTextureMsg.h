/**
 *
 * @file UMVector2Msg.h
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
				name_,
				file_name_,
				relative_file_name_,
				blend_mode_,
				alpha_);
#endif //WITH_MSGPACK
