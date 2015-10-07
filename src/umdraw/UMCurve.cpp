/**
 * @file UMCurve.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include "UMCurve.h"

namespace umdraw
{

static double B0(double t) {
	return (1.0 - t) * (1.0 - t) * (1.0 - t);
}

static double B1(double t) {
	return 3.0 * t * (1.0 - t) * (1.0 - t);
}

static double B2(double t) {
	return 3.0 * t * t * (1.0 - t);
}

static double B3(double t) {
	return t * t * t;
}

UMVec3d UMCurve::calculate(const UMCurve::Curve& curve, double t)
{
	return curve.p0 * B0(t) + curve.p1 * B1(t) + curve.p2 * B2(t) + curve.p3 * B3(t);
}

} // umdraw
