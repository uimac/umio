/**
 *
 * @file UMNodeBos.h
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
		ar & m[0][0];
		ar & m[0][1];
		ar & m[0][2];
		ar & m[0][3];
		ar & m[1][0];
		ar & m[1][1];
		ar & m[1][2];
		ar & m[1][3];
		ar & m[2][0];
		ar & m[2][1];
		ar & m[2][2];
		ar & m[2][3];
		ar & m[3][0];
		ar & m[3][1];
		ar & m[3][2];
		ar & m[3][3];
	}
#endif //WITH_BOOST_SERIALIZATION
	