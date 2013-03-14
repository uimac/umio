/**
 *
 * UMPoseBos.h
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
		ar & bind_pose_;
		ar & rest_pose_;
		ar & node_id_list_;
		ar & matrix_is_local_list_;
		ar & matrix_list_;
	}
#endif //WITH_BOOST_SERIALIZATION