/**
 *
 * @file UMSkeletonBos.h
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
		ar & type_;
		ar & limb_length_;
		ar & size_;
	}
#endif //WITH_BOOST_SERIALIZATION
	