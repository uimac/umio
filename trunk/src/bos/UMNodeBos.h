/**
 *
 * @file UMNodeBos.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_BOOST_SERIALIZATION
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & parent_id_;
		ar & id_;
		ar & name_;
		ar & local_translation_;
		ar & local_rotation_;
		ar & local_scaling_;
		ar & rotation_offset_;
		ar & rotation_pivot_;
		ar & pre_rotation_;
		ar & post_rotation_;
		ar & scaling_offset_;
		ar & scaling_pivot_;
		ar & geometric_translation_;
		ar & geometric_rotation_;
		ar & geometric_scaling_;
		ar & local_transform_;
		ar & global_transform_;
	}
#endif //WITH_BOOST_SERIALIZATION
	