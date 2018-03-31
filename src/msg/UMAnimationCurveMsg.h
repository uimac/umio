/**
*
* @file UMAnimationCurveMsg.h
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
	UMAnimationCurveKey::KeyMap
> animation_curve_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_curve_msg_type(key_map_));
}

void msgpack_unpack(msgpack::object o) {
	animation_curve_msg_type v;
	o.convert(v);

	key_map_ = v.get<0>();
}
#endif //WITH_MSGPACK
