#include "stdafx.h"
#include "FunctionScope.h"
#include "Internal.h"
#include "Utils.h"
#include "ScriptCompiler.h"
#include "Program.h"
#include "Supportfunctions.h"
#include "ScriptFunction.h"

namespace ffscript {
	FunctionScope::FunctionScope(ScriptScope* parent, const std::string& name, const ScriptType& returnType) :
		ContextScope(parent, this),
		_name(name),
		_returnType(returnType) {		
	}

	FunctionScope::~FunctionScope(){
	}

	const wchar_t* FunctionScope::parseFunctionParameters(const wchar_t* text, const wchar_t* end, std::vector<ScriptType>& paramTypes) {
		const wchar_t* c;
		const wchar_t* d;
		std::string token1;
		ScriptType type;
		Variable* pVariable;

		c = trimLeft(text, end);
		if (*c != '(') return nullptr;
		c++;
		c = trimLeft(c, end);
		if (*c == ')') {
			return ++c;
		}

		paramTypes.clear();
		decltype(c) e;
		while (c < end) {
			e = c;
			c = parseType(c, end, type);
			if (c == nullptr) {
				break;
			}
			d = trimLeft(c, end);
			c = lastCharInToken(d, end);
			if (*c == 0) {
				return nullptr;
			}

			paramTypes.push_back(type);

			if (c == d) {
				pVariable = registVariable();				
			}
			else {
				token1 = convertToAscii(d, c - d);
				pVariable = registVariable(token1);
			}
			if (pVariable == nullptr) {
				return nullptr;
			}

			pVariable->setDataType(type);
			pVariable->setGroupType(VariableGroupType::FuntionParameter);

			c = trimLeft(c, end);
			if (*c == ')') {
				c++;
				break;
			}
			if (*c != ',') {
				c = lastCharInToken(c, end);
				token1.resize(c - e);
				token1.assign(e, c);
				getCompiler()->setErrorText("invalid function parameter '" + token1  + "'");
				c = nullptr;
				break;
			}
			c++;
		}

		return c;
	}

	const wchar_t* FunctionScope::parse(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;

		//register auto variable to store return value
		Variable* returnDataStorage = registVariable();
		//check later to know need to call constructor for return variable or not - check later
		//CheckVariableToRunConstructor(returnDataStorage);
#if USE_DIRECT_COPY_FOR_RETURN
		returnDataStorage->setDataType(_returnType.makeRef());
#else
		returnDataStorage->setDataType(_returnType);
#endif

		std::vector<ScriptType> paramTypes;
		paramTypes.reserve(8);
		c = parseFunctionParameters(text, end, paramTypes);

		if (c == nullptr) {
			return nullptr;
		}

		return parseBody(c, end, _returnType, paramTypes);
	}

	const wchar_t* FunctionScope::parseBody(const wchar_t* text, const wchar_t* end, const ScriptType& returnType, const std::vector<ScriptType>& paramTypes) {
		_functionId = ((GlobalScope*)getParent())->registScriptFunction(_name, returnType, paramTypes);

		if (_functionId < 0) {
			return nullptr;
		}

		//EnterFunction* enterFunction = new EnterFunction(this);
		//putCommandUnit(enterFunction);

		auto c = ContextScope::parse(text, end);

		if (c != nullptr) {
			//get exit context scope command to update restore call flag value to flase
			//if we do not do this, the command exit function will never be executed
			//last command of a context scope must be exit scope command
			CommandConstRefIter lastCommand = getLastExpression();

			ExitScopeBuilder* exitContextScope = (ExitScopeBuilder*)(*lastCommand).get();
			exitContextScope->setRestoreCallFlag(false);

			ExitFunctionBuilder* exitFunction = new ExitFunctionBuilder();
			putCommandUnit(exitFunction);
		}
		return c;
	}

	bool FunctionScope::extractCode(Program* program) {
		bool res = ContextScope::extractCode(program);
		return res;
	}

	int FunctionScope::getFunctionId() const {
		return _functionId;
	}

	const std::string& FunctionScope::getName() const {
		return _name;
	}

	bool FunctionScope::updateCodeForControllerCommands(Program* program) {
		bool res = ContextScope::updateCodeForControllerCommands(program);
		if (res) {
			program->setFunctionPlainCode(getFunctionId(), *getCode());
		}
		return res;
	}

	///////////////////////////////////////////////////////////////////////
	AnonymousFunctionScope::AnonymousFunctionScope(ScriptScope* parent, const std::list<ExecutableUnitRef>& captureList) :
	FunctionScope(parent, "", ScriptType()),
		_captureList(&captureList)
		{ }

	AnonymousFunctionScope::~AnonymousFunctionScope() {

	}

	const wchar_t* AnonymousFunctionScope::parse(const wchar_t* text, const wchar_t* end) {
		//register auto variable to store return value
		Variable* returnDataStorage = registVariable();
		//check later to know need to call constructor for return variable or not - check later
		//CheckVariableToRunConstructor(returnDataStorage);

		std::vector<ScriptType> paramTypes;
		paramTypes.reserve(8);
		auto c = parseFunctionParameters(text, end, paramTypes);

		if (c == nullptr) {
			return nullptr;
		}
		c = trimLeft(c, end);
		if (c >= end) return nullptr;

		ScriptCompiler* scriptCompiler = getCompiler();
		if (*c == '{') {
			_returnType.updateType(scriptCompiler, "void");
		}
		else if(end - c > 2 && *c == '-'&& *(c+1) == '>') { // compare with specifier "->"
			c += 2;
			auto d = trimLeft(c, end);
			if (d >= end) return nullptr;
			c = lastCharInToken(d, end);
			string sType = convertToAscii(d, c - d);
			_returnType.updateType(scriptCompiler, sType);
		}
		else {
			scriptCompiler->setErrorText("invalid lambda return type expression");
			return nullptr;
		}
		if (_returnType.isUnkownType()) {
			scriptCompiler->setErrorText("Unknown type: '" + _returnType.sType() + "'");
			return nullptr;
		}
		returnDataStorage->setDataType(_returnType.makeRef());
		auto functionCount = scriptCompiler->getFunctionLib()->getFunctionCount();
		_name = "_anoynymous_" + std::to_string(functionCount);

		//now this time to push capture list to anoynymous function's body
		if (_captureList) {
			auto& params = *_captureList;
			//check if the parameter are variables, because last param is constant(function id of anoynymous function)
			for (auto it = params.begin(); it != params.end(); it++) {
				auto& paramUnit = *it;
				CXOperand* operand = nullptr;
				bool semiRef = false;

				if (paramUnit->getType() != EXP_UNIT_ID_XOPERAND) {
					if (paramUnit->getType() != EXP_UNIT_ID_SEMI_REF) {
						scriptCompiler->setErrorText("Capture list only allow variables, not expression");
						return nullptr;
					}
					if (((Function*)paramUnit.get())->getChild(0)->getType() != EXP_UNIT_ID_XOPERAND) {
						scriptCompiler->setErrorText("Capture list only allow variables, not expression");
						return nullptr;
					}

					operand = (CXOperand*)((Function*)paramUnit.get())->getChild(0).get();
					semiRef = true;
				}
				else {
					operand = (CXOperand*)it->get();
				}
				
				auto& variableType = operand->getReturnType();
				if (variableType.isUnkownType()) {
					scriptCompiler->setErrorText("Undefined variable '" + operand->toString() + "'");
					return nullptr;
				}

				auto variable = registVariable(operand->toString());
				if(semiRef) variable->setDataType(variableType.makeSemiRef());
				else  variable->setDataType(variableType);
			}
		}

		return parseBody(c, end, _returnType, paramTypes);
	}
}