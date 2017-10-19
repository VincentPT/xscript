#include "FFPolygon.h"

#include <cinder\gl\gl.h>

using namespace ci;
using namespace std;

FFPolygon::FFPolygon() {
	_lineComp = make_shared<FFCompLine>();
	addComponent(_lineComp);
}
FFPolygon::~FFPolygon() {}

FFLocation FFPolygon::getLocation() {
	return FFLocation(0, 0);
}

int FFPolygon::addPoint(const float&x, const float&y) {
	_points.push_back(FFLocation(x, y));
	return (int)_points.size();
}

int FFPolygon::getPointCount() const {
	return (int)_points.size();
}

// this function will be call each frame
void FFPolygon::draw() {
	if (isVisible() == false) {
		return;
	}

	auto& color = _lineComp->getColor();
	auto lineW = _lineComp->getLineWidth();

	gl::ScopedColor scopeColor(BCRED(color) / 255.0f, BCGREEN(color) / 255.0f, BCBLUE(color) / 255.0f, BCALPHA(color) / 255.0f);
	gl::ScopedLineWidth scopeLineWidth(lineW);
	
	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line loop and 'end' will execute the draw calls on the GPU.
	gl::begin(GL_LINE_LOOP);
	for (auto &point : _points) {
		vec2 p(point.x, point.y);
		gl::vertex(p);
	}
	gl::end();
}