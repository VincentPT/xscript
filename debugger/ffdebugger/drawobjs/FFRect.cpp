#include "FFRect.h"

#include <cinder\gl\gl.h>

using namespace ci;
using namespace std;

FFRect::FFRect() : FFRect(0,0,0,0) {
}

FFRect::FFRect(const float& x, const float& y, const float& w, const float& h) : _location(x,y), _size(w,h){
	_lineComp = make_shared<FFCompLine>();
	addComponent(_lineComp);

	_textComp = make_shared<FFCompText>();
	addComponent(_textComp);
}

FFRect::~FFRect() {}

FFLocation FFRect::getLocation() {
	return _location;
}

const FFLocation& FFRect::getLocation() const {
	return _location;
}

const FFLocation& FFRect::getSize() const {
	return _size;
}

void FFRect::setText(const std::string& text) {
	_textComp->setText(text);
}

// this function will be call each frame
void FFRect::draw() {
	if (isVisible() == false) {
		return;
	}
	{
		auto& color = _lineComp->getColor();
		auto lineW = _lineComp->getLineWidth();
		gl::ScopedColor scopeColor(BCRED(color) / 255.0f, BCGREEN(color) / 255.0f, BCBLUE(color) / 255.0f, BCALPHA(color) / 255.0f);
		auto rect = ci::Rectf(_location.x, _location.y, _location.x + _size.x, _location.y + _size.y);
		gl::drawStrokedRect(rect, lineW);
	}
	{
		auto& rgba = _textComp->getColor();
		auto& text = _textComp->getText();
		float fontSize = _textComp->getFontSize();
		ColorA color(BCRED(rgba) / 255.0f, BCGREEN(rgba) / 255.0f, BCBLUE(rgba) / 255.0f, BCALPHA(rgba) / 255.0f);

		cinder::Font font(_textComp->getFontName(), fontSize);
		auto fontHeight = font.getAscent() +  font.getDescent() ;
		vec2 pos(_location.x + _size.x / 2, _location.y + _size.y / 2 - fontHeight/ 2);

		gl::drawStringCentered(text, pos, color, font);
	}
}