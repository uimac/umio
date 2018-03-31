/**
*
* @file UMAnimationStackMsg.h
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
	std::string,
	std::string,
	UMTime,
	UMTime,
	UMTime,
	UMTime,
	UMAnimationLayer::LayerList
> animation_stack_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_stack_msg_type(
		name_,
		description_,
		local_start_,
		local_stop_,
		reference_start_,
		reference_stop_,
		layer_list_
	));
}

void msgpack_unpack(msgpack::object o) {
	animation_stack_msg_type v;
	o.convert(v);

	name_ = v.get<0>();
	description_ = v.get<1>();
	local_start_ = v.get<2>();
	local_stop_ = v.get<3>();
	reference_start_ = v.get<4>();
	reference_stop_ = v.get<5>();
	layer_list_ = v.get<6>();
}
#endif //WITH_MSGPACK
