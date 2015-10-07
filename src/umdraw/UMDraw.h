/**
 * @file UMDraw.h
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 * Licensed  under the MIT license. 
 *
 */
#pragma once

#include "UMScene.h"

/// Uimac model draw library
namespace umdraw
{

class UMDraw;
typedef std::shared_ptr<UMDraw> UMDrawPtr;

/**
 * umdraw interface
 */
class UMDraw
{
	DISALLOW_COPY_AND_ASSIGN(UMDraw);

public:
	enum DrawType {
		eSoftware,
		eOpenGL,
		eDirectX,
	};
	
	virtual ~UMDraw() {}
	
	/**
	 * create instance
	 */
	static UMDrawPtr create(DrawType type);

	/**
	 * get draw type
	 */
	DrawType draw_type() const { return  draw_type_; }

	/**
	 * initialize
	 */
	virtual bool init(void* hWnd, umdraw::UMScenePtr scene) = 0;
	
	/**
	 * update scene
	 */
	virtual bool update(unsigned long time) = 0;

	/**
	 * draw frame
	 */
	virtual bool draw() = 0;

	/**
	 * create view/depth
	 */
	virtual bool clear() = 0;

	/**
	 * resize
	 */
	virtual void resize(int width, int height) = 0;
	
	/**
	 * get umdraw scene
	 */
	virtual UMScenePtr scene() const = 0;

protected:
	UMDraw() {}

private:
	DrawType draw_type_;
};

} // umdraw
