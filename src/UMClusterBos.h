/**
 *
 * @file UMClusterBos.h
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
		ar & index_list_;
		ar & weight_list_;
		ar & link_mode_;
		ar & link_node_id_;
	}
#endif //WITH_BOOST_SERIALIZATION
	