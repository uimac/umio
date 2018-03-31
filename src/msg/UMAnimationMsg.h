/**
*
* @file UMAnimationMsg.h
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
		UMAnimationStack::AnimationStackList
	> animation_msg_type;

	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		o.pack(animation_msg_type(animation_stack_list_));
	}

	void msgpack_unpack(msgpack::object o) {
		animation_msg_type v;
		o.convert(v);

		animation_stack_list_ = v.get<0>();
	}
#endif //WITH_MSGPACK
