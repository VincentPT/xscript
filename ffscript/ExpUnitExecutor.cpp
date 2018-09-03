/******************************************************************
* File:        ExpUnitExecutor.cpp
* Description: implement ExpUnitExecutor class, part 1.
*              A class contains the
*              commands generated from an expression.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "function\CdeclFunction.hpp"
#include "function\DynamicFunction2.h"
#include "function\DynamicFunction.h"
#include "ExpUnitExecutor.h"
#include "ScriptScope.h"
#include "Variable.h"
#include "Expression.h"
#include "Context.h"
#include "GlobalScope.h"
#include "ScriptCompiler.h"
#include "BasicType.h"
#include "RefFunction.h"
#include "ScriptFunction.h"
#include "CodeUpdater.h"
#include "ObjectBlock.hpp"
#include "InstructionCommand.h"

namespace ffscript {	
	ExpUnitExecutor::ExpUnitExecutor(ScriptScope* scope) :
		_currentScope(scope),
		_returnOffset(0),
		_localOffset(0),
		_localSize(0){
	}

	ExpUnitExecutor::~ExpUnitExecutor() {
	}

	int ExpUnitExecutor::getCurrentLocalOffset() const {
		return _localOffset + getLocalSize();
	}

	int ExpUnitExecutor::getReturnOffset() const {
		return _returnOffset;
	}

	void ExpUnitExecutor::moveLocalOffset(int size) {
		_localSize += size;
	}

	int ExpUnitExecutor::getLocalSize() const {
		return _localSize;
	}

	void ExpUnitExecutor::resetLocalOffset() {
		ScriptScope* scriptScope = getScope();
		if (scriptScope) {
			_localOffset = scriptScope->getBaseOffset() + scriptScope->getDataSize();
		}
	}

	ScriptScope* ExpUnitExecutor::getScope() const {
		return _currentScope;
	}

	int ExpUnitExecutor::getOffset(ExecutableUnit* node) const {
		auto it = _unitOffsetMap.find(node);
		if (it == _unitOffsetMap.end()) {
			return -1;
		}
		return it->second;
	}

	RuntimeFunctionInfo* ExpUnitExecutor::buildRuntimeInfoForConstant(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& constantUnit) {
		void* constantValue = (void*)constantUnit->Execute();
		auto memoryBlock = new ObjectBlock<RuntimeFunctionInfo>(*((RuntimeFunctionInfo*)constantValue));

		RuntimeFunctionInfo* usedRuntimeInfoObject = (RuntimeFunctionInfo*)memoryBlock->getDataRef();
		int functionId = (int)(size_t)usedRuntimeInfoObject->address;
		if (usedRuntimeInfoObject->info.type == RuntimeFunctionType::ScriptFunction) {

			auto updateLaterMan = CodeUpdater::getInstance(this->getScope());
			Program* program = scriptCompiler->getProgram();

			auto updateScriptFunctionFunc = new CdeclFunction<void, Program*, RuntimeFunctionInfo*, int>(CodeUpdater::updateScriptFunctionObject);
			updateScriptFunctionFunc->pushParam(program);
			updateScriptFunctionFunc->pushParam((void*)usedRuntimeInfoObject);
			updateScriptFunctionFunc->pushParam((void*)(size_t)functionId);

			updateLaterMan->addUpdateLaterTask(updateScriptFunctionFunc);
		}
		else if (usedRuntimeInfoObject->info.type == RuntimeFunctionType::NativeFunction) {
			auto nativeFunction = (NativeFunction*)scriptCompiler->createFunctionFromId(functionId);
			auto& nativeRef = nativeFunction->getNative();
			auto nativeRefContainer = new ObjectBlock<DFunction2Ref>(nativeRef);
			_memoryBlocks.push_back(MemoryBlockRef(nativeRefContainer));
			usedRuntimeInfoObject->address = nativeRef.get();
			delete nativeFunction;
		}
		_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));
		return usedRuntimeInfoObject;
	}

	TargetedCommand* ExpUnitExecutor::extractCodeForOperandRef(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset) {
		int currentOffset = getCurrentLocalOffset();
		int beginParamOffset = currentOffset;
		TargetedCommand* assitFunction;

		if (node->getType() == EXP_UNIT_ID_XOPERAND) {
			Variable* pVariable = ((CXOperand*)node.get())->getVariable();
			ScriptScope* ownerScope = pVariable->getScope();

			GlobalScope* globalScope = dynamic_cast<GlobalScope*>(ownerScope);
			MemberVariable* pMemberVariable = dynamic_cast<MemberVariable*>(pVariable);
			if (pMemberVariable == nullptr) {
				if (globalScope) {
					void* paramData = globalScope->getGlobalAddress(pVariable->getOffset());
					auto pushParamRefFunc = new PushParamRef();
					pushParamRefFunc->setCommandData(paramData, returnOffset);

					assitFunction = pushParamRefFunc;
				}
				else {
					auto pushParamRefFunc = new PushParamRefOffset();
					pushParamRefFunc->setCommandData(pVariable->getOffset(), returnOffset);

					assitFunction = pushParamRefFunc;
				}
			}
			else {
				auto accessors = extractParamForMemberVariableOperand(pMemberVariable);
				auto pushParamRefFunc = new PushMemberVariableParamRef();
				pushParamRefFunc->setCommandData(accessors, returnOffset);

				assitFunction = pushParamRefFunc;
			}
		}
		else if (node->getReturnType().isFunctionType()) {
			RuntimeFunctionInfo* usedRuntimeInfoObject = buildRuntimeInfoForConstant(scriptCompiler, node);
			auto pushParamFunc = new PushParamRef();
			pushParamFunc->setCommandData(usedRuntimeInfoObject, returnOffset);
			assitFunction = pushParamFunc;
		}
		else /*EXP_UNIT_ID_CONST*/ {
			void* constantValue = (void*)node->Execute();
			//int dataSize = ((ConstOperandBase*)node.get())->getDataSize();

			//MemoryBlock* memoryBlock = new BufferBlock(dataSize);
			//byte* constantData = (byte*)memoryBlock->getDataRef();

			//memcpy_s(constantData, dataSize, constantValue, dataSize);
			//_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));

			const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();
			MemoryBlock* memoryBlock;
			if (basicType.TYPE_STRING == node->getReturnType().iType()) {
				memoryBlock = new ObjectBlock<std::string>(*((std::string*)constantValue));
				constantValue = memoryBlock->getDataRef();
			}
			else if (basicType.TYPE_WSTRING == node->getReturnType().iType()) {
				memoryBlock = new ObjectBlock<std::wstring>(*((std::wstring*)constantValue));
				constantValue = memoryBlock->getDataRef();
			}
			else {
				memoryBlock = new ObjectBlock<ExecutableUnitRef>(node);
			}

			_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));

			auto pushParamFunc = new PushParamRef();
			pushParamFunc->setCommandData(constantValue, returnOffset);

			assitFunction = pushParamFunc;
		}

		return assitFunction;
	}

	void ExpUnitExecutor::convert2Code(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset) {
		int currentOffset = getCurrentLocalOffset();
		int beginParamOffset = currentOffset;
		TargetedCommand* assitFunction;

		auto updateLaterMan = CodeUpdater::getInstance(this->getScope());

		if (ISFUNCTION(node)) {
			RefFunction* refFunction = dynamic_cast<RefFunction*>(node.get());
			if (refFunction) {
				ExecutableUnitRef& refUnit = refFunction->getValueOfVariable();
				if (ISFUNCTION(refUnit)) {
					int functionReturnSize = scriptCompiler->getTypeSize(refUnit->getReturnType());
					//move local offset
					moveLocalOffset(functionReturnSize);

					//extract code for the function, when it execute the funtion will write the return value
					//to current offset
					convert2Code(scriptCompiler, refUnit, currentOffset);

					//add code to copy return address of the function to return offset of ref function
					auto pushParamRefFunc = new PushParamRefOffset();
					pushParamRefFunc->setCommandData(currentOffset, returnOffset);

					assitFunction = pushParamRefFunc;
				}
				else {
					assitFunction = extractCodeForOperandRef(scriptCompiler, refUnit, returnOffset);
				}
			}
			else {
				auto functionUnit = dynamic_cast<Function*>(node.get());
				int n = functionUnit->getChildCount();
				int paramSize = 0;
				int i;
				for (i = 0; i < n; i++) {
					ExecutableUnitRef& paramUnit = functionUnit->getChild(i);
					paramSize += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());
				}
				moveLocalOffset(paramSize);

				NativeFunction* expFunctionUnit = dynamic_cast<NativeFunction*>(node.get());
				if (expFunctionUnit) {
					//because for float and double params, we cannot direct pass them to the native function
					//we can only pass interger params directly
					//so we use param reference to pass to the function instead of direct value
					//and we need an additional space to do it and the native function must also handle at reference value
					int additionalParamSize = 0;
					//int realParamsSize = 0;
					int realParamCount = 0;
					int* assitParamsInfo = nullptr;
					int typeSize;

					const BasicTypes& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();
					//if (basicTypes) {
						//for (i = 0; i < n; i++) {
						//	ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
						//	typeSize = scriptCompiler->getTypeSize(paramUnit->getReturnType());
						//	if (paramUnit->getReturnType().iType() == basicTypes.TYPE_FLOAT ||
						//		paramUnit->getReturnType().iType() == basicTypes.TYPE_DOUBLE ||
						//		typeSize > sizeof(void*)) {
						//		//additionalParamSize += sizeof(void*);
						//		additionalParamSize += typeSize;
						//		realParamCount++;
						//	}
						//}
						//if (realParamCount) {
						//	moveLocalOffset(additionalParamSize);
						//	MemoryBlock* memoryBlock = new BufferBlock(realParamCount * 2 * sizeof(int));
						//	_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));

						//	assitParamsInfo = (int*)memoryBlock->getDataRef();							
						//}
					//}

					//int realParamOffset = beginParamOffset + (function->getParamsDataSize() - realParamsSize + additionalParamSize);
					int additionalParamOffset = getCurrentLocalOffset() - additionalParamSize;
					int j = 0;
					//follow is offset of params			
					for (i = 0; i < n; i++) {
						ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
						typeSize = scriptCompiler->getTypeSize(paramUnit->getReturnType());
						//if (paramUnit->getReturnType().iType() == basicTypes.TYPE_FLOAT ||
						//	paramUnit->getReturnType().iType() == basicTypes.TYPE_DOUBLE ||
						//	typeSize > sizeof(void*)) {
						//	convert2Code(scriptCompiler, paramUnit, additionalParamOffset);

						//	//move adress of real param to current param offset
						//	//or we can call this is a making reference process
						//	assitParamsInfo[j++] = additionalParamOffset;
						//	assitParamsInfo[j++] = currentOffset;

						//	additionalParamOffset += typeSize;

						//	currentOffset += sizeof(void*);
						//}
						//else {
							convert2Code(scriptCompiler, paramUnit, currentOffset);
							currentOffset += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());
						//}
					}

					const DFunction2Ref& nativeFunction = expFunctionUnit->getNative();
					if (!assitParamsInfo) {
						auto runNativeFuncFunc = new CallNativeFuntion();
						runNativeFuncFunc->setCommandData(returnOffset, beginParamOffset, nativeFunction);

						assitFunction = runNativeFuncFunc;
					}
					else {
						auto runNativeFuncFunc = new CallNativeFuntionWithAssitInfo();
						runNativeFuncFunc->setCommandData(returnOffset, beginParamOffset, nativeFunction);
						runNativeFuncFunc->initAssitInfo(realParamCount, assitParamsInfo);

						assitFunction = runNativeFuncFunc;
					}
				}
				else {
					ScriptFunction* scriptFunction = dynamic_cast<ScriptFunction*>(node.get());

					//follow is offset of params			
					for (i = 0; i < n; i++) {
						ExecutableUnitRef& paramUnit = scriptFunction->getChild(i);
						convert2Code(scriptCompiler, paramUnit, currentOffset);
						currentOffset += scriptCompiler->getTypeSize(paramUnit->getReturnType().iType());
					}
#if USE_DIRECT_COPY_FOR_RETURN
					auto callScriptFunctionFunc = new CallScriptFuntion2();
					callScriptFunctionFunc->setCommandData(returnOffset, beginParamOffset, paramSize);
#else
					int resultSize = scriptCompiler->getTypeSize(node->getReturnType());
					auto callScriptFunctionFunc = new CallScriptFuntion();
					callScriptFunctionFunc->setCommandData(resultSize, beginParamOffset, paramSize);
#endif
					callScriptFunctionFunc->setFunctionName(node->toString());

					Program* program = scriptCompiler->getProgram();
					bool found = false;
					if (program) {
						CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(scriptFunction->getId());
						if (pFunctionCode) {
							callScriptFunctionFunc->setTargetCommand(pFunctionCode->first - 1);
							found = true;
						}
					}

					if (!found) {
						if (updateLaterMan) {
							////when this function is called, the command pointer of the script function is not determine yet
							////so we need to add to update later list of program to complete the arguments.
#if USE_DIRECT_COPY_FOR_RETURN
							auto updateScriptFunctionFunc = new CdeclFunction<void, Program*, CallScriptFuntion2*, int>(CodeUpdater::updateScriptFunction);
#else
							auto updateScriptFunctionFunc = new CdeclFunction<void, Program*, CallScriptFuntion*, int>(CodeUpdater::updateScriptFunction);
#endif
							updateScriptFunctionFunc->pushParam(program);
							updateScriptFunctionFunc->pushParam((void*)callScriptFunctionFunc);
							updateScriptFunctionFunc->pushParam((void*)(size_t)scriptFunction->getId());

							updateLaterMan->addUpdateLaterTask(updateScriptFunctionFunc);
						}
						else {
							callScriptFunctionFunc->setTargetCommand(nullptr);
						}
					}
#if USE_DIRECT_COPY_FOR_RETURN
					assitFunction = callScriptFunctionFunc;
#else				
					addCommand(callScriptFunctionFunc);
					auto retreiveResultFunc = new RetreiveScriptFunctionResult();
					retreiveResultFunc->setCommandData(returnOffset, resultSize);
					assitFunction = retreiveResultFunc;
#endif // USE_DIRECT_COPY_FOR_RETURN
				}
			}
		}
		else {
			assitFunction = extractCodeForOperand(scriptCompiler, node, returnOffset);
		}

		addCommand(assitFunction);
	}


	bool ExpUnitExecutor::extractCode(ScriptCompiler* compiler, const Expression* pExp) {
		const ExecutableUnitRef& root = pExp->getRoot();
		return extractCode(compiler, root);
	}

#if !USE_FUNCTION_TREE
	bool ExpUnitExecutor::extractCode(ScriptCompiler* compiler, const ExecutableUnitRef& rootUnit) {
		resetLocalOffset();
		this->getCode()->clear();
		_returnOffset = this->getCurrentLocalOffset();
		ScriptScope* scope = getScope();
		int returnDataSize = compiler->getTypeSize(rootUnit->getReturnType());
		moveLocalOffset(returnDataSize);
		this->convert2Code(compiler, rootUnit, _returnOffset);
		if (scope) {

			int memToRunCode = scope->getScopeSize() - scope->getDataSize();
			if (_localSize > memToRunCode) {
				scope->allocate(_localSize - memToRunCode);
			}
		}
		return (_returnOffset >= 0);
	}
#endif
	void* ExpUnitExecutor::getReturnData() {
		Context* currentContext = Context::getCurrent();
		void* returnAdress = currentContext->getAbsoluteAddress(currentContext->getCurrentOffset() + _returnOffset);
		return returnAdress;
	}
}