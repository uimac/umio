/**
 *
 * @file UMMeshBos.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_BOOST_SERIALIZATION
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<UMNode>(*this);
		ar & vertex_index_list_;
		ar & vertex_list_;
		ar & material_index_;
		ar & material_list_;
		ar & skin_list_;
		ar & blend_shape_list_;
		ar & layered_uv_list_;
		ar & layered_normal_list_;
		ar & layered_vertex_color_list_;
	}
#endif //WITH_BOOST_SERIALIZATION
	