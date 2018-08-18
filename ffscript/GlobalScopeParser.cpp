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
			setLastCompilerChar(d);
			return nullptr;
		}
		//move to next token
		d = trimLeft(c, end);
		//expect an struct begin with char '{' after the name
		if (d >= end || *d != '{') {
			setLastCompilerChar(d);
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
				setLastCompilerChar(c);
				c = nullptr;
				break;
			}
			if (ScriptCompiler::isCommandBreakSign(*c)) {
				aStruct->addMember(type, token);
				c++;
			}
			else
			{
				scriptCompiler->setErrorText("Missing ';'");
				setLastCompilerChar(c);
				c = nullptr;
				break;
			}
		}

		//register the struct as a type to compiler
		if (c != nullptr) {
			if (*c != '}') {
				scriptCompiler->setErrorText("Missing '}'");
				setLastCompilerChar(c);
				c = nullptr;
			}
			else {
				int type = scriptCompiler->registStruct(aStruct);
				if (IS_UNKNOWN_TYPE(type)) {
					scriptCompiler->setErrorText("Register struct " + aStruct->getName() + " failed");
					setLastCompilerChar(c);
					c = nullptr;
				}
			}
		}

		if (c == nullptr) {
			delete aStruct;
		}
		setLastCompilerChar(c);
		return c;
	}

	const wchar_t* GlobalScope::detectKeyword(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		std::string token;

		static const std::string k_struct("struct");

		d = trimLeft(text, end);
		if (d == end) {
			return d;
		}
		c = lastCharInToken(d, end);

		//token 1 is expected as a data type
		token = convertToAscii(d, c - d);
		
		if (k_struct == token) {
			return this->parseStruct(c, end);
		}
		//else if (k_struct == token1) {

		//}
		
		return nullptr;
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

		ScriptCompiler* scriptCompiler = getCompiler();
		Program* program = scriptCompiler->getProgram();
		CodeUpdater* updateLater = getCodeUpdater();
		updateLater->clear();

		/* int a */
		/* int sum(int a, int b)*/

		c = text;
		while (c < end) {

			d = this->detectKeyword(c, end);
			if (d != nullptr) {
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
				checkVariableToRunConstructor(pVariable);
				c++;
				continue;
			}

			if (*c == '=' ) {
				pVariable = findVariable(token1);
				//pVariable = registVariable(token1);
				if (pVariable) {
					//variable is already exist but is declared again
					if (!type.isUnkownType()) {
						c = nullptr;
						scriptCompiler->setErrorText("variable '" + token1 + "' is already declared");
						break;
					}
				}
				else if (!type.isUnkownType()) {
					pVariable = registVariable(token1);
					pVariable->setDataType(type);
				} 

				// begin replace
				// begin remove
				// c = parseExpression(e, end);
				// end remove
				// begin add
				std::list<ExpUnitRef> unitList;
				{
					ScopedCompilingScope autoScope(scriptCompiler, this);
					ExpressionParser parser(getCompiler());
					EExpressionResult eResult = E_FAIL;
					c = parser.readExpression(d, end, eResult, unitList);

					if (eResult != E_SUCCESS || c == nullptr) {
						return nullptr;
					}
					if (unitList.size() == 0) {
						scriptCompiler->setErrorText("incompleted expression");
						return nullptr;
					}
					if (unitList.size() >= 2) {
						auto it = unitList.begin();
						auto& firstUnit = *it++;
						auto& secondtUnit = *it++;
						if (firstUnit->getType() == EXP_UNIT_ID_XOPERAND && secondtUnit->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT) {
							auto xOperand = unitList.front();
							MaskType mask = (xOperand->getMask() | UMASK_DECLAREINEXPRESSION);
							firstUnit->setMask(mask);

							auto operatorEntry = scriptCompiler->findPredefinedOperator(DEFAULT_COPY_OPERATOR);
							auto defaultAssigmentUnit = new DynamicParamFunction(operatorEntry->name, operatorEntry->operatorType, operatorEntry->priority, operatorEntry->maxParam);
							secondtUnit.reset(defaultAssigmentUnit);
						}
					}
				}
				if (parseExpressionInternal(unitList) != E_SUCCESS) {
					c = nullptr;
				}
				// end add
				// end replace
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
				}
				else if (*c == '(') {
					FunctionScope* functionScope = new FunctionScope(this, token1, type);
					std::vector<ScriptType> paramTypes;
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
					// ...if not success, try to parse the text as an expression
					c = parseExpression(e, end);
					if (c == nullptr) {
						break;
					}
				}
				else {
					c = parseExpression(e, end);
					if (c == nullptr) {
						break;
					}
				}
			}
			c++;			
		}
		return c;
	}

	bool GlobalScope::extractCode(Program* program) {

		updateVariableOffset();

		int expressionCount = this->getExpressionCount();
		std::list<Executor*> globalExcutors;
		for (auto it = getFirstExpression(); expressionCount > 0; ++it, --expressionCount) {
			const CommandUnitRef& commandUnit = *it;
			const ExecutableUnitRef& extUnit = dynamic_pointer_cast<ExecutableUnit>(commandUnit);
			
			if(extUnit) {
				ExpUnitExecutor * pExecutor = new ExpUnitExecutor(this);
				if (pExecutor->extractCode(getCompiler(), extUnit) == false) {
					return false;
				}
				globalExcutors.push_back(pExecutor);
				program->addExecutor( (ExecutorRef)(pExecutor));
			}
			else {
				CommandBuilder* controllerUnit = (CommandBuilder*)commandUnit.get();
				Executor* pExecutor = controllerUnit->buildNativeCommand();
				globalExcutors.push_back(pExecutor);
				program->addExecutor((ExecutorRef)(pExecutor));
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

	const WCHAR* GlobalScope::getLastCompileChar() const {
		return _lastCompileChar;
	}

	void GlobalScope::setLastCompilerChar(const WCHAR* c) {
		if (c != nullptr) {
			if (c > _lastCompileChar) {
				_lastCompileChar = c;
			}
		}
	}
}