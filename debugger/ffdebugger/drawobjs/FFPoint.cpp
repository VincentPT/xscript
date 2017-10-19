#include "FFPoint.h"

#include <cinder\gl\gl.h>

using namespace ci;
using namespace std;

FFPoint::FFPoint() : _location(0, 0), _radius(3) {
	_colorComp = make_shared<FFDrawComponent>();
}

FFPoint::FFPoint(const float& x, const float& y) : _location(x,y), _radius(3) {
}

FFPoint::~FFPoint() {

}

FFLocation FFPoint::getLocation() {
	return _location;
}

const FFLocation& FFPoint::getLocation() const {
	return _location;
}

void FFPoint::setRadius(float radius) {
	_radius = radius;
}

void FFPoint::draw() {
	if (isVisible() == false) {
		return;
	}
	auto& color = _colorComp->getColor();
	gl::ScopedColor scopeColor(BCRED(color) / 255.0f, BCGREEN(color) / 255.0f, BCBLUE(color) / 255.0f, BCALPHA(color) / 255.0f);
	gl::drawSolidCircle(glm::vec2(_location.x, _location.y), _radius);
}