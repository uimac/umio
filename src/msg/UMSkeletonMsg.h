/**
 *
 * @file UMSkeletonMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	
	typedef msgpack::type::tuple<
	node_msg_type,
	int,
	double,
	double
	> skeleton_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		node_msg_type node_type;
		get_node_msg_type(node_type);
		o.pack(skeleton_msg_type(node_type, type_, limb_length_, size_));
	}
	
	void msgpack_unpack(skeleton_msg_type v) {
		UMNode::msgpack_unpack(v.get<0>());
		type_ = v.get<1>();
		limb_length_ = v.get<2>();
		size_ = v.get<3>();
	}
	
#endif //WITH_MSGPACK
	