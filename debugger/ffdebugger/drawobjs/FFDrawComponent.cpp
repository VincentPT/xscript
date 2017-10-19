#include "FFDrawComponent.h"

FFDrawComponent::FFDrawComponent() : _rgba(0x000000FF) {}

FFDrawComponent::~FFDrawComponent() {}


void FFDrawComponent::setColor(const FFColor& rgba) {
	_rgba = rgba;
}

const FFColor& FFDrawComponent::getColor() const {
	return _rgba;
}

const char* FFDrawComponent::getName() const {
	return BASE_COMPONENT_NAME;
}