/**
 *
 * @file UMIOSetting.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2012 Kazuma Hatta
 * Dual licensed under the MIT or GPL Version 2 licenses. 
 *
 */
#pragma once

#include <map>

#ifdef WITH_BOOST_SERIALIZATION
	#include <boost/serialization/serialization.hpp>
	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/map.hpp>
	#include <boost/serialization/string.hpp>
	#include <boost/serialization/array.hpp>
#endif

namespace umio
{

class UMIOSetting
{
public:
	// from fbxiosettingpath
	enum EExpSettingType {
		eExpColladaTriangulate,
		eExpColladaSingleMatrix,
		eExpColladaFrameRate,
		eExpDxfTriangulate,
		eExpDxfDeformation,
		eExpObjTriangulate,
		eExpObjDeformation,
		eExp3dsReferenceNode,
		eExp3dsTexture,
		eExp3dsMaterial,
		eExp3dsAnimation,
		eExp3dsMesh,
		eExp3dsLight,
		eExp3dsCamera,
		eExp3dsAmbientLight,
		eExp3dsRescaling,
		eExp3dsTexuvbypoly,
		eExpFbxTemplate,
		eExpFbxPivot,
		eExpFbxGlobalSettings,
		eExpFbxCharacter,
		eExpFbxConstraint,
		eExpFbxGobo,
		eExpFbxShape,
		eExpFbxMaterial,
		eExpFbxTexture,
		eExpFbxModel,
		eExpFbxAnimation,
		eExpFbxEmbedded,
		eExpFbxPassword,
		eExpFbxPasswordEnable,
		eExpFbxCollapseExternals,
		eExpFbxCompressArrays,
		eExpFbxCompressLevel,
		eExpFbxCompressMinsize,
		eExpFbxEmbeddedPropertiesSkipExpFbx,
		eExpFbxExportFileVersion,
	};

	// from fbxiosettingpath
	enum EImpSettingType {
		eImpDxfWeldVertices,
		eImpDxfObjectDerivation,
		eImpDxfReferenceNode,
		eImpObjReferenceNode,
		eImp3dsReferenceNode,
		eImp3dsTexture,
		eImp3dsMaterial,
		eImp3dsAnimation,
		eImp3dsMesh,
		eImp3dsLight,
		eImp3dsCamera,
		eImp3dsAmbientLight,
		eImp3dsRescaling,
		eImp3dsFilter,
		eImp3dsSmoothgroup,
		eImpFbxModelCount,
		eImpFbxDeviceCount,
		eImpFbxCharacterCount,
		eImpFbxActorCount,
		eImpFbxConstraintCount,
		eImpFbxMediaCount,
		eImpFbxTemplate,
		eImpFbxPivot,
		eImpFbxGlobalSettings,
		eImpFbxCharacter,
		eImpFbxConstraint,
		eImpFbxMergeLayerAndTimewarp,
		eImpFbxGobo,
		eImpFbxShape,
		eImpFbxLink,
		eImpFbxMaterial,
		eImpFbxTexture,
		eImpFbxModel,
		eImpFbxAnimation,
		eImpFbxPassword,
		eImpFbxPasswordEnable,
		eImpFbxCurrentTakeName,
	};
	
	// UM original
	enum EUMImpSettingType {
		eUMImpFBX, // using fbx sdk
		eUMImpTriangulate,
		eUMImpNurbs,
		eUMImpPatch,

	};

	// UM original
	enum EUMExpSettingType {
		eUMExpFBX, // using fbx sdk
		eUMExpText,
		eUMExpOldFBX,
	};

	// FbxAxisSetting
	enum EFbxAxisType
	{
		// from FbxAxisSystem
		eFbxAxisMayaZUp,
		eFbxAxisMayaYUp,
		eFbxAxisMax,
		eFbxAxisMotionBuilder,
		eFbxAxisOpenGL,
		eFbxAxisDirectX,
		eFbxAxisLightWave,
	};
	
	enum EFbxSystemUnitType
	{
		eFbxSystemUnitMM, // millimeters
		eFbxSystemUnitDM, // decimeters
		eFbxSystemUnitCM, // centimeters
		eFbxSystemUnitM, // meters
		eFbxSystemUnitKM, // kilometers
		eFbxSystemUnitInch, // inches
		eFbxSystemUnitFoot, // feet
		eFbxSystemUnitMile, // miles
		eFbxSystemUnitYard, // yard
	};

	UMIOSetting()
	{
		bl_imp_prop_map_[eUMImpFBX]				= true;
		bl_exp_prop_map_[eUMExpFBX]				= true;

		imp_prop_map_[eImpFbxMaterial]			= true;
		imp_prop_map_[eImpFbxTexture]			= true;
		imp_prop_map_[eImpFbxLink]				= true;
		imp_prop_map_[eImpFbxShape]				= true;
		imp_prop_map_[eImpFbxGobo]				= true;
		imp_prop_map_[eImpFbxAnimation]			= true;
		imp_prop_map_[eImpFbxGlobalSettings]	= true;
		exp_prop_map_[eExpFbxMaterial]			= true;
		exp_prop_map_[eExpFbxTexture]			= true;
		exp_prop_map_[eExpFbxEmbedded]			= false;
		exp_prop_map_[eExpFbxShape]				= true;
		exp_prop_map_[eExpFbxGobo]				= true;
		exp_prop_map_[eExpFbxAnimation]			= true;
		exp_prop_map_[eExpFbxGlobalSettings]	= true;
		axis_type_ = eFbxAxisMayaYUp;
		system_unit_type_ = eFbxSystemUnitCM;
	}
	~UMIOSetting(){}

	typedef std::map<EExpSettingType, bool> ExpSettingMap;
	typedef std::map<EImpSettingType, bool> ImpSettingMap;
	typedef std::map<EUMExpSettingType, bool> UMExpSettingMap;
	typedef std::map<EUMImpSettingType, bool> UMImpSettingMap;
	
	// getter
	const ExpSettingMap& exp_prop_map() const { return exp_prop_map_; }
	const ImpSettingMap& imp_prop_map() const { return imp_prop_map_; }
	const UMExpSettingMap& bl_exp_prop_map() const { return bl_exp_prop_map_; }
	const UMImpSettingMap& bl_imp_prop_map() const { return bl_imp_prop_map_; }
	const EFbxAxisType axis_type() const { return axis_type_; }
	const EFbxSystemUnitType system_unit_type() const { return system_unit_type_; }

	// external use
	void set_exp_bool_prop(EExpSettingType type, bool prop) {
		exp_prop_map_[type] = prop;
	}
	void set_imp_bool_prop(EImpSettingType type, bool prop) {
		imp_prop_map_[type] = prop;
	}
	void set_bl_exp_bool_prop(EUMExpSettingType type, bool prop) {
		bl_exp_prop_map_[type] = prop;
	}
	void set_bl_imp_bool_prop(EUMImpSettingType type, bool prop) {
		bl_imp_prop_map_[type] = prop;
	}
	void set_axis_type(EFbxAxisType type) {
		axis_type_ = type;
	}
	void set_system_unit_type(EFbxSystemUnitType type) {
		system_unit_type_ = type;
	}
	
private:

	// for bos serialization
	#include "UMIOSettingBos.h"

	ExpSettingMap exp_prop_map_;
	ImpSettingMap imp_prop_map_;
	UMExpSettingMap bl_exp_prop_map_;
	UMImpSettingMap bl_imp_prop_map_;
	EFbxAxisType axis_type_;
	EFbxSystemUnitType system_unit_type_;
};

} // namespace umio
