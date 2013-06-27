/**
 *
 * @file UMListTypes.h
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
#include <array>
#include "UMVector.h"
#include "UMMatrix.h"

namespace umio
{

typedef std::vector<int> BoolList;
typedef std::vector<int> IntList;
typedef std::vector<unsigned int> UIntList;
typedef std::vector<double> DoubleList;
typedef std::vector<std::string> StringList;
typedef std::vector<IntList> IntListVec;
typedef std::vector<DoubleList> DoubleListVec;
typedef std::vector<DoubleListVec> LayeredDoubleListVec;
typedef std::vector<UMMat44d> Mat44dList;


} //namespace umio
