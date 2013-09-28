/**
 * @file UMVector.h
 * vector
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 */
#pragma once

#include <cmath>
#include <cfloat>
#include <limits>

#ifdef WITH_BOOST_SERIALIZATION
	#include <boost/serialization/serialization.hpp>
#endif

namespace umio
{

template <class T> class UMVector2;
template <class T> class UMVector3;
template <class T> class UMVector4;

typedef UMVector2<int>    UMVec2i;
typedef UMVector2<float>  UMVec2f;
typedef UMVector2<double> UMVec2d;
typedef UMVector3<int>    UMVec3i;
typedef UMVector3<float>  UMVec3f;
typedef UMVector3<double> UMVec3d;
typedef UMVector4<int>    UMVec4i;
typedef UMVector4<float>  UMVec4f;
typedef UMVector4<double> UMVec4d;

/**
 * vector 2
 */
template <class T>
class UMVector2
{
public:
	T x;
	T y;
	
	/**
	 * constructor
	 */
	UMVector2() : x(0), y(0) {}

	/**
	 * constructor
	 */
	UMVector2(T a) : x(a), y(a) {}

	/**
	 * constructor
	 */
	UMVector2(T a, T b) : x(a), y(b) {}
	
	/**
	 * copy constructor
	 */
	UMVector2 (const UMVector2 &v) : x(v.x), y(v.y) {}
	
	/**
	 * assign
	 */
	const UMVector2& operator = (const UMVector2 &v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }
	
	/**
	 * get
	 */
	T& at(int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& at(int i) const { return (&x)[i]; }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector2 &v) const {
		return (x == v.x && y == v.y);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector2 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector2 operator + (const UMVector2 &v) const {
		return UMVector2(x + v.x, y + v.y);
	}
	
	/**
	 * plus equal
	 */
	UMVector2 operator += (const UMVector2 &v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	/**
	 * minus
	 */
	UMVector2 operator - () const {
		return UMVector2(-x, -y);
	}

	/**
	 * minus
	 */
	UMVector2 operator - (const UMVector2 &v) const {
		return UMVector2(x - v.x, y - v.y);
	}
	
	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector2 operator * (const U &s) const {
		return UMVector2(x * s, y * s);
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector2 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}
	
	/**
	 * dot
	 */
	double dot(const UMVector2 &v) const {
		return (x*v.x + y*v.y);
	}

	/**
	 * get normalized
	 */
	UMVector2 normalized() {
		UMVector2 dst(*this);
		T a = x * x + y * y;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
		} else {
			dst.x = dst.y = static_cast<T>(0);
		}
		return dst;
	}
private:
	// boost serialization
	#include "UMVector2Bos.h"
};

/**
 * vector 3
 */
template <class T>
class UMVector3
{
public:
	T x;
	T y;
	T z;
	
	/**
	 * constructor
	 */
	UMVector3() : x(0), y(0), z(0) {}

	/**
	 * constructor
	 */
	UMVector3(T a) : x(a), y(a), z(a) {}

	/**
	 * constructor
	 */
	UMVector3(T a, T b, T c) : x(a), y(b), z(c) {}
	
	/**
	 * copy constructor
	 */
	UMVector3 (const UMVector3 &v) : x(v.x), y(v.y), z(v.z) {}
	
	/**
	 * assign
	 */
	const UMVector3& operator = (const UMVector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	
	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }
	
	/**
	 * get
	 */
	T& at(int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& at(int i) const { return (&x)[i]; }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector3 &v) const {
		return (x == v.x && y == v.y && z== v.z);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector3 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector3 operator + (const UMVector3 &v) const {
		return UMVector3(x + v.x, y + v.y, z + v.z);
	}
	
	/**
	 * plus equal
	 */
	UMVector3 operator += (const UMVector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	
	/**
	 * minus
	 */
	UMVector3 operator - () const {
		return UMVector3(-x, -y, -z);
	}
	
	/**
	 * minus
	 */
	UMVector3 operator - (const UMVector3 &v) const {
		return UMVector3(x - v.x, y - v.y, z - v.z);
	}
	
	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector3 operator * (const U &s) const {
		return UMVector3(x * s, y * s, z * s);
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector3 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}

	/**
	 * dot
	 */
	double dot(const UMVector3 &v) const {
		return (x*v.x + y*v.y + z*v.z);
	}

	/**
	 * get normalized
	 */
	UMVector3 normalized() {
		UMVector3 dst(*this);
		T a = x * x + y * y + z * z;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
			dst.z = z * b;
		} else {
			dst.x = dst.y = dst.z = static_cast<T>(0);
		}
		return dst;
	}
private:
	// boost serialization
	#include "UMVector3Bos.h"
};

/**
 * vector 4
 */
template <class T>
class UMVector4
{
public:
	T x;
	T y;
	T z;
	T w;
	
	/**
	 * constructor
	 */
	UMVector4() : x(0), y(0), z(0), w(0) {}

	/**
	 * constructor
	 */
	UMVector4(T a) : x(a), y(a), z(a), w(a) {}

	/**
	 * constructor
	 */
	UMVector4(T a, T b, T c, T d) : x(a), y(b), z(c), w(d) {}
	
	/**
	 * copy constructor
	 */
	UMVector4 (const UMVector4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	
	/**
	 * assign
	 */
	const UMVector4& operator = (const UMVector4 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	
	/**
	 * get
	 */
	T& operator [] (int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& operator [] (int i) const { return (&x)[i]; }
	
	/**
	 * get
	 */
	T& at(int i) { return (&x)[i]; }
	
	/**
	 * get
	 */
	const T& at(int i) const { return (&x)[i]; }

	/**
	 * compare equal
	 */
	bool operator == (const UMVector4 &v) const {
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMVector4 &v) const {
		return !(*this == v);
	}
	
	/**
	 * plus
	 */
	UMVector4 operator + (const UMVector4 &v) const {
		return UMVector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	
	/**
	 * plus equal
	 */
	UMVector4 operator += (const UMVector4 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	/**
	 * minus
	 */
	UMVector4 operator - () const {
		return UMVector4(-x, -y, -z, -w);
	}

	/**
	 * minus
	 */
	UMVector4 operator - (const UMVector4 &v) const {
		return UMVector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	/**
	 * multiply scolor
	 */
	template <class U>
	UMVector4 operator * (const UMVector4 &s) const {
		return UMVector4(x * s, y * s, z * s, w * s);
	}

	/**
	 * divide by scolor
	 */
	template <class U>
	UMVector4 operator / (const U &s) const {
		return (*this) * static_cast<U>(1 / s);
	}
	
	/**
	 * dot
	 */
	double dot(const UMVector4 &v) const {
		return (x*v.x + y*v.y + z*v.z + w*v.w);
	}

	/**
	 * get normalized
	 */
	UMVector4 normalized() {
		UMVector4 dst(*this);
		T a = x * x + y * y + z * z + w * w;
		if (a > std::numeric_limits<T>::epsilon()) {
			T b = static_cast<T>(1.0 / sqrt(a));
			dst.x = x * b;
			dst.y = y * b;
			dst.z = z * b;
			dst.w = w * b;
		} else {
			dst.x = dst.y = dst.z = dst.w = static_cast<T>(0);
		}
		return dst;
	}
private:
	// boost serialization
	#include "UMVector4Bos.h"

};

} // umio
