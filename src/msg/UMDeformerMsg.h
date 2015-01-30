/**
 *
 * @file UMDeformerMsg.h
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
	int,
	int
	> deformer_msg_type;
	
	void get_deformer_msg_type(deformer_msg_type deformer_type) const
	{
		deformer_type.a0 = geometry_node_id_;
		deformer_type.a1 = deformer_type_;
	}
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		deformer_msg_type deformer_type;
		get_deformer_msg_type(deformer_type);
		o.pack(deformer_type);
	}
	
	void msgpack_unpack(deformer_msg_type v) {
		geometry_node_id_ = v.get<0>();
		deformer_type_ = v.get<1>();
	}
	
#endif //WITH_MSGPACK