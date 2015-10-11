/**
 * @file UMAbcMesh.cpp
 * any
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#include <algorithm>
#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreFactory/All.h>

#include "UMAbcMesh.h"
#include "UMStringUtil.h"
#include "UMAbcConvert.h"
#include "UMMaterial.h"

#ifdef WITH_OPENGL
#include "UMOpenGLAbcMesh.h"
#endif

#ifdef WITH_DIRECTX
#include "UMDirectX11AbcMesh.h"
#include "UMDirectX11.h"
#endif

namespace umabc
{
	
	using namespace Alembic::Abc;
	using namespace Alembic::AbcGeom;

	class UMAbcMesh::Impl : public UMAbcObject
	{
		DISALLOW_COPY_AND_ASSIGN(Impl);
	public:
		Impl(IPolyMeshPtr poly_mesh)
			: UMAbcObject(poly_mesh)
			, poly_mesh_(poly_mesh)
			, is_cw_(true) 
		{}

		~Impl() {}

		/**
		* initialize
		* @param [in] recursive do children recursively
		* @retval succsess or fail
		*/
		bool init(bool recursive);

		/**
		* set current time
		* @param [in] time time
		* @param [in] recursive do children recursively
		*/
		void set_current_time(unsigned long time, bool recursive);

		/**
		* update box
		* @param [in] recursive do children recursively
		*/
		void update_box(bool recursive);

		///**
		//* draw
		//* @param [in] recursive do children recursively
		//*/
		//void draw(bool recursive, UMAbc::DrawType type);

		/**
		* get opengl mesh
		*/
		UMOpenGLAbcMeshPtr opengl_mesh() { return opengl_mesh_; }

		/**
		* get directx mesh
		*/
		UMDirectX11AbcMeshPtr directx_mesh() { return directx_mesh_; }

		/**
		* get polygon count
		*/
		int polygon_count() const;

		/**
		* update mesh all
		*/
		void update_mesh_all();

		Alembic::AbcGeom::P3fArraySamplePtr vertex() { return vertex_; }
		Alembic::AbcGeom::Int32ArraySamplePtr vertex_index() { return vertex_index_; }
		Alembic::AbcGeom::Int32ArraySamplePtr face_count() { return face_count_; }
		Alembic::AbcGeom::IN3fGeomParam::Sample& normal() { return normal_; }
		Alembic::AbcGeom::IV2fGeomParam::Sample& uv() { return uv_; }
		IndexList& triangle_index() { return triangle_index_; }
		std::vector<Imath::V3f>& normals() { return original_normal_; }

		/**
		* get material list
		*/
		const umdraw::UMMaterialList& material_list() const { return material_list_; }

		/**
		* get material list
		*/
		umdraw::UMMaterialList& mutable_material_list() { return material_list_; }

		/**
		* get material from face index
		*/
		umdraw::UMMaterialPtr material_from_face_index(int face_index) const;

		/**
		* get faceset name list
		*/
		const std::vector<umstring>& faceset_name_list() const { return faceset_name_list_; }

		/**
		* get faceset polycount list
		*/
		const std::vector<int>& faceset_polycount_list() const { return faceset_polycount_list_; }

		/**
		* set cw or not
		*/
		void set_cw(bool is_cw) { is_cw_ = is_cw; }

		UMAbcObjectPtr self_reference()
		{
			return self_reference_.lock();
		}

		/**
		* set minumum time
		*/
		void set_min_time(unsigned long time)
		{
			UMAbcObject::set_min_time(time);
		}

		/**
		* set maximum time
		*/
		void set_max_time(unsigned long time)
		{
			UMAbcObject::set_max_time(time);
		}

		UMAbcMeshWeakPtr self_reference_;
	private:
		/**
		* update normal
		*/
		void update_normal();

		/**
		* update uv
		*/
		void update_uv();

		/**
		* update vertex
		*/
		void update_vertex(Alembic::AbcGeom::IPolyMeshSchema::Sample& sample);

		/**
		* update vertex index
		*/
		void update_vertex_index(Alembic::AbcGeom::IPolyMeshSchema::Sample& sample);

		/**
		* update vertex index
		*/
		void update_vertex_index_by_faceset(Alembic::AbcGeom::IPolyMeshSchema::Sample& sample);

		/**
		* update material (face set)
		*/
		void update_material();

		IPolyMeshPtr poly_mesh_;
		Alembic::AbcGeom::IPolyMeshSchema::Sample initial_sample_;
		Alembic::AbcGeom::IBox3dProperty initial_bounds_prop_;

		Alembic::AbcGeom::P3fArraySamplePtr vertex_;
		Alembic::AbcGeom::Int32ArraySamplePtr vertex_index_;
		Alembic::AbcGeom::Int32ArraySamplePtr face_count_;

		Alembic::AbcGeom::IN3fGeomParam::Sample normal_;
		Alembic::AbcGeom::IV2fGeomParam::Sample uv_;

		std::vector<Imath::V3f> original_normal_;
		std::map<std::string, Alembic::AbcGeom::IFaceSetSchema::Sample> faceset_;

		IndexList triangle_index_;

		UMOpenGLAbcMeshPtr opengl_mesh_;
		UMDirectX11AbcMeshPtr directx_mesh_;
		umdraw::UMMaterialList material_list_;
		std::vector<umstring> faceset_name_list_;
		std::vector<std::string> faceset_names_;
		std::vector<int> faceset_polycount_list_;
		std::vector<int> faceset_original_polycount_list_;
		bool is_cw_;
	};

