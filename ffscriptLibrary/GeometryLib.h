/******************************************************************
* File:        GeometryLib.h
* Description: declare some geometry functions and object and
*              declare an interface to import them into the
*              script compiler.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once

#include "ffscript.h"

namespace ffscript {
	class ScriptCompiler;

#pragma pack(push, 1)
	struct Point {
		float x;
		float y;
	};

	struct Ray {
		Point start;
		Point dir;
	};
#pragma pack(pop)

	Point operator-(Point P);
	Point operator+(Point P, Point Q);
	const Point& operator+=(Point& P, Point Q);
	Point operator-(Point P, Point Q);
	const Point& operator-=(Point& P, Point Q);
	// dot product
	float operator*(Point P, Point Q);

	Point operator*(Point P, float k);
	const Point& operator*=(Point& P, float k);
	Point operator/(Point P, float k);
	const Point& operator/=(Point& P, float k);

	void includeGeoLibToCompiler(ScriptCompiler* scriptCompiler);
}