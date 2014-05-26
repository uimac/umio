/**
 *
 * @file UMPoseMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	MSGPACK_DEFINE(
		bind_pose_,
		rest_pose_,
		node_id_list_,
		matrix_is_local_list_,
		matrix_list_);
#endif //WITH_MSGPACK