/**
 * create
 */
	UMAbcMeshPtr UMAbcMesh::create(IPolyMeshPtr poly_mesh)
{
	UMAbcMeshPtr instance = UMAbcMeshPtr(new UMAbcMesh(poly_mesh));
	instance->impl_->self_reference_ = instance;
	return instance;
}

UMAbcMesh::UMAbcMesh(IPolyMeshPtr poly_mesh)
	: UMAbcObject(poly_mesh)
	, impl_(new UMAbcMesh::Impl(poly_mesh))
{}

/**
 * destructor
 */
UMAbcMesh::~UMAbcMesh()
{
}

/**
 * initialize
 */
bool UMAbcMesh::Impl::init(bool recursive)
{
	if (!is_valid()) return false;
	
	initial_bounds_prop_ = poly_mesh_->getSchema().getSelfBoundsProperty();

	size_t num_samples = poly_mesh_->getSchema().getNumSamples();
	if (num_samples > 0)
	{
		// get constant sample
		poly_mesh_->getSchema().get(initial_sample_);
		
		// if not consistant, we get time
		if (!poly_mesh_->getSchema().isConstant())
		{
			TimeSamplingPtr time = poly_mesh_->getSchema().getTimeSampling();
			set_min_time(static_cast<unsigned long>(time->getSampleTime(0) * 1000));
			set_max_time(static_cast<unsigned long>(time->getSampleTime(num_samples - 1) * 1000));
		}
	}
	
	faceset_name_list_.clear();
	faceset_names_.clear();
	poly_mesh_->getSchema().getFaceSetNames(faceset_names_);
	
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);

	for (int i = 0, size = static_cast<int>(faceset_names_.size()); i < size; ++i)
	{
		std::string name = faceset_names_.at(i);
		faceset_name_list_.push_back(umbase::UMStringUtil::utf8_to_utf16(name));
		
		IFaceSet faceset = poly_mesh_->getSchema().getFaceSet(name);
		IFaceSetSchema::Sample faceset_sample;
		faceset.getSchema().get(faceset_sample, selector);
		
		Int32ArraySamplePtr faces = faceset_sample.getFaces();
		size_t facesize = faces->size();
		faceset_[name] = faceset_sample;
	}

