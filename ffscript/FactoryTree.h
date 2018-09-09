/******************************************************************
* File:        FactoryTree.h
* Description: declare FactoryTree class. A class is designed to
*              store function factory use the tree structure.
*              Now it is no longer used.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include <map>
#include <functional>
#include <memory>
#include <vector>
#include <string>
#include "ExpresionParser.h"

namespace ffscript {

	class FactoryTree {
		int _functionId;
		std::map<int, FactoryTree*> _branches;
	public:
		typedef std::function<bool(FactoryTree*)> FindNodeFunc;
	public:
		FactoryTree();
		~FactoryTree();

		FactoryTree* insertBranch(int paramType, FactoryTree*);
		FactoryTree* getBranch(int paramType) const;
		const std::map<int, FactoryTree*>::const_iterator getBranchFistBranch() const;
		int getBranchCount() const;
		int getFunctionId() const;
		void setFactoryId(int factoryId);
		FactoryTree* findNode(const FindNodeFunc& findFunc);
	};
}