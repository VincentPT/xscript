/******************************************************************
* File:        InstructionCommand.h
* Description: declare command classes. Instances of these classes
*              are the code of the C Lambda script. These are 
*              processor units of a script program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"

#include <string>
#include <vector>
#include <list>

class DFunction2;


#define BEGIN_INSTRUCTION_COMMAND_DECLARE(className, baseClass) \
 	class className : public baseClass { \
	public: \
		className(); \
		virtual ~className(); \
		virtual void execute(); \
		virtual void buildCommandText(std::list<std::string>& strCommands)

#define END_INSTRUCTION_COMMAND_DECLARE(className) }

namespace ffscript {

	class Context;
	class MemberVariableAccessor;

	class InstructionCommand
	{
	public:
		InstructionCommand();
		virtual ~InstructionCommand();
		virtual void execute() = 0;
		virtual void buildCommandText(std::list<std::string>& strCommands) = 0;
	};

	class TargetedCommand : public InstructionCommand
	{
		int _targetOffset;
		int _targetSize;		
	public:
		TargetedCommand();
		TargetedCommand(int targetOffset, int targetSize);
		virtual ~TargetedCommand();

		inline int getTargetOffset() const {
			return _targetOffset;
		}
		inline void setTargetOffset(int targetOffset) {
			_targetOffset = targetOffset;
		}

		inline int getTargetSize() const {
			return _targetSize;
		}
		inline void setTargetSize(int targetSize) {
			_targetSize = targetSize;
		}
	};

	/////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(EnterContextScope, InstructionCommand);
private:
	int _scopeDataSize;
	int _scopeCodeSize;
	int _constructorCommandCount;
	ScopeAutoRunList* _scopeAutoRunList;
public:
	void setScopeInfo(int dataSize, int codeSize, int constructorCommandCount);
	void storeAutoRunCommand(ScopeAutoRunList& autoRunCommandList);
	END_INSTRUCTION_COMMAND_DECLARE(EnterContextScope);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ExitContextScope, InstructionCommand);
private:
	int _scopeDataSize;
	int _scopeCodeSize;	
	ScopeAutoRunList* _scopeAutoRunList;
	bool _restoreCall;
public:
	void setScopeInfo(int dataSize, int codeSize);
	void setRestoreCallFlag(bool blRestoreCall);
	void storeAutoRunCommand(ScopeAutoRunList& autoRunCommandList);
	END_INSTRUCTION_COMMAND_DECLARE(ExitContextScope);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushParamRef, TargetedCommand);
	private:
		void* _param;
	public:
		void setCommandData(void* sourceParam, int targetOffset);
	};

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushParamRefOffset, TargetedCommand);
private:
	int _sourceOffset;
public:
	void setCommandData(int sourceOffset, int targetOffset);	
	END_INSTRUCTION_COMMAND_DECLARE(PushParamRefOffset);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushParam, TargetedCommand);
private:	
	void* _param;
public:
	void setCommandData(void* sourceParam, int dataSize, int dataOffset);
	void* getSourceData() const;
	END_INSTRUCTION_COMMAND_DECLARE(PushParam);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushParamOffset, TargetedCommand);
private:
	int _sourceOffset;
public:
	void setCommandData(int sourceOffset, int paramSize, int targetOffset);
	int getSourceOffset() const;
	END_INSTRUCTION_COMMAND_DECLARE(PushParamOffset);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CopyDataToRef, TargetedCommand);
private:	
	int _sourceOffset;
public:
	void setCommandData(int sourceOffset, int paramSize, int targetOffsetRef);
	END_INSTRUCTION_COMMAND_DECLARE(CopyDataToRef);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(RetreiveScriptFunctionResult, TargetedCommand);
public:
	void setCommandData(int storageOffset, int resultSize);
	END_INSTRUCTION_COMMAND_DECLARE(RetreiveScriptFunctionResult);

	///////////////////////////////////////////////////
	class CallFuntion : public TargetedCommand
	{
	protected:
		int _beginParamOffset;		
		std::string _functionName;
	public:
		CallFuntion();
		int getBeginParamOffset() const;
		void setFunctionName(const std::string& functionName);
		virtual ~CallFuntion();		
	};

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallNativeFuntion, CallFuntion);
private:
	DFunction2Ref _targetFunction;
public:
	void setCommandData(int returnOffset, int beginParamOffset, const DFunction2Ref& targetFunction);
	END_INSTRUCTION_COMMAND_DECLARE(CallNativeFuntion);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(FunctionForwarder, CallFuntion);
private:
	int _funtionInfoOffset;
	int _paramSize;
public:
	void setCommandData(int funtionInfoOffset, int returnOffset, int beginParamOffset, int paramSize);
	END_INSTRUCTION_COMMAND_DECLARE(FunctionForwarder);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallNativeFuntionWithAssitInfo, CallNativeFuntion);
protected:
	int _pairCount;
	int* _pairs;
public:
	void initAssitInfo(int pairCount, int* pairs);
	END_INSTRUCTION_COMMAND_DECLARE(CallNativeFuntionWithAssitInfo);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallDynamicFuntion, CallNativeFuntionWithAssitInfo);
	protected:
		int* _scriptTypes;
		char** _typeNames;
		int* _sizes;
	public:
		void setParamsType(int* scriptTypes, char** typeNames, int* sizes);
	END_INSTRUCTION_COMMAND_DECLARE(CallDynamicFuntion);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallScriptFuntion, CallFuntion);
private:	
	int _paramSize;
	CommandPointer _targetFunction;	
public:
	void setCommandData(int resultSize, int beginParamOffset, int paramSize);
	void setTargetCommand(CommandPointer targetFunction);
	END_INSTRUCTION_COMMAND_DECLARE(CallScriptFuntion);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallScriptFuntion2, CallFuntion);
protected:
	int _paramSize;
	CommandPointer _targetFunction;
public:
	void setCommandData(int returnOffset, int beginParamOffset, int paramSize);
	void setTargetCommand(CommandPointer targetFunction);
	END_INSTRUCTION_COMMAND_DECLARE(CallScriptFuntion2);

	////////////////////////////////////////////////////
	class CallScriptFuntion3 : public CallScriptFuntion2 {
	public:
		CallScriptFuntion3();
		void execute();
	};

	////////////////////////////////////////////////////
	class CallLambdaFuntion : public CallScriptFuntion2 {
		AnoynymousDataInfo* _anoynymousInfo;
	public:
		CallLambdaFuntion(AnoynymousDataInfo* data);
		void execute();
	};

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(Jump, InstructionCommand);
protected:
	CommandPointer _targetCommand;
public:
	void setCommandData(CommandPointer targetCommand);
	END_INSTRUCTION_COMMAND_DECLARE(Jump);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(JumpIf, InstructionCommand);
protected:
	int _conditionOffset;
	CommandPointer _targetCommandTrue;
public:
	void setCommandData(int conditionOffset, CommandPointer targetCommand);
	END_INSTRUCTION_COMMAND_DECLARE(JumpIf);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(JumpIfElse, JumpIf);
protected:
	CommandPointer _targetCommandFalse;
public:
	void setCommandElse(CommandPointer targetCommand);
	END_INSTRUCTION_COMMAND_DECLARE(JumpIfElse);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(MultipleCommand, InstructionCommand);
protected:
	CommandList _commands;
public:
	CommandList& getCommands();
	END_INSTRUCTION_COMMAND_DECLARE(MultipleCommand);

	///////////////////////////////////////////////////
	class BreakCommand : public MultipleCommand {
	public:
		BreakCommand();
		virtual ~BreakCommand();
		virtual void buildCommandText(std::list<std::string>& strCommands);
	};

	///////////////////////////////////////////////////
	class ExitScriptFuntionAtReturn : public MultipleCommand {
		int _indexPreventDestructorRun;
	public:
		ExitScriptFuntionAtReturn();
		virtual ~ExitScriptFuntionAtReturn();
		virtual void buildCommandText(std::list<std::string>& strCommands);
		virtual void execute();
		void setCommandData(int indexPreventDestructorRun);
	};

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ExitFunctionAtTheEnd, InstructionCommand);
	END_INSTRUCTION_COMMAND_DECLARE(ExitFunctionAtTheEnd);	

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(ContinueCommand, MultipleCommand);
private:
	CommandPointer _loopCommand;
public:
	void setLoopCommand(CommandPointer loopCommand);
	END_INSTRUCTION_COMMAND_DECLARE(ContinueCommand);

	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushMemberVariableParam, TargetedCommand);
protected:
	std::vector<MemberVariableAccessor*>* _accessors;
public:
	void setCommandData(std::vector<MemberVariableAccessor*>* accessors, int paramSize, int targetOffset);
	END_INSTRUCTION_COMMAND_DECLARE(PushMemberVariableParam);

	BEGIN_INSTRUCTION_COMMAND_DECLARE(PushMemberVariableParamRef, TargetedCommand);
protected:
	std::vector<MemberVariableAccessor*>* _accessors;
public:
	void setCommandData(std::vector<MemberVariableAccessor*>* accessors, int targetOffset);
	END_INSTRUCTION_COMMAND_DECLARE(PushMemberVariableParamRef);

	////////////////////////////////////////////////////
	BEGIN_INSTRUCTION_COMMAND_DECLARE(CallCreateLambda, TargetedCommand);
private:
	int _srcDataOffset;
	int _destDataOffset;
	int _dataSize;
	CommandPointer _anoynymousTargetFunction;
public:
	void setCommandData(int returnOffset, int dataOffset, int dataSize);
	void setLambdaAddress(CommandPointer anoynymousTargetFunction, int destDataOffset);
	END_INSTRUCTION_COMMAND_DECLARE(CallCreateLambda);
}