#ifdef WITH_OPENGL
	if (UMAbcMeshPtr self = self_reference_.lock())
	{
		opengl_mesh_ = UMOpenGLAbcMeshPtr(new UMOpenGLAbcMesh(self));
	}
#endif // WITH_OPENGL

#ifdef WITH_DIRECTX
	if (UMAbcMeshPtr self = self_reference_.lock())
	{
		directx_mesh_ = UMDirectX11AbcMeshPtr(new UMDirectX11AbcMesh(self));
	}
#endif // WITH_DIRECTX
	return true;
}

/**
 * set current time
 */
void UMAbcMesh::Impl::set_current_time(unsigned long time, bool recursive)
{
	update_mesh_all();
	UMAbcObject::set_current_time(time, false);
}

/** 
 * update normal
 */
void UMAbcMesh::Impl::update_normal()
{
	if (!is_valid()) return;
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
	IN3fGeomParam normal_param = poly_mesh_->getSchema().getNormalsParam();

	bool is_face_varying = false;
	if (normal_param.getNumSamples()) {

		// get scope and sample
		const GeometryScope scope = normal_param.getScope();
		if (scope == kVaryingScope
			|| scope == kVertexScope
			|| scope == kFacevaryingScope)
		{
			if (normal_param.isIndexed())
			{
				normal_param.getIndexed(normal_, selector);
			}
			else
			{
				normal_param.getExpanded(normal_, selector);
			}
		}
		is_face_varying = scope == kFacevaryingScope;
	}

	bool is_vertex_varying = false;

	if (!normal_.getVals() || normal_.getVals()->size() <= 0)
	{
		// make vertex varying normals
		is_vertex_varying = true;
		original_normal_.resize(vertex_->size());
		for (size_t i = 0, isize = original_normal_.size(); i < isize; ++i)
		{
			original_normal_[i] = Imath::V3f(0);
		}
		for (size_t i = 0, isize = triangle_index_.size(); i < isize; ++i)
		{
			const UMVec3ui& index = triangle_index_.at(i);
			const V3f& v0 = (*vertex_)[index[0]];
			const V3f& v1 = (*vertex_)[index[1]];
			const V3f& v2 = (*vertex_)[index[2]];
			V3f normal = (v1-v0).cross(v2-v0);
			original_normal_[index[0]] += normal;
			original_normal_[index[1]] += normal;
			original_normal_[index[2]] += normal;
		}
		// normalize
		for (size_t i = 0, isize = vertex_->size(); i < isize; ++i)
		{
			original_normal_[i].normalize();
		}
	}
	else
	{
		const V3f *normals =  normal_.getVals()->get();
		size_t normal_size = normal_.getVals()->size();
		size_t vertex_size = vertex_ ? vertex_->size() : 0;
		is_vertex_varying = normal_size == vertex_size;

		original_normal_.resize(normal_size);
		for (size_t i = 0; i < normal_size; ++i)
		{
			original_normal_[i] = normals[i];
			original_normal_[i].normalize();
		}
	}
	
#ifdef WITH_OPENGL
	if (opengl_mesh_ && !umdraw::UMDirectX11::current_device_pointer())
	{
		if (is_face_varying)
		{
			opengl_mesh_->update_normal(triangle_index_, original_normal_, UMOpenGLAbcMesh::eFaceVarying, is_cw_);
		}
		else
		{
			opengl_mesh_->update_normal(triangle_index_, original_normal_, UMOpenGLAbcMesh::eVertexVarying, is_cw_);
		}
	}
#endif // WITH_OPENGL

#ifdef WITH_DIRECTX
	if (directx_mesh_ && umdraw::UMDirectX11::current_device_pointer())
	{
		if (is_face_varying)
		{
			directx_mesh_->update_normal(
				reinterpret_cast<ID3D11Device*>(umdraw::UMDirectX11::current_device_pointer()),
				triangle_index_,
				original_normal_,
				UMDirectX11AbcMesh::eFaceVarying,
				is_cw_);
		}
		else
		{
			directx_mesh_->update_normal(
				reinterpret_cast<ID3D11Device*>(umdraw::UMDirectX11::current_device_pointer()),
				triangle_index_,
				original_normal_,
				UMDirectX11AbcMesh::eVertexVarying,
				is_cw_);
		}
	}
#endif // WITH_DIRECTX
}

/** 
 * update uv
 */
void UMAbcMesh::Impl::update_uv()
{
	if (!is_valid()) return;
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
	IV2fGeomParam uv_param = poly_mesh_->getSchema().getUVsParam();

	if (uv_param.getNumSamples() <= 0) return;

	// get scope and sample
	const GeometryScope scope = uv_param.getScope();
	if (scope == kVaryingScope
		|| scope == kVertexScope
		|| scope == kFacevaryingScope)
	{
		if (uv_param.isIndexed())
		{
			uv_param.getIndexed(uv_, selector);
		}
		else
		{
			uv_param.getExpanded(uv_, selector);
		}
	}
	
#ifdef WITH_OPENGL
	if (opengl_mesh_ && !umdraw::UMDirectX11::current_device_pointer())
	{
		if (scope == kFacevaryingScope)
		{
			opengl_mesh_->update_uv(triangle_index_, uv_, UMOpenGLAbcMesh::eFaceVarying, is_cw_);
		}
		else
		{
			opengl_mesh_->update_uv(triangle_index_, uv_, UMOpenGLAbcMesh::eVertexVarying, is_cw_);
		}
	}
#endif // WITH_OPENGL

#ifdef WITH_DIRECTX
	if (directx_mesh_ && umdraw::UMDirectX11::current_device_pointer())
	{
		if (scope == kFacevaryingScope)
		{
			directx_mesh_->update_uv(
				reinterpret_cast<ID3D11Device*>(triangle_index_, umdraw::UMDirectX11::current_device_pointer()),
				triangle_index_,
				uv_,
				UMDirectX11AbcMesh::eFaceVarying,
				is_cw_);
		}
		else
		{
			directx_mesh_->update_uv(
				reinterpret_cast<ID3D11Device*>(triangle_index_, umdraw::UMDirectX11::current_device_pointer()),
				triangle_index_,
				uv_,
				UMDirectX11AbcMesh::eVertexVarying,
				is_cw_);
		}
	}
#endif // WITH_DIRECTX
}

/**
 * update vertex
 */
void UMAbcMesh::Impl::update_vertex(IPolyMeshSchema::Sample& sample)
{
	if (!is_valid()) return;
	
	P3fArraySamplePtr vertex = sample.getPositions();

	vertex_ = vertex;
	
#ifdef WITH_OPENGL
	if (opengl_mesh_ && !umdraw::UMDirectX11::current_device_pointer())
	{
		opengl_mesh_->update_vertex(triangle_index_, vertex_, is_cw_);
	}
#endif // WITH_OPENGL

#ifdef WITH_DIRECTX
	if (directx_mesh_ && umdraw::UMDirectX11::current_device_pointer())
	{
		directx_mesh_->update_vertex(
			reinterpret_cast<ID3D11Device*>(umdraw::UMDirectX11::current_device_pointer()),
			triangle_index_,
			vertex_,
			is_cw_);
	}
#endif // WITH_DIRECTX
}

/**
 * update vertex index
 */
void UMAbcMesh::Impl::update_vertex_index_by_faceset(IPolyMeshSchema::Sample& sample)
{
	P3fArraySamplePtr vertex = sample.getPositions();
	Int32ArraySamplePtr vertex_index = sample.getFaceIndices();
	Int32ArraySamplePtr face_count = sample.getFaceCounts();
	if (!vertex) return;
	if (!vertex_index) return;
	if (!face_count) return;
	
	vertex_index_ = vertex_index;
	face_count_ = face_count;

	// update index buffer
	triangle_index_.clear();
	faceset_polycount_list_.clear();
	faceset_name_list_.clear();
	const size_t vertex_size = vertex->size();
	const size_t vertex_index_size_ = vertex_index_->size();
	const size_t face_count_size = face_count_->size();
	if (vertex_size <= 0) return;
	if (vertex_index_size_ <= 0) return;
	if (face_count_size <= 0) return;

	size_t face_index_begin = 0;
	size_t face_index_end = 0;
	std::vector<int> face_begin_index_list;
	face_begin_index_list.reserve(face_count_size);
	for (size_t face = 0; face < face_count_size; ++face)
	{
		face_index_begin = face_index_end;
		size_t count = (*face_count_)[face];
		face_begin_index_list.push_back(static_cast<int>(face_index_begin));
		face_index_end = face_index_begin + count;
		
		//if (face_index_end > vertex_index_size_ ||
		//	face_index_end < face_index_begin)
		//{
		//	break;
		//}
	}
	
	faceset_names_.clear();
	poly_mesh_->getSchema().getFaceSetNames(faceset_names_);
		
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
	for (int i = 0, size = static_cast<int>(faceset_names_.size()); i < size; ++i)
	{
		std::string name = faceset_names_.at(i);
		faceset_name_list_.push_back(umbase::UMStringUtil::utf8_to_utf16(name));
		
		IFaceSet faceset = poly_mesh_->getSchema().getFaceSet(name);
		IFaceSetSchema::Sample faceset_sample;
		faceset.getSchema().get(faceset_sample, selector);
		Int32ArraySamplePtr faces = faceset_sample.getFaces();
		size_t facesize = faces->size();
		//Int32ArraySamplePtr faces = faceset_[name].getFaces();
		int pre_polygon_count = static_cast<int>(triangle_index_.size());
		
		for (int k = 0; k < static_cast<int>(faces->size()); ++k)
		{
			int face = faces->get()[k];
			size_t begin_index = face_begin_index_list[face];
			size_t count = (*face_count_)[face];
			size_t end_index = begin_index + count;

			bool is_good_face = true;
			for (size_t n = begin_index; n < end_index; ++n)
			{
				if (static_cast<size_t>((*vertex_index_)[n]) >= vertex_size)
				{
					is_good_face = false;
					break;
				}
			}
			if (!is_good_face) break;

			if (count > 2)
			{
				if (is_cw_)
				{
					// this is alembic default
					triangle_index_.push_back(
						UMVec3ui(
						(*vertex_index_)[begin_index + 0],
						(*vertex_index_)[begin_index + 1],
						(*vertex_index_)[begin_index + 2]));
				}
				else
				{
					// wrong data. flip.
					triangle_index_.push_back(
						UMVec3ui(
						(*vertex_index_)[begin_index + 0],
						(*vertex_index_)[begin_index + 2],
						(*vertex_index_)[begin_index + 1]));
				}

				for (size_t n = 3; n < count; ++n)
				{
					if (is_cw_)
					{
						triangle_index_.push_back(
							UMVec3ui(
								(*vertex_index_)[begin_index + 0],
								(*vertex_index_)[begin_index + n-1],
								(*vertex_index_)[begin_index + n]));
					}
					else
					{
						triangle_index_.push_back(
							UMVec3ui(
								(*vertex_index_)[begin_index + 0],
								(*vertex_index_)[begin_index + n],
								(*vertex_index_)[begin_index + n-1]));
					}
				}
			}
		}
		faceset_polycount_list_.push_back(static_cast<int>(triangle_index_.size()) - pre_polygon_count);
	}
		
	update_material();
}

/**
 * update vertex index
 */
void UMAbcMesh::Impl::update_vertex_index(IPolyMeshSchema::Sample& sample)
{
	if (!is_valid()) return;

	if (!faceset_names_.empty())
	{
		update_vertex_index_by_faceset(sample);
		return;
	}
	
	P3fArraySamplePtr vertex = sample.getPositions();
	Int32ArraySamplePtr vertex_index = sample.getFaceIndices();
	Int32ArraySamplePtr face_count = sample.getFaceCounts();
	if (!vertex) return;
	if (!vertex_index) return;
	if (!face_count) return;
	
	vertex_index_ = vertex_index;
	face_count_ = face_count;

	// update index buffer
	triangle_index_.clear();
	faceset_polycount_list_.clear();
	const size_t vertex_size = vertex->size();
	const size_t vertex_index_size_ = vertex_index_->size();
	const size_t face_count_size = face_count_->size();
	if (vertex_size <= 0) return;
	if (vertex_index_size_ <= 0) return;
	if (face_count_size <= 0) return;

	size_t face_index_begin = 0;
	size_t face_index_end = 0;

	for (size_t face = 0; face < face_count_size; ++face)
	{
		face_index_begin = face_index_end;
		size_t count = (*face_count_)[face];
		face_index_end = face_index_begin + count;

		if (face_index_end > vertex_index_size_ ||
			face_index_end < face_index_begin)
		{
			break;
		}

		bool is_good_face = true;
		for (size_t i = face_index_begin; i < face_index_end; ++i)
		{
			if (static_cast<size_t>((*vertex_index_)[i]) >= vertex_size)
			{
				is_good_face = false;
				break;
			}
		}
		if (!is_good_face) break;

		if (count > 2)
		{
			if (is_cw_)
			{
				// this is alembic default
				triangle_index_.push_back(
					UMVec3ui(
					(*vertex_index_)[face_index_begin + 0],
					(*vertex_index_)[face_index_begin + 1],
					(*vertex_index_)[face_index_begin + 2]));
			}
			else
			{
				// wrong data. flip.
				triangle_index_.push_back(
					UMVec3ui(
					(*vertex_index_)[face_index_begin + 0],
					(*vertex_index_)[face_index_begin + 2],
					(*vertex_index_)[face_index_begin + 1]));
			}

			for (size_t i = 3; i < count; ++i)
			{
				if (is_cw_)
				{
					triangle_index_.push_back(
						UMVec3ui(
							(*vertex_index_)[face_index_begin + 0],
							(*vertex_index_)[face_index_begin + i-1],
							(*vertex_index_)[face_index_begin + i]));
				}
				else
				{
					triangle_index_.push_back(
						UMVec3ui(
							(*vertex_index_)[face_index_begin + 0],
							(*vertex_index_)[face_index_begin + i],
							(*vertex_index_)[face_index_begin + i-1]));
				}
			}
		}
	}
}

/** 
 * update mesh all
 */
void UMAbcMesh::Impl::update_mesh_all()
{
	ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);
	IPolyMeshSchema::Sample sample;

	if (poly_mesh_->getSchema().isConstant())
	{
		sample = initial_sample_;
	}
	else if (poly_mesh_->getSchema().getNumSamples() > 0)
	{
		poly_mesh_->getSchema().get(sample, selector);
	}

	P3fArraySamplePtr vertex = sample.getPositions();
	Int32ArraySamplePtr vertex_index = sample.getFaceIndices();
	Int32ArraySamplePtr face_count = sample.getFaceCounts();

	// update same size buffer
	if (vertex_ && vertex_index_ && face_count_)
	{
		if (vertex_->size() == vertex->size()
			&& vertex_index_->size() == vertex_index->size()
			&& face_count_->size() == face_count->size())
		{
			if (!faceset_names_.empty())
			{
				update_vertex_index(sample);
				//update_vertex(sample);
			}
			if (vertex_ == vertex)
			{
				update_normal();
				update_uv();
			}
			else
			{
				update_vertex(sample);
				update_normal();
				update_uv();
			}
			return;
		}
	}

	/// update different size buffer
	
	update_vertex_index(sample);
	update_vertex(sample);
	update_normal();
	update_uv();
}

void UMAbcMesh::Impl::update_material()
{
	umdraw::UMMaterialList sorted_material_list;
	sorted_material_list.reserve(material_list().size());

	for (int i = 0, size = static_cast<int>(faceset_names_.size()); i < size; ++i)
	{
		umstring name = faceset_name_list_.at(i);
		umdraw::UMMaterialList::iterator it = mutable_material_list().begin();
		for (; it != mutable_material_list().end(); ++it)
		{
			umdraw::UMMaterialPtr mat = *it;
			if (mat->name() == name)
			{
				if (mat->polygon_count() != faceset_polycount_list().at(i))
				{
					printf("diff %s, %s, %d, %d\n", umbase::UMStringUtil::utf16_to_utf8(name).c_str(), faceset_names_.at(i).c_str(), mat->polygon_count() ,faceset_polycount_list().at(i));
				}
				mat->set_polygon_count(faceset_polycount_list().at(i));
				sorted_material_list.push_back(mat);
				break;
			}
		}
	}
	if (material_list_.size() == sorted_material_list.size())
	{
		material_list_ = sorted_material_list;
	}
}

/**
 * update box
 */
void UMAbcMesh::Impl::update_box(bool recursive)
{
	if (!is_valid()) return;
	mutable_box().init();

	if (initial_bounds_prop_ && initial_bounds_prop_.getNumSamples() > 0)
	{
		ISampleSelector selector(current_time(), ISampleSelector::kNearIndex);

		mutable_box() = 
			UMAbcConvert::imath_box_to_um(
				initial_bounds_prop_.getValue(selector));
	}

	if (mutable_box().is_empty() && vertex_)
	{
		size_t vertex_count = vertex_->size();
		for (size_t i = 0; i < vertex_count; ++i)
		{
			const Imath::V3f& p = (*vertex_)[i];
			mutable_box().extend(UMAbcConvert::imath_vec_to_um(p));
		}
	}
}

/**
 * get polgon count
 */
int UMAbcMesh::Impl::polygon_count() const
{
	return static_cast<int>(triangle_index_.size());
}

///**
// * draw
// */
//void UMAbcMesh::Impl::draw(bool recursive, UMAbc::DrawType type)
//{
//	if (!is_valid()) return;
//	if (triangle_index_.empty()) return;
//	if (!vertex_) return;
//	
//#ifdef WITH_OPENGL
//	if (type == UMAbc::eOpenGL && !umdraw::UMDirectX11::current_device_pointer())
//	{
//		if (opengl_mesh_)
//		{
//			opengl_mesh_->draw(umdraw::UMOpenGLDrawParameterPtr());
//		}
//	}
//#endif // WITH_OPENGL
//
//#ifdef WITH_DIRECTX
//	if (type == UMAbc::eDirectX && umdraw::UMDirectX11::current_device_pointer())
//	{
//		if (directx_mesh_)
//		{
//			directx_mesh_->draw(
//				reinterpret_cast<ID3D11Device*>(umdraw::UMDirectX11::current_device_pointer()));
//		}
//	}
//#endif // WITH_DIRECTX
//}

/** 
 * get material from vertex index
 */
umdraw::UMMaterialPtr UMAbcMesh::Impl::material_from_face_index(int face_index) const
{
	int pos = 0;
	umdraw::UMMaterialList::const_iterator it = material_list_.begin();
	for (; it != material_list_.end(); ++it)
	{
		const int polygon_count = (*it)->polygon_count();
		if (face_index >= pos && face_index < (pos+polygon_count)) {
			return *it;
		}
		pos += polygon_count;
	}
	return umdraw::UMMaterialPtr();
}

/**
* initialize
* @param [in] recursive do children recursively
* @retval succsess or fail
*/
bool UMAbcMesh::init(bool recursive, UMAbcObjectPtr parent)
{
	impl_->init(recursive);
	return UMAbcObject::init(recursive, parent);
}

/**
* set current time
* @param [in] time time
* @param [in] recursive do children recursively
*/
void UMAbcMesh::set_current_time(unsigned long time, bool recursive)
{
	if (!impl_->is_valid()) return;
	UMAbcObject::set_current_time(time, recursive);
	impl_->set_current_time(time, recursive);
}

/**
 * update box
 * @param [in] recursive do children recursively
 */
void UMAbcMesh::update_box(bool recursive)
{
	impl_->update_box(recursive);
}

///**
// * draw
// * @param [in] recursive do children recursively
// */
//void UMAbcMesh::draw(bool recursive, UMAbc::DrawType type)
//{
//	impl_->draw(recursive, type);
//	UMAbcObject::draw(recursive, type);
//}


/**
* get opengl mesh
*/
UMOpenGLAbcMeshPtr UMAbcMesh::opengl_mesh()
{
	return impl_->opengl_mesh();
}

/**
* get directx mesh
*/
UMDirectX11AbcMeshPtr UMAbcMesh::directx_mesh()
{
	return impl_->directx_mesh();
}

/**
* get polygon count
*/
int UMAbcMesh::polygon_count() const
{
	return impl_->polygon_count();
}

/**
* update mesh all
*/
void UMAbcMesh::update_mesh_all()
{
	impl_->update_mesh_all();
}

UMAbcMesh::IndexList& UMAbcMesh::triangle_index()
{
	return impl_->triangle_index();
}

std::vector<Imath::V3f>& UMAbcMesh::normals()
{
	return impl_->normals();
}

/**
* get material list
*/
const umdraw::UMMaterialList& UMAbcMesh::material_list() const
{
	return impl_->material_list();
}

/**
* get material list
*/
umdraw::UMMaterialList& UMAbcMesh::mutable_material_list()
{
	return impl_->mutable_material_list();
}

/**
* get material from face index
*/
umdraw::UMMaterialPtr UMAbcMesh::material_from_face_index(int face_index) const
{
	return impl_->material_from_face_index(face_index);
}

/**
* get faceset name list
*/
const std::vector<umstring>& UMAbcMesh::faceset_name_list() const
{
	return impl_->faceset_name_list();
}

/**
* get faceset polycount list
*/
const std::vector<int>& UMAbcMesh::faceset_polycount_list() const
{
	return impl_->faceset_polycount_list();
}

/**
* set cw or not
*/
void UMAbcMesh::set_cw(bool is_cw)
{
	impl_->set_cw(is_cw);
}

UMAbcObjectPtr UMAbcMesh::self_reference()
{
	return impl_->self_reference();
}

/**
* get vertex
*/
const Imath::V3f * UMAbcMesh::vertex() const
{
	return impl_->vertex()->get();
}

/**
 * get vertex size
 */
unsigned int UMAbcMesh::vertex_size() const
{
	if (impl_->vertex())
		return static_cast<unsigned int>(impl_->vertex()->size());
	else
		return 0;
}

/**
* get uv
*/
const Imath::V2f * UMAbcMesh::uv() const
{
	return impl_->uv().getVals()->get();
}

/**
 * get uv size
 */
unsigned int UMAbcMesh::uv_size() const
{
	if (!impl_->uv().getVals()) {
		return 0;
	}
	return static_cast<unsigned int>(impl_->uv().getVals()->size());
}

/**
* get minumum time
*/
unsigned long UMAbcMesh::min_time() const
{
	return impl_->min_time();
}

/**
* get maximum time
*/
unsigned long UMAbcMesh::max_time() const
{
	return impl_->max_time();
}

/**
* set minumum time
*/
void UMAbcMesh::set_min_time(unsigned long time)
{
	impl_->set_min_time(time);
}

/**
* set maximum time
*/
void UMAbcMesh::set_max_time(unsigned long time)
{
	impl_->set_max_time(time);
}

/**
* get current time
*/
double UMAbcMesh::current_time() const
{
	return impl_->current_time();
}


} // umabc
