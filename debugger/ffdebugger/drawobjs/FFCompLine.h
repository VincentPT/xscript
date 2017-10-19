#pragma once
#include "FFDrawComponent.h"

class FFCompLine : public FFDrawComponent {
protected:
	float _lineWidth;
public:
	FFCompLine();
	virtual ~FFCompLine();
	void setLineWidth(float lineWidth);
	float getLineWidth() const;

	virtual const char* getName() const;
};

#define LINE_COMPONENT_NAME "FFCompLine"