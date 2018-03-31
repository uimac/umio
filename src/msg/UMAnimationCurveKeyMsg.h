/**
*
* @file UMAnimationCurveKeyMsg.h
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
	UMTime,
	DataMap,
	float,
	int,
	int,
	int,
	int,
	int,
	int,
	bool
> animation_curve_key_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_curve_key_msg_type(
		time_,
		data_map_,
		value_,
		interpolation_type_,
		tangent_mode_,
		weighted_mode_,
		velocity_mode_,
		constant_mode_,
		tangent_visibility_,
		is_break_
	));
}

void msgpack_unpack(msgpack::object o) {
	animation_curve_key_msg_type v;
	o.convert(v);

	time_ = v.get<0>();
	data_map_ = v.get<1>();
	value_ = v.get<2>();
	interpolation_type_ = v.get<3>();
	tangent_mode_ = v.get<4>();
	weighted_mode_ = v.get<5>();
	velocity_mode_ = v.get<6>();
	constant_mode_ = v.get<7>();
	tangent_visibility_ = v.get<8>();
	is_break_ = v.get<9>();
}
#endif //WITH_MSGPACK
