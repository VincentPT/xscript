#include "stdafx.h"
#include "GlobalScope.h"
#include "ScriptCompiler.h"
#include "Program.h"
#include "ScriptFunction.h"
#include "CodeUpdater.h"
#include "ScriptRunner.h"
#include "CLamdaProg.h"

namespace ffscript {
	GlobalScope::GlobalScope(StaticContext* staticContext, ScriptCompiler* scriptCompiler):
		ScriptScope(scriptCompiler), _errorCompiledChar(nullptr), _beginCompileChar(nullptr)
	{
		_updateLaterMan = new CodeUpdater(this);
		_refContext = false;
		_staticContextRef.reset(staticContext);
	}

	GlobalScope::GlobalScope(int globalMemSize, ScriptCompiler* scriptCompiler) : ScriptScope(scriptCompiler), _errorCompiledChar(nullptr), _beginCompileChar(nullptr) {
		_staticContextRef.reset(new StaticContext(globalMemSize));
		_refContext = true;
		_updateLaterMan = new CodeUpdater(this);
	}
	
	GlobalScope::~GlobalScope(){
		//ScriptCompiler* scriptCompiler = getCompiler();
		//for (int functionId : _registeredFuntions) {
		//	scriptCompiler->unregisterFunction(functionId);
		//}
		if (_refContext == false) {
			_staticContextRef.release();
		}
		delete _updateLaterMan;
	}

	void* GlobalScope::getGlobalAddress(int offset) {
		return _staticContextRef->getAbsoluteAddress(_staticContextRef->getCurrentOffset() + offset);
	}

	void GlobalScope::runGlobalCode() {
		int constructorCount = this->getConstructorCommandCount();
		int dataSize = getDataSize();
		int codeSize = getScopeSize() - dataSize;

		_staticContextRef->pushContext(constructorCount);
		_staticContextRef->scopeAllocate(dataSize, codeSize);

		_staticContextRef->run();

		//_staticContextRef->scopeUnallocate(dataSize, codeSize);
		//_staticContextRef->popContext();
	}

	void GlobalScope::cleanupGlobalMemory() {
		int dataSize = getDataSize();
		int codeSize = getScopeSize() - dataSize;

		_staticContextRef->runDestructorCommands();

		_staticContextRef->scopeUnallocate(dataSize, codeSize);
		_staticContextRef->popContext();
	}

	int GlobalScope::registScriptFunction(const std::string& name, const ScriptType& returnType, const std::vector<ScriptType>& paramTypes) {
		ScriptCompiler* scriptCompiler = this->getCompiler();
		Program* program = scriptCompiler->getProgram();

		ScriptFunctionFactory* scriptFunctionFactory = new ScriptFunctionFactory(name.c_str(), scriptCompiler);
		scriptFunctionFactory->setReturnType(returnType);
		scriptCompiler->takeOwnership(scriptFunctionFactory);
		int functionId = scriptCompiler->registFunction(name, paramTypes, scriptFunctionFactory);
		
		if (functionId >= 0) {
			FunctionInfo functionInfo;
			functionInfo.paramDataSize = scriptFunctionFactory->getParamsDataSize();
			functionInfo.returnStorageSize = scriptCompiler->getTypeSize(returnType);
			program->setFunctionInfo(functionId, functionInfo);

			_registeredFuntions.push_back(functionId);
			program->mapFunction(name, paramTypes, functionId);
		}
		else {
			scriptCompiler->setErrorText("The function '" + name + "' is already exist");
		}
		return functionId;
	}

	CLamdaProg* GlobalScope::detachScriptProgram(Program* program) {
		CLamdaProg* scriptProgram = new CLamdaProg(program);
		if (_refContext == false) {
			throw std::runtime_error("Cannot detach script program from a shared context");
		}
		if (_staticContextRef.get() == nullptr) {
			throw std::runtime_error("Cannot detach script program from a null context");
		}

		if (program == nullptr) {
			throw std::runtime_error("Cannot create script program from a null program");
		}
		int dataSize = getDataSize();
		int codeSize = getScopeSize() - dataSize;

		scriptProgram->_globalCodeSize = codeSize;
		scriptProgram->_globalDataSize = dataSize;
		scriptProgram->_globalConstructorCount = getConstructorCommandCount();

		scriptProgram->setContext(std::shared_ptr<StaticContext>(_staticContextRef.release()));

		auto& variables = getVariables();
		for (auto it = variables.begin(); it != variables.end(); it++) {
			scriptProgram->addVariable( std::shared_ptr<Variable>(it->clone(false)));
		}

		return scriptProgram;
	}
}