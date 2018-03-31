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
	std::string,
	UMAnimationCurve::CurveMap,
	UMAnimationCurveStack::TimeToGlobalTransformMap,
	UMAnimationCurveStack::TimeToLocalTransformMap
> animation_curve_stack_msg_type;

template <typename Stream>
void msgpack_pack(msgpack::packer<Stream>& o) const {
	o.pack(animation_curve_stack_msg_type(
		link_node_name_,
		curve_map_,
		global_transform_map_,
		local_transform_map_
	));
}

void msgpack_unpack(msgpack::object o) {
	animation_curve_stack_msg_type v;
	o.convert(v);

	link_node_name_ = v.get<0>();
	curve_map_ = v.get<1>();
	global_transform_map_ = v.get<2>();
	local_transform_map_ = v.get<3>();
}
#endif //WITH_MSGPACK
