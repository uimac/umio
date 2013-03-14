/**
 *
 * UMListTypes.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#pragma once

#include <vector>
#include <string>

#ifdef WITH_BOOST_SERIALIZATION
	#include <boost/array.hpp>
#else
	#include <array> 
#endif //WITH_BOOST_SERIALIZATION

namespace UM
{

typedef std::vector<int> BoolList;
typedef std::vector<int> IntList;
typedef std::vector<unsigned int> UIntList;
typedef std::vector<double> DoubleList;
typedef std::vector<std::string> StringList;
typedef std::vector<IntList> IntListVec;
typedef std::vector<DoubleList> DoubleListVec;
typedef std::vector<DoubleListVec> LayeredDoubleListVec;

#ifdef WITH_BOOST_SERIALIZATION
	typedef boost::array<double, 4> Array4d;
	typedef boost::array<Array4d, 4> Array44d;
#else
	typedef std::array<double, 4> Array4d;
	typedef std::array<Array4d, 4> Array44d;
#endif

typedef std::vector<Array44d> Array44dList;

} //namespace UM