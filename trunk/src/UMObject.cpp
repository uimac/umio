/**
 *
 * @file UMObject.cpp
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#include "UMObject.h"

namespace umio
{

bool UMObject::re_bind_all_nodes(UMObjectPtr object)
{
	if (!object) return false;

	// re assign id_to_node_map all
	{
		// mesh
		{
			UMMesh::IDToMeshMap::iterator it = object->mutable_mesh_map().begin();
			for (; it != object->mutable_mesh_map().end(); ++it)
			{
				if (object->id_to_node_map().find(it->first) == object->id_to_node_map().end())
				{
					object->mutable_id_to_node_map()[it->first] = &it->second;
				}
			}
		}
		// skeleton
		{
			UMSkeleton::IDToSkeletonMap::iterator it = object->mutable_skeleton_map().begin();
			for (; it != object->mutable_skeleton_map().end(); ++it)
			{
				if (object->id_to_node_map().find(it->first) == object->id_to_node_map().end())
				{
					object->mutable_id_to_node_map()[it->first] = &it->second;
				}
			}
		}
		// camera
		{
			UMCamera::IDToCameraMap::iterator it = object->mutable_camera_map().begin();
			for (; it != object->mutable_camera_map().end(); ++it)
			{
				if (object->id_to_node_map().find(it->first) == object->id_to_node_map().end())
				{
					object->mutable_id_to_node_map()[it->first] = &it->second;
				}
			}
		}
		// other
		{
			UMNode::IDToNodeMap::iterator it = object->mutable_other_node_map().begin();
			for (; it != object->mutable_other_node_map().end(); ++it)
			{
				if (object->id_to_node_map().find(it->first) == object->id_to_node_map().end())
				{
					object->mutable_id_to_node_map()[it->first] = &it->second;
				}
			}
		}
	}

	// parent/children
	{
		UMObject::IDToNodePointerMap::iterator it = object->mutable_id_to_node_map().begin();
		for (; it != object->mutable_id_to_node_map().end(); ++it)
		{
			if (UMNode* node = (*it).second)
			{
				const int parent_id = static_cast<int>(node->parent_id());
				if (parent_id >= 0)
				{
					UMObject::IDToNodePointerMap::iterator pt = object->mutable_id_to_node_map().find(parent_id);
					if (pt != object->mutable_id_to_node_map().end())
					{
						UMNode* parent = (pt->second);
						node->set_parent(parent);
						parent->mutable_children().push_back(node);
					}
				}
			}
		}
	}

	// deformer
	{
		UMMesh::IDToMeshMap::iterator it = object->mutable_mesh_map().begin();
		for (; it != object->mutable_mesh_map().end(); ++it)
		{
			UMMesh& mesh = (*it).second;
			
			// skin
			{
				UMSkin::SkinList::iterator st = mesh.mutable_skin_list().begin();
				for (; st != mesh.mutable_skin_list().end(); ++st)
				{
					UMSkin& skin = (*st);
					int geometory_id = static_cast<int>(skin.geometry_node_id());
					if (geometory_id >= 0)
					{
						UMObject::IDToNodePointerMap::const_iterator gt = object->id_to_node_map().find(geometory_id);
						if (gt != object->id_to_node_map().end())
						{
							skin.set_geometry_node(gt->second);
						}
					}

					UMCluster::ClusterList::iterator ct = skin.mutable_cluster_list().begin();
					for (; ct != skin.mutable_cluster_list().end(); ++ct)
					{
						UMCluster& cluster = (*ct);
						const int link_id = static_cast<int>(cluster.link_node_id());
						if (link_id >= 0)
						{
							UMObject::IDToNodePointerMap::const_iterator lt = object->id_to_node_map().find(link_id);
							if (lt != object->id_to_node_map().end())
							{
								cluster.set_link_node(lt->second);
							}
						}
					}
				}
			}

			// blend shape
			{
				UMBlendShape::BlendShapeList::iterator bt = mesh.mutable_blend_shape_list().begin();
				for (; bt != mesh.mutable_blend_shape_list().end(); ++bt)
				{
					UMBlendShape& blend_shape = (*bt);
					int geometory_id = static_cast<int>(blend_shape.geometry_node_id());
					if (geometory_id >= 0)
					{
						UMObject::IDToNodePointerMap::const_iterator gt = object->id_to_node_map().find(geometory_id);
						if (gt != object->id_to_node_map().end())
						{
							blend_shape.set_geometry_node(gt->second);
						}
					}

					UMBlendShapeChannel::ChannelList::iterator ct = blend_shape.mutable_blend_shape_channel_list().begin();
					for (; ct != blend_shape.mutable_blend_shape_channel_list().end(); ++ct)
					{
						UMBlendShapeChannel& channel = (*ct);
						UMShape::ShapeList::iterator st = channel.mutable_target_shape_list().begin();
						for (; st != channel.mutable_target_shape_list().end(); ++st)
						{
							UMShape& shape = (*st);
							const int node_id = static_cast<int>(shape.base_geometry_node_id());
							if (node_id > 0)
							{
								UMObject::IDToNodePointerMap::const_iterator nt = object->id_to_node_map().find(geometory_id);
								if (nt != object->id_to_node_map().end())
								{
									shape.set_base_geometry_node(nt->second);
								}
							}
						}
					}
				}
			}
		}
	}
	
	return true;
}

} // namespace umio
