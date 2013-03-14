/**
 *
 * UMBlendShapeChannel.h
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
		ar & target_shape_list_;
		ar & full_weight_list_;
		ar & deform_percent_;
	}
#endif //WITH_BOOST_SERIALIZATION
	