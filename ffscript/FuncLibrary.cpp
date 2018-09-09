/******************************************************************
* File:        FuncLibrary.cpp
* Description: implement FuncLibrary class. A class used to store and
*              managed registered function of a script program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "FuncLibrary.h"
#include <algorithm>
#include "ScriptCompiler.h"

using namespace std;
namespace ffscript {

	extern bool parseArgumentTypes(ScriptCompiler* scriptCompiler, const std::string& sargs, vector<ScriptType>& args);
	
	FuncLibrary::FuncLibrary()
	{
	}

	FuncLibrary::~FuncLibrary()
	{
	}

	auto findOverloadingItem(const list<OverLoadingItem>& overloadingItems, const std::vector<ScriptType>& in_args) {
		for (auto iter = overloadingItems.begin(); iter != overloadingItems.end(); ++iter) {
			auto& args = (*iter).paramTypes;
			if (args.size() == in_args.size()) {
				auto typeit = in_args.begin();
				for (auto ptype = args.begin(); typeit != in_args.end(); ++typeit) {
					if (*(*ptype) != *typeit) {
						break;
					}
					ptype++;
				}

				if (typeit == in_args.end()) {
					return iter;
				}
			}
		}
		return overloadingItems.end();
	}


	int FuncLibrary::findFunction(ScriptCompiler* scriptCompiler, const std::string& name, const std::string& sargs) {
		auto overloadingItems = findOverloadingFuncRoot(name);
		if (overloadingItems == nullptr) {
			return -1;
		}
		std::vector<ScriptType> args;
		if (!parseArgumentTypes(scriptCompiler, sargs, args)) {
			return -1;
		}
		auto fit = findOverloadingItem(*overloadingItems, args);
		if (fit != overloadingItems->end()) {
			return fit->functionId;
		}

		auto dfit = _dynamicFunctionMap.find(name);
		if (dfit != _dynamicFunctionMap.end()) {
			return dfit->second;
		}

		return -1;
	}

	bool FuncLibrary::mapFunction(const std::string& name, const std::vector<ScriptType>& paramTypes, int functionId) {
		list<OverLoadingItem> overloadingTemp;
		list<OverLoadingItem>* pOverloadingFuncs;
		auto it = _functionsMap.insert(std::make_pair(name, overloadingTemp));

		pOverloadingFuncs = &(it.first->second);

		//check if the function with same argument is exist
		auto iit = findOverloadingItem(*pOverloadingFuncs, paramTypes);
		if (iit != pOverloadingFuncs->end()) {
			return false;
		}

		OverLoadingItem factoryItemTmp;
		pOverloadingFuncs->push_back(factoryItemTmp);
		OverLoadingItem& factoryItem = pOverloadingFuncs->back();
		factoryItem.functionId = functionId;
		factoryItem.itemName = &(it.first->first);
		factoryItem.mask = 0;
		//copy argument types
		auto& args = factoryItem.paramTypes;
		args.resize(paramTypes.size());
		auto pit = paramTypes.begin();
		for (auto ait = args.begin(); ait != args.end(); ait++, pit++) {
			*ait = std::make_shared<ScriptType>(*pit);
		}
		_overloadingIdMap[functionId] = &factoryItem;

		return true;
	}

	bool FuncLibrary::mapDynamicFunction(const std::string& name, int functionId) {
		auto itres = _dynamicFunctionMap.insert(std::make_pair(name, functionId));
		if (!itres.second) {
			//a function has same name is already exist
			return false;
		}
		OverLoadingItem factoryItem;
		factoryItem.functionId = functionId;
		//factoryItem.paramCount = -1; //-1 is the mark of dynamic functions
		factoryItem.itemName = &(itres.first->first);
		//factoryItem.paramTypes = nullptr;
		factoryItem.mask = ITEM_MASK_DYNAMIC_FUNCTION;

		_overLoadingContainer.push_back(factoryItem);
		_overloadingIdMap[functionId] = &_overLoadingContainer.back();
		return true;
	}

	void FuncLibrary::unmapFunction(const std::string& name, int functionId) {
		list<OverLoadingItem>* pOverloadingFuncs;
		auto it = _functionsMap.find(name);
		if (it == _functionsMap.end()) {
			return;
		}

		pOverloadingFuncs = &it->second;
		int functionIdTemp = functionId;
		void* allocatedMem = nullptr;
		auto rmIt = std::remove_if(pOverloadingFuncs->begin(), pOverloadingFuncs->end(), [functionIdTemp, &allocatedMem](const OverLoadingItem& item) ->bool {
			return item.functionId == functionIdTemp;
		});
		pOverloadingFuncs->erase(rmIt, pOverloadingFuncs->end());
	}

	const list<OverLoadingItem>* FuncLibrary::findOverloadingFuncRoot(const std::string& name) const {
		auto it = _functionsMap.find(name);
		if (it == _functionsMap.end()) {
			return nullptr;
		}
		return &(it->second);
	}

	int FuncLibrary::findDynamicFunctionOnly(const std::string& name) {
		auto it = _dynamicFunctionMap.find(name);
		if (it != _dynamicFunctionMap.end()) {
			return it->second;
		}

		return -1;
	}

	void FuncLibrary::beginUserLib() {
		_systemLibMarkEnd = (LibraryMarkInfoRef)(new LibraryMarkInfo);
		_systemLibMarkEnd->dynamicFuncIter = _dynamicFunctionMap.end();
		_systemLibMarkEnd->funcFactoryIter = _functionsMap.end();
	}

	void FuncLibrary::clearUserLib() {
		if (_systemLibMarkEnd) {
			_dynamicFunctionMap.erase(_systemLibMarkEnd->dynamicFuncIter, _dynamicFunctionMap.end());
			_functionsMap.erase(_systemLibMarkEnd->funcFactoryIter, _functionsMap.end());
			_systemLibMarkEnd.reset();
		}
	}

	//const string& FuncLibrary::searchFunctionName(int functionId) const {
	//	for (auto functionGroup : _functionsMap) {
	//		for (const auto& item : functionGroup.second) {
	//			if (item.functionId == functionId) {
	//				return functionGroup.first;
	//			}
	//		}
	//	}
	//	for (auto dynamicFunctionIt : _dynamicFunctionMap) {
	//		if (dynamicFunctionIt.second == functionId) {
	//			return dynamicFunctionIt.first;
	//		}
	//	}

	//	static string emptyString;
	//	return emptyString;
	//}

	const OverLoadingItem* FuncLibrary::findFunctionInfo(int functionId) {
		auto it = _overloadingIdMap.find(functionId);
		if (it != _overloadingIdMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	size_t FuncLibrary::getFunctionCount() const {
		return _overloadingIdMap.size();
	}
}