/**
 * @file UMMatrix.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 */
#pragma once

#ifdef WITH_BOOST_SERIALIZATION
	#include <boost/serialization/serialization.hpp>
#endif

#ifdef WITH_MSGPACK
	#include <msgpack.hpp>
#endif

namespace umio
{
	
template <class T> class UMMatrix44;

typedef UMMatrix44<float>  UMMat44f;
typedef UMMatrix44<double> UMMat44d;

/**
 * matrix 4x4
 */
template <class T>
class UMMatrix44
{
public:
	/// column-major order matrix
	T m[4][4];
	
	/**
	 * constructor
	 */
	UMMatrix44() {
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k)
			{
				m[i][k] = 0;
			}
		}
	}

	/**
	 * constructor
	 */
	UMMatrix44(const T *mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat[i*4 + k];
			}
		}
	}

	/**
	 * constructor
	 */
	UMMatrix44(
		T _m11, T _m12, T _m13, T _m14,
		T _m21, T _m22, T _m23, T _m24,
		T _m31, T _m32, T _m33, T _m34,
		T _m41, T _m42, T _m43, T _m44)
	{
		m[0][0] = _m11; m[0][1] = _m12; m[0][2] = _m13; m[0][3] = _m14;
		m[1][0] = _m21; m[1][2] = _m22; m[1][2] = _m23; m[1][3] = _m24;
		m[2][0] = _m31; m[2][1] = _m32; m[2][2] = _m33; m[2][3] = _m34;
		m[3][0] = _m41; m[3][1] = _m42; m[3][2] = _m43; m[3][3] = _m44;
	}
	
	/**
	 * copy constructor
	 */
	UMMatrix44(const UMMatrix44 &mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat.m[i][k];
			}
		}
	}
	
	/**
	 * assign
	 */
	const UMMatrix44& operator = (const UMMatrix44 &mat) {
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] = mat.m[i][k];
			}
		}
		return *this;
	}

	/**
	 * assign
	 */
	void set(int row, int column, T value)
	{
		m[row][column] = value;
	}
	
	/**
	 * compare equal
	 */
	bool operator == (const UMMatrix44 &v) const {
		bool equal = true;
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				if (m[i][k] != v.m[i][k]) {
					equal = false; break;
				}
			}
		}
		return equal;
	}
	
	/**
	 * compare not equal
	 */
	bool operator != (const UMMatrix44 &mat) const {
		return !(*this == mat);
	}

	/**
	 * plus
	 */
	UMMatrix44 operator + (const UMMatrix44& mat) const
	{
		UMMatrix44<T> dst(mat);
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				dst.m[i][k] += mat.m[i][k];
			}
		}
		return dst;
	}

	/**
	 * plus equal
	 */
	UMMatrix44 operator += (const UMMatrix44& mat)
	{
		for (int i = 0; i < 4; ++i) {
			for (int k = 0; k < 4; ++k) {
				m[i][k] += mat.m[i][k];
			}
		}
		return *this;
	}
	
	/**
	 * minus
	 */
	UMMatrix44 operator - () const {
		return UMMatrix44(
			-m[0][0], -m[0][1], -m[0][2], -m[0][3],
			-m[1][0], -m[1][1], -m[1][2], -m[1][3],
			-m[2][0], -m[2][1], -m[2][2], -m[2][3],
			-m[3][0], -m[3][1], -m[3][2], -m[3][3]);
	}

	/**
	 * minus
	 */
	UMMatrix44 operator - (const UMMatrix44 &mat) const {
		return UMMatrix44( *this + (-mat) );
	}

	// msgpack serialization
	#include "UMMatrixMsg.h"
private:
	// boost serialization
	#include "UMMatrixBos.h"
};

} //umio