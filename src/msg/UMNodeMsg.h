/**
 *
 * @file UMNodeMsg.h
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
	int,
	int,
	std::string
	> node_info_type;
	
	typedef msgpack::type::tuple<
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d,
	UMVec4d
	> double_lists;
	
	typedef msgpack::type::tuple<
	UMMat44d,
	UMMat44d
	> double_list_vecs;
	
	typedef msgpack::type::tuple<
	node_info_type,
	double_lists,
	double_list_vecs
	> node_msg_type;
	
	void get_node_msg_type(node_msg_type& type) const
	{
		node_info_type infos = node_info_type(
			parent_id_,
			id_,
			name_
			);
		
		double_lists dls = double_lists(
			local_translation_,
			local_rotation_,
			local_scaling_,
			rotation_offset_,
			rotation_pivot_,
			pre_rotation_,
			post_rotation_,
			scaling_offset_,
			scaling_pivot_,
			geometric_translation_,
			geometric_rotation_,
			geometric_scaling_
			);
		
		double_list_vecs dlvs = double_list_vecs(
			local_transform_,
			global_transform_
			);
		
		type = node_msg_type(infos, dls, dlvs);
	}
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		node_msg_type type;
		get_node_msg_type(type);
		o.pack(type);
	}

	void _msgpack_unpack(node_msg_type v) {
		node_info_type infos = v.get<0>();
		double_lists dls = v.get<1>();
		double_list_vecs dlvs = v.get<2>();

		parent_id_ = infos.get<0>();
		id_ = infos.get<1>();
		name_ = infos.get<2>();

		local_translation_ = dls.get<0>();
		local_rotation_ = dls.get<1>();
		local_scaling_ = dls.get<2>();
		rotation_offset_ = dls.get<3>();
		rotation_pivot_ = dls.get<4>();
		pre_rotation_ = dls.get<5>();
		post_rotation_ = dls.get<6>();
		scaling_offset_ = dls.get<7>();
		scaling_pivot_ = dls.get<8>();
		geometric_translation_ = dls.get<9>();
		geometric_rotation_ = dls.get<10>();
		geometric_scaling_ = dls.get<11>();

		local_transform_ = dlvs.get<0>();
		global_transform_ = dlvs.get<1>();
	}
	
	void msgpack_unpack(msgpack::object o) {
		node_msg_type v;
		o.convert(v);
		_msgpack_unpack(v);
	}
#endif //WITH_MSGPACK
	