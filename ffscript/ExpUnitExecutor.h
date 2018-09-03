/******************************************************************
* File:        ExpUnitExecutor.h
* Description: Declare ExpUnitExecutor class. A class contains the
*              commands generated from an expression.
* Author:      Vincent Pham
*
* (C) Copyright 2018, The ffscript project, All rights reserved.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "Executor.h"
#include <list>
#include <vector>
#include <memory>
#include <map>
#include "expressionunit.h"
#include "function\DynamicFunction.h"
#include "Context.h"
#include "MemoryBlock.h"

namespace ffscript {
	class  Expression;
	class ScriptScope;
	class ScriptCompiler;
	class Variable;
	class CommandTree;
	class FunctionCommand;
	class MemberVariableAccessor;
	class ScriptFunction;
	class TargetedCommand;
	class OptimizedLogicCommand;

	class ExpUnitExecutor :
		public Executor
	{
		typedef shared_ptr<byte> ConstantValue;

		ScriptScope* _currentScope;		
		int _localOffset;
		int _localSize;
		int _returnOffset;
		std::map<ExecutableUnit*, int> _unitOffsetMap;
	private:
		void moveLocalOffset(int size);
		void resetLocalOffset();

	public:
		ExpUnitExecutor(ScriptScope* scope);
		virtual ~ExpUnitExecutor();

		bool extractCode(ScriptCompiler* compiler, const Expression* pExpression);
		bool extractCode(ScriptCompiler* compiler, const ExecutableUnitRef& rootUnit);
		void* getReturnData();
		int getCurrentLocalOffset() const;
		int getReturnOffset() const;
		int getLocalSize() const;
		ScriptScope* getScope() const;
		int getOffset(ExecutableUnit* node) const;
	protected:
		void convert2Code(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset);
		TargetedCommand* convert2Code2(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset);
		TargetedCommand* extractCodeForFunction(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset);
		TargetedCommand* extractCodeForOperand(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset);
		TargetedCommand* extractCodeForOperandRef(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& node, int returnOffset);

		void extractParamForNativeFunction(ScriptCompiler* scriptCompiler, FunctionCommand* commander, NativeFunction* expFunctionUnit, int beginParamOffset, int returnOffset);
		void extractParamForDynamicFunction(ScriptCompiler* scriptCompiler, FunctionCommand* commander, NativeFunction* expFunctionUnit, int beginParamOffset, int returnOffset);
		void extractParamScriptFunction(ScriptCompiler* scriptCompiler, FunctionCommand* commander, ScriptFunction* expFunctionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamForForwardFunction(ScriptCompiler* scriptCompiler, Function* expFunctionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamForCreateLambdaFunction(ScriptCompiler* scriptCompiler, Function* expFunctionUnit, int beginParamOffset, int returnOffset);
		std::vector<MemberVariableAccessor*>* extractParamForMemberVariableOperand(MemberVariable* pMemberVariable);
		TargetedCommand* extractParamConditionalOperator(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamDefaultCopyOperator(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamDefaultCopyOperatorRef(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamAccessStaticArray(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamForConstructorComposite(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamForFwdConstructor(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		TargetedCommand* extractParamForStructInitializing(ScriptCompiler* scriptCompiler, Function* functionUnit, int beginParamOffset, int returnOffset);
		OptimizedLogicCommand* createParamLogicAndCommand(ScriptCompiler* scriptCompiler, Function* functionUnit);
		OptimizedLogicCommand* createParamLogicOrCommand(ScriptCompiler* scriptCompiler, Function* functionUnit);
		TargetedCommand* extractParamForOptimizedLogicCommand(ScriptCompiler* scriptCompiler,
			OptimizedLogicCommand* optimizedCommand,
			Function* functionUnit, int beginParamOffset, int returnOffset);
		RuntimeFunctionInfo* buildRuntimeInfoForConstant(ScriptCompiler* scriptCompiler, const ExecutableUnitRef& constantUnit);
	};
}
