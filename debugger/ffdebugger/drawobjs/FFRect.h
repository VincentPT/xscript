#pragma once
#include "FFDrawObj.h"
#include "FFCompLine.h"
#include "FFCompText.h"

class FFRect : public FFDrawObj {
protected:
	FFLocation _location;
	FFLocation _size;
	std::shared_ptr<FFCompLine> _lineComp;
	std::shared_ptr<FFCompText> _textComp;
public:
	FFRect();
	FFRect(const float& x, const float& y, const float& w, const float& h);
	virtual ~FFRect();

	virtual FFLocation getLocation();
	const FFLocation& getLocation() const;
	const FFLocation& getSize() const;

	void setText(const std::string& text);

	// this function will be call each frame
	virtual void draw();
};