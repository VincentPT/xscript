/******************************************************************
* File:        DefaultCommands.cpp
* Description: implement default command classes.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "DefaultCommands.h"
#include "Context.h"
#include "ffscript.h"
#include "ScopeRuntimeData.h"
#include "ScriptCompiler.h"
#include "Program.h"
#include "ScriptScope.h"

#include <sstream>

#include <thread>

namespace ffscript {

	DefaultAssigmentCommand::DefaultAssigmentCommand(int returnOffset, int blockSize/*, int offset1, int offset2*/) :
		 /*_returnOffset(returnOffset), _blockSize(blockSize), _offset1(offset1), _offset2(offset2),*/
		TargetedCommand(returnOffset, sizeof(void*)), _blockSize(blockSize),
		_command1(nullptr), _command2(nullptr) {}

	DefaultAssigmentCommand::~DefaultAssigmentCommand() {}

	void DefaultAssigmentCommand::buildCommandText(std::list<std::string>& strCommands) {
		std::stringstream ss;
		_command1->buildCommandText(strCommands);
		_command2->buildCommandText(strCommands);
		ss << "write(|[" << _command1->getTargetOffset() << "]|, " << _blockSize << ", [" << _command2->getTargetOffset() << "])";
		strCommands.emplace_back(ss.str());

		ss.clear();
		ss << "lea ([" << getTargetOffset() << "|[" << _command1->getTargetOffset() << "]|";
		strCommands.emplace_back(ss.str());
	}

	void DefaultAssigmentCommand::execute() {
		_command1->execute();
		_command2->execute();

		Context* context = Context::getCurrent();
		int param2Offset = _command2->getTargetOffset() + context->getCurrentOffset();

		//offset 1 contain an adress of varialble or r-value
		int param1OffsetRef = _command1->getTargetOffset() + context->getCurrentOffset();
		void* param1Adress = context->getAbsoluteAddress(param1OffsetRef);
		void* variableAdress = (void*)(*((size_t*)param1Adress));

		void* source = context->getAbsoluteAddress(param2Offset);

		//copy param 2 to param 1
		memcpy_s(variableAdress, _blockSize, source, _blockSize);

		//copy ref of param 2 to return offset
		context->lea(getTargetOffset() + context->getCurrentOffset(), variableAdress);
	}

	int DefaultAssigmentCommand::pushCommandParam(TargetedCommand* command) {
		if (_command1 == nullptr) {
			_command1 = command;
			return 1;
		}
		if(_command2 == nullptr)
		{
			_command2 = command;
			return 0;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	DefaultAssigmentCommandForSemiRef::DefaultAssigmentCommandForSemiRef(int returnOffset, int blockSize/*, int offset1, int offset2*/) :
		/*_returnOffset(returnOffset), _blockSize(blockSize), _offset1(offset1), _offset2(offset2),*/
		TargetedCommand(returnOffset, sizeof(void*)), _blockSize(blockSize),
		_command1(nullptr), _command2(nullptr) {}

	DefaultAssigmentCommandForSemiRef::~DefaultAssigmentCommandForSemiRef() {}

	void DefaultAssigmentCommandForSemiRef::buildCommandText(std::list<std::string>& strCommands) {
		std::stringstream ss;
		_command1->buildCommandText(strCommands);
		_command2->buildCommandText(strCommands);
		ss << "write(|[" << _command1->getTargetOffset() << "]|, " << _blockSize << ", |[" << _command2->getTargetOffset() << "]|)";

		strCommands.emplace_back(ss.str());
	}

	void DefaultAssigmentCommandForSemiRef::execute() {
		_command1->execute();
		_command2->execute();

		Context* context = Context::getCurrent();
		int param2Offset = _command2->getTargetOffset() + context->getCurrentOffset();
		void* param2Adress = context->getAbsoluteAddress(param2Offset);
		void* source = (void*)(*((size_t*)param2Adress));

		//offset 1 contain an adress of varialble or r-value
		int param1OffsetRef = _command1->getTargetOffset() + context->getCurrentOffset();
		void* param1Adress = context->getAbsoluteAddress(param1OffsetRef);
		void* variableAdress = (void*)(*((size_t*)param1Adress));

		//copy param 2 to param 1
		memcpy_s(variableAdress, _blockSize, source, _blockSize);

		//copy ref of param 2 to return offset
		context->lea(getTargetOffset() + context->getCurrentOffset(), variableAdress);
	}

	int DefaultAssigmentCommandForSemiRef::pushCommandParam(TargetedCommand* command) {
		if (_command1 == nullptr) {
			_command1 = command;
			return 1;
		}
		if (_command2 == nullptr)
		{
			_command2 = command;
			return 0;
		}
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	void* fetchArrayToStruct(void* pStruct, SimpleVariantArray* pArray) {
		char* data = (char*)pStruct;
		SimpleVariant* elm = &(pArray->elems[0]);
		SimpleVariant* elmEnd = elm + pArray->size;

		while (elm < elmEnd)
		{
			if (strcmp("ref " SYSTEM_ARRAY_STRUCT, elm->typeName) == 0) {
				data = (char*)fetchArrayToStruct(data, *((SimpleVariantArray**)elm->pData));
			}
			else {
				memcpy_s(data, elm->size, elm->pData, elm->size);
				data += elm->size;
			}
			elm++;
		}
		return data;
	}

	ArrayToStructCommand::ArrayToStructCommand() {
	}

	void ArrayToStructCommand::call(void* pReturnVal, void* params[]) {
		void* pStruct = params[0];
		SimpleVariantArray* pArray = (SimpleVariantArray*)params[1];
		char* endOffset = (char*)fetchArrayToStruct(pStruct, pArray);
		int structSize = (int)(endOffset - (char*)pStruct);

		memcpy_s(pReturnVal, structSize, pStruct, structSize);
	}

	DFunction2* ArrayToStructCommand::clone() {
		return new ArrayToStructCommand();
	}

	//////////////////////////////////////////////////////////////////////////
	MemberAccessCommand1::MemberAccessCommand1(int returnSize) : _returnSize(returnSize) {
	}

	void MemberAccessCommand1::call(void* pReturnVal, void* params[]) {
		char* pStruct = (char*)params[0];
		int offset = (int)(size_t)params[1];

		//copy value of member to address of return value
		memcpy_s(pReturnVal, _returnSize, (pStruct + offset), _returnSize);
	}

	DFunction2* MemberAccessCommand1::clone() {
		return new MemberAccessCommand1(_returnSize);
	}
	//////////////////////////////////////////////////////////////////////////
	MemberAccessCommand2::MemberAccessCommand2() {
	}

	void MemberAccessCommand2::call(void* pReturnVal, void* params[]) {
		char* pStruct = (char*)params[0];
		int offset = (int)(size_t)params[1];

		//copy address of member to address of return value
		*((size_t*)pReturnVal) = (size_t)(pStruct + offset);
		if (false) {
			memcpy_s(pReturnVal, 4, (pStruct + offset), 4);
		}
	}

	DFunction2* MemberAccessCommand2::clone() {
		return new MemberAccessCommand2();
	}

	//////////////////////////////////////////////////////////////////////////
	ElementAccessCommand1::ElementAccessCommand1(int elementSize) : _elementSize(elementSize) {
	}

	void ElementAccessCommand1::call(void* pReturnVal, void* params[]) {
		char* root = (char*)params[0];
		int index = (int)(size_t)params[1];

		//copy value of member to address of return value
		memcpy_s(pReturnVal, _elementSize, (root + index*_elementSize), _elementSize);
	}

	DFunction2* ElementAccessCommand1::clone() {
		return new ElementAccessCommand1(_elementSize);
	}
	//////////////////////////////////////////////////////////////////////////
	ElementAccessCommand2::ElementAccessCommand2(int elementSize) : _elementSize(elementSize) {
	}

	void ElementAccessCommand2::call(void* pReturnVal, void* params[]) {
		char* root = (char*)params[0];
		int index = (int)(size_t)params[1];

		//copy address of member to address of return value
		*((size_t*)pReturnVal) = (size_t)(root + index*_elementSize);
	}

	DFunction2* ElementAccessCommand2::clone() {
		return new ElementAccessCommand2(_elementSize);
	}
	//////////////////////////////////////////////////////////////////////////
	ElementAccessCommand4::ElementAccessCommand4(int elementSize) : _elementSize(elementSize) {
	}

	void ElementAccessCommand4::call(void* pReturnVal, void* params[]) {
		void** pRoot = (void**)params[0];
		char* root = (char*)(*pRoot);
		int index = (int)(size_t)params[1];

		//copy address of member to address of return value
		*((size_t*)pReturnVal) = (size_t)(root + index*_elementSize);
	}

	DFunction2* ElementAccessCommand4::clone() {
		return new ElementAccessCommand4(_elementSize);
	}
	//////////////////////////////////////////////////////////////////////////
	DeRefCommand::DeRefCommand(int typeSize) : _typeSize(typeSize) {
	}

	void DeRefCommand::call(void* pReturnVal, void* params[]) {
		char* pVal = (char*)params[0];

		//copy data to address of return value
		memcpy_s(pReturnVal, _typeSize, pVal, _typeSize);
	}

	DFunction2* DeRefCommand::clone() {
		return new DeRefCommand(_typeSize);
	}

	//////////////////////////////////////////////////////////////////////////
	DeRefCommand2::DeRefCommand2(int typeSize) : _typeSize(typeSize) {
	}

	void DeRefCommand2::call(void* pReturnVal, void* params[]) {
		void** ppVal = (void**)params[0];
		auto pVal = *ppVal;

		//copy data to address of return value
		memcpy_s(pReturnVal, _typeSize, pVal, _typeSize);
	}

	DFunction2* DeRefCommand2::clone() {
		return new DeRefCommand2(_typeSize);
	}

	//////////////////////////////////////////////////////////////////////////
	AssignFunctionToNull::AssignFunctionToNull() {
	}

	void AssignFunctionToNull::call(void* pReturnVal, void* params[]) {
		RuntimeFunctionInfo* pVal = (RuntimeFunctionInfo*)params[0];
		runtimeFunctionInfoDestructor(pVal);

		memset(pVal, 0, sizeof(RuntimeFunctionInfo));

		//copy data to address of return value
		memcpy_s(pReturnVal, sizeof(RuntimeFunctionInfo), pVal, sizeof(RuntimeFunctionInfo));
	}

	DFunction2* AssignFunctionToNull::clone() {
		return new AssignFunctionToNull();
	}

	//////////////////////////////////////////////////////////////////////////
	CompareRuntimeFunction::CompareRuntimeFunction() {
	}

	void CompareRuntimeFunction::call(void* pReturnVal, void* params[]) {
		RuntimeFunctionInfo* pVal1 = (RuntimeFunctionInfo*)params[0];
		RuntimeFunctionInfo* pVal2 = (RuntimeFunctionInfo*)params[1];
		
		bool& res = *((bool*)pReturnVal);
		if(pVal1 == pVal2) {
			res = true;
		}
		else if (pVal1 == nullptr || pVal2 == nullptr) {
			res = false;
		}
		else
		{
			res = pVal1->address == pVal2->address && pVal1->info.type == pVal2->info.type;
		}
	}

	DFunction2* CompareRuntimeFunction::clone() {
		return new CompareRuntimeFunction();
	}

	/////////////////////////////////////////////////////////////////////////
	// trigger functions for constructor and destructor
	//
	/////////////////////////////////////////////////////////////////////////
	BeforeConstructorCall::BeforeConstructorCall(
		InstructionCommand* pushObjectToConstructorParamCommand, unsigned int objectOffsetRef) :
		_constructObjectOffsetRef(objectOffsetRef),
		_pushObjectToConstructorParamCommand(pushObjectToConstructorParamCommand),
		_currentScopeCodeSize(0)
	{		
	}

	BeforeConstructorCall::~BeforeConstructorCall() {
		if (_pushObjectToConstructorParamCommand) {
			delete _pushObjectToConstructorParamCommand;
		}

		for (auto it = _constructorItems.begin(); it != _constructorItems.end(); it++) {
			delete (*it);
		}

		_constructorItems.clear();
		_itemOffsets.clear();
	}

	//DFunction* BeforeConstructorCall::clone() {
	//	return new BeforeConstructorCall(_pushObjectToConstructorParamCommand, _constructObjectOffsetRef);
	//}

	void BeforeConstructorCall::buildOperator(ScriptCompiler* scriptCompiler, ScriptScope* currentScope, const std::list<OperatorBuidItemInfo> &operatorInfoList) {
		int maxReturnSize = 0;
		int returnSize;
		int maxParamSize = sizeof(void*);

		//this function only avaible for a program of script, not for single expression
		auto program = scriptCompiler->getProgram();
		auto& constructorItems = _constructorItems;

		_currentScopeCodeSize = currentScope->getScopeSize() - currentScope->getDataSize();
		int beginOffset = currentScope->getBaseOffset() + currentScope->getScopeSize();

		EnterContextScope* enterOperatorContext = new EnterContextScope();
		constructorItems.push_back(enterOperatorContext);

		InstructionCommand* itemConstructor;
		for (auto it = operatorInfoList.begin(); it != operatorInfoList.end(); it++) {
			const OperatorBuidItemInfo& buildItemInfo = *it;			

			Function* operatorFunction = scriptCompiler->createFunctionFromId(buildItemInfo.functionId);
			returnSize = scriptCompiler->getTypeSize(operatorFunction->getReturnType());

			if (returnSize > maxReturnSize) {
				maxReturnSize = returnSize;
			}

			NativeFunction* operatorNativeFunction = dynamic_cast<NativeFunction*>(operatorFunction);
			if (operatorNativeFunction) {
				auto runNativeFuncFunc = new CallNativeFuntion();
				runNativeFuncFunc->setCommandData(beginOffset + maxParamSize, beginOffset, operatorNativeFunction->getNative());

				itemConstructor = runNativeFuncFunc;
			}
			else {
				CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(buildItemInfo.functionId);

				auto callScriptFunctionFunc = new CallScriptFuntion3();
				callScriptFunctionFunc->setCommandData(beginOffset + maxParamSize, beginOffset, returnSize);
				callScriptFunctionFunc->setFunctionName(operatorFunction->toString());
				callScriptFunctionFunc->setTargetCommand(pFunctionCode->first);

				itemConstructor = callScriptFunctionFunc;
			}

			constructorItems.push_back(itemConstructor);
			_itemOffsets.push_back(buildItemInfo.itemOffset);
		}

		enterOperatorContext->setScopeInfo(0, maxReturnSize + maxParamSize, 0);

		ExitContextScope* exitOperatorConext = new ExitContextScope();
		exitOperatorConext->setScopeInfo(0, maxReturnSize + maxParamSize);
		constructorItems.push_back(exitOperatorConext);
	}

	void BeforeConstructorCall::call() {
		auto context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		//this command will move address of object to param space of constructor operator
		if(_pushObjectToConstructorParamCommand) _pushObjectToConstructorParamCommand->execute();

		//now we can read address of object from param offset
		size_t objectAddess;
		context->read(currentOffset + _constructObjectOffsetRef, &objectAddess, sizeof(objectAddess));
		//context->getAbsoluteAddress()

		//the constructor items will be run in a new scope
		//before do that we must copy address of object to the begining offset of the new scope
		//the constructor item will retreive address of object from the beginning offset of new scope

#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		//incase REDUCE_SCOPE_ALLOCATING_MEM is defined, we still need allocate code size of current context
		//because one command of current scope is excecuted after this function
		//and if we don't do that, some constructor script function will owverite its data to param space of constructor operator
		context->scopeAllocate(_currentScopeCodeSize, 0);
#endif
		//calculate begining offset of new scope
		int beginningOffsetOfNewScope = currentOffset + context->getCurrentScopeSize();
		//check if the stack is overflow or not
		context->prepareWrite(beginningOffsetOfNewScope, sizeof(objectAddess));
		size_t* pItemAddress = (size_t*)context->getAbsoluteAddress(beginningOffsetOfNewScope);

		//this will mark the back point is current command
		//and current command is null
		//the current command is null is not a problem, because we are executing in a local function
		//not in context thread
		//and the back point will be use later, when constructor scope is end
		context->jump(nullptr);

		auto it = _constructorItems.begin();

		//execute enter constructor's scope
		(*it)->execute();

		auto constructorEnd = _constructorItems.end();
		constructorEnd--;

		auto itOffset = _itemOffsets.begin();

		for (it++; it != constructorEnd; it++, itOffset++) {
			*pItemAddress = (objectAddess + *itOffset);
			(*it)->execute();
		}

		//execute exit constructor's scope
		(*it)->execute();

#ifdef REDUCE_SCOPE_ALLOCATING_MEM
		context->scopeUnallocate(_currentScopeCodeSize, 0);
#endif
	}

	/////////////////////////////////////////
	void dummyOperator(void*) {}

	void afterCallConstructor(int contructorIndex) {
		Context* context = Context::getCurrent();
		auto scopeRuntimeData = context->getScopeRuntimeData();

		scopeRuntimeData->markContructorExecuted(contructorIndex);
	}

	unsigned char beforeCallDestructor(int contructorIndex) {
		Context* context = Context::getCurrent();
		auto scopeRuntimeData = context->getScopeRuntimeData();

		return scopeRuntimeData->isContructorExecuted(contructorIndex);
	}

	void defaultRuntimeFunctionInfoConstructor(RuntimeFunctionInfo* obj) {
		memset(obj, 0, sizeof(RuntimeFunctionInfo));
	}

	void runtimeFunctionInfoCopyConstructor(RuntimeFunctionInfo* obj1, RuntimeFunctionInfo* obj2) {
		memcpy_s(obj1, sizeof(RuntimeFunctionInfo), obj2, sizeof(RuntimeFunctionInfo));
		auto& anoynymousInfo = obj2->anoynymousInfo;
		if (anoynymousInfo.data && anoynymousInfo.dataSize) {
			obj1->anoynymousInfo.data = malloc(anoynymousInfo.dataSize);
			memcpy_s(obj1->anoynymousInfo.data, anoynymousInfo.dataSize, anoynymousInfo.data, anoynymousInfo.dataSize);
		}
	}

	void runtimeFunctionInfoConstructByNull(RuntimeFunctionInfo* obj1, void*) {
		memset(obj1, 0, sizeof(RuntimeFunctionInfo));
	}

	void runtimeFunctionInfoDestructor(RuntimeFunctionInfo* obj) {
		if (obj->anoynymousInfo.data) {
			free(obj->anoynymousInfo.data);
			obj->anoynymousInfo.data = nullptr;
		}
	}

	/////////////////////////////////////////
	CreateThreadCommand::CreateThreadCommand() : CreateThreadCommand(0,0) {}
	CreateThreadCommand::CreateThreadCommand(int returnSize, int paramSize) : _returnSize(returnSize), _paramSize(paramSize) {}

	void CreateThreadCommand::setCommandData(int returnSize, int paramSize) {
		_returnSize = returnSize;
		_paramSize = paramSize;
	}

	void CreateThreadCommand::call(void* pReturnVal, void* param[]) {
		RuntimeFunctionInfo* runtimeInfo = (RuntimeFunctionInfo*)param[0];
		void* functionParam = (void*)(&param[1]);
		std::thread* pThread = new std::thread([this, runtimeInfo, functionParam]() {
			Context context(1024*1024);

			int paramSize = _paramSize;
			int returnOffset = SCRIPT_FUNCTION_RETURN_STORAGE_OFFSET;
			int paramOffset = returnOffset + _returnSize;			

			if (paramSize > 0) {
				context.write(paramOffset, functionParam, paramSize);
			}

			if (runtimeInfo->info.type == RuntimeFunctionType::NativeFunction) {
				CallNativeFuntion callNativeFunction;
				//ref without delete the instance
				DFunction2Ref refFunction((DFunction2*)runtimeInfo->address, [](DFunction2*) {});
				callNativeFunction.setCommandData(returnOffset, paramOffset, refFunction);
				callNativeFunction.execute();
			}
			else {
				CommandPointer targetCommand = (CommandPointer)runtimeInfo->address;
				context.setCurrentCommand(targetCommand);
				context.setEndCommand(nullptr);

				int allocatedSize = _returnSize + paramSize;
				context.scopeAllocate(allocatedSize, 0);

				if (runtimeInfo->anoynymousInfo.data == nullptr || runtimeInfo->anoynymousInfo.dataSize == 0) {
					CallScriptFuntion3 callScriptFunction;
					callScriptFunction.setTargetCommand(targetCommand);
					callScriptFunction.setCommandData(returnOffset, paramOffset, paramSize);
					callScriptFunction.execute();
				}
				else {
					CallLambdaFuntion callLambdaFunction(&runtimeInfo->anoynymousInfo);
					callLambdaFunction.setTargetCommand(targetCommand);
					callLambdaFunction.setCommandData(returnOffset, paramOffset, paramSize);
					callLambdaFunction.execute();
				}

				context.scopeUnallocate(allocatedSize, 0);
			}
		});

		*(THREAD_HANDLE*)pReturnVal = pThread;
	}

	DFunction2* CreateThreadCommand::clone() {
		return new CreateThreadCommand(_returnSize, _paramSize);
	}

	void joinThread(THREAD_HANDLE handle) {
		std::thread* pThread = (std::thread*)handle;
		if (pThread->joinable()) {
			pThread->join();
		}
	}

	void closeThread(THREAD_HANDLE handle) {
		std::thread* pThread = (std::thread*)handle;
		if (pThread->joinable()) {
			pThread->join();
		}
		delete pThread;
	}

	///
	/// access to an element in static array
	///
	ElementAccessCommand3::ElementAccessCommand3(int arrayOffset, int returnOffset, int elmSize, bool isAddress) :
		_arrayOffset(arrayOffset),
		_elmSize(elmSize),
		_command1(nullptr),
		_command2(nullptr),
		_isAddress(isAddress),
		TargetedCommand(returnOffset, sizeof(void*))
		{}

	ElementAccessCommand3::~ElementAccessCommand3() {
		if (_command1) {
			delete _command1;
		}
		if (_command2) {
			delete _command2;
		}
	}

	void ElementAccessCommand3::buildCommandText(std::list<std::string>& strCommands) {

	}

	void ElementAccessCommand3::execute() {
		auto context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		if (_command1) {
			_command1->execute();
		}
		_command2->execute();
		int indexOffset = currentOffset + _command2->getTargetOffset();
		char* returnAdress;
		int index;
		context->read(indexOffset, &index, sizeof(index));

		// check if array offset is contain a address
		if (_isAddress) {
			// read address of array
			context->read(_arrayOffset + currentOffset, &returnAdress, sizeof(void*));
		}
		else {
			returnAdress = (char*)context->getAbsoluteAddress(_arrayOffset + currentOffset);
		}

		// copy adress of element into return offset
		context->lea(getTargetOffset() + currentOffset, (returnAdress + index*_elmSize));
	}

	void ElementAccessCommand3::setCommand1(TargetedCommand* command) {
		_command1 = command;
	}

	void ElementAccessCommand3::setCommand2(TargetedCommand* command) {
		_command2 = command;
	}

	///
	///
	///
	ElementAccessForGlobalCommand::ElementAccessForGlobalCommand(void* arrayData, int returnOffset, int elmSize) :
		_arrayData(arrayData),
		_elmSize(elmSize),
		TargetedCommand(returnOffset, sizeof(void*)) {

	}
	ElementAccessForGlobalCommand::~ElementAccessForGlobalCommand() {
		if (_indexCommand) {
			delete _indexCommand;
		}
	}
	void ElementAccessForGlobalCommand::buildCommandText(std::list<std::string>& strCommands) {}

	void ElementAccessForGlobalCommand::execute() {
		auto context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		_indexCommand->execute();
		int indexOffset = currentOffset + _indexCommand->getTargetOffset();
		char* returnAdress = (char*)_arrayData;
		int index;
		context->read(indexOffset, &index, sizeof(index));

		// copy adress of element into return offset
		context->lea(getTargetOffset() + currentOffset, (returnAdress + index*_elmSize));
	}

	void ElementAccessForGlobalCommand::setIndexCommand(TargetedCommand* command) {
		_indexCommand = command;
	}
}