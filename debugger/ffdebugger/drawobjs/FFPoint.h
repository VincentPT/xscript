#pragma once
#include "FFDrawObj.h"
#include "FFDrawComponent.h"

class FFPoint : public FFDrawObj {
protected:
	FFLocation _location;
	float _radius;
	FFDrawComponentRef _colorComp;
public:
	FFPoint();
	FFPoint(const float& x, const float& y);
	virtual ~FFPoint();

	virtual FFLocation getLocation();
	const FFLocation& getLocation() const;

	void setRadius(float radius);

	// this function will be call each frame
	virtual void draw();
};