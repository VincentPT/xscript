#pragma once
#include "FFDrawObj.h"
#include <vector>
#include "FFCompLine.h"

class FFPolygon : public FFDrawObj {
protected:
	std::vector<FFLocation> _points;
	std::shared_ptr<FFCompLine> _lineComp;
public:
	FFPolygon();
	virtual ~FFPolygon();

	virtual FFLocation getLocation();
	virtual int addPoint(const float&x, const float&y);
	virtual int getPointCount() const;

	// this function will be call each frame
	virtual void draw();
};