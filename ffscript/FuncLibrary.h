/******************************************************************
* File:        FuncLibrary.h
* Description: declare FuncLibrary class. A class used to store and
*              managed registered function of a script program.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "FactoryTree.h"
#include "MemoryBlock.h"
#include "ScriptType.h"
#include <vector>
#include <list>
#include <map>
#include <string>

namespace ffscript {
	class ScriptCompiler;

	class FuncLibrary
	{
		std::list<MemoryBlockRef> _memoryBlocks;
		std::map<std::string, std::list<OverLoadingItem>> _functionsMap; /*map function name to list of overloading functions*/
		std::map<std::string, int> _dynamicFunctionMap; /* map for dynamic funtions, are functions can accept what ever parameter count, map function name to function factory id*/
		std::list<OverLoadingItem> _overLoadingContainer;
		std::map<int, OverLoadingItem*> _overloadingIdMap;
		struct LibraryMarkInfo {
			decltype(_dynamicFunctionMap.begin()) dynamicFuncIter;
			decltype(_functionsMap.begin()) funcFactoryIter;
		};
		typedef std::shared_ptr<LibraryMarkInfo> LibraryMarkInfoRef;
		LibraryMarkInfoRef _systemLibMarkEnd;

	public:
		FuncLibrary();
		~FuncLibrary();
		const std::list<OverLoadingItem>* findOverloadingFuncRoot(const std::string& name) const;
		int findFunction(ScriptCompiler* scriptCompiler, const std::string& name, const std::string& sargs);
		int findDynamicFunctionOnly(const std::string& name);
		bool mapFunction(const std::string& name, const std::vector<ScriptType>& paramTypes, int functionId);
		bool mapDynamicFunction(const std::string& name, int functionId);
		void unmapFunction(const std::string& name, int functionId);		
		void beginUserLib();
		void clearUserLib();
		//const std::string& searchFunctionName(int functionId) const;
		const OverLoadingItem* findFunctionInfo(int functionId);
		size_t getFunctionCount() const;
	};

	typedef std::shared_ptr<FuncLibrary> FuncLibraryRef;
}