#include "stdafx.h"
#include "GlobalScope.h"
#include "ScriptCompiler.h"
#include "Program.h"
#include "ScriptFunction.h"
#include "CodeUpdater.h"

namespace ffscript {
	GlobalScope::GlobalScope(StaticContext* staticContext, ScriptCompiler* scriptCompiler): ScriptScope(scriptCompiler)
	{
		_updateLaterMan = new CodeUpdater(this);
		_refContext = false;
		_staticContextRef.reset(staticContext);
	}

	GlobalScope::GlobalScope(int globalMemSize, ScriptCompiler* scriptCompiler) : ScriptScope(scriptCompiler) {
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
		_staticContextRef->run();
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
}