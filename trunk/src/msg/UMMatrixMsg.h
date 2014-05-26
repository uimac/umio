/**
 *
 * @file UMMatrixMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 2 or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	MSGPACK_DEFINE(
		m[0][0],
		m[0][1],
		m[0][2],
		m[0][3],
		m[1][0],
		m[1][1],
		m[1][2],
		m[1][3],
		m[2][0],
		m[2][1],
		m[2][2],
		m[2][3],
		m[3][0],
		m[3][1],
		m[3][2],
		m[3][3]);
#endif //WITH_MSGPACK
	