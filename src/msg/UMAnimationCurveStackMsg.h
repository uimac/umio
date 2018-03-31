/**
*
* @file UMAnimationCurveStackMsg.h
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
	UMAnimationCurve::CurveMap
> animation_curve_stack_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_curve_stack_msg_type(curve_map_));
}

void msgpack_unpack(msgpack::object o) {
	animation_curve_stack_msg_type v;
	o.convert(v);

	curve_map_ = v.get<0>();
}
#endif //WITH_MSGPACK
