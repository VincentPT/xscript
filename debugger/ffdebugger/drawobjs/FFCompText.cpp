#include "FFCompText.h"

FFCompText::FFCompText() : _fontSize(30.0f), _fontName("Times New Roman"){
}

FFCompText::FFCompText(const std::string& text) : _text(text), _fontSize(30.0f), _fontName("Times New Roman") {

}

FFCompText::FFCompText(std::string&& text) : _text(text), _fontSize(30.0f), _fontName("Times New Roman") {

}

FFCompText::~FFCompText() {}

const char* FFCompText::getName() const {
	return TEXT_COMPONENT_NAME;
}

void FFCompText::setFontSize(float fontSize) {
	_fontSize = fontSize;
}

float FFCompText::getFontSize() const {
	return _fontSize;
}

void FFCompText::setText(const std::string& text) {
	_text = text;
}

void FFCompText::setText(std::string&& text) {
	_text = text;
}

const std::string& FFCompText::getText() const {
	return _text;
}

void FFCompText::setFontName(const std::string& fontName) {
	_fontName = fontName;
}

const std::string& FFCompText::getFontName() const {
	return _fontName;
}