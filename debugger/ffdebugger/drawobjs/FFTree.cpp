#include "FFTree.h"

#include <cinder\gl\gl.h>

using namespace ci;

FFTree::FFTree() {

}

FFTree::~FFTree() {

}

int FFTree::addSubNode(const FFTreeRef& buzzDrawObjRef) {
	_children.push_back(buzzDrawObjRef);
	return (int)_children.size();
}

// this function will be call each frame
void FFTree::draw() {
	if (isVisible() == false) {
		return;
	}
	for (FFTreeRef& obj : _children) {
		obj->draw();
	}
}

const std::list<FFTreeRef>& FFTree::getSubNodes() const {
	return _children;
}

void FFTree::arrangeNodes(const FFLocation& startLocation, float colSpcacing, float rowSpacing) {

}

int measureTreeSpaces(FFTree* tree, int& cols) {
	cols++;

	int rows = 1;

	auto& subNodes = tree->getSubNodes();
	if (subNodes.size()) {
		int rowInBranch, maxRowInBranch;

		maxRowInBranch = 0;
		for (const FFTreeRef& obj : subNodes) {
			rowInBranch = measureTreeSpaces(obj.get(), cols);
			if (maxRowInBranch < rowInBranch) {
				maxRowInBranch = rowInBranch;
			}
		}

		rows += maxRowInBranch;
	}

	return rows;
}