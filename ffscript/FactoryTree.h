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