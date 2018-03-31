/**
 *
 * @file UMAnimation.h
 *
 * @author tori31001 at gmail.com
 *
 *
 * Copyright (C) 2014 Kazuma Hatta
 * Licensed under the MIT or GPL Version 3 licenses. 
 *
 */
#pragma once

#include "UMListTypes.h"

#include <map>
#include <vector>
#include <list>
#include <memory>

#ifdef WITH_PYTHON
	//#include <boost/shared_ptr.hpp>
#endif

#include "UMObject.h"

namespace umio
{

typedef unsigned long long UMTime;

class UMAnimation;
#ifdef WITH_PYTHON
	typedef std::shared_ptr<UMAnimation> UMAnimationPtr;
	//typedef boost::shared_ptr<UMAnimation> UMAnimationPtr;
#else
	typedef std::shared_ptr<UMAnimation> UMAnimationPtr;
#endif // WITH_PYTHON

/*---------------------------------------------------------------------------*
 * UMAnimationCurveKeyTypes
 *---------------------------------------------------------------------------*/
class UMAnimationCurveKeyTypes {
public:

	enum ECurveType {
		// node
		eLocalTranslationX,
		eLocalTranslationY,
		eLocalTranslationZ,
		eLocalRotationX,
		eLocalRotationY,
		eLocalRotationZ,
		eLocalScalingX,
		eLocalScalingY,
		eLocalScalingZ,
		// blendshape
		eBlendShapeValue,
		// camera
		eFieldOfView,
		eFieldOfViewX,
		eFieldOfViewY,
		eOpticalCenterX,
		eOpticalCenterY,
		eRoll,
		// light
		eLightIntensity,
		eLightOuterAngle,
		eLightFog,
		// color
		eColorRed,
		eColorGreen,
		eColorBlue,
	};

	enum ETangentMode {
		eTangentAuto = (1 << 0),
		eTangentTCB  = (1 << 1),
		eTangentUser = (1 << 2),
		eTangentGenericBreak = (1 << 3),
		eTangentBreak = eTangentGenericBreak|eTangentUser,
		eTangentAutoBreak = eTangentGenericBreak|eTangentAuto,
		eTangentGenericClamp = (1 << 4),
		eTangentGenericTimeIndependent  = (1 << 5),
		eTangentGenericClampProgressive = (1 << 6)|eTangentGenericTimeIndependent 
	};

	enum EInterpolationType {
		eInterpolationConstant = (1 << 0),
		eInterpolationLinear = (1 << 1),
		eInterpolationCubic = (1 << 2)
	};

	enum EWeightedMode {
		eWeightedNone = 0,
		eWeightedRight = (1 << 0),
		eWeightedNextLeft = (1 << 1),
		eWeightedAll = eWeightedRight|eWeightedNextLeft 
	};

	enum EConstantMode {
		eConstantStandard = 0,
		 eConstantNext = 1
	};

	enum EVelocityMode {
		eVelocityNone = 0,
		eVelocityRight = (1 << 0),
		eVelocityNextLeft = (1 << 1),
		eVelocityAll = eVelocityRight|eVelocityNextLeft
	};

	enum ETangentVisibility {
		eTangentShowNone = 0,
		eTangentShowLeft =(1 << 0),
		eTangentShowRight =(1 << 1),
		eTangentShowBoth = eTangentShowLeft|eTangentShowRight
	};

	enum EDataIndex {
		eRightSlope = 0,
		eNextLeftSlope = 1,
		eWeights = 2,
		eRightWeight = 2,
		eNextLeftWeight = 3,
		eVelocity = 4,
		eRightVelocity = 4,
		eNextLeftVelocity = 5,
		eTCBTension = 0,
		eTCBContinuity = 1,
		eTCBBias = 2
	};
};

/*---------------------------------------------------------------------------*
 * UMAnimationCurveKey
 *---------------------------------------------------------------------------*/
class UMAnimationCurveKey {
public:
	typedef std::map<UMTime, UMAnimationCurveKey> KeyMap;
	typedef std::map<int, float> DataMap;
	
	// getter
	UMTime time() const { return time_; }
	const DataMap& data_map() const { return data_map_; }
	float value() const { return value_; }
	int interpolation_type() const { return interpolation_type_; }
	int tangent_mode() const { return tangent_mode_; }
	int weighted_mode() const { return weighted_mode_; }
	int velocity_mode() const { return velocity_mode_; }
	int constant_mode() const { return constant_mode_; }
	int tangent_visibility() const { return tangent_visibility_; }
	bool is_break() const { return is_break_; }

	// setter
	void set_time(UMTime time) { time_ = time; }
	DataMap& mutable_data_map() { return data_map_; }
	void set_value(float value) { value_ = value; }
	void set_interpolation_type(int type) { interpolation_type_ = type; }
	void set_tangent_mode(int mode) { tangent_mode_ = mode; }
	void set_weighted_mode(int mode) { weighted_mode_ = mode; }
	void set_velocity_mode(int mode) { velocity_mode_ = mode; }
	void set_constant_mode(int mode) { constant_mode_ = mode; }
	void set_tangent_visibility(int mode) { tangent_visibility_ = mode; }
	void set_break(bool b) { is_break_ = b; }

	// msgpack serialization
	#include "msg/UMAnimationCurveKeyMsg.h"
private:
	UMTime time_;

	// EDataIndex, value
	DataMap data_map_;

	float value_;
	// EInterpolationType
	int interpolation_type_;
	// ETangentMode
	int tangent_mode_;
	// EWeightedMode
	int weighted_mode_;
	// EVelocityMode
	int velocity_mode_;
	// EConstantMode
	int constant_mode_;
	// ETangentVisibility
	int tangent_visibility_;
	
	bool is_break_;
};

/*---------------------------------------------------------------------------*
 * UMAnimationCurve
 *---------------------------------------------------------------------------*/
class UMAnimationCurve {
public:
	// key = ECurveType
	typedef std::map<int, UMAnimationCurve> CurveMap;
	
	UMAnimationCurve() {}

	// getter
	const UMAnimationCurveKey::KeyMap& key_map() const { return key_map_; }
	
	// setter
	UMAnimationCurveKey::KeyMap& mutable_key_map() { return key_map_; }
	
	//// standard curve node names
	//static std::string CurveNodeTransform;
	//static std::string CurveNodeTranslation;
	//static std::string CurveNodeRotation;
	//static std::string CurveNodeScaling;
	//static std::string CurveNodeComponentX;
	//static std::string CurveNodeComponentY;
	//static std::string CurveNodeComponentZ;
	//static std::string CurveNodeColor;
	//static std::string CurveNodeColorRed;
	//static std::string CurveNodeColorGreen;
	//static std::string CurveNodeColorBlue;

	// msgpack serialization
	#include "msg/UMAnimationCurveMsg.h"
private:
	UMAnimationCurveKey::KeyMap key_map_;

	// TODO tangent info
};

//std::string UMAnimationCurve::CurveNodeTransform = "Transform";
//std::string UMAnimationCurve::CurveNodeTranslation = "T";
//std::string UMAnimationCurve::CurveNodeRotation = "R";
//std::string UMAnimationCurve::CurveNodeScaling = "S";
//std::string UMAnimationCurve::CurveNodeComponentX = "X";
//std::string UMAnimationCurve::CurveNodeComponentY = "Y";
//std::string UMAnimationCurve::CurveNodeComponentZ = "Z";
//std::string UMAnimationCurve::CurveNodeColor = "Color";
//std::string UMAnimationCurve::CurveNodeColorRed = "X";
//std::string UMAnimationCurve::CurveNodeColorGreen = "Y";
//std::string UMAnimationCurve::CurveNodeColorBlue = "Z";

/*---------------------------------------------------------------------------*
 * UMAnimationCurveStack
 *---------------------------------------------------------------------------*/
class UMAnimationCurveStack {
public:
	typedef std::map<int, UMAnimationCurveStack> IDToCurveStackMap;

	UMAnimationCurveStack() {}
	~UMAnimationCurveStack() {}

	// getter
	const UMAnimationCurve::CurveMap& curve_map() const { return curve_map_; }

	// setter
	UMAnimationCurve::CurveMap& mutable_curve_map() { return curve_map_; }

	// msgpack serialization
	#include "msg/UMAnimationCurveStackMsg.h"
private:
	UMAnimationCurve::CurveMap curve_map_;
};

/*---------------------------------------------------------------------------*
 * UMAnimationLayer
 *---------------------------------------------------------------------------*/
class UMAnimationLayer {
public:
	typedef std::vector<UMAnimationLayer> LayerList;

	enum EBlendMode {
		eBlendAdditive,
		eBlendOverride,
		eBlendOverridePassthrough
	};

	enum ERotationAccumulationMode {
		eRotationByLayer,
		eRotationByChannel
	};

	enum EScaleAccumulationMode {
		eScaleMultiply,
		eScaleAdditive
	};

	UMAnimationLayer() :
		weight_(0), mute_(false), solo_(false), lock_(false),
		blend_mode_(-1),
		rotation_accumulation_mode_(-1),
		scale_accumulation_mode_(-1)
		{}
		
	~UMAnimationLayer() {}

	// getter
	double weight() const { return weight_; }
	bool mute() const { return mute_; }
	bool solo() const { return solo_; }
	bool lock() const { return lock_; }
	const UMVec4d& color() const { return color_; }
	int blend_mode() const { return blend_mode_; }
	int rotation_accumulation_mode() const { return rotation_accumulation_mode_; }
	int scale_accumulation_mode() const { return scale_accumulation_mode_; }
	const UMAnimationCurveStack::IDToCurveStackMap& curve_stack_map() const { return curve_stack_map_; }

	// setter
	void set_weight(double weight) { weight_ = weight; }
	void set_mute(bool mute) { mute_ = mute; }
	void set_solo(bool solo) { solo_ = solo; }
	void set_lock(bool lock) { lock_ = lock; }
	UMVec4d& mutable_color() { return color_; }
	void set_blend_mode(int mode) { blend_mode_ = mode; }
	void set_rotation_accumulation_mode(int mode) { rotation_accumulation_mode_ = mode; }
	void set_scale_accumulation_mode(int mode) { scale_accumulation_mode_ = mode; }
	UMAnimationCurveStack::IDToCurveStackMap& mutable_curve_stack_map() { return curve_stack_map_; }

	// msgpack serialization
	#include "msg/UMAnimationLayerMsg.h"
private:
	double weight_;
	bool mute_;
	bool solo_;
	bool lock_;
	UMVec4d color_;
	int blend_mode_;
	int rotation_accumulation_mode_;
	int scale_accumulation_mode_;
	UMAnimationCurveStack::IDToCurveStackMap curve_stack_map_;
};

/*---------------------------------------------------------------------------*
 * UMAnimationStack
 *---------------------------------------------------------------------------*/
class UMAnimationStack {
public:
	typedef std::vector<UMAnimationStack> AnimationStackList;

	UMAnimationStack() :
		local_start_(-1), 
		local_stop_(-1), 
		reference_start_(-1), 
		reference_stop_(-1)
		{}

	~UMAnimationStack() {}
	
	// getter 
	const std::string& name() const { return name_; }
	const std::string& description() const { return description_; }
	UMTime local_start() const { return local_start_; }
	UMTime local_stop() const { return local_stop_; }
	UMTime reference_start() const { return reference_start_; }
	UMTime reference_stop() const { return reference_stop_; }
	const UMAnimationLayer::LayerList& layer_list() const { return layer_list_; }

	// setter
	void set_name(const std::string& name) { name_ = name; }
	void set_description(const std::string & desc) { description_ = desc; }
	void set_local_start(UMTime start) { local_start_ = start; }
	void set_local_stop(UMTime stop) { local_start_ = stop; }
	void set_reference_start(UMTime start) { reference_start_ = start; }
	void set_reference_stop(UMTime stop) { reference_stop_ = stop; }
	UMAnimationLayer::LayerList& mutable_layer_list() { return layer_list_; }

	// msgpack serialization
	#include "msg/UMAnimationStackMsg.h"
private:
	std::string name_;
	std::string description_;
	UMTime local_start_;
	UMTime local_stop_;
	UMTime reference_start_;
	UMTime reference_stop_;
	UMAnimationLayer::LayerList layer_list_;
};

/*---------------------------------------------------------------------------*
 * UMAnimation
 *---------------------------------------------------------------------------*/
class UMAnimation {
public:	

	UMAnimation() {}
	virtual ~UMAnimation() {}

	static UMAnimationPtr create_animation() { return UMAnimationPtr(new UMAnimation()); }

	// getter
	const UMAnimationStack::AnimationStackList& animation_stack_list() const { return animation_stack_list_; }

	// setter
	UMAnimationStack::AnimationStackList& mutable_animation_stack_list() { return animation_stack_list_; }

	// msgpack serialization
	#include "msg/UMAnimationMsg.h"
private:
	UMAnimationStack::AnimationStackList animation_stack_list_;
};

} // namespace umio
