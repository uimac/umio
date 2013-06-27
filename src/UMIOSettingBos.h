/**
 *
 * @file UMIOSettingBos.h
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
		ar & exp_prop_map_;
		ar & imp_prop_map_;
		ar & bl_exp_prop_map_;
		ar & bl_imp_prop_map_;
	}
#endif //WITH_BOOST_SERIALIZATION
