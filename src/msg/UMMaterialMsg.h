/**
 *
 * @file UMMaterialMsg.h
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
	double,
	double,
	double,
	double,
	double,
	double,
	double,
	double
	> doubles;
	
	typedef msgpack::type::tuple<
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
	std::string,
	double_lists,
	doubles,
	TextureList,
	LayeredTextureList
	> material_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		double_lists dls =
		double_lists(
					 ambient_,
					 diffuse_,
					 specular_,
					 emissive_,
					 refrection_,
					 transparent_,
					 bump_,
					 vector_displacement_);
		
		doubles ds =
		doubles(
				shininess_,
				transparency_factor_,
				reflection_factor_,
				diffuse_factor_,
				specular_factor_,
				emissive_factor_,
				ambient_factor_,
				bump_factor_,
				vector_displacement_factor_);
		
		o.pack(material_msg_type(
							name_,
							dls,
							ds,
							texture_list_,
							layered_texture_list_));
	}
	
	void msgpack_unpack(msgpack::object o) {
		material_msg_type v;
		o.convert(v);
		name_= v.get<0>();
		double_lists dls = v.get<1>();
		doubles ds = v.get<2>();
		texture_list_ = v.get<3>();
		layered_texture_list_ = v.get<4>();
		
		ambient_ = dls.get<0>();
		diffuse_ = dls.get<1>();
		specular_ = dls.get<2>();
		emissive_ = dls.get<3>();
		refrection_ = dls.get<4>();
		transparent_ = dls.get<5>();
		bump_ = dls.get<6>();
		vector_displacement_ = dls.get<7>();
		
		shininess_ = ds.get<0>();
		transparency_factor_ = ds.get<1>();
		reflection_factor_ = ds.get<2>();
		diffuse_factor_ = ds.get<3>();
		specular_factor_ = ds.get<4>();
		emissive_factor_ = ds.get<5>();
		ambient_factor_ = ds.get<6>();
		bump_factor_ = ds.get<7>();
		vector_displacement_factor_ = ds.get<8>();
	}
#endif //WITH_MSGPACK