#pragma once
#include "FFDrawObj.h"
#include <list>

class FFContainer : public FFDrawObj {
protected:
	std::list<FFDrawObjRef> _children;
public:
	FFContainer();
	virtual ~FFContainer();

	virtual FFLocation getLocation();
	virtual int addObject(FFDrawObjRef buzzDrawObjRef);
	const std::list<FFDrawObjRef>& getChildren() const;

	// this function will be call each frame
	virtual void draw();
};