/**
 *
 * @file UMShapeBos.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#ifdef WITH_BOOST_SERIALIZATION
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & name_;
		ar & base_geometry_node_id_;
		ar & vertex_index_list_;
		ar & normal_index_list_;
		ar & vertex_list_;
		ar & normal_list_;
	}
#endif //WITH_BOOST_SERIALIZATION
	