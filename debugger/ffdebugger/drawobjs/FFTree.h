#pragma once
#include "FFRect.h"
#include <list>

class FFTree;

typedef std::shared_ptr<FFTree> FFTreeRef;

class FFTree : public FFRect {
protected:
	std::list<FFTreeRef> _children;
public:
	FFTree();
	virtual ~FFTree();

	int addSubNode(const FFTreeRef& node);
	const std::list<FFTreeRef>& getSubNodes() const;
	virtual void draw();

	void arrangeNodes(const FFLocation& startLocation, float colSpcacing, float rowSpacing);
};