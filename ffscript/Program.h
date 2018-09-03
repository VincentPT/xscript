/******************************************************************
* File:        Program.h
* Description: declare Program class. A class that manage the
*              compiled code(command) of a program.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include <list>
#include <memory>
#include "FunctionRegisterHelper.h"
#include <map>
#include <string.h>
#include "Executor.h"
#include "FuncLibrary.h"

namespace ffscript {

	class Executor;

	struct FunctionInfo {
		unsigned short returnStorageSize;
		unsigned short paramDataSize;		
	};

	class Program
	{
		std::list<std::shared_ptr<Executor>> _commandContainer;
		std::map<Executor*, CodeSegmentEntry> _expCmdMap;
		std::map<int, CodeSegmentEntry> _functionMap;
		std::map<int, FunctionInfo> _functionInfoMap;
		//FuncLibraryRef _assitantFuncLib;

		CommandPointer _programCode;
		int _commandCounter;
		//static Program* g_instance;
	public:
		Program();
		virtual ~Program();

		void addExecutor(const ExecutorRef& executor);
		//int findFunction(const std::string& name, const std::vector<int>& paramTypes);
		//int mapFunction(const std::string& name, const std::vector<ScriptType>& paramTypes, int functionId);
		//int mapDynamicFunction(const std::string& name, int functionId);
		//const OverLoadingItem* findFunctionInfo2(int functionId);

		//this method must be called after all executors is and before the other methods
		void convertToPlainCode();
		CommandPointer getFirstCommand() const;
		CommandPointer getEndCommand() const;

		CodeSegmentEntry* getCode(Executor* pExcutor);

		CodeSegmentEntry* getFunctionPlainCode(int functionId);
		void setFunctionPlainCode(int functionId, const CodeSegmentEntry& functionCode);

		FunctionInfo* getFunctionInfo(int functionId);
		void setFunctionInfo(int functionId, const FunctionInfo& functionInfo);
	};
}
