/**
 *
 * @file UMMeshMsg.h
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
	node_msg_type,
	IntListVec,
	FloatListVec,
	IntList,
	UMMaterial::MaterialList,
	UMSkin::SkinList,
	UMBlendShape::BlendShapeList,
	LayeredFloatListVec,
	LayeredFloatListVec,
	LayeredFloatListVec
	> mesh_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		node_msg_type node_type;
		get_node_msg_type(node_type);
		o.pack(mesh_msg_type(
							node_type,
							vertex_index_list_,
							vertex_list_,
							material_index_,
							material_list_,
							skin_list_,
							blend_shape_list_,
							layered_uv_list_,
							layered_normal_list_,
							layered_vertex_color_list_));
	}
	
	void msgpack_unpack(msgpack::object o) {
		mesh_msg_type v;
		o.convert(v);
		UMNode::_msgpack_unpack(v.get<0>());
		vertex_index_list_ = v.get<1>();
		vertex_list_ = v.get<2>();
		material_index_ = v.get<3>();
		material_list_ = v.get<4>();
		skin_list_ = v.get<5>();
		blend_shape_list_ = v.get<6>();
		layered_uv_list_ = v.get<7>();
		layered_normal_list_ = v.get<8>();
		layered_vertex_color_list_ = v.get<9>();
	}
#endif //WITH_MSGPACK
	