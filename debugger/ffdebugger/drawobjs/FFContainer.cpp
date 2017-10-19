#include "FFContainer.h"

#include <cinder\gl\gl.h>

using namespace ci;

FFContainer::FFContainer() {

}

FFContainer::~FFContainer() {

}

FFLocation FFContainer::getLocation() {
	return FFLocation(0, 0);
}

int FFContainer::addObject(FFDrawObjRef buzzDrawObjRef) {
	_children.push_back(buzzDrawObjRef);
	return (int)_children.size();
}

// this function will be call each frame
void FFContainer::draw() {
	if (isVisible() == false) {
		return;
	}
	for (FFDrawObjRef& obj : _children) {
		obj->draw();
	}
}

const std::list<FFDrawObjRef>& FFContainer::getChildren() const {
	return _children;
}