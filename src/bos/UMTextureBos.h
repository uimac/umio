/**
 *
 * @file UMTextureBos.h
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
		ar & name_;
		ar & file_name_;
		ar & relative_file_name_;
		ar & blend_mode_;
		ar & alpha_;
	}
#endif //WITH_BOOST_SERIALIZATION