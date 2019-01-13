/******************************************************************
* File:        DefaultCommands.h
* Description: declare default command classes.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "InstructionCommand.h"
#include "expressionunit.h"
#include "BasicFunctionFactory.hpp"
#include "function/CachedDelegate.h"

namespace ffscript {
	class DefaultAssigmentCommand : public TargetedCommand {
		//int _returnOffset;
		int _blockSize;
		//int _offset1;
		//int _offset2;
		TargetedCommand* _command1;
		TargetedCommand* _command2;
	public:
		DefaultAssigmentCommand(int returnOffset, int blockSize/*, int offset1, int offset2*/);
		virtual ~DefaultAssigmentCommand();
		void buildCommandText(std::list<std::string>& strCommands);
		virtual void execute();
		virtual int pushCommandParam(TargetedCommand* command);
	};

	class DefaultAssigmentCommandForSemiRef : public TargetedCommand {
		//int _returnOffset;
		int _blockSize;
		//int _offset1;
		//int _offset2;
		TargetedCommand* _command1;
		TargetedCommand* _command2;
	public:
		DefaultAssigmentCommandForSemiRef(int returnOffset, int blockSize/*, int offset1, int offset2*/);
		virtual ~DefaultAssigmentCommandForSemiRef();
		void buildCommandText(std::list<std::string>& strCommands);
		virtual void execute();
		virtual int pushCommandParam(TargetedCommand* command);
	};

	/////////////////////////////////////////////////////////////////////////
	// classes for assign an variant array to struct
	//
	/////////////////////////////////////////////////////////////////////////
	//command class
	class ArrayToStructCommand : public DFunction2 {
	public:
		ArrayToStructCommand();
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	/////////////////////////////////////////////////////////////////////////
	// classes for access member in struct
	//
	/////////////////////////////////////////////////////////////////////////
	//class for accessing member and return l-value of member
	class MemberAccessCommand1 : public DFunction2 {
	private:
		int _returnSize;
	public:
		MemberAccessCommand1(int returnSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};
	//class for accessing member and return r-value of member
	class MemberAccessCommand2: public DFunction2 {
	public:
		MemberAccessCommand2();
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	/////////////////////////////////////////////////////////////////////////
	// classes for access element in array
	//
	/////////////////////////////////////////////////////////////////////////
	//class for accessing member and return l-value of member
	class ElementAccessCommand1 : public DFunction2 {
	private:
		int _elementSize;
	public:
		ElementAccessCommand1(int elementSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	//class for accessing member and return r-value of member
	class ElementAccessCommand2 : public DFunction2 {
	private:
		int _elementSize;
	public:
		ElementAccessCommand2(int elementSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	//class for accessing member of an 'ref T&' type and return r-value of member
	class ElementAccessCommand4 : public DFunction2 {
	private:
		int _elementSize;
	public:
		ElementAccessCommand4(int elementSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	//class for deref a pointer and return r-value
	class DeRefCommand : public DFunction2 {
	private:
		int _typeSize;
	public:
		DeRefCommand(int typeSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	//class for deref a pointer and return r-value
	class DeRefCommand2 : public DFunction2 {
	private:
		int _typeSize;
	public:
		DeRefCommand2(int typeSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	class AssignFunctionToNull : public DFunction2 {
	public:
		AssignFunctionToNull();
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	class CompareRuntimeFunction : public DFunction2 {
	public:
		CompareRuntimeFunction();
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	/////////////////////////////////////////////////////////////////////////
	// trigger functions for constructor and destructor
	//
	/////////////////////////////////////////////////////////////////////////
	class BeforeConstructorCall : public CachedDelegate {
		unsigned int _constructObjectOffsetRef;
		InstructionCommand* _pushObjectToConstructorParamCommand;
		std::list<InstructionCommand*> _constructorItems;
		std::list<int> _itemOffsets;
		int _currentScopeCodeSize;

	public:
		BeforeConstructorCall(InstructionCommand* pushObjectToConstructorParamCommand, unsigned int objectOffsetRef);
		virtual ~BeforeConstructorCall();
		void call();

		void buildOperator(ScriptCompiler* scriptCompiler, ScriptScope* currentScope, const std::list<OperatorBuidItemInfo> &operatorInfoList);

		//std::list<InstructionCommand*>& getConstructorItems();
	};
	//this function will do nothing, it just used for constructor/destructor that not registered
	//but children elements inside have constructors/destructors
	void dummyOperator(void*);
	void afterCallConstructor(int contructorIndex);
	unsigned char beforeCallDestructor(int contructorIndex);


	void defaultRuntimeFunctionInfoConstructor(RuntimeFunctionInfo* obj);
	void runtimeFunctionInfoCopyConstructor(RuntimeFunctionInfo* obj1, RuntimeFunctionInfo* obj2);
	void runtimeFunctionInfoConstructByNull(RuntimeFunctionInfo* obj1, void*);
	void runtimeFunctionInfoDestructor(RuntimeFunctionInfo* obj);

	class CreateThreadCommand : public DFunction2 {
		int _returnSize;
		int _paramSize;
	public:
		CreateThreadCommand();
		CreateThreadCommand(int returnSize, int paramSize);
		void setCommandData(int returnSize, int paramSize);
		void call(void* pReturnVal, void* param[]);
		DFunction2* clone();
	};

	void joinThread(THREAD_HANDLE);
	void closeThread(THREAD_HANDLE);

	///
	/// access to an element in static array
	///
	class ElementAccessCommand3 : public TargetedCommand {
		int _elmSize;
		int _arrayOffset;
		bool _isAddress;
		TargetedCommand* _command1;
		TargetedCommand* _command2;
	public:
		ElementAccessCommand3(int arrayOffset, int returnOffset, int elmSize, bool isAddress);
		virtual ~ElementAccessCommand3();
		void buildCommandText(std::list<std::string>& strCommands);
		virtual void execute();
		void setCommand1(TargetedCommand* command);
		void setCommand2(TargetedCommand* command);
	};

	///
	/// access to an element in static array
	///
	class ElementAccessForGlobalCommand : public TargetedCommand {
		int _elmSize;
		void* _arrayData;
		TargetedCommand* _indexCommand;
	public:
		ElementAccessForGlobalCommand(void* arrayData, int returnOffset, int elmSize);
		virtual ~ElementAccessForGlobalCommand();
		void buildCommandText(std::list<std::string>& strCommands);
		virtual void execute();
		void setIndexCommand(TargetedCommand* command);
	};
}