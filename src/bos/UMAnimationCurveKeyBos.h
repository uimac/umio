/**
*
* @file UMAnimationCurveKeyBos.h
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
	ar & time_;
	ar & data_map_;
	ar & value_;
	ar & interpolation_type_;
	ar & tangent_mode_;
	ar & weighted_mode_;
	ar & velocity_mode_;
	ar & constant_mode_;
	ar & tangent_visibility_;
	ar & is_break_;
}
#endif //WITH_BOOST_SERIALIZATION
