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

	Point operator-(const Point& P);
	Point operator+(const Point& P, const Point& Q);
	const Point& operator+=(Point& P, const Point& Q);
	Point operator-(const Point& P, const Point& Q);
	const Point& operator-=(Point& P, const Point& Q);
	// dot product
	float operator*(const Point& P, const Point& Q);

	Point operator*(const Point& P, float k);
	const Point& operator*=(Point& P, float k);
	Point operator/(const Point& P, float k);
	const Point& operator/=(Point& P, float k);

	void includeGeoLibToCompiler(ScriptCompiler* scriptCompiler);
}