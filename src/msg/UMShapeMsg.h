/**
 *
 * @file UMShapeMsg.h
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
				name_,
				base_geometry_node_id_,
				vertex_index_list_,
				normal_index_list_,
				vertex_list_,
				normal_list_);
#endif //WITH_MSGPACK
	