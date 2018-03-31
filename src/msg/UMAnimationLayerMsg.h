/**
*
* @file UMAnimationLayerMsg.h
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
	double,
	bool,
	bool,
	bool,
	UMVec4d,
	int,
	int,
	int,
	UMAnimationCurveStack::IDToCurveStackMap
> animation_layer_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_layer_msg_type(
		weight_,
		mute_,
		solo_,
		lock_,
		color_,
		blend_mode_,
		rotation_accumulation_mode_,
		scale_accumulation_mode_,
		curve_stack_map_
	));
}

void msgpack_unpack(msgpack::object o) {
	animation_layer_msg_type v;
	o.convert(v);

	weight_ = v.get<0>();
	mute_ = v.get<1>();
	solo_ = v.get<2>();
	lock_ = v.get<3>();
	color_ = v.get<4>();
	blend_mode_ = v.get<5>();
	rotation_accumulation_mode_ = v.get<6>();
	scale_accumulation_mode_ = v.get<7>();
	curve_stack_map_ = v.get<8>();
}
#endif //WITH_MSGPACK
