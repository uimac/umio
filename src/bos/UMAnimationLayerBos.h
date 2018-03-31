/**
*
* @file UMAnimationLayerBos.h
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
	ar & weight_;
	ar & mute_;
	ar & solo_;
	ar & lock_;
	ar & color_;
	ar & blend_mode_;
	ar & rotation_accumulation_mode_;
	ar & scale_accumulation_mode_;
	ar & curve_stack_map_;
}
#endif //WITH_BOOST_SERIALIZATION
