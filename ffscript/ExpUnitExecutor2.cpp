/******************************************************************
* File:        ExpUnitExecutor2.cpp
* Description: implement ExpUnitExecutor class, part 2.
*              A class contains the
*              commands generated from an expression.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "function/CachedFunction.hpp"
#include "function/CachedMethod.hpp"
#include "function/DynamicFunction2.h"
#include "function/DynamicFunction.h"
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
#include "CommandTree.h"
#include "MemberVariableAccessors.h"
#include "LogicCommands.hpp"
#include "DefaultCommands.h"
#include "Supportfunctions.h"
#include "CompositeConstrutorUnit.h"
#include "FwdCompositeConstrutorUnit.h"

namespace ffscript {	
	
	std::vector<MemberVariableAccessor*>* ExpUnitExecutor::extractParamForMemberVariableOperand(MemberVariable* pMemberVariable) {
		list<Variable*> parents;
		MemberVariable* pMemberVariableTmp = pMemberVariable;
		ScriptScope* ownerScope = pMemberVariableTmp->getScope();
		Variable* pVariable;
		while (pMemberVariableTmp)
		{
			pVariable = pMemberVariableTmp->getParent();
			parents.push_front(pVariable);
			pMemberVariableTmp = dynamic_cast<MemberVariable*>(pVariable);
		}
		int offset = 0;

		vector<MemberVariableAccessor*>* accessors = new vector<MemberVariableAccessor*>();

		GlobalScope* globalScope = dynamic_cast<GlobalScope*>(ownerScope);
		if (globalScope) {
			void* address = globalScope->getGlobalAddress(pVariable->getOffset());
			accessors->push_back(new MVGlobalAccessor(address));
		}
		else {
			accessors->push_back(new MVContextAccessor());
		}

		for (auto var : parents) {
			auto& type = var->getDataType();
			offset = var->getOffset() - offset;

			accessors->push_back(new MVOffsetAccessor(offset));

			if (type.isRefType() || type.isRefType()) {
				accessors->push_back(new MVPointerAccessor());
			}
		}

		offset = pMemberVariable->getOffset() - offset;
		accessors->push_back(new MVOffsetAccessor(offset));
		return accessors;
	}

	TargetedCommand* ExpUnitExecutor::extractCodeForOperand(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset) {
		TargetedCommand* assitFunction;

		//moveLocalOffset(sizeof(void*));
		if (node->getType() == EXP_UNIT_ID_XOPERAND) {
			Variable* pVariable = ((CXOperand*)node.get())->getVariable();
			ScriptScope* ownerScope = pVariable->getScope();
			int dataSize = scriptCompiler->getTypeSize(pVariable->getDataType());

			MemberVariable* pMemberVariable = dynamic_cast<MemberVariable*>(pVariable);
			if (pMemberVariable == nullptr) {
				GlobalScope* globalScope = dynamic_cast<GlobalScope*>(ownerScope);
				if (globalScope) {
					void* paramData = globalScope->getGlobalAddress(pVariable->getOffset());
					auto pushParamRefFunc = new PushParam();
					pushParamRefFunc->setCommandData(paramData, dataSize, returnOffset);

					assitFunction = pushParamRefFunc;
				}
				else {
					auto pushParamRefFunc = new PushParamOffset();
					pushParamRefFunc->setCommandData(pVariable->getOffset(), dataSize, returnOffset);

					assitFunction = pushParamRefFunc;
				}
			}
			else {
				vector<MemberVariableAccessor*>* accessors = extractParamForMemberVariableOperand(pMemberVariable);

				auto pushParamFunc = new PushMemberVariableParam();
				pushParamFunc->setCommandData(accessors, dataSize, returnOffset);

				assitFunction = pushParamFunc;
			}
		}
		else /*EXP_UNIT_ID_CONST*/ {
			void* constantValue = (void*)node->Execute();
			const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();
			TargetedCommand* pushParamFunc;
			MemoryBlock* memoryBlock;
			if (basicType.TYPE_STRING == node->getReturnType().iType()) {
				memoryBlock = new ObjectBlock<std::string>(*((std::string*)constantValue));

				pushParamFunc = new PushParamRef();
				((PushParamRef*)pushParamFunc)->setCommandData(memoryBlock->getDataRef(), returnOffset);
			}
			else if (basicType.TYPE_WSTRING == node->getReturnType().iType()) {
				memoryBlock = new ObjectBlock<std::wstring>(*((std::wstring*)constantValue));

				pushParamFunc = new PushParamRef();
				((PushParamRef*)pushParamFunc)->setCommandData(memoryBlock->getDataRef(), returnOffset);
			}
			else if (node->getReturnType().isFunctionType() && !node->getReturnType().isRefType()) {
				int dataSize = ((ConstOperandBase*)node.get())->getDataSize();
				RuntimeFunctionInfo* usedRuntimeInfoObject = buildRuntimeInfoForConstant(scriptCompiler, node);
				pushParamFunc = new PushParam();
				((PushParam*)pushParamFunc)->setCommandData(usedRuntimeInfoObject, dataSize, returnOffset);
				memoryBlock = nullptr;
			}
			else {
				int dataSize = ((ConstOperandBase*)node.get())->getDataSize();
				memoryBlock = new BufferBlock(dataSize);
				memcpy_s(memoryBlock->getDataRef(), dataSize, constantValue, dataSize);
				pushParamFunc = new PushParam();
				((PushParam*)pushParamFunc)->setCommandData(memoryBlock->getDataRef(), dataSize, returnOffset);
			}

			assitFunction = pushParamFunc;
			_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));
		}

		return assitFunction;
	}

	void ExpUnitExecutor::extractParamForNativeFunction(ScriptCompiler* scriptCompiler, FunctionCommand* functionCommandTree, NativeFunction* expFunctionUnit, int beginParamOffset, int returnOffset) {
		int n = expFunctionUnit->getChildCount();
		TargetedCommand* paramCommand;
		TargetedCommand* originCommand;
		int currentOffset = beginParamOffset;

		int paramSize = 0;
		int i;
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramSize += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());			
		}
		moveLocalOffset(paramSize);

		const BasicTypes& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();
		
		//follow is offset of params			
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramCommand = convert2Code2(scriptCompiler, paramUnit, currentOffset);
			currentOffset += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());

			functionCommandTree->pushCommandParam(paramCommand);
		}
		const DFunction2Ref& nativeFunction = expFunctionUnit->getNative();
		auto runNativeFuncFunc = new CallNativeFuntion();
		if (expFunctionUnit->getType() == EXP_UNIT_ID_CREATE_THREAD) {
			auto newFunction = (CreateThreadCommand*)nativeFunction->clone();
			ExecutableUnitRef& functionObjectUnit = expFunctionUnit->getChild(0);
			auto functionType = functionObjectUnit->getReturnType().origin();
			ScriptType functionReturnType;
			list<ScriptType> argTypes;
			bool isDynamicFunction;
			scriptCompiler->parseFunctionType(functionType, functionReturnType, argTypes, isDynamicFunction);
			paramSize = 0;
			for (auto& argType : argTypes) {
				paramSize += scriptCompiler->getTypeSizeInStack(argType.iType());
			}

			newFunction->setCommandData(scriptCompiler->getTypeSize(functionReturnType), paramSize);
			runNativeFuncFunc->setCommandData(returnOffset, beginParamOffset, DFunction2Ref(newFunction));
		}
		else {
			runNativeFuncFunc->setCommandData(returnOffset, beginParamOffset, nativeFunction);
		}
		runNativeFuncFunc->setFunctionName(expFunctionUnit->getName());
		originCommand = runNativeFuncFunc;
		functionCommandTree->setCommand(originCommand);		
	}

	void ExpUnitExecutor::extractParamScriptFunction(ScriptCompiler* scriptCompiler, FunctionCommand* functionCommandTree, ScriptFunction* scriptFunction, int beginParamOffset, int returnOffset) {
		int n = scriptFunction->getChildCount();
		TargetedCommand* paramCommand;
		TargetedCommand* originCommand;
		int currentOffset = beginParamOffset;

		int paramSize = 0;
		int i;
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = scriptFunction->getChild(i);
			paramSize += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());
		}

		moveLocalOffset(paramSize);

		//follow is offset of params			
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = scriptFunction->getChild(i);
			paramCommand = convert2Code2(scriptCompiler, paramUnit, currentOffset);
			currentOffset += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());

			functionCommandTree->pushCommandParam(paramCommand);
		}

		auto callScriptFunctionFunc = new CallScriptFuntion3();
		callScriptFunctionFunc->setCommandData(returnOffset, beginParamOffset, paramSize);
		callScriptFunctionFunc->setFunctionName(scriptFunction->toString());

		Program* program = scriptCompiler->getProgram();
		bool found = false;
		if (program) {
			CodeSegmentEntry* pFunctionCode = program->getFunctionPlainCode(scriptFunction->getId());
			if (pFunctionCode) {
				callScriptFunctionFunc->setTargetCommand(pFunctionCode->first);
				found = true;
			}
		}

		if (!found) {
			auto updateLaterMan = CodeUpdater::getInstance(this->getScope());
			if (updateLaterMan) {
				////when this function is called, the command pointer of the script function is not determine yet
				////so we need to add to update later list of program to complete the arguments.

				auto updateScriptFunctionFunc = new FT::CachedFunctionDelegate<void, Program*, CallScriptFuntion2*, int>(CodeUpdater::updateScriptFunction);
				updateScriptFunctionFunc->setArgs(program, callScriptFunctionFunc, scriptFunction->getId());

				updateLaterMan->addUpdateLaterTask((DelegateRef)updateScriptFunctionFunc);
			}
			else {
				callScriptFunctionFunc->setTargetCommand(nullptr);
			}
		}

		originCommand = callScriptFunctionFunc;
		functionCommandTree->setCommand(originCommand);
	}

	TargetedCommand* ExpUnitExecutor::extractParamForForwardFunction(ScriptCompiler* scriptCompiler, Function* expFunctionUnit, int beginParamOffset, int returnOffset) {
		int n = expFunctionUnit->getChildCount();
		TargetedCommand* paramCommand;
		TargetedCommand* originCommand;
		int currentOffset = beginParamOffset;

		int paramSize = 0;
		int i;
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramSize += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());
		}
		moveLocalOffset(paramSize);

		FunctionCommand* functionCommandTree;
		switch (n)
		{
		case 1:
			functionCommandTree = new FunctionCommand1P();
			break;
		case 2:
			functionCommandTree = new FunctionCommand2P();
			break;
		default:
			functionCommandTree = new FunctionCommandNP(n);
			break;
		}

		const BasicTypes& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		//follow is offset of params			
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramCommand = convert2Code2(scriptCompiler, paramUnit, currentOffset);
			currentOffset += scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());

			functionCommandTree->pushCommandParam(paramCommand);
		}
		ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(0);
		auto functionInfoSize = scriptCompiler->getTypeSizeInStack(paramUnit->getReturnType().iType());

		auto runForwader = new FunctionForwarder();
		runForwader->setCommandData(beginParamOffset, returnOffset, beginParamOffset + functionInfoSize, paramSize - functionInfoSize);
		originCommand = runForwader;

		functionCommandTree->setCommand(originCommand);
		return functionCommandTree;
	}

	TargetedCommand* ExpUnitExecutor::extractParamForCreateLambdaFunction(ScriptCompiler* scriptCompiler, Function* expFunctionUnit, int beginParamOffset, int returnOffset) {
		int n = expFunctionUnit->getChildCount() - 1;
		TargetedCommand* paramCommand;
		TargetedCommand* originCommand;
		int currentOffset = beginParamOffset;

		int paramSize = 0;
		int i;
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramSize += scriptCompiler->getTypeSize(paramUnit->getReturnType().iType());
		}

		moveLocalOffset(paramSize);

		FunctionCommand* functionCommandTree;
		switch (n)
		{
		case 1:
			functionCommandTree = new FunctionCommand1P();
			break;
		case 2:
			functionCommandTree = new FunctionCommand2P();
			break;
		default:
			functionCommandTree = new FunctionCommandNP(n);
			break;
		}

		//follow is offset of params			
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramCommand = convert2Code2(scriptCompiler, paramUnit, currentOffset);
			currentOffset += scriptCompiler->getTypeSize(paramUnit->getReturnType());

			functionCommandTree->pushCommandParam(paramCommand);
		}
		ExecutableUnitRef& functionIdUnit = expFunctionUnit->getChild(n);
		auto functionId = *((int*)functionIdUnit->Execute());
		auto updateLaterMan = CodeUpdater::getInstance(this->getScope());
		Program* program = scriptCompiler->getProgram();

		auto callCreateLambda = new CallCreateLambda();
		//set return offset and size
		callCreateLambda->setCommandData(returnOffset, beginParamOffset, paramSize);

		//set lambda target function address
		auto updateCreateLambdaFunctionFunc = std::make_shared<FT::CachedFunctionDelegate<void, Program*, CallCreateLambda*, int>>(CodeUpdater::updateLamdaScriptFunctionObject);
		updateCreateLambdaFunctionFunc->setArgs(program,callCreateLambda,functionId);
		updateLaterMan->addUpdateLaterTask(updateCreateLambdaFunctionFunc);
		//////////////////////////

		originCommand = callCreateLambda;
		functionCommandTree->setCommand(originCommand);

		return functionCommandTree;

	}

	VariantType scriptTypeToNative(const BasicTypes& basicTypes, int scriptType) {
		if (basicTypes.TYPE_BOOL == scriptType) {
			return VariantType::Bool;
		}
		if (basicTypes.TYPE_DOUBLE == scriptType) {
			return VariantType::Double;
		}
		if (basicTypes.TYPE_FLOAT == scriptType) {
			return VariantType::Float;
		}
		if (basicTypes.TYPE_INT == scriptType) {
			return VariantType::Integer;
		}
		if (basicTypes.TYPE_LONG == scriptType) {
			return VariantType::Long;
		}
		if (basicTypes.TYPE_STRING == scriptType) {
			return VariantType::String;
		}
		if (basicTypes.TYPE_WSTRING == scriptType) {
			return VariantType::WString;
		}
		if (basicTypes.TYPE_VARIANTARRAY == scriptType) {
			return VariantType::Array;
		}

		if ((basicTypes.TYPE_BOOL | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::BoolRef;
		}
		if ((basicTypes.TYPE_DOUBLE | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::DoubleRef;
		}
		if ((basicTypes.TYPE_FLOAT | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::FloatRef;
		}
		if ((basicTypes.TYPE_INT | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::IntegerRef;
		}
		if ((basicTypes.TYPE_LONG | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::LongRef;
		}
		if ((basicTypes.TYPE_STRING | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::StringRef;
		}
		if ((basicTypes.TYPE_WSTRING | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::WStringRef;
		}
		if ((basicTypes.TYPE_VARIANTARRAY | DATA_TYPE_POINTER_MASK) == scriptType) {
			return VariantType::ArrayRef;
		}

		return VariantType::Unkown;
	}

	void ExpUnitExecutor::extractParamForDynamicFunction(ScriptCompiler* scriptCompiler, FunctionCommand* functionCommandTree, NativeFunction* expFunctionUnit, int beginParamOffset, int returnOffset) {

		int n = expFunctionUnit->getChildCount();
		TargetedCommand* paramCommand;
		TargetedCommand* originCommand;

		//fisrt param space is address of array SimpleVariantArray
		moveLocalOffset(sizeof(void*));

		//reversed the space for the list holds pointer of each param and param size		
		moveLocalOffset(sizeof(SimpleVariantArray) - sizeof(SimpleVariantArray::elems) + n * sizeof(SimpleVariantArray::elems[0]));

		int i;
		int paramSpace = 0;

		const BasicTypes& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			paramSpace += scriptCompiler->getTypeSize(paramUnit->getReturnType());
		}

		int additionalParamOffset = getCurrentLocalOffset();
		int* assitParamsInfo = nullptr;

		MemoryBlock* memoryBlock = new BufferBlock(n * sizeof(int));
		_memoryBlocks.push_back(MemoryBlockRef(memoryBlock));
		assitParamsInfo = (int*)memoryBlock->getDataRef();

		int* scriptTypes = (int*)malloc(n * sizeof(int));
		char** typeNames = (char**)malloc(n * sizeof(char*));
		int* sizes = (int*)malloc(n * sizeof(int));

		//reverse the space for whole param
		moveLocalOffset(paramSpace);

		//follow is offset of params			
		for (i = 0; i < n; i++) {
			ExecutableUnitRef& paramUnit = expFunctionUnit->getChild(i);
			
			paramCommand = convert2Code2(scriptCompiler, paramUnit, additionalParamOffset);
			auto& scriptType = paramUnit->getReturnType();
			scriptTypes[i] = scriptType.iType();
			typeNames[i] = (char*)malloc(scriptType.sType().size() + 1);
			memcpy_s(typeNames[i], scriptType.sType().size(), scriptType.sType().c_str(), scriptType.sType().size());
			typeNames[i][scriptType.sType().size()] = 0;
			sizes[i] = scriptCompiler->getTypeSize(scriptTypes[i]);

			//move adress of param to current param offset
			//or we can call this is a making reference process
			assitParamsInfo[i] = additionalParamOffset;

			additionalParamOffset += sizes[i];

			functionCommandTree->pushCommandParam(paramCommand);
		}

		const DFunction2Ref& nativeFunction = expFunctionUnit->getNative();
		
		auto runNativeFuncFunc = new CallDynamicFuntion();
		runNativeFuncFunc->setCommandData(returnOffset, beginParamOffset, nativeFunction);
		runNativeFuncFunc->initAssitInfo(n, assitParamsInfo);
		runNativeFuncFunc->setParamsType(scriptTypes, typeNames, sizes);
		runNativeFuncFunc->setFunctionName(expFunctionUnit->getName());
		originCommand = runNativeFuncFunc;

		functionCommandTree->setCommand(originCommand);
	}

	TargetedCommand* ExpUnitExecutor::extractParamConditionalOperator(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		int n = functionUnit->getChildCount();
		int currentOffset = beginParamOffset;
		ConditionalCommand * conditionalOperatorCommand = new ConditionalCommand();

		//allocate param space for the operator
		//for conditional operator, we have three parameter, condition expression, if clause and else clause
		//but we need param's space for only one param, due to two reason below:
		//1. At a time, condition expression and only one of if clause and else clause is executed.
		//2. use return offset of this operator as return offset of each clause
		//condition operator like if and else stament with only one line of code and has no context scope inside
		//it is not same native function, so we don't need to care some stuff like how to call parameter with real types
		//or how to call parameter for reference arguments
		//or what is neccessary size for one parameter, it is ok if we use one byte or size of void*(param size in stack) for bool type
		moveLocalOffset(sizeof(void*));

		//now we extract code for conditon expression
		ExecutableUnitRef& conditionUnit = functionUnit->getChild(0);
		ExecutableUnitRef& ifClause = functionUnit->getChild(1);
		ExecutableUnitRef& elseClause = functionUnit->getChild(2);

		auto conditionCommand = convert2Code2(scriptCompiler, conditionUnit, beginParamOffset);
		auto ifCommand = convert2Code2(scriptCompiler, ifClause, returnOffset);
		auto elseCommand = convert2Code2(scriptCompiler, elseClause, returnOffset);

		conditionalOperatorCommand->setCommandData(conditionCommand, ifCommand, elseCommand);

		return conditionalOperatorCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamDefaultCopyOperator(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		int n = functionUnit->getChildCount();
		int currentOffset = beginParamOffset;

		TargetedCommand* assigmentCommand = nullptr;

		ExecutableUnitRef& block1Ref = functionUnit->getChild(0);
		ExecutableUnitRef& block2 = functionUnit->getChild(1);

		// optimized for xoperand
		auto refFunction = dynamic_cast<RefFunction*>(block1Ref.get());
		if (refFunction) {
			auto& destUnitRef = refFunction->getChild(0);			
			if (destUnitRef->getType() == EXP_UNIT_ID_XOPERAND) {
				auto xOpeand = dynamic_cast<CXOperand*>(destUnitRef.get());
				auto pVariable = xOpeand->getVariable();

				// cannot optimized copying memory to a variable in global scope
				auto globalScope = dynamic_cast<GlobalScope*>(pVariable->getScope());
				if (!globalScope) {
					auto param2Command = convert2Code2(scriptCompiler, block2, pVariable->getOffset());
					auto copyAddressOfVarible = new PushParamRefOffset();
					copyAddressOfVarible->setCommandData(pVariable->getOffset(), returnOffset);

					auto defaltCopyCommand = new FunctionCommand1P();
					defaltCopyCommand->pushCommandParam(param2Command);
					defaltCopyCommand->setCommand(copyAddressOfVarible);

					assigmentCommand = defaltCopyCommand;
				}
			}
		}
		if (!assigmentCommand) {
			int param1Size = scriptCompiler->getTypeSize(block1Ref->getReturnType());
			int param2Size = scriptCompiler->getTypeSize(block2->getReturnType());

			DefaultAssigmentCommand* defaultAssigmentCommand = new DefaultAssigmentCommand(returnOffset, param2Size/*, currentOffset, currentOffset + param1Size*/);
			moveLocalOffset(param1Size + param2Size);

			auto param1Command = convert2Code2(scriptCompiler, block1Ref, currentOffset);
			auto param2Command = convert2Code2(scriptCompiler, block2, currentOffset + param1Size);

			defaultAssigmentCommand->pushCommandParam(param1Command);
			defaultAssigmentCommand->pushCommandParam(param2Command);

			assigmentCommand = defaultAssigmentCommand;
		}

		return assigmentCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamDefaultCopyOperatorRef(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		int n = functionUnit->getChildCount();
		int currentOffset = beginParamOffset;

		ExecutableUnitRef& block1Ref = functionUnit->getChild(0);
		ExecutableUnitRef& block2Ref = functionUnit->getChild(1);

		int param1Size = scriptCompiler->getTypeSize(block1Ref->getReturnType());
		int param2Size = scriptCompiler->getTypeSize(block2Ref->getReturnType());
		auto blockType = block2Ref->getReturnType().deSemiRef();
		int blockSize = scriptCompiler->getTypeSize(blockType);

		DefaultAssigmentCommandForSemiRef* assigmentCommand = new DefaultAssigmentCommandForSemiRef(returnOffset, blockSize/*, currentOffset, currentOffset + param1Size*/);
		moveLocalOffset(param1Size + param2Size);

		auto param1Command = convert2Code2(scriptCompiler, block1Ref, currentOffset);
		auto param2Command = convert2Code2(scriptCompiler, block2Ref, currentOffset + param1Size);

		assigmentCommand->pushCommandParam(param1Command);
		assigmentCommand->pushCommandParam(param2Command);

		return assigmentCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamAssigmentSemiRef(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		int n = functionUnit->getChildCount();
		int currentOffset = beginParamOffset;

		ExecutableUnitRef& param1 = functionUnit->getChild(0);
		ExecutableUnitRef& param2 = functionUnit->getChild(1);

		FunctionCommand1P* assigmentCommand = new FunctionCommand1P();

		Variable* pVariable1 = ((CXOperand*)param1.get())->getVariable();
		Variable* pVariable2 = ((CXOperand*)param2.get())->getVariable();
		ScriptScope* ownerScope = pVariable1->getScope();
		GlobalScope* globalScope = dynamic_cast<GlobalScope*>(ownerScope);
		ScriptScope* ownerScope2 = pVariable2->getScope();
		GlobalScope* globalScope2 = dynamic_cast<GlobalScope*>(ownerScope2);

		TargetedCommand* copyCommand = nullptr;
		TargetedCommand* mainCommand = nullptr;
		void* paramData = nullptr;

		if (globalScope) {
			paramData = globalScope->getGlobalAddress(pVariable1->getOffset());
			if (globalScope2) {
				auto leaCommand = new LeaAddressToAddress();
				leaCommand->setCommandData(globalScope2->getGlobalAddress(pVariable2->getOffset()), paramData);

				auto pushParam = new LeaAddressToOffset();
				pushParam->setCommandData(globalScope2->getGlobalAddress(pVariable2->getOffset()), returnOffset);

				mainCommand = pushParam;
				copyCommand = (TargetedCommand*)leaCommand;
			}
			else {
				auto leaCommand = new LeaOffsetToAddress();
				leaCommand->setCommandData(pVariable2->getOffset(), paramData);

				auto pushParam = new LeaOffsetToOffset();
				pushParam->setCommandData(pVariable2->getOffset(), returnOffset);

				mainCommand = pushParam;
				copyCommand = (TargetedCommand*)leaCommand;
			}
		}
		else {
			if (globalScope2) {
				auto pushParamRefFunc = new PushParamRef();
				pushParamRefFunc->setCommandData(globalScope2->getGlobalAddress(pVariable2->getOffset()), pVariable1->getOffset());
				copyCommand = pushParamRefFunc;

				auto pushParam = new LeaAddressToOffset();
				pushParam->setCommandData(globalScope2->getGlobalAddress(pVariable2->getOffset()), returnOffset);
				mainCommand = pushParam;
			}
			else {
				auto pushParamRefFunc = new PushParamRefOffset();
				pushParamRefFunc->setCommandData(pVariable2->getOffset(), pVariable1->getOffset());
				copyCommand = pushParamRefFunc;

				auto pushParam = new LeaOffsetToOffset();
				pushParam->setCommandData(pVariable2->getOffset(), returnOffset);
				mainCommand = pushParam;
			}
		}

		assigmentCommand->setCommand(mainCommand);
		assigmentCommand->pushCommandParam(copyCommand);

		return assigmentCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamAccessStaticArray(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		int n = functionUnit->getChildCount();
		int currentOffset = beginParamOffset;

		ExecutableUnitRef& param1 = functionUnit->getChild(0);
		ExecutableUnitRef& param2 = functionUnit->getChild(1);

		auto& arrayType = param1->getReturnType();

		int param1Size = scriptCompiler->getTypeSize(arrayType);
		int param2Size = scriptCompiler->getTypeSize(param2->getReturnType());

		int arrayOffset = beginParamOffset;
		int elemSize = scriptCompiler->getTypeSize(functionUnit->getReturnType().origin());
		bool isAddress = arrayType.isSemiRefType();
		
		// for static array, it is always used as reference instead of l-value of array
		// to pass into another command in parameter.
		// to avoid copy data from an static array, we must check data type of param 1
		// to know it is need to convert to reference or not
		if (isAddress == false) {
			// check if param 1 is a function which return l-value static array
			if (ISFUNCTION(param1)) {
				// nothing need to do here
				;
			}
			else {
				// param 1 is not a function, but its type is a l-value static array
				// then we changed param1Size to zero, because it is no need to store
				// address of static array to fist param space of the function. Instead of that,
				// we store address of static array in array offset argument.
				// see ElementAccessCommand3 for more details.
				param1Size = 0;
			}
		}
		
		moveLocalOffset(param1Size + param2Size);

		auto param1Command = convert2Code2(scriptCompiler, param1, currentOffset);
		auto param2Command = convert2Code2(scriptCompiler, param2, currentOffset + param1Size);

		TargetedCommand* command = nullptr;

		// check if data type of param 1 is a l-value static array
		if (isAddress == false) {
			// check if param 1 is a static array and coressponding command is a copy static array command...
			auto copyStaticArrayCommand = dynamic_cast<PushParamOffset*>(param1Command);
			if (copyStaticArrayCommand) {
				// ...for static array, it shouldn't use copy command before pass to another command for processing
				// instead of that, it should use array reference
				// for ElementAccessCommand3, set command 1 to null and set array offset is offset of param1
				// to use static array as a reference
				arrayOffset = copyStaticArrayCommand->getSourceOffset();
				delete param1Command;
				param1Command = nullptr;
			}
			else {
				auto copyGlobalStaticArrayCommand = dynamic_cast<PushParam*>(param1Command);
				if (copyGlobalStaticArrayCommand) {
					auto acessCommand = new ElementAccessForGlobalCommand(copyGlobalStaticArrayCommand->getSourceData(), returnOffset, elemSize);
					delete param1Command;
					param1Command = nullptr;
					acessCommand->setIndexCommand(param2Command);

					command = acessCommand;
				}
			}
		}
		else {
			auto copyGlobalStaticArrayCommand = dynamic_cast<PushParam*>(param1Command);
			if (copyGlobalStaticArrayCommand) {
				void** pArray = (void**)copyGlobalStaticArrayCommand->getSourceData();
				auto acessCommand = new ElementAccessForGlobalCommand(*pArray, returnOffset, elemSize);
				delete param1Command;
				param1Command = nullptr;
				acessCommand->setIndexCommand(param2Command);

				command = acessCommand;
			}
		}
		if (command == nullptr) {
			auto acessCommand = new ElementAccessCommand3(arrayOffset, returnOffset, elemSize, isAddress);
			acessCommand->setCommand1(param1Command);
			acessCommand->setCommand2(param2Command);

			command = acessCommand;
		}		

		return command;
	}

	TargetedCommand* ExpUnitExecutor::extractParamForStructInitializing(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		auto& constructorObject = functionUnit->getChild(0);
		auto xOperand = dynamic_cast<CXOperand*>(constructorObject.get());
		if (!xOperand) {
			// throw exception here
		}
		auto pVariable = xOperand->getVariable();
		int variableOffset = pVariable->getOffset();

		auto objectType = pVariable->getDataType();
		auto pStruct = scriptCompiler->getStruct(objectType.iType());
		if(!pStruct) {
			// throw exception here
		}

		auto typeVoid = scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID;

		MemberInfo memberInfo;
		string memberName;
		
		FunctionCommandNP* functionCommand = new FunctionCommandNP(functionUnit->getChildCount() - 1);

		auto res = pStruct->getMemberFirst(&memberName, &memberInfo);
		// break assigment for each member of the struct
		for (int i = 1; res; i ++) {
			auto paramUnit = functionUnit->getChild(i);
			auto targetCommand = convert2Code2(scriptCompiler, paramUnit, variableOffset + memberInfo.offset);
			res = pStruct->getMemberNext(nullptr, &memberInfo);

			functionCommand->pushCommandParam(targetCommand);
		}


		return functionCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamForConstructorComposite(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {		
		auto compositeConstructorUnit = dynamic_cast<CompositeConstrutorUnit*>(functionUnit);
		if (!compositeConstructorUnit) {
			// throw exception here
		}
		auto assigments = compositeConstructorUnit->getAssigments();

		FunctionCommandNP* functionCommand = new FunctionCommandNP((int)assigments.size() - 1);
		int i = 0;
		for (auto it = assigments.begin(); it != assigments.end(); it++, i++) {
			auto pVariable = it->first;
			auto& unitRef = it->second;

			auto targetCommand = convert2Code2(scriptCompiler, unitRef, pVariable->getOffset());
			if (!targetCommand) {		
				delete functionCommand;
				functionCommand = nullptr;
				break;
			}
			if (i < functionCommand->getParamCap()) {
				functionCommand->pushCommandParam(targetCommand);
			}
			else {
				functionCommand->setCommand(targetCommand);
			}
		}

		return functionCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractParamForFwdConstructor(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset) {
		auto fwdConstructorUnit = dynamic_cast<FwdConstrutorUnit*>(functionUnit);
		if (!fwdConstructorUnit) {
			// throw exception here
		}
		auto constructorUnitRef = fwdConstructorUnit->getConstructorUnit();		
		return convert2Code2(scriptCompiler, constructorUnitRef, returnOffset);
	}

	template <template <typename, typename> class CommandT>
	OptimizedLogicCommand* createLogicCommand(ScriptCompiler* scriptCompiler, const ScriptType& sType1, const ScriptType& sType2) {
		OptimizedLogicCommand* functionCommandTree = nullptr;
		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();

		bool param1IsRef = sType1.isRefType() || sType1.isSemiRefType();
		bool param2IsRef = sType2.isRefType() || sType2.isSemiRefType();

		//reset pointer mask bit
		int paramType1 = sType1.origin();
		int paramType2 = sType2.origin();

		if (paramType1 == basicTypes.TYPE_INT) {
			if (paramType2 == basicTypes.TYPE_INT) {
				functionCommandTree = new CommandT<int, int>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_LONG) {
				functionCommandTree = new CommandT<int, long long>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_DOUBLE) {
				functionCommandTree = new CommandT<int, double>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_FLOAT) {
				functionCommandTree = new CommandT<int, float>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_BOOL) {
				functionCommandTree = new CommandT<int, char>(param1IsRef, param2IsRef);
			}
		}
		else if (paramType1 == basicTypes.TYPE_LONG) {
			if (paramType2 == basicTypes.TYPE_INT) {
				functionCommandTree = new CommandT<long long, int>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_LONG) {
				functionCommandTree = new CommandT<long long, long long>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_DOUBLE) {
				functionCommandTree = new CommandT<long long, double>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_FLOAT) {
				functionCommandTree = new CommandT<long long, float>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_BOOL) {
				functionCommandTree = new CommandT<long long, char>(param1IsRef, param2IsRef);
			}
		}
		else if (paramType1 == basicTypes.TYPE_DOUBLE) {
			if (paramType2 == basicTypes.TYPE_INT) {
				functionCommandTree = new CommandT<double, int>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_LONG) {
				functionCommandTree = new CommandT<double, long long>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_DOUBLE) {
				functionCommandTree = new CommandT<double, double>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_FLOAT) {
				functionCommandTree = new CommandT<double, float>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_BOOL) {
				functionCommandTree = new CommandT<double, char>(param1IsRef, param2IsRef);
			}
		}
		else if (paramType1 == basicTypes.TYPE_FLOAT) {
			if (paramType2 == basicTypes.TYPE_INT) {
				functionCommandTree = new CommandT<float, int>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_LONG) {
				functionCommandTree = new CommandT<float, long long>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_DOUBLE) {
				functionCommandTree = new CommandT<float, double>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_FLOAT) {
				functionCommandTree = new CommandT<float, float>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_BOOL) {
				functionCommandTree = new CommandT<float, char>(param1IsRef, param2IsRef);
			}
		}
		else if (paramType1 == basicTypes.TYPE_BOOL) {
			if (paramType2 == basicTypes.TYPE_INT) {
				functionCommandTree = new CommandT<char, int>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_LONG) {
				functionCommandTree = new CommandT<char, long long>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_DOUBLE) {
				functionCommandTree = new CommandT<char, double>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_FLOAT) {
				functionCommandTree = new CommandT<char, float>(param1IsRef, param2IsRef);
			}
			else if (paramType2 == basicTypes.TYPE_BOOL) {
				functionCommandTree = new CommandT<char, char>(param1IsRef, param2IsRef);
			}
		}
		if (functionCommandTree == nullptr) {
			functionCommandTree = new CommandT<char, char>(param1IsRef, param2IsRef);
		}
		
		return functionCommandTree;
	}

	OptimizedLogicCommand* ExpUnitExecutor::createParamLogicAndCommand(ScriptCompiler* scriptCompiler, Function* node) {
		auto& paramType1 = node->getChild(0)->getReturnType();
		auto& paramType2 = node->getChild(1)->getReturnType();
		return createLogicCommand<LogicAndCommandT>(scriptCompiler, paramType1, paramType2);
	}

	OptimizedLogicCommand* ExpUnitExecutor::createParamLogicOrCommand(ScriptCompiler* scriptCompiler, Function* node) {
		auto& paramType1 = node->getChild(0)->getReturnType();
		auto& paramType2 = node->getChild(1)->getReturnType();
		return createLogicCommand<LogicOrCommandT>(scriptCompiler, paramType1, paramType2);
	}

	TargetedCommand* ExpUnitExecutor::extractParamForOptimizedLogicCommand(
		ScriptCompiler* scriptCompiler,
		OptimizedLogicCommand* optimizedCommand,
		Function* functionUnit,
		int beginParamOffset, int returnOffset) {

		int currentOffset = beginParamOffset;

		auto& param1 = functionUnit->getChild(0);
		auto& param2 = functionUnit->getChild(1);

		int param1Size = scriptCompiler->getTypeSize(param1->getReturnType());
		int param2Size = scriptCompiler->getTypeSize(param2->getReturnType());

		moveLocalOffset(param1Size + param2Size);

		auto param1Command = convert2Code2(scriptCompiler, param1, currentOffset);
		auto param2Command = convert2Code2(scriptCompiler, param2, currentOffset + param1Size);

		optimizedCommand->pushCommandParam(param1Command);
		optimizedCommand->pushCommandParam(param2Command);

		return optimizedCommand;
	}

	TargetedCommand* ExpUnitExecutor::extractCodeForFunction(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset) {
		int currentOffset = getCurrentLocalOffset();
		int beginParamOffset = currentOffset;
		TargetedCommand* assitFunction;
		FunctionCommand* functionCommandTree = nullptr;
		TargetedCommand* paramCommand;

		auto updateLaterMan = CodeUpdater::getInstance(this->getScope());

		RefFunction* refFunction = dynamic_cast<RefFunction*>(node.get());
		if (refFunction) {
			ExecutableUnitRef& refUnit = refFunction->getValueOfVariable();
			if (ISFUNCTION(refUnit)) {
				int functionReturnSize = scriptCompiler->getTypeSize(refUnit->getReturnType());
				//move local offset
				moveLocalOffset(functionReturnSize);

				//extract code for the function, when it execute the funtion will write the return value
				//to current offset
				paramCommand = convert2Code2(scriptCompiler, refUnit, currentOffset);

				//add code to copy return address of the function to return offset of ref function
				auto pushParamRefFunc = new PushParamRefOffset();
				pushParamRefFunc->setCommandData(currentOffset, returnOffset);

				functionCommandTree = new FunctionCommand1P();
				functionCommandTree->setCommand(pushParamRefFunc);
				functionCommandTree->pushCommandParam(paramCommand);

				assitFunction = functionCommandTree;
			}
			else {
				assitFunction = extractCodeForOperandRef(scriptCompiler, refUnit, returnOffset);
			}
		}
		else if (node.get()->getType() == EXP_UNIT_ID_SEMI_REF) {
			ExecutableUnitRef& refUnit = ((Function*)node.get())->getChild(0);
			if (ISFUNCTION(refUnit)) {
			}
			else {
				assitFunction = extractCodeForOperandRef(scriptCompiler, refUnit, returnOffset);
			}
		}
		else if (node.get()->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL) {
			assitFunction = extractParamConditionalOperator(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR) {
			assitFunction = extractParamDefaultCopyOperator(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR_REF) {
			assitFunction = extractParamDefaultCopyOperatorRef(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_ASSIGMENT_SEMIREF) {
			assitFunction = extractParamAssigmentSemiRef(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_STATIC_ARRAY_SUBSCRIPT) {
			assitFunction = extractParamAccessStaticArray(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_FORWARD_CALL) {
			assitFunction = extractParamForForwardFunction(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_CREATE_LAMBDA) {
			assitFunction = extractParamForCreateLambdaFunction(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_CONSTRUCTOR_COMPOSITE) {
			assitFunction = extractParamForConstructorComposite(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else if (node.get()->getType() == EXP_UNIT_ID_CREATE_OBJECT_COMPOSITE || node.get()->getType() == EXP_UNIT_ID_CREATE_OBJECT) {
			assitFunction = extractParamForFwdConstructor(scriptCompiler, (Function*)node.get(), beginParamOffset, returnOffset);
		}
		else {
			NativeFunction* expFunctionUnit = dynamic_cast<NativeFunction*>(node.get());
			int n = ((Function*)node.get())->getChildCount();

			switch (n)
			{
			case 0:
				functionCommandTree = new FunctionCommand0P();
				break;
			case 1:
				functionCommandTree = new FunctionCommand1P();
				break;
			case 2:
				if (node->getType() == EXP_UNIT_ID_OPERATOR_LOGIC_AND) {
					return extractParamForOptimizedLogicCommand(scriptCompiler,
						createParamLogicAndCommand(scriptCompiler, (Function*)node.get()), (Function*)node.get(), beginParamOffset, returnOffset);
				}
				else if (node->getType() == EXP_UNIT_ID_OPERATOR_LOGIC_OR) {
					return extractParamForOptimizedLogicCommand(scriptCompiler,
						createParamLogicOrCommand(scriptCompiler, (Function*)node.get()), (Function*)node.get(), beginParamOffset, returnOffset);
				}
				else {
					functionCommandTree = new FunctionCommand2P();
				}
				break;
			default:
				functionCommandTree = new FunctionCommandNP(n);
				break;
			}

			assitFunction = functionCommandTree;

			if (expFunctionUnit) {
				if (expFunctionUnit->getType() == EXP_UNIT_ID_DYNAMIC_FUNC) {
					extractParamForDynamicFunction(scriptCompiler, functionCommandTree, expFunctionUnit, beginParamOffset, returnOffset);
				}
				else {
					extractParamForNativeFunction(scriptCompiler, functionCommandTree, expFunctionUnit, beginParamOffset, returnOffset);
				}
			}
			else {
				ScriptFunction * scriptFunction = dynamic_cast<ScriptFunction*>(node.get());
				extractParamScriptFunction(scriptCompiler, functionCommandTree, scriptFunction, beginParamOffset, returnOffset);
			}
		}		

		return assitFunction;
	}

#if USE_FUNCTION_TREE
	bool ExpUnitExecutor::extractCode(ScriptCompiler* compiler, const ExecutableUnitRef& rootUnit) {
		resetLocalOffset();
		this->getCode()->clear();
		_unitOffsetMap.clear();

		_returnOffset = this->getCurrentLocalOffset();
		ScriptScope* scope = getScope();
		int returnDataSize = compiler->getTypeSize(rootUnit->getReturnType());
		moveLocalOffset(returnDataSize);
		TargetedCommand* assitFunction = this->convert2Code2(compiler, rootUnit, _returnOffset);
		if (scope) {

			int memToRunCode = scope->getScopeSize() - scope->getDataSize();
			if (_localSize > memToRunCode) {
				scope->allocate(_localSize - memToRunCode);
			}
		}
		addCommand(assitFunction);
		return (_returnOffset >= 0);
	}
#endif
	TargetedCommand* ExpUnitExecutor::convert2Code2(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset) {
		TargetedCommand* assitFunction;
		if (_currentScope/* && (node->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR) */) {
			auto pVariable = _currentScope->findTempVariable(node.get());
			if (pVariable) {
				//pVariable->setOffset(_returnOffset);
				pVariable->setOffset(returnOffset);
			}
		}

		if (ISFUNCTION(node)) {
			int beginParamOffset = getCurrentLocalOffset();
			assitFunction = extractCodeForFunction(scriptCompiler, node, returnOffset);

			if (node->getUserData()) {
				auto operatorType = ((Function*)node.get())->getMask();
				if ( (operatorType & UMASK_DEFAULT_CTOR) || (operatorType & UMASK_CONSTRUCTOR) || (operatorType & UMASK_CONSTRUCT_FACTOR)) {
					TriggerCommand* triggerCommand = new TriggerCommand();
					triggerCommand->setCommand(assitFunction);
					
					auto userBlockRef = dynamic_pointer_cast<ObjectBlock<OperatorBuidInfo>>(node->getUserData());
					OperatorBuidInfo* buildInfo = (OperatorBuidInfo*)userBlockRef->getDataRef();
					if (buildInfo->operatorIndex >= 0) {
						auto constructorTrigger = std::make_shared<FT::CachedFunctionDelegate<void, int>>(afterCallConstructor);
						//push constructor index to function afterCallConstructor
						constructorTrigger->setArgs(buildInfo->operatorIndex);
						triggerCommand->setAfterTrigger(constructorTrigger, "setctor(" + std::to_string(buildInfo->operatorIndex) + ")");
					}

					if (buildInfo->buildItems.size()) {
						TargetedCommand* pushParamRefOffset = nullptr;
						auto constructorCommandTree = (FunctionCommand*)assitFunction;

						list<TargetedCommand*> paramCommandLists;
						TargetedCommand* paramCommand;
						while (paramCommand = constructorCommandTree->popCommandParam())
						{
							paramCommandLists.push_front(paramCommand);
						}

						// take the the first command param(address of current object need to construct)
						pushParamRefOffset = paramCommandLists.front();
						paramCommandLists.pop_front();

						FunctionCommand* newConstuctCommand = nullptr;

						if (paramCommandLists.size() == 0) {
							newConstuctCommand = nullptr;
						}
						else if (paramCommandLists.size() == 1) {
							newConstuctCommand = new FunctionCommand1P();
						}
						else if (paramCommandLists.size() == 2) {
							newConstuctCommand = new FunctionCommand2P();
						}
						else {
							newConstuctCommand = new FunctionCommandNP((int)paramCommandLists.size());
						}

						for (auto it = paramCommandLists.begin(); it != paramCommandLists.end(); it++) {
							newConstuctCommand->pushCommandParam(*it);
						}
						if (newConstuctCommand) {
							newConstuctCommand->setCommand(constructorCommandTree->getCommand());
							triggerCommand->setCommand(newConstuctCommand);
						}
						else {
							triggerCommand->setCommand(constructorCommandTree->getCommand());
						}
						constructorCommandTree->setCommand(nullptr);
						delete constructorCommandTree;

						auto runChildConstructors = std::make_shared<BeforeConstructorCall>(pushParamRefOffset, beginParamOffset);
						triggerCommand->setBeforeTrigger(runChildConstructors, "runctors(TBD)");

						//set param to build constructor for children member
						auto codeUpdateLater = CodeUpdater::getInstance(getScope());

						auto constructorUpdater = std::make_shared<FT::CachedMethodDelegate<
							BeforeConstructorCall, void, ScriptCompiler*,
							ScriptScope*, const std::list<OperatorBuidItemInfo>&>>(
								runChildConstructors.get(), &BeforeConstructorCall::buildOperator);
						//add params for function BeforeConstructorCall::buildOperator
						constructorUpdater->setArgs(scriptCompiler,getScope(),buildInfo->buildItems);

						codeUpdateLater->addUpdateLaterTask(constructorUpdater);
					}

					assitFunction = triggerCommand;
				}
				else if ( (operatorType & UMASK_DESTRUCTOR) || (operatorType & UMASK_DESTRUCTORFORRETURNDATA)) {
					TriggerCommand* triggerCommand = nullptr;
					auto userBlockRef = dynamic_pointer_cast<ObjectBlock<OperatorBuidInfo>>(node->getUserData());
					OperatorBuidInfo* buildInfo = (OperatorBuidInfo*)userBlockRef->getDataRef();
					if (operatorType & UMASK_DESTRUCTOR) {
						triggerCommand = new ConditionTriggerCommand();
						auto destructorTrigger = std::make_shared<FT::CachedFunctionDelegate<unsigned char, int>>(beforeCallDestructor);
						//push constructor index to function beforeCallDestructor
						destructorTrigger->setArgs(buildInfo->operatorIndex);
						//set condition command
						triggerCommand->setBeforeTrigger(destructorTrigger, "checkctor(" + std::to_string(buildInfo->operatorIndex) + ")");
					}
					else {
						triggerCommand = new TriggerCommand();
					}
					//set constructor command
					triggerCommand->setCommand(assitFunction);

					if (buildInfo->buildItems.size()) {
						auto runChilddestructors = std::make_shared<BeforeConstructorCall>(nullptr, beginParamOffset);
						triggerCommand->setAfterTrigger(runChilddestructors, "rundtors(TBD)");

						//set param to build constructor for children member
						auto codeUpdateLater = CodeUpdater::getInstance(getScope());

						auto destructorUpdater = std::make_shared<FT::CachedMethodDelegate<
							BeforeConstructorCall, void, ScriptCompiler*,
							ScriptScope*, const std::list<OperatorBuidItemInfo>&>>(
								runChilddestructors.get(), &BeforeConstructorCall::buildOperator);
						//add params for function BeforeConstructorCall::buildOperator
						destructorUpdater->setArgs(scriptCompiler, getScope(), buildInfo->buildItems);

						codeUpdateLater->addUpdateLaterTask(destructorUpdater);
					}

					assitFunction = triggerCommand;
				}
			}
		}
		else {
			assitFunction = extractCodeForOperand(scriptCompiler, node, returnOffset);
		}

		if (assitFunction) {
			assitFunction->setTargetOffset(returnOffset);
		}
		return assitFunction;
	}
}