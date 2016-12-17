/**
 *
 * @file UMSkinMsg.h
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
	deformer_msg_type,
	std::string,
	UMCluster::ClusterList
	> skin_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		deformer_msg_type deformer_type;
		get_deformer_msg_type(deformer_type);
		o.pack(skin_msg_type(deformer_type, name_, cluster_list_));
	}
	
	void msgpack_unpack(msgpack::object o) {
		skin_msg_type v;
		o.convert(v);
		deformer_msg_type deformer_type = v.get<0>();
		UMDeformer::msgpack_unpack(deformer_type);
		name_ = v.get<1>();
		cluster_list_ = v.get<2>();
	}
#endif //WITH_MSGPACK
	