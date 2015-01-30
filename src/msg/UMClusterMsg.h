/**
 *
 * @file UMClusterMsg.h
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
				index_list_,
				weight_list_,
				link_mode_,
				link_node_id_);
#endif //WITH_MSGPACK
	