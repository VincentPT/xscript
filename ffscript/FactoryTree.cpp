/******************************************************************
* File:        FactoryTree.cpp
* Description: implement FactoryTree class. A class is designed to
*              store function factory use the tree structure.
*              Now it is no longer used.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "FactoryTree.h"


namespace ffscript {
	FactoryTree::FactoryTree() :_functionId(-1) {}

	FactoryTree::~FactoryTree() {
		for (auto it = _branches.begin(); it != _branches.end(); ++it) {
			delete it->second;
		}
	}

	int FactoryTree::getFunctionId() const {
		return _functionId;
	}

	void FactoryTree::setFactoryId(int factoryId) {
		_functionId = factoryId;
	}

	FactoryTree* FactoryTree::insertBranch(int paramType, FactoryTree* branch) {
		auto it = _branches.insert(std::make_pair(paramType, branch));
		return it.first->second;
	}

	FactoryTree* FactoryTree::getBranch(int paramType) const {
		auto it = _branches.find(paramType);
		if (it == _branches.end()) {
			return nullptr;
		}
		return it->second;
	}

	const std::map<int, FactoryTree*>::const_iterator FactoryTree::getBranchFistBranch() const {
		return _branches.begin();
	}

	int FactoryTree::getBranchCount() const {
		return (int)_branches.size();
	}

	FactoryTree* FactoryTree::findNode(const FindNodeFunc& findFunc) {
		if (findFunc(this)) {
			return this;
		}

		FactoryTree* factoryNode = nullptr;
		for (auto it = _branches.begin(); it != _branches.end(); ++it) {
			factoryNode = it->second;
			if (factoryNode = factoryNode->findNode(findFunc)) {
				return factoryNode;
			}
		}
		return nullptr;
	}
}