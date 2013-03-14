/**
 *
 * UMObjectBos.h
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
		ar & mesh_map_;
		ar & skeleton_map_;
		ar & camera_map_;
		ar & other_node_map_;
		ar & pose_list_;
	}
#endif //WITH_BOOST_SERIALIZATION
	