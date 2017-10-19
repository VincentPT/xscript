#include "stdafx.h"
#include "InstructionCommand.h"
#include "Context.h"
#include "function\DynamicFunction2.h"
#include "MemberVariableAccessors.h"
#include "OperatorObject.h"
#include "ScopeRuntimeData.h"

#include <iomanip>
#include <sstream>

//#include "CppUnitTest.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ffscript {
	InstructionCommand::InstructionCommand(){
	}

	InstructionCommand::~InstructionCommand(){
	}

	void InstructionCommand::setCommandText(const std::string& commandText) {
		_commandText = commandText;
	}

	const std::string& InstructionCommand::toString() {
		//if (_commandText.empty()) {
			buildCommandText();
		//}
		return _commandText;
	}

	///
	///
	///
	TargetedCommand::TargetedCommand() : _targetOffset(0), _targetSize(0) {
	}

	TargetedCommand::TargetedCommand(int targetOffset, int targetSize) : _targetOffset(targetOffset), _targetSize(targetSize) {
	}

	TargetedCommand::~TargetedCommand() {
	}
	/////////////////////////////////////////////////////////////////////////////////////
	EnterContextScope::EnterContextScope() : _scopeCodeSize(0), _scopeDataSize(0), _constructorCommandCount(0), _scopeAutoRunList(nullptr) {
	}
	EnterContextScope::~EnterContextScope() {
		if (_scopeAutoRunList) {
			delete _scopeAutoRunList;
		}
	}

	void EnterContextScope::setScopeInfo(int dataSize, int codeSize, int constructorCommandCount) {
		_scopeDataSize = dataSize;
		_scopeCodeSize = codeSize;
		_constructorCommandCount = constructorCommandCount;
	}

	void EnterContextScope::storeAutoRunCommand(ScopeAutoRunList& autoRunCommandList) {
		if (_scopeAutoRunList) {
			delete _scopeAutoRunList;
			_scopeAutoRunList = nullptr;
		}
		if (autoRunCommandList.size()) {
			_scopeAutoRunList = new ScopeAutoRunList();
			_scopeAutoRunList->splice(_scopeAutoRunList->begin(), autoRunCommandList, autoRunCommandList.begin(), autoRunCommandList.end());
		}
	}

	void EnterContextScope::buildCommandText() {
		setCommandText("allocate " + std::to_string(_scopeDataSize + _scopeCodeSize));
	}

	void EnterContextScope::execute() {
		Context* context = Context::getCurrent();
		context->pushContext(_constructorCommandCount);
		context->scopeAllocate(_scopeDataSize, _scopeCodeSize);
#ifndef THROW_EXCEPTION_ON_ERROR
		if (context->isError()) {
			return;
		}
#endif

		if (_scopeAutoRunList) {
			for (auto it = _scopeAutoRunList->begin(); it != _scopeAutoRunList->end(); it++) {
				(*it)->execute();
			}
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ExitContextScope::ExitContextScope() : _scopeDataSize(0), _scopeCodeSize(0), _scopeAutoRunList(nullptr), _restoreCall(true) {}
	ExitContextScope::~ExitContextScope() {
		if (_scopeAutoRunList) {
			delete _scopeAutoRunList;
		}
	}

	void ExitContextScope::setScopeInfo(int dataSize, int codeSize) {
		_scopeDataSize = dataSize;
		_scopeCodeSize = codeSize;
	}

	void ExitContextScope::setRestoreCallFlag(bool blRestoreCall) {
		_restoreCall = blRestoreCall;
	}

	void ExitContextScope::storeAutoRunCommand(ScopeAutoRunList& autoRunCommandList) {
		if (_scopeAutoRunList) {
			delete _scopeAutoRunList;
			_scopeAutoRunList = nullptr;
		}
		if (autoRunCommandList.size()) {
			_scopeAutoRunList = new ScopeAutoRunList();
			_scopeAutoRunList->splice(_scopeAutoRunList->begin(), autoRunCommandList, autoRunCommandList.begin(), autoRunCommandList.end());
		}
	}

	void ExitContextScope::buildCommandText() {
		setCommandText("unallocate " + std::to_string(_scopeDataSize + _scopeCodeSize));
	}

	void ExitContextScope::execute() {
		Context* context = Context::getCurrent();
#ifndef THROW_EXCEPTION_ON_ERROR
		if (context->isError()) {
			context->scopeUnallocate(_scopeSize);
			context->popContext();
			return;
		}
#endif
		if (_scopeAutoRunList) {
			for (auto it = _scopeAutoRunList->begin(); it != _scopeAutoRunList->end(); it++) {
				(*it)->execute();
			}
		}
		
		context->scopeUnallocate(_scopeDataSize, _scopeCodeSize);

		if (_restoreCall) {
			context->popContext();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushParamRef::PushParamRef() : _param(nullptr), TargetedCommand(0, sizeof(void*)) {}
	PushParamRef::~PushParamRef() {}

	void PushParamRef::setCommandData(void* sourceParam, int targetOffset) {
		_param = sourceParam;
		setTargetOffset(targetOffset);
	}

	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}

	void PushParamRef::buildCommandText() {		
		std::stringstream ss;
		ss << "lea " << int_to_hex((size_t)_param) << " " << getTargetOffset();
		setCommandText(ss.str());
	}

	void PushParamRef::execute() {
		Context* context = Context::getCurrent();
		int offset = getTargetOffset() + context->getCurrentOffset();
		context->lea(offset, _param);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushParamRefOffset::PushParamRefOffset() : _sourceOffset(0), TargetedCommand(0, sizeof(void*)) {}
	PushParamRefOffset::~PushParamRefOffset() {}
	void PushParamRefOffset::setCommandData(int sourceOffset, int targetOffset) {
		_sourceOffset = sourceOffset;
		setTargetOffset(targetOffset);
	}

	void PushParamRefOffset::buildCommandText() {		
		std::stringstream ss;
		ss << "lea [" << _sourceOffset << "] " << getTargetOffset();
		setCommandText(ss.str());
	}

	void PushParamRefOffset::execute() {
		Context* context = Context::getCurrent();
		int sourceOffset = _sourceOffset + context->getCurrentOffset();
		int targetOffset = getTargetOffset() + context->getCurrentOffset();
		context->lea(targetOffset, context->getAbsoluteAddress(sourceOffset));
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushParam::PushParam() : _param(nullptr) {}
	PushParam::~PushParam() {}
	void PushParam::setCommandData(void* sourceParam, int dataSize, int dataOffset) {
		_param = sourceParam;
		setTargetOffset(dataOffset);
		setTargetSize(dataSize);
	}

	void* PushParam::getSourceData() const {
		return _param;
	}

	void PushParam::buildCommandText() {		
		std::stringstream ss;
		ss << "mov " << int_to_hex((size_t)_param) << " " << getTargetOffset() << " " << getTargetSize();
		setCommandText(ss.str());
	}

	void PushParam::execute() {
		Context* context = Context::getCurrent();
		int offset = getTargetOffset() + context->getCurrentOffset();
		context->write(offset, _param, getTargetSize());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushParamOffset::PushParamOffset() : _sourceOffset(0) {}
	PushParamOffset::~PushParamOffset() {}

	int PushParamOffset::getSourceOffset() const {
		return _sourceOffset;
	}

	void PushParamOffset::setCommandData(int sourceOffset, int paramSize, int targetOffset) {
		_sourceOffset = sourceOffset;
		setTargetSize(paramSize);
		setTargetOffset(targetOffset);
	}

	void PushParamOffset::buildCommandText() {		
		std::stringstream ss;
		ss << "mov " << _sourceOffset << " " << getTargetOffset() << " " << getTargetSize();
		setCommandText(ss.str());
	}

	void PushParamOffset::execute() {
		Context* context = Context::getCurrent();
		int sourceOffset = _sourceOffset + context->getCurrentOffset();
		int targetOffset = getTargetOffset() + context->getCurrentOffset();
		context->write(targetOffset, context->getAbsoluteAddress(sourceOffset), getTargetSize());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CopyDataToRef::CopyDataToRef() : _sourceOffset(0) {}
	CopyDataToRef::~CopyDataToRef() {}
	void CopyDataToRef::setCommandData(int sourceOffset, int paramSize, int targetOffsetRef) {
		_sourceOffset = sourceOffset;
		setTargetSize(paramSize);
		setTargetOffset(targetOffsetRef);
	}

	void CopyDataToRef::buildCommandText() {		
		std::stringstream ss;
		ss << "mov " << _sourceOffset << " [" << getTargetOffset() << "] " << getTargetSize();
		setCommandText(ss.str());
	}

	void CopyDataToRef::execute() {
		Context* context = Context::getCurrent();
		int targetOffsetRef = getTargetOffset() + context->getCurrentOffset();
		int sourceOffset = _sourceOffset + context->getCurrentOffset();

		void* sourceAddress = context->getAbsoluteAddress(sourceOffset);
		void* targetAddressRef = context->getAbsoluteAddress(targetOffsetRef);

		size_t valueAddress = *((size_t*)targetAddressRef);

		memcpy_s((void*)valueAddress, getTargetSize(), sourceAddress, getTargetSize());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	RetreiveScriptFunctionResult::RetreiveScriptFunctionResult() {}
	RetreiveScriptFunctionResult::~RetreiveScriptFunctionResult() {}
	void RetreiveScriptFunctionResult::setCommandData(int storageOffset, int resultSize) {
		setTargetOffset(storageOffset);
		setTargetSize(resultSize);
	}

	void RetreiveScriptFunctionResult::buildCommandText() {
		std::stringstream ss;
		ss << "pop " << getTargetSize() << " " << getTargetOffset();
		setCommandText(ss.str());
	}

	void RetreiveScriptFunctionResult::execute() {
		Context* context = Context::getCurrent();
		int functionResultOffset = context->getCurrentOffset() + context->getCurrentScopeSize();
		int targetOffset = context->getCurrentOffset() + getTargetOffset();

		void* sourceAddress = context->getAbsoluteAddress(functionResultOffset);

		context->write(targetOffset, sourceAddress, getTargetSize());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallFuntion::CallFuntion() : _beginParamOffset(0){}
	CallFuntion::~CallFuntion() {}	
	void CallFuntion::setFunctionName(const std::string& functionName) {
		_functionName = functionName;
	}

	int CallFuntion::getBeginParamOffset() const {
		return _beginParamOffset;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallNativeFuntion::CallNativeFuntion() : _targetFunction(nullptr) {}
	CallNativeFuntion::~CallNativeFuntion() {}
	void CallNativeFuntion::setCommandData(int returnOffset, int beginParamOffset, const DFunction2Ref& targetFunction) {
		setTargetOffset(returnOffset);
		_beginParamOffset = beginParamOffset;
		_targetFunction = targetFunction;
	}

	void CallNativeFuntion::buildCommandText() {
		std::stringstream ss;
		ss << "call " << _functionName << " " << getTargetOffset() << " " << _beginParamOffset;
		setCommandText(ss.str());
	}	

	void CallNativeFuntion::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		//return offset is at begining of function data offset
		int returnOffset = getTargetOffset() + currentOffset;

		//params if any follow by returnOffset
		int beginParamOffset = _beginParamOffset + currentOffset;

		//abosulute addresses will be calculated when the code is run
		void* returnVal = context->getAbsoluteAddress(returnOffset);
		void** params = (void**)context->getAbsoluteAddress(beginParamOffset);

		//call the registered function with prepared params and give the return buffer (returnVal) to function
		//the function will write the result at returnVal
		_targetFunction->call(returnVal, params);

		//Logger::WriteMessage(("native function " + std::to_string(*(int*)returnVal)).c_str());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	FunctionForwarder::FunctionForwarder() {}
	FunctionForwarder::~FunctionForwarder() {}
	void FunctionForwarder::setCommandData(int funtionInfoOffset, int returnOffset, int beginParamOffset, int paramSize) {
		setTargetOffset(returnOffset);
		_beginParamOffset = beginParamOffset;
		_funtionInfoOffset = funtionInfoOffset;
		_paramSize = paramSize;
	}

	void FunctionForwarder::buildCommandText() {
		std::stringstream ss;
		ss << "forward call " << getTargetOffset() << " " << _beginParamOffset;
		setCommandText(ss.str());
	}

	void FunctionForwarder::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		//params if any follow by returnOffset
		int functionInfoOffset = _funtionInfoOffset + currentOffset;

		RuntimeFunctionInfo* runtimeInfo = (RuntimeFunctionInfo*)context->getAbsoluteAddress(functionInfoOffset);
		
		if (runtimeInfo->info.type == RuntimeFunctionType::NativeFunction) {
			CallNativeFuntion callNativeFunction;
			//ref without delete the instance
			DFunction2Ref refFunction( (DFunction2*) runtimeInfo->address, [](DFunction2*) {});
			callNativeFunction.setCommandData(getTargetOffset(), _beginParamOffset, refFunction);
			callNativeFunction.execute();
		}
		else if(runtimeInfo->anoynymousInfo.data == nullptr || runtimeInfo->anoynymousInfo.dataSize == 0) {
			CallScriptFuntion3 callScriptFunction;
			callScriptFunction.setTargetCommand((CommandPointer)runtimeInfo->address);
			callScriptFunction.setCommandData(getTargetOffset(), _beginParamOffset, _paramSize);
			callScriptFunction.execute();
		}
		else {
			CallLambdaFuntion callLambdaFunction(&runtimeInfo->anoynymousInfo);
			callLambdaFunction.setTargetCommand((CommandPointer)runtimeInfo->address);
			callLambdaFunction.setCommandData(getTargetOffset(), _beginParamOffset, _paramSize);
			callLambdaFunction.execute();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallNativeFuntionWithAssitInfo::CallNativeFuntionWithAssitInfo() : _pairCount(0), _pairs(nullptr) {}
	CallNativeFuntionWithAssitInfo::~CallNativeFuntionWithAssitInfo() {		
	}
	void CallNativeFuntionWithAssitInfo::initAssitInfo(int pairCount, int* pairs) {
		_pairCount = pairCount;
		_pairs = pairs;
	}

	void CallNativeFuntionWithAssitInfo::buildCommandText() {
		CallNativeFuntion::buildCommandText();
	}

	void CallNativeFuntionWithAssitInfo::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		int nParam = _pairCount;
		int* pInfo = _pairs;
		int* pEnd = pInfo + nParam * 2;

		void* paramAddress;

		while (pInfo < pEnd)
		{
			//calculate addtional param address - '*pInfo + currentOffset' is additional param offset
			paramAddress = context->getAbsoluteAddress(*pInfo + currentOffset);
			pInfo++;
			
			//calculate param address - now, '*pInfo + currentOffset' is param offset
			context->lea(*pInfo + currentOffset, paramAddress);
			pInfo++;
		}

		CallNativeFuntion::execute();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallDynamicFuntion::CallDynamicFuntion() : 
		_scriptTypes(nullptr), _typeNames(nullptr), _sizes(nullptr) {
	}

	CallDynamicFuntion::~CallDynamicFuntion() {
		if (_scriptTypes) {
			free(_scriptTypes);
		}
		if (_typeNames) {
			for (int i = 0; i < _pairCount; i++) {
				free(_typeNames[i]);
			}
			free(_typeNames);
		}
		if (_sizes) {
			free(_sizes);
		}
	}

	void CallDynamicFuntion::buildCommandText() {
		CallNativeFuntionWithAssitInfo::buildCommandText();
	}

	void CallDynamicFuntion::setParamsType(int* scriptTypes, char** typeNames, int* sizes) {
		_scriptTypes = scriptTypes;
		_typeNames = typeNames;
		_sizes = sizes;
	}

	void CallDynamicFuntion::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();
		int nParam = _pairCount;

		unsigned int totalSizeNeed =
			sizeof(void*) +																		/*space for address of SimpleVariantArray*/
			sizeof(SimpleVariantArray::size) + nParam * sizeof(SimpleVariantArray::elems[0]);	/*space SimpleVariantArray*/
		if (!context->prepareWrite(currentOffset, totalSizeNeed)) {
			return;
		}

		char* baseAddress = (char*)context->getAbsoluteAddress(currentOffset);
		
		//write number of param to first 4 bytes of begin param offset
		SimpleVariantArray* simpleArray = (SimpleVariantArray*)(baseAddress + _beginParamOffset + sizeof(void*));
		simpleArray->size = nParam;

		SimpleVariant* elem = simpleArray->elems;
		SimpleVariant* elemEnd = elem + nParam;
		char* paramAddress;
		int* pInfo = _pairs;
		int* sTypes = _scriptTypes;
		char** typeNames = _typeNames;
		int* sizes = _sizes;

		//write address of simpleArray to begin param offset
		*((size_t*)(baseAddress + _beginParamOffset)) = (size_t)simpleArray;

		while(elem < elemEnd) {
			//calculate addtional param address - '*pInfo + currentOffset' is additional param offset
			paramAddress = baseAddress + *pInfo;

			elem->scriptType = *sTypes;
			elem->typeName = *typeNames;
			elem->size = *sizes;
			*((size_t*)(&elem->pData)) = (size_t)paramAddress;

			pInfo++;
			sTypes++;
			typeNames++;
			sizes++;
			elem++;
		}

		CallNativeFuntion::execute();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallScriptFuntion::CallScriptFuntion() : _targetFunction(nullptr), _paramSize(0) {}
	CallScriptFuntion::~CallScriptFuntion() {}
	void CallScriptFuntion::setCommandData(int resultSize, int beginParamOffset, int paramSize) {
		setTargetSize(resultSize);
		_beginParamOffset = beginParamOffset;
		_paramSize = paramSize;
	}

	void CallScriptFuntion::setTargetCommand(CommandPointer targetFunction) {
		_targetFunction = targetFunction;
	}

	void CallScriptFuntion::buildCommandText() {
		std::stringstream ss;
		ss << "call " << _functionName << " " << _beginParamOffset;
		setCommandText(ss.str());
	}

	void CallScriptFuntion::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		//int returnOffset = _returnOffset + currentOffset;
		int beginParamOffset = _beginParamOffset + currentOffset;

		//void* returnAddress = context->getAbsoluteAddress(returnOffset);
		void* beginParamAddress = context->getAbsoluteAddress(beginParamOffset);

		//initialize scope size of the function, begin is zero
		//the function scope size will be increased by function allocateMemory
		//allocateMemory function will be called when a context scope is entered		
		context->pushScope();

		//after move offset the current offset will change to begin of function memory space
		currentOffset = context->getCurrentOffset();

		//store return address at first block of function
		//context->lea(currentOffset, returnAddress);
		//move parameters's data to local funtion variable
		//first variables are always local function's parameters
		context->write(currentOffset + getTargetSize(), beginParamAddress, _paramSize);
#ifndef THROW_EXCEPTION_ON_ERROR
		if (context->isError()) {
			return;
		}
#endif
		//set command cursor is the previous command of the function
		//to allow the thread context will execute the first command in the next loop
		context->jump(_targetFunction);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallScriptFuntion2::CallScriptFuntion2() : _targetFunction(nullptr), _paramSize(0) {}
	CallScriptFuntion2::~CallScriptFuntion2() {}
	void CallScriptFuntion2::setCommandData(int returnOffset, int beginParamOffset, int paramSize) {
		setTargetOffset(returnOffset);
		_beginParamOffset = beginParamOffset;
		_paramSize = paramSize;
	}

	void CallScriptFuntion2::setTargetCommand(CommandPointer targetFunction) {
		_targetFunction = targetFunction;
	}

	void CallScriptFuntion2::buildCommandText() {
		std::stringstream ss;
		ss << "call " << _functionName << " " << _beginParamOffset;
		setCommandText(ss.str());
	}

	int inline getReturnOffset(Context* context) {
		//return address at first block of function
		return context->getCurrentOffset() + BEGIN_FUNCTION_OFFSET_DATA;
	}

	int inline getBeginParamOffset(Context* context) {
		//move parameters's data to local funtion variable
		//first variables are always local function's parameters
		return getReturnOffset(context) + sizeof(void*);
	}

	void CallScriptFuntion2::execute() {
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		int returnOffset = getTargetOffset() + currentOffset;
		int beginParamOffset = _beginParamOffset + currentOffset;

		void* returnAddress = context->getAbsoluteAddress(returnOffset);
		void* beginParamAddress = context->getAbsoluteAddress(beginParamOffset);

		//initialize scope size of the function, begin is zero
		//the function scope size will be increased by function allocateMemory
		//allocateMemory function will be called when a context scope is entered		
		context->pushScope();

		//after move offset the current offset will change to begin of function memory space
		currentOffset = ffscript::getReturnOffset(context);

		//store return address at first block of function
		context->lea(currentOffset, returnAddress);
		
		currentOffset = ffscript::getBeginParamOffset(context);
		context->write(currentOffset, beginParamAddress, _paramSize);
#ifndef THROW_EXCEPTION_ON_ERROR
		if (context->isError()) {
			return;
		}
#endif
		//set command cursor is the previous command of the function
		//to allow the thread context will execute the first command in the next loop
		context->jump(_targetFunction);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallScriptFuntion3::CallScriptFuntion3(){}	
	
	void CallScriptFuntion3::execute() {
		CallScriptFuntion2::execute();
		Context* context = Context::getCurrent();
		context->runFunctionScript();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallLambdaFuntion::CallLambdaFuntion(AnoynymousDataInfo* data) : _anoynymousInfo(data) {}

	void CallLambdaFuntion::execute() {
		CallScriptFuntion2::execute();

		Context* context = Context::getCurrent();
		auto beginParamOffset = ffscript::getBeginParamOffset(context);
		auto anoynymousDataOffset = beginParamOffset + _paramSize;
		context->write(anoynymousDataOffset, _anoynymousInfo->data, _anoynymousInfo->dataSize);

		context->runFunctionScript();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	Jump::Jump() : _targetCommand(nullptr) {}
	Jump::~Jump() {}
	void Jump::setCommandData(CommandPointer targetCommand) {
		_targetCommand = targetCommand;
	}

	void Jump::buildCommandText() {
		std::stringstream ss;
		ss << "jump " << " " << int_to_hex((size_t)(_targetCommand + 1));
		setCommandText(ss.str());
	}

	void Jump::execute() {
		Context* context = Context::getCurrent();
		context->jump(_targetCommand);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	JumpIf::JumpIf() : _targetCommandTrue(nullptr) , _conditionOffset(0) {}
	JumpIf::~JumpIf() {}
	void JumpIf::setCommandData(int conditionOffset, CommandPointer targetCommand) {
		_conditionOffset = conditionOffset;		
		_targetCommandTrue = targetCommand;
	}

	void JumpIf::buildCommandText() {
		std::stringstream ss;
		ss << "jump " << _conditionOffset << " " <<  int_to_hex((size_t)(_targetCommandTrue + 1));
		setCommandText(ss.str());
	}

	void JumpIf::execute() {
		Context* context = Context::getCurrent();
 		int conditionOffset = _conditionOffset + context->getCurrentOffset();

		bool* conditionValue = (bool*)context->getAbsoluteAddress(conditionOffset);
		//Logger::WriteMessage(("JumpIf " + std::to_string(*conditionValue)).c_str());
		if (*conditionValue) {
			context->jump(_targetCommandTrue);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	JumpIfElse::JumpIfElse() : _targetCommandFalse(nullptr) {}
	JumpIfElse::~JumpIfElse() {}
	void JumpIfElse::setCommandElse(CommandPointer targetCommand) {
		_targetCommandFalse = targetCommand;
	}

	void JumpIfElse::buildCommandText() {
		std::stringstream ss;
		ss << "jump " << _conditionOffset << " " << int_to_hex((size_t)(_targetCommandTrue + 1)) << " " << int_to_hex((size_t)(_targetCommandFalse + 1));
		setCommandText(ss.str());
	}

	void JumpIfElse::execute() {
		Context* context = Context::getCurrent();
		int conditionOffset = _conditionOffset + context->getCurrentOffset();
		bool* conditionValue = (bool*)context->getAbsoluteAddress(conditionOffset);

		if (*conditionValue) {
			context->jump(_targetCommandTrue);
		}
		else {
			context->jump(_targetCommandFalse);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ExitScriptFuntionAtReturn::ExitScriptFuntionAtReturn() : _indexPreventDestructorRun(-1) {}
	ExitScriptFuntionAtReturn::~ExitScriptFuntionAtReturn() {}

	void ExitScriptFuntionAtReturn::buildCommandText() {
		setCommandText("exit function");
	}

	void ExitScriptFuntionAtReturn::setCommandData(int indexPreventDestructorRun) {
		_indexPreventDestructorRun = indexPreventDestructorRun;
	}

	void ExitScriptFuntionAtReturn::execute() {
		if (_indexPreventDestructorRun >= 0) {
			Context* context = Context::getCurrent();
			auto scopeRuntimeData = context->getScopeRuntimeData();
			scopeRuntimeData->markContructorNotExecuted(_indexPreventDestructorRun);
		}
		MultipleCommand::execute();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ExitFunctionAtTheEnd::ExitFunctionAtTheEnd() {}
	ExitFunctionAtTheEnd::~ExitFunctionAtTheEnd() {}
	
	void ExitFunctionAtTheEnd::buildCommandText() {		
		setCommandText("exit function");
	}

	void ExitFunctionAtTheEnd::execute() {
		Context* context = Context::getCurrent();		
		context->popContext();
		context->popScope();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	MultipleCommand::MultipleCommand() {}
	MultipleCommand::~MultipleCommand() {}

	CommandList& MultipleCommand::getCommands() {
		return _commands;
	}

	void MultipleCommand::buildCommandText() {}

	void MultipleCommand::execute() {
		for (auto it = _commands.begin(); it != _commands.end(); it++) {
			(*it)->execute();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////
	BreakCommand::BreakCommand() {}
	BreakCommand::~BreakCommand() {}
	void BreakCommand::buildCommandText() {
		std::stringstream ss;
		ss << "break";
		setCommandText(ss.str());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	ContinueCommand::ContinueCommand() : _loopCommand(nullptr){}
	ContinueCommand::~ContinueCommand() {}
	void ContinueCommand::setLoopCommand(CommandPointer loopCommand) {
		_loopCommand = loopCommand;
	}

	void ContinueCommand::buildCommandText() {
		std::stringstream ss;
		ss << "continue " << int_to_hex((size_t)(_loopCommand + 1));
		setCommandText(ss.str());
	}

	void ContinueCommand::execute() {
		MultipleCommand::execute();
		Context* context = Context::getCurrent();

		context->jump(_loopCommand);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushMemberVariableParam::PushMemberVariableParam() : _accessors(0) {}
	PushMemberVariableParam::~PushMemberVariableParam() {
		if (_accessors) {

			for (auto it = _accessors->begin(); it != _accessors->end(); it++) {
				delete *it;
			}

			delete _accessors;
		}
	}
	void PushMemberVariableParam::setCommandData(std::vector<MemberVariableAccessor*>* accessors, int paramSize, int targetOffset) {
		_accessors = accessors;
		setTargetSize(paramSize);
		setTargetOffset(targetOffset);
	}

	void PushMemberVariableParam::buildCommandText() {
		std::stringstream ss;
		ss << "mov member variable " << getTargetOffset() << " " << getTargetSize();
		setCommandText(ss.str());
	}

	void PushMemberVariableParam::execute() {
		Context* context = Context::getCurrent();
		MemberVariableAccessor** accessors = _accessors->data();
		size_t count = _accessors->size();
		MemberVariableAccessor** end = accessors + count;

		void* address = (*accessors)->access(nullptr);

		for (accessors++; accessors < end; accessors++) {
			address = (*accessors)->access(address);
		}

		int targetOffset = getTargetOffset() + context->getCurrentOffset();
		context->write(targetOffset, address, getTargetSize());
	}

	/////////////////////////////////////////////////////////////////////////////////////
	PushMemberVariableParamRef::PushMemberVariableParamRef() : _accessors(0), TargetedCommand(0, sizeof(void*)) {}
	PushMemberVariableParamRef::~PushMemberVariableParamRef() {
		if (_accessors) {

			for (auto it = _accessors->begin(); it != _accessors->end(); it++) {
				delete *it;
			}

			delete _accessors;
		}
	}
	void PushMemberVariableParamRef::setCommandData(std::vector<MemberVariableAccessor*>* accessors, int targetOffset) {
		_accessors = accessors;
		setTargetOffset(targetOffset);
	}

	void PushMemberVariableParamRef::buildCommandText() {
		std::stringstream ss;
		ss << "lea member variable " << getTargetOffset();
		setCommandText(ss.str());
	}

	void PushMemberVariableParamRef::execute() {
		Context* context = Context::getCurrent();
		MemberVariableAccessor** accessors = _accessors->data();
		size_t count = _accessors->size();
		MemberVariableAccessor** end = accessors + count;

		void* address = (*accessors)->access(nullptr);

		for (accessors++; accessors < end; accessors++) {
			address = (*accessors)->access(address);
		}

		int targetOffset = getTargetOffset() + context->getCurrentOffset();
		context->lea(targetOffset, address);
	}

	/////////////////////////////////////////////////////////////////////////////////////
	CallCreateLambda::CallCreateLambda() : TargetedCommand(0, sizeof(RuntimeFunctionInfo)), _srcDataOffset(0), _destDataOffset(0), _dataSize(0), _anoynymousTargetFunction(nullptr){}
	CallCreateLambda::~CallCreateLambda() {
	}
	void CallCreateLambda::setCommandData(int returnOffset, int dataOffset, int dataSize) {
		setTargetOffset(returnOffset);
		_srcDataOffset = dataOffset;
		_dataSize = dataSize;
	}

	void CallCreateLambda::setLambdaAddress(CommandPointer anoynymousTargetFunction, int destDataOffset) {
		_anoynymousTargetFunction = anoynymousTargetFunction;
		_destDataOffset = destDataOffset;
	}

	void CallCreateLambda::buildCommandText() {
		std::stringstream ss;
		ss << "create lambda";
		setCommandText(ss.str());
	}

	void CallCreateLambda::execute() {		
		Context* context = Context::getCurrent();
		int currentOffset = context->getCurrentOffset();

		//return offset is at begining of function data offset
		int returnOffset = getTargetOffset() + currentOffset;

		int beginDataOffset = _srcDataOffset + currentOffset;

		//abosulute addresses will be calculated when the code is run
		void* returnVal = context->getAbsoluteAddress(returnOffset);
		void* dataAddress = context->getAbsoluteAddress(beginDataOffset);

		RuntimeFunctionInfo* runtimeData = (RuntimeFunctionInfo*)returnVal;
		runtimeData->address = _anoynymousTargetFunction;
		runtimeData->anoynymousInfo.data = malloc(_dataSize);
		runtimeData->anoynymousInfo.targetOffset = _destDataOffset;
		runtimeData->anoynymousInfo.dataSize = _dataSize;
		memcpy_s(runtimeData->anoynymousInfo.data, _dataSize, dataAddress, _dataSize);
	}
}