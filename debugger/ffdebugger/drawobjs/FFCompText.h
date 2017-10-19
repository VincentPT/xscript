#pragma once
#include "FFDrawComponent.h"

#include <string>

class FFCompText : public FFDrawComponent {
protected:
	float _fontSize;
	std::string _text;
	std::string _fontName;
public:
	FFCompText();
	FFCompText(const std::string& text);
	FFCompText(std::string&& text);
	virtual ~FFCompText();

	virtual const char* getName() const;

	void setFontSize(float fontSize);
	float getFontSize() const;

	void setText(const std::string& text);
	void setText(std::string&& text);
	const std::string& getText() const;

	void setFontName(const std::string& fontName);
	const std::string& getFontName() const;
};

#define TEXT_COMPONENT_NAME "FFCompText"