/**
 *
 * @file UMMaterialBos.h
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
		ar & ambient_;
		ar & diffuse_;
		ar & specular_;
		ar & emissive_;
		ar & refrection_;
		ar & bump_;
		ar & vector_displacement_;
		ar & shininess_;
		ar & transparency_factor_;
		ar & reflection_factor_;
		ar & diffuse_factor_;
		ar & specular_factor_;
		ar & emissive_factor_;
		ar & specular_factor_;
		ar & ambient_factor_;
		ar & bump_factor_;
		ar & vector_displacement_factor_;
		ar & texture_list_;
		ar & layered_texture_list_;
	}
#endif //WITH_BOOST_SERIALIZATION