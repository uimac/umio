/**
 *
 * @file UMObjectMsg.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#ifdef WITH_MSGPACK
	typedef msgpack::type::tuple<
	UMMesh::IDToMeshMap,
	UMSkeleton::IDToSkeletonMap,
	UMCamera::IDToCameraMap,
	UMNode::IDToNodeMap,
	UMPose::PoseList,
	UMObject::EmbeddedFileMap
	> object_msg_type;
	
	template <typename Stream>
	void msgpack_pack(msgpack::packer<Stream>& o) const {
		o.pack(object_msg_type(mesh_map_, skeleton_map_,camera_map_, other_node_map_, pose_list_, embedded_file_map_));
	}
	
	void msgpack_unpack(msgpack::object o){
		object_msg_type v;
		o.convert(v);

		mesh_map_ = v.get<0>();
		skeleton_map_ = v.get<1>();
		camera_map_ = v.get<2>();
		other_node_map_ = v.get<3>();
		pose_list_ = v.get<4>();
		embedded_file_map_ = v.get<5>();
	}
#endif //WITH_MSGPACK
	