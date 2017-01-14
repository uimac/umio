/**
 *
 * @file UMListTypes.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
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
typedef std::vector<float> FloatList;
typedef std::vector<std::string> StringList;
typedef std::vector<IntList> IntListVec;
typedef std::vector<FloatList> FloatListVec;
typedef std::vector<FloatListVec> LayeredFloatListVec;
typedef std::vector<UMMat44d> Mat44dList;


} //namespace umio
