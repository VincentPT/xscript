#include "FFCompLine.h"

FFCompLine::FFCompLine() : _lineWidth(1) {
}

FFCompLine::~FFCompLine() {}

void FFCompLine::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

float FFCompLine::getLineWidth() const {
	return _lineWidth;
}

const char* FFCompLine::getName() const {
	return LINE_COMPONENT_NAME;
}