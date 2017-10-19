#include "FFDrawObj.h"

FFDrawObj::FFDrawObj() : _isVisible(true) {

}

FFDrawObj::~FFDrawObj() {

}

void FFDrawObj::update() {}

const std::string& FFDrawObj::getName() const {
	return _name;
}

void FFDrawObj::setName(const std::string& name) {
	_name = name;
}

void FFDrawObj::setVisible(bool visible) {
	_isVisible = visible;
}

bool FFDrawObj::isVisible() const {
	return _isVisible;
}

void FFDrawObj::addComponent(const FFDrawComponentRef& component) {
	_components[component->getName()] = component;
}

FFDrawComponent* FFDrawObj::getComponent(const char* name) const {
	auto it = _components.find(name);
	if (it != _components.end()) {
		return it->second.get();
	}
	return nullptr;
}