/**
 *
 * @file UMBlendShapeMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	typedef msgpack::type::tuple<
	deformer_msg_type,
	std::string,
	UMBlendShapeChannel::ChannelList
	> blend_shape_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		deformer_msg_type deformer_type;
		get_deformer_msg_type(deformer_type);
		o.pack(blend_shape_msg_type(deformer_type, name_, blend_shape_channel_list_));
	}
	
	void msgpack_unpack(blend_shape_msg_type v) {
		deformer_msg_type deformer_type = v.get<0>();
		UMDeformer::msgpack_unpack(deformer_type);
		name_ = v.get<1>();
		blend_shape_channel_list_ = v.get<2>();
	}
#endif //WITH_MSGPACK
	