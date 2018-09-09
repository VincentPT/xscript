/******************************************************************
* File:        GlobalScope.cpp
* Description: implement GlobalScope class, parsing script part.
*              A class used to parse the script of a program. It
*              stores all global variables and command builders of
*              the whole program.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "stdafx.h"
#include "GlobalScope.h"
#include "ScriptCompiler.h"
#include "Utils.h"
#include "Internal.h"
#include "FunctionScope.h"
#include "Program.h"
#include "ExpUnitExecutor.h"
#include "CodeUpdater.h"
#include "Supportfunctions.h"
#include "ControllerExecutor.h"
#include "ContextScope.h"
#include "StructClass.h"
#include "ScopedCompilingScope.h"

#include <string>

namespace ffscript {

	CodeUpdater* GlobalScope::getCodeUpdater() const {
		return _updateLaterMan;
	}

	const wchar_t* GlobalScope::parseStruct(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		std::string token;
		ScriptCompiler* scriptCompiler = getCompiler();

		d = trimLeft(text, end);
		c = lastCharInToken(d, end);

		//token 1 is expected as a name of struct
		token = convertToAscii(d, c - d);
		//name cannot be empty
		if (token.size() == 0) {
			setErrorCompilerChar(d);
			return nullptr;
		}
		//move to next token
		d = trimLeft(c, end);
		//expect an struct begin with char '{' after the name
		if (d >= end || *d != '{') {
			scriptCompiler->setErrorText("Missing '{'");
			setErrorCompilerChar(d);
			return nullptr;
		}

		StructClass* aStruct = new StructClass(getCompiler(), token);
		//move to the next char after '{'
		c = d + 2;
		while (c < end && *c != '}')
		{
			ScriptType type;
			c = this->parseType(c, end, type);
			if (c == nullptr) {
				break;
			}

			d = trimLeft(c, end);
			c = lastCharInToken(d, end);
			//token1 is expected as member name
			token = convertToAscii(d, c - d);
			//name cannot be empty
			if (token.size() == 0) {
				scriptCompiler->setErrorText("Missing member name");
				setErrorCompilerChar(c);
				c = nullptr;
				break;
			}
			if (ScriptCompiler::isCommandBreakSign(*c)) {
				aStruct->addMember(type, token);
				c++;
				c = trimLeft(c, end);
			}
			else
			{
				scriptCompiler->setErrorText("Missing ';'");
				setErrorCompilerChar(c);
				c = nullptr;
				break;
			}
		}

		//register the struct as a type to compiler
		if (c != nullptr) {
			if (*c != '}') {
				scriptCompiler->setErrorText("Missing '}'");
				setErrorCompilerChar(c);
				c = nullptr;
			}
			else {
				int type = scriptCompiler->registStruct(aStruct);
				if (IS_UNKNOWN_TYPE(type)) {
					scriptCompiler->setErrorText("Register struct " + aStruct->getName() + " failed");
					setErrorCompilerChar(c);
					c = nullptr;
				}
			}
		}

		if (c == nullptr) {
			delete aStruct;
		}
		setErrorCompilerChar(c);
		return c;
	}

	const wchar_t* GlobalScope::detectKeyword(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		std::string token;

		static const std::string k_struct("struct");

		d = trimLeft(text, end);
		setErrorCompilerChar(d);
		if (d == end) {
			return d;
		}
		c = lastCharInToken(d, end);

		//token 1 is expected as a data type
		token = convertToAscii(d, c - d);
		
		if (k_struct == token) {
			setErrorCompilerChar(c);
			return this->parseStruct(c, end);
		}
		//else if (k_struct == token1) {

		//}
		
		return text;
	}

	const wchar_t* GlobalScope::parseAnonymous(const wchar_t* text, const wchar_t* end, const std::list<ExecutableUnitRef>& captureList, int& functionId) {
		AnonymousFunctionScope* functionScope = new AnonymousFunctionScope(this, captureList);
		auto c = functionScope->parse(text, end);
		if (c) {
			functionId = functionScope->getFunctionId();
		}
		else {
			functionId = -1;
		}
		return c;
	}

	const wchar_t* GlobalScope::parse(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		const wchar_t* e;
		std::string token1;
		int iRes = 0;
		Variable* pVariable;

		_beginCompileChar = text;

		unique_ptr<WCHAR, std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)text, [this, &c](WCHAR*) {
			setErrorCompilerChar(c);
		});

		ScriptCompiler* scriptCompiler = getCompiler();
		Program* program = scriptCompiler->getProgram();
		CodeUpdater* updateLater = getCodeUpdater();
		updateLater->clear();

		/* int a */
		/* int sum(int a, int b)*/

		c = text;
		while (c < end) {

			d = this->detectKeyword(c, end);
			if (d == nullptr) {
				c = d;
				break;
			}
			else if(d > c) {
				if (d == end) {
					break;
				}
				c = d + 1;
				continue;
			}

			ScriptType type;
			d = this->parseType(c, end, type);
			if (d != nullptr) {
				c = d;
			}

			d = trimLeft(c, end);
			//store expression
			e = d;
			c = lastCharInToken(d, end);
			//token1 is expected as variable name or function name
			token1 = convertToAscii(d, c - d);

			c = trimLeft(c, end);
			if (*c == 0) {
				return nullptr;
			}
			if (ScriptCompiler::isCommandBreakSign(*c)) {
				pVariable = registVariable(token1);
				if (pVariable == nullptr) {
					//variable is already exist
					break;
				}
				if (type.isUnkownType()) {
					pVariable->setDataType(type);
					scriptCompiler->setErrorText("cannot determine the data type of variable " + token1);
					return nullptr;
				}
				pVariable->setDataType(type);
				// use x operand unit to store variable and source char index then
				// the function checkVariableToRunConstructor will use it to set setSourceCharIndex
				// for some generated units if necessary
				CXOperand xOperand(this, pVariable);
				xOperand.setSourceCharIndex((int)(d - text));
				checkVariableToRunConstructor(&xOperand);
				c++;
				continue;
			}

			if (*c == '=' ) {
				pVariable = findVariable(token1);
				// check if variable is already declared
				if (pVariable) {
					// check if type is recognized then the variable is delclared again...
					if (!type.isUnkownType()) {
						c = nullptr;
						scriptCompiler->setErrorText("variable '" + token1 + "' is already declared");
						break;
					}
					// ...if not, this is just a normal expression
					c = parseExpression(e, end);
					if (c == nullptr) {
						break;
					}
					if (ScriptCompiler::isCommandBreakSign(*c) == false) {
						scriptCompiler->setErrorText("missing ';'");
						setErrorCompilerChar(c);
						return nullptr;
					}
					c++;
					continue;
				}
				else if (!type.isUnkownType()) {
					pVariable = registVariable(token1);
					pVariable->setDataType(type);
				} 

				c = parseDeclaredExpression(e, end);
				if (c == nullptr) {
					//parse expression failed
					break;
				}
			}
			else {
				d = c;
				
				if (type.isUnkownType()) {
					c = parseExpression(e, end);
					if (c == nullptr) {
						break;
					}
					if (ScriptCompiler::isCommandBreakSign(*c) == false) {
						scriptCompiler->setErrorText("missing ';'");
						setErrorCompilerChar(c);
						return nullptr;
					}
				}
				else if (*c == '(') {
					FunctionScope* functionScope = new FunctionScope(this, token1, type);
					std::vector<ScriptType> paramTypes;
					std::string errorCompileOfFunction;
					// try to parse the text as a function header ...
					if ((c = functionScope->parseHeader(d, end, paramTypes))) {
						// ...if success, continue to parse body function
						if ((c = functionScope->parseBody(c, end, functionScope->getReturnType(), paramTypes))) {
							// parse the body success
							continue;
						}
						// parse the body failed
						break;
					}
					else {
						errorCompileOfFunction = scriptCompiler->getLastError();
					}
					// ...if not success, try to parse the text as an expression
					c = parseExpression(e, end);
					if (c == nullptr) {
						std::string error("tried compiling as function failed: ");
						error.append(errorCompileOfFunction);
						error.append(1, '\n');
						error.append("tried compiling as expression failed: ");
						error.append(scriptCompiler->getLastError());
						scriptCompiler->setErrorText(error);
						break;
					}
					if (ScriptCompiler::isCommandBreakSign(*c) == false) {
						scriptCompiler->setErrorText("missing ';'");
						setErrorCompilerChar(c);
						return nullptr;
					}
				}
				else {
					c = parseExpression(e, end);
					if (c == nullptr) {
						break;
					}
					if (ScriptCompiler::isCommandBreakSign(*c) == false) {
						scriptCompiler->setErrorText("missing ';'");
						setErrorCompilerChar(c);
						return nullptr;
					}
				}
			}
			c++;			
		}

		//auto lastErrorCompileChar = getErrorCompiledChar();

		//int typeVoid = scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID;
		//ScriptType voidType(typeVoid, scriptCompiler->getType(typeVoid));
		//FunctionScope* cleanupFunctionScope = new FunctionScope(this, "__globalScopeCleanupFunction__", voidType);
		//static wchar_t emptyParamHeade r[] = L"()";

		//// this variable is required to use parseHeader but it never changed
		//// because of we use empty param header above
		//static std::vector<ScriptType> paramTypes;
		//if (cleanupFunctionScope->parseHeader(&emptyParamHeader[0], &emptyParamHeader[2], paramTypes) == nullptr) {
		//	throw std::runtime_error("parse an empty function header shoud be success");
		//}
		//if (paramTypes.size() > 0) {
		//	throw std::runtime_error("parse an empty function header but there is parameter is the list");
		//}
		//static wchar_t emptyBody[] = L"{}";

		//// move destructors of global scope to clean up function of global scope
		//cleanupFunctionScope->setParseBodyEventHandler([this](auto contextScope, auto eventType) {
		//	if (eventType == ContextScopeParseEvent::AfterParseBody) {
		//		auto desctructorList = getDestructorList();
		//		for (auto it = desctructorList->begin(); it != desctructorList->end(); it++) {
		//			//auto& destructorUnit = *it;
		//			contextScope->putCommandUnit(*it);
		//		}
		//		desctructorList->clear();
		//	}
		//});
		//if (cleanupFunctionScope->parseBody(&emptyBody[0], &emptyBody[2], cleanupFunctionScope->getReturnType(), paramTypes) == nullptr) {
		//	throw std::runtime_error("parse an empty function body shoud be success");
		//}
		//
		//_cleanupFunctionOfGlobalScope = cleanupFunctionScope->getFunctionId();

		//setErrorCompilerChar(lastErrorCompileChar, true);
		return c;
	}

	inline bool extractCodeForUnit(Program* program, GlobalScope* scope, const CommandUnitRef& commandUnit, std::list<Executor*>& excutorContainer) {
		const ExecutableUnitRef& extUnit = dynamic_pointer_cast<ExecutableUnit>(commandUnit);

		if (extUnit) {
			ExpUnitExecutor * pExecutor = new ExpUnitExecutor(scope);
			if (pExecutor->extractCode(scope->getCompiler(), extUnit) == false) {
				return false;
			}
			excutorContainer.push_back(pExecutor);
			program->addExecutor((ExecutorRef)(pExecutor));
		}
		else {
			CommandBuilder* controllerUnit = (CommandBuilder*)commandUnit.get();
			Executor* pExecutor = controllerUnit->buildNativeCommand();
			excutorContainer.push_back(pExecutor);
			program->addExecutor((ExecutorRef)(pExecutor));
		}

		return true;
	}

	bool GlobalScope::extractCode(Program* program) {

		updateVariableOffset();

		int expressionCount = this->getExpressionCount();
		std::list<Executor*> globalExcutors;
		for (auto it = getFirstExpression(); expressionCount > 0; ++it, --expressionCount) {
			const CommandUnitRef& commandUnit = *it;
			if (extractCodeForUnit(program, this, commandUnit, globalExcutors) == false) {
				return false;
			}
		}

		std::list<Executor*> destructionExcutors;
		auto destructorUnits = getDestructorList();
		for (auto it = destructorUnits->begin(); it != destructorUnits->end(); it++) {
			const CommandUnitRef& commandUnit = *it;
			if (extractCodeForUnit(program, this, commandUnit, destructionExcutors) == false) {
				return false;
			}
		}

		const ScopeRefList& children = getChildren();
		for (auto it = children.begin(); it != children.end(); ++it) {
			if ((*it)->extractCode(program) == false) return false;
		}

		program->convertToPlainCode();

		ContextScope* contextScope;
		for (auto it = children.begin(); it != children.end(); ++it) {
			contextScope = dynamic_cast<ContextScope*>((*it).get());
			if (contextScope) {
				if (contextScope->updateCodeForControllerCommands(program) == false) {
					return false;
				}
			}
		}

		getCodeUpdater()->runUpdate();

		CommandPointer beginCommand;
		CommandPointer endCommand; 

		CodeSegmentEntry* pExcutorCodeEntry;
		for (auto it = globalExcutors.begin(); it != globalExcutors.end(); ++it) {
			pExcutorCodeEntry = program->getCode(*it);
			if (pExcutorCodeEntry) {
				beginCommand = pExcutorCodeEntry->first;
				endCommand = pExcutorCodeEntry->second;

				for (CommandPointer commandPointer = beginCommand; commandPointer <= endCommand; ++commandPointer) {
					_staticContextRef->addCommand(commandPointer);
				}
			}
		}

		for (auto it = destructionExcutors.begin(); it != destructionExcutors.end(); ++it) {
			pExcutorCodeEntry = program->getCode(*it);
			if (pExcutorCodeEntry) {
				beginCommand = pExcutorCodeEntry->first;
				endCommand = pExcutorCodeEntry->second;

				for (CommandPointer commandPointer = beginCommand; commandPointer <= endCommand; ++commandPointer) {
					_staticContextRef->addDestructorCommand(commandPointer);
				}
			}
		}

		return true;
	}

	int GlobalScope::correctAndOptimize(Program* program) {
		const ScopeRefList& children = getChildren();
		int iRes = 0;
		for (auto it = children.begin(); it != children.end() && iRes == 0; ++it) {			
			iRes = (*it)->correctAndOptimize(program);
		}

		return iRes;
	}

	const WCHAR* GlobalScope::getErrorCompiledChar() const {
		return _errorCompiledChar;
	}

	const WCHAR* GlobalScope::getBeginCompileChar() const {
		return _beginCompileChar;
	}

	void GlobalScope::setErrorCompilerChar(const WCHAR* c, bool force) {
		if (force) {
			_errorCompiledChar = c;
		}
		else if (c != nullptr) {
			if (c > _errorCompiledChar) {
				_errorCompiledChar = c;
			}
		}
	}

	void GlobalScope::setErrorCompilerCharIndex(int idx) {
		if (idx >= 0 && _beginCompileChar != nullptr) {
			_errorCompiledChar = _beginCompileChar + idx;
		}
	}

	void GlobalScope::setBeginCompileChar(const WCHAR* c) {
		_beginCompileChar = c;
	}

	void GlobalScope::convertSourceCharIndexToGlobal(const WCHAR* source, std::list<ExpUnitRef>& units) {
		int offset = (int)(source - _beginCompileChar);
		for (auto it = units.begin(); it != units.end(); it++) {
			auto unit = it->get();
			if (ISFUNCTION(unit)) {
				ExpressionParser::recursiveOffsetSourceCharIndex((Function*)unit, offset);
			}
			else if (unit->getSourceCharIndex() >= 0) {
				unit->setSourceCharIndex(unit->getSourceCharIndex() + offset);
			}
		}
	}
}