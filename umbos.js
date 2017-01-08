// client side parser using messagepack-lite
(function () {
	function UMNodeMsg(data) {
		this.parent_id = data[0][0];
		this.id = data[0][1];
		this.name = data[0][2];

		this.local_translation = data[1][0];
		this.local_rotation = data[1][1];
		this.local_scaling = data[1][2];
		this.rotation_offset = data[1][3];
		this.rotation_pivot = data[1][4];
		this.pre_rotation = data[1][5];
		this.post_rotation = data[1][6];
		this.scaling_offset = data[1][7];
		this.scaling_pivot = data[1][8];
		this.geometric_translation = data[1][9];
		this.geometric_rotation = data[1][10];
		this.geometric_scaling = data[1][11];

		this.local_transform = data[2][0];
		this.global_transform = data[2][1];
	}

	function UMTextureMsg(data) {
		this.name = data[0];
		this.file_name = data[1];
		this.relative_file_name = data[2];
		this.blend_mode = data[3];
		this.alpha = data[4];
	}

	function UMMaterialMsg(data) {
		var i, k,
			textures;

		this.name = data[0];
		this.ambient = data[1][0];
		this.diffuse = data[1][1];
		this.specular = data[1][2];
		this.emissive = data[1][3];
		this.refrection = data[1][4];
		this.transparent = data[1][5];
		this.bump = data[1][6];
		this.vector_displacement = data[1][7];

		this.shininess = data[2][0];
		this.transparency_factor = data[2][1];
		this.reflection_factor = data[2][2];
		this.diffuse_factor = data[2][3];
		this.specular_factor = data[2][4];
		this.emissive_factor = data[2][5];
		this.ambient_factor = data[2][6];
		this.bump_factor = data[2][7];
		this.vector_displacement_factor = data[2][8];

		this.texture_list = [];
		for (i = 0; i < data[3].length; i = i + 1) {
			this.texture_list.push(new UMTextureMsg(data[3][i]));
		}
		this.layered_texture_list = [];
		for (i = 0; i < data[4].length; i = i + 1) {
			textures = [];
			for (k = 0; k < data[4][i].length; k = k + 1) {
				textures.push(new UMTextureMsg(data[4][i][k]));
			}
			this.layered_texture_list.push(textures);
		}
	}

	function UMDeformerMsg(data) {
		this.geometry_node_id = data[0];
		this.deformer_type = data[1];
	}

	function UMClusterMsg(data) {
		this.name = data[0];
		this.index_list = data[1];
		this.weight_list = data[2];
		this.link_mode = data[3];
		this.link_node_id = data[4];
	}

	function UMSkinMsg(data) {
		UMDeformerMsg.call(this, data[0]);
		var i;
		this.name = data[1];
		this.cluster_list = [];
		for (i = 0; i < data[2].length; i = i + 1) {
			this.cluster_list.push(new UMClusterMsg(data[2][i]))
		}
	}
	UMSkinMsg.prototype = Object.create(UMDeformerMsg.prototype);

	function UMShapeMsg(data) {
		this.name = data[0];
		this.base_geometry_node_id = data[1];
		this.vertex_index_list = data[2];
		this.normal_index_list = data[3];
		this.vertex_list = data[4];
		this.normal_list = data[5];
	}

	function UMBlendSHapeChannelMsg(data) {
		var i;
		this.target_shape_list = [];
		for (i = 0; i < data[0].length; i = i + 1) {
			this.target_shape_list.push(new UMShapeMsg(data[0][i]));
		}
		this.full_weight_list = data[1];
		this.deform_percent = data[2];
	}

	function UMBlendShapeMsg(data) {
		UMDeformerMsg.call(this, data[0]);
		var i;
		this.name = data[0];
		this.blend_shape_channel_list = [];
		for (i = 0; i < data[2].length; i = i + 1) {
			this.blend_shape_channel_list.push(new UMBlendSHapeChannelMsg(data[2][i]))
		}
	}
	UMBlendShapeMsg.prototype = Object.create(UMDeformerMsg.prototype);

	function UMMeshMsg(data) {
		UMNodeMsg.call(this, data[0]);

		var i;
		this.vertex_index_list = data[1];
		this.vertex_list = data[2];
		this.material_index = data[3];
		this.material_list = [];
		for (i = 0; i < data[4].length; i = i + 1) {
			this.material_list.push(new UMMaterialMsg(data[4][i]));
		}
		this.skin_list = [];
		for (i = 0; i < data[5].length; i = i + 1) {
			this.skin_list.push(new UMSkinMsg(data[5][i]));
		}
		this.blend_shape_list = [];
		for (i = 0; i < data[6].length; i = i + 1) {
			this.blend_shape_list.push(new UMBlendShapeMsg(data[6][i]));
		}
		this.layered_uv_list = data[7];
		this.layered_normal_list = data[8];
		this.layered_vertex_color_list = data[9];
	}
	UMMeshMsg.prototype = Object.create(UMNodeMsg.prototype);

	function UMSkeletonMsg(data) {
		UMNodeMsg.call(this, data[0]);
		this.type = data[1];
		this.limb_length = data[2];
		this.size = data[3];
	}
	UMSkeletonMsg.prototype = Object.create(UMNodeMsg.prototype);

	function UMPoseMsg(data) {
		this.bind_pose = data[0];
		this.rest_pose = data[1];
		this.node_id_list = data[2];
		this.matrix_is_local_list = data[3];
		this.matrix_list = data[4];
	}

	function UMObjectMsg (data) {
		var i;
		this.mesh_map = {};
		for (i in data[0]) {
			this.mesh_map[i] = new UMMeshMsg(data[0][i]);
		}
		this.skeleton_map = {};
		for (i in data[1]) {
			this.skeleton_map[i] = new UMSkeletonMsg(data[1][i]);
		}
		// this.camera_map = {};
		// for (i in data[2]) {
		// 	this.camera_map[i] = new UMCameraMsg(data[2][i]);
		// }
		this.other_node_map_ = {};
		for (i in data[3]) {
			this.other_node_map_[i] = new UMNodeMsg(data[3][i]);
		}
		this.pose_list = {};
		for (i = 0; i < data[4].length; i = i + 1) {
			this.pose_list[i] = new UMPoseMsg(data[4][i]);
		}
	}

	function load(array) {
		var data = msgpack.decode(array);
		return new UMObjectMsg(data);
	}

	window.umbos = {};
	window.umbos.load = load;

}());
