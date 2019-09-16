#include "LoopScope.h"
#include "ContextScope.h"
#include "Internal.h"
#include "Utils.h"
#include "ScriptCompiler.h"
#include "FunctionScope.h"
#include "Supportfunctions.h"
#include "ExpUnitExecutor.h"
#include "ControllerExecutor.h"
#include "CodeUpdater.h"
/******************************************************************
* File:        ContextScope.cpp
* Description: implement ContextScope class. A class use to compile
*              codes in a specific scope other than global scope.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "FunctionFactory.h"
#include "BasicType.h"
#include "ObjectBlock.hpp"
#include "Program.h"
#include "RefFunction.h"
#include "ScopedCompilingScope.h"
#include "DestructorContextScope.h"

namespace ffscript {
	extern std::string key_while;
	extern std::string key_if;
	extern std::string key_else;
	extern std::string key_elseIf;
	extern std::string key_return;
	extern std::string key_break;
	extern std::string key_continue;
	extern std::string key_pointer;
	extern std::string key_array;

	ContextScope::ContextScope(ScriptScope* parent, FunctionScope* functionScope) : 
		_functionScope(functionScope),
		_loopScope(nullptr),
		ScriptScope(parent->getCompiler()),
		_beginExitScopeUnit(nullptr),
		_beginExitScopeCommand(nullptr)
	{
		this->setParent(parent);
		parent->addChild(this);
	}


	ContextScope::~ContextScope() {
	}

	void ContextScope::setLoopScope(LoopScope* loopScope) {
		_loopScope = loopScope;
	}

	LoopScope* ContextScope::getLoopScope() const {
		return _loopScope;
	}

	FunctionScope* ContextScope::getFunctionScope() const {
		return _functionScope;
	}

	void ContextScope::setName(const std::string& name) {
		_name = name;
	}

	const wchar_t* ContextScope::parseCondition(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		unique_ptr<WCHAR, std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)text, [this, &c](WCHAR*) {
			((GlobalScope*)getRoot())->setErrorCompilerChar(c);
		});

		ScriptCompiler* scriptCompiler = this->getCompiler();

		c = trimLeft(text, end);
		if (*c != '(') {
			return nullptr;
		}
		int openBracket = 1;
		c++;
		d = c;
		while (c < end && openBracket) {
			if (*c == '(') {
				openBracket++;
			}
			else if (*c == ')') {
				openBracket--;
			}
			c++;
		}
		if (openBracket != 0) {
			scriptCompiler->setErrorText("missing close bracket");
			return nullptr;
		}

		auto iTypeBool = scriptCompiler->getTypeManager()->getBasicTypes().TYPE_BOOL;
		ScriptType expectedReturnTypeOfCondition(iTypeBool, scriptCompiler->getType(iTypeBool));

		int commandCount1 = getCommandUnitCount();
		parseExpressionInternal(d, c - 1, &expectedReturnTypeOfCondition);
		int commandCount2 = getCommandUnitCount();

		if (commandCount2 - commandCount1 != 1) {
			if (scriptCompiler->getLastError().size() == 0) {
				scriptCompiler->setErrorText("invalid condition expression");
			}
			return nullptr;
		}		
		return c;
	}

	const wchar_t* ContextScope::parseIf(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		const wchar_t* e;
		std::string token1;
		std::string token2;

		unique_ptr<WCHAR, std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)text, [this, &c](WCHAR*) {
			((GlobalScope*)getRoot())->setErrorCompilerChar(c);
		});

		c = trimLeft(text, end);
		c = parseCondition(c, end);
		if (c == nullptr) {
			return nullptr;
		}

		CommandConstRefIter conditionRef = getLastCommandUnitRefIter();
		CodeUpdater::getInstance(this)->setUpdateInfo(conditionRef->get(), nullptr);

		IfCommandBuilder* ifCommand = new IfCommandBuilder(this);
		ifCommand->setConditionExpression(conditionRef->get());
		putCommandUnit(ifCommand);

		ContextScope* ifScope = new ContextScope(this, _functionScope);
		ifScope->setLoopScope(_loopScope);
		ifScope->setName(key_if);

		c = ifScope->parse(c, end);
		if (c == nullptr) {
			return nullptr;
		}

		ifCommand->setIfScope(ifScope);

		d = trimLeft(c, end);
		c = lastCharInToken(d,end);
		token1 = convertToAscii(d, c - d);
		bool finished = false;
		if (token1 == key_else) {

			//detecting else if scope
			if (*c == ' ') {
				e = lastCharInToken(c + 1, end);
				token2 = convertToAscii(d, e - d);
				if (token2 == key_elseIf) {
					ContextScope* elseScope = new ContextScope(this, _functionScope);
					elseScope->setLoopScope(_loopScope);
					ifCommand->setElseScope(elseScope);

					EnterScopeBuilder* enterScope = new EnterScopeBuilder(elseScope);
					elseScope->putCommandUnit(enterScope);

					c = elseScope->parseIf(e, end);
					if (c == nullptr) {
						return nullptr;
					}

					elseScope->applyExitScopeCommand();

					finished = true;
				}
			}

			if (!finished) {
				ContextScope* elseScope = new ContextScope(this, _functionScope);
				elseScope->setLoopScope(_loopScope);
				elseScope->setName(key_if);
				c = elseScope->parse(c, end);
				if (c == nullptr) {
					return nullptr;
				}

				ifCommand->setElseScope(elseScope);
			}
		}
		else {
			//no else scope found
			c = d;
		}

		return c;
	}

	const wchar_t* ContextScope::parseWhile(const wchar_t* text, const wchar_t* end) {		
		LoopScope* loopScope = new LoopScope(this, getFunctionScope());
		return loopScope->parse(text, end);
	}

	//bool ContextScope::tryApplyConstructorForDeclarationExpression(Variable* pVariable, std::list<ExpUnitRef>& unitList, const ScriptType* expectedReturnType, EExpressionResult& eResult) {
	//	ScriptCompiler* scriptCompiler = getCompiler();
	//	ScopedCompilingScope autoScope(scriptCompiler, this);
	//	ExpressionParser parser(getCompiler());

	//	eResult = EE_SUCCESS;

	//	std::list<ExpressionRef> expList;
	//	bool res = parser.compile(unitList, expList);
	//	if (res == false) {
	//		eResult = EE_TYPE_UNKNOWN;
	//		return false;
	//	}

	//	if (expList.size() > 1) {
	//		scriptCompiler->setErrorText("multi expression cannot begin by a declaration expresion");
	//		eResult = EE_FAIL;
	//		return false;
	//	}

	//	CandidateCollectionRef candidates = std::make_shared<CandidateCollection>();
	//	eResult = parser.link(expList.front().get(), candidates);

	//	if (eResult != EE_SUCCESS) {
	//		return false;
	//	}
	//	if (candidates->size() == 0) {
	//		scriptCompiler->setErrorText("unknown error");
	//		eResult = EE_TYPE_UNKNOWN;
	//		return false;
	//	}
	//	auto& firstCandidate = candidates->front();
	//	ExecutableUnitRef& param2 = firstCandidate;

	//	auto& param1Type = pVariable->getDataType();
	//	auto& param2Type = param2->getReturnType();

	//	ExecutableUnitRef param1 = ExecutableUnitRef(new CXOperand(this, pVariable, param1Type));
	//	
	//	//try to find copy contructor for current types
	//	auto constructorFunc = scriptCompiler->applyConstructor(param1, param2);

	//	if (constructorFunc) {
	//		putCommandUnit(ExecutableUnitRef(constructorFunc));

	//		return true;
	//	}
	//	return false;
	//}

	const wchar_t* ContextScope::parse(const wchar_t* text, const wchar_t* end) {
		const wchar_t* c;
		const wchar_t* d;
		const wchar_t* e;
		std::string token1;
		std::string token2;
		bool isPointer;
		Variable* pVariable;
		EKeyword keywordId;

		ScriptCompiler* scriptCompiler = this->getCompiler();

		c = trimLeft(text, end);

		unique_ptr<WCHAR, std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)text, [this, &c](WCHAR*) {
			((GlobalScope*)getRoot())->setErrorCompilerChar(c);
		});

		if (!ScriptCompiler::isOpenScopeSign(*c)) {
			scriptCompiler->setErrorText("missing open bracket '{'");
			return nullptr;
		}
		c++;

		//fist command of scope is always enter scope command
		EnterScopeBuilder* enterScope = new EnterScopeBuilder(this);
		putCommandUnit(enterScope);

		if (_parseContextBodyEventHandler) {
			_parseContextBodyEventHandler(this, ContextScopeParseEvent::BeforeParseBody);
		}

		bool keywordDetected = false;

		auto updateLaterMan = CodeUpdater::getInstance(this);

		while (c < end) {
			d = trimLeft(c, end);
			if (ScriptCompiler::isOpenScopeSign(*d)) {
				ContextScope* subScope = new ContextScope(this, _functionScope);
				subScope->setLoopScope(_loopScope);
				c = subScope->parse(d, end);
				if (c == nullptr) {
					return nullptr;
				}

				auto jumpToSubScope = new JumpToSubScopeCommandBuilder(subScope);
				putCommandUnit(jumpToSubScope);
				continue;
			}
			if (ScriptCompiler::isCloseScopeSign(*d)) {
				c = d;
				break;
			}
			//c = lastCharInToken(d, end);
			//token1 = convertToAscii(d, c - d);
			ScriptType type;
			c = scriptCompiler->readType(d, end, type);
			token1 = type.sType();

			keywordDetected = false;

			if (token1 == key_if) {				
				c = parseIf(c, end);
				if (c == nullptr) {
					return nullptr;
				}
				keywordDetected = true;
				continue;
			}
			if (token1 == key_while) {
				//current while is not implemented
				c = parseWhile(c, end);
				if (c == nullptr) {
					return nullptr;
				}
				keywordDetected = true;
				continue;
			}
			isPointer = (token1 == key_pointer);
			if (isPointer) {
				auto e = d;
				do {
					d = trimLeft(c, end);
					c = lastCharInToken(d, end);
					//token 1 is expected as a data type
					token1 = convertToAscii(d, c - d);
					if (!scriptCompiler->isRefSign(token1)) {
						break;
					}
				} while (c < end);
				d = e;
				token1 = convertToAscii(d, c - d);
				keywordDetected = true;
			}
			//type = ScriptType::parseType(scriptCompiler, token1);

			if (type.isUnkownType()) {
				if (isPointer) {
					scriptCompiler->setErrorText("missing data type for reference variable");
					return nullptr;
				}
			}
			else {
				//token 1 is expected as a variable name
				d = trimLeft(c, end);
				c = lastCharInToken(d, end);
				token1 = convertToAscii(d, c - d);

				if (scriptCompiler->findKeyword(token1) != KEYWORD_UNKNOWN) {
					scriptCompiler->setErrorText("token '" + token1 + "' is not expected here");
					return nullptr;
				}
				
				pVariable = registVariable(token1);
				if (!pVariable) {
					scriptCompiler->setErrorText("variable '" + token1 + "' is already defined");
					return nullptr;
				}
				pVariable->setDataType(type);				
				e = trimLeft(c, end);
				if (*e == '=') {
					if (e + 1 >= end) {
						scriptCompiler->setErrorText("incompleted expression");
						// always keep last compilied char in c before exit this function
						c = e;
						return nullptr;
					}

					e++;
					c = parseDeclaredExpression(d, end);
					if (c == nullptr) {
						return nullptr;
					}
					c++;
					continue;
				}
				if (ScriptCompiler::isCommandBreakSign(*e)) {
					// use x operand unit to store variable and source char index then
					// the function checkVariableToRunConstructor will use it to set setSourceCharIndex
					// for some generated units if necessary
					CXOperand xOperand(this, pVariable);
					xOperand.setSourceCharIndex((int)(d - text));

					checkVariableToRunConstructor(&xOperand);
					c++;
					continue;
				}
				keywordDetected = true;
			}
			if (!keywordDetected && (keywordId = scriptCompiler->findKeyword(token1)) != KEYWORD_UNKNOWN) {
				FunctionFactory* functionFactory = scriptCompiler->getFunctionFactory(_functionScope->getFunctionId());
				auto& returnType = functionFactory->getReturnType();
				ScriptType typeVoid(scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID, "void");

				c = trimLeft(c, end);
				if (ScriptCompiler::isCommandBreakSign(*c) == false) {
					if (keywordId == KEYWORD_RETURN) {
						if (returnType != typeVoid) {
							c = parseExpressionInternal(c, end, &returnType);
							if (c != nullptr) {
								if (ScriptCompiler::isCommandBreakSign(*c) == false) {
									scriptCompiler->setErrorText("missing ';'");
									return nullptr;
								}
								auto returnExrpressionIter = getLastCommandUnitRefIter();
#if USE_DIRECT_COPY_FOR_RETURN
								ReturnCommandBuilder2* returnCommand = new ReturnCommandBuilder2(this, _functionScope);
#else
								ReturnCommandBuilder* returnCommand = new ReturnCommandBuilder(this, _functionScope);
#endif						
								ExecutableUnit* unitForReturn = (ExecutableUnit*)returnExrpressionIter->get();

#pragma region prevent calling destructor for return unit
								//set ExcludeFromDestructor to notice that return object from this unit will not be destroy by destructor
								//this mask will make affect if this unit is function unit
								//for variable unit(or X Opeand unit), we must use another way
								unitForReturn->setMask(unitForReturn->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR);

								//try another way to notice that return object from this unit will not be destroy by destructor
								//in case the return unit is a EXP_UNIT_ID_XOPERAND
								if (unitForReturn->getType() == EXP_UNIT_ID_XOPERAND) {
									auto pVariable = ((CXOperand*)unitForReturn)->getVariable();
									//now try to find destructor command that was generated for the variable
									auto destructorList = getDestructorList();
									OperatorBuidInfo* destructorBuildInfo = nullptr;
									if (destructorList) {
										for (auto it = destructorList->begin(); it != destructorList->end(); it++) {
											auto destructorUnit = (Function*)it->get();
											auto& paramUnit = destructorUnit->getChild(0);
											//check param unit is a ref function, because X Operand is always follow by a ref function
											//in destructor unit
											if (paramUnit->getType() == EXP_UNIT_ID_MAKE_REF) {
												auto& xOperandUnit = ((RefFunction*)paramUnit.get())->getValueOfVariable();
												if (xOperandUnit->getType() == EXP_UNIT_ID_XOPERAND) {
													auto pVariableInDestructor = ((CXOperand*)xOperandUnit.get())->getVariable();
													if (pVariableInDestructor == pVariable) {
														auto& blockRef = destructorUnit->getUserData();
														if (blockRef) {
															destructorBuildInfo = (OperatorBuidInfo*)blockRef->getDataRef();
														}
														break;
													}
												}
											}
										}
									}
									if (destructorBuildInfo != nullptr) {
										returnCommand->setDestructorIndex(destructorBuildInfo->operatorIndex);
									}
								}
#pragma endregion
								returnCommand->setReturnExpression(unitForReturn);
								putCommandUnit(returnCommand);
								c++;

								updateLaterMan->setUpdateInfo(unitForReturn, nullptr);
								continue;
							} else {
								return nullptr;
							}
						}
						scriptCompiler->setErrorText("function '" + _functionScope->getName() + "' does not allow return value");
						return nullptr;
					}

					scriptCompiler->setErrorText("missing ';' before '" + token1 + "'");
					return nullptr;
				}
				if (keywordId == KEYWORD_BREAK && _loopScope) {
					BreakCommandBuilder* breakCommand = new BreakCommandBuilder(this, _loopScope);
					putCommandUnit(breakCommand);
					c++;
					continue;
				}
				if (keywordId == KEYWORD_CONTINUE && _loopScope) {
					ContinueCommandBuilder* continueCommand = new ContinueCommandBuilder(this, _loopScope);
					putCommandUnit(continueCommand);
					c++;
					continue;
				}
				if (keywordId == KEYWORD_RETURN) {
					if (returnType != typeVoid) {
						scriptCompiler->setErrorText("function '" + _functionScope->getName() + "' must return value");
						return nullptr;
					}
#if USE_DIRECT_COPY_FOR_RETURN
					ReturnCommandBuilder2* returnCommand = new ReturnCommandBuilder2(this, _functionScope);
#else
					ReturnCommandBuilder* returnCommand = new ReturnCommandBuilder(this, _functionScope);
#endif			
					putCommandUnit(returnCommand);
					c++;
					continue;
				}
				scriptCompiler->setErrorText("token '" + token1 + "' is not expected here");
				return nullptr;
			}
			if (type.isUnkownType()) {
				c = d;
			}
			c = parseExpressionInternal(c, end);
			if (c == nullptr) {
				return nullptr;
			}
			if (ScriptCompiler::isCommandBreakSign(*c) == false) {
				scriptCompiler->setErrorText("missing ';'");
				return nullptr;
			}
			c++;
		}

		if (!ScriptCompiler::isCloseScopeSign(*c)) {
			scriptCompiler->setErrorText("missing close bracket");
			return nullptr;
		}

		if (_parseContextBodyEventHandler) {
			_parseContextBodyEventHandler(this, ContextScopeParseEvent::AfterParseBody);
		}

		_beginExitScopeUnit = nullptr;

		//move commands for destructor to end of context scope before add exit scope command
		auto desctructorList = getDestructorList();
		for (auto it = desctructorList->begin(); it != desctructorList->end(); it++) {
			putCommandUnit(*it);
			if (_beginExitScopeUnit == nullptr) {
				_beginExitScopeUnit = this->getLastCommandUnitRefPtr()->get();
			}
		}

		applyExitScopeCommand();

		c++;
		return c;
	}

	void ContextScope::applyExitScopeCommand() {
		auto updateLaterMan = CodeUpdater::getInstance(this);

		//last command of scope is always exit scope command
		ExitScopeBuilder* exitSope = new ExitScopeBuilder(this);
		putCommandUnit(exitSope);
		if (_beginExitScopeUnit == nullptr) {
			_beginExitScopeUnit = this->getLastCommandUnitRefPtr()->get();
		}
		updateLaterMan->setUpdateInfo(_beginExitScopeUnit, nullptr);
	}

	const CodeSegmentEntry* ContextScope::getCode() const {
		return &_codeSegment;
	}

	void ContextScope::setCodeBegin(CommandPointer startCode) {
		_codeSegment.first = startCode;
	}

	void ContextScope::setCodeEnd(CommandPointer endCode) {
		_codeSegment.second = endCode;
	}

	//Executor* ContextScope::getExcutorBegin() const {
	//	return _beginExecutor;
	//}

	//Executor* ContextScope::getExcutorEnd() const {
	//	return _endExecutor;
	//}

	bool ContextScope::extractCode(Program* program) {
		updateVariableOffset();

		auto updateLaterMan = CodeUpdater::getInstance(this);

		int expressionCount = this->getCommandUnitCount();
		for (auto it = getFirstCommandUnitRefIter(); expressionCount > 0; ++it, --expressionCount) {
			const CommandUnitRef& commandUnit = *it;
			const ExecutableUnitRef& extUnit = dynamic_pointer_cast<ExecutableUnit>(commandUnit);

			if (extUnit.get()) {
				//ExecutorRef pExecutor = (ExecutorRef)(new ExpUnitExecutor(_functionScope));
				ExecutorRef pExecutor = (ExecutorRef)(new ExpUnitExecutor(this));
				if (((ExpUnitExecutor*)pExecutor.get())->extractCode(getCompiler(), extUnit) == false) {
					return false;
				}
				program->addExecutor(pExecutor);
				
				if (_beginExecutor == nullptr) {
					_beginExecutor = pExecutor;
				}
				_endExecutor = pExecutor;
			}
			else {
				CommandBuilder* controllerUnit = (CommandBuilder*)commandUnit.get();
				ExecutorRef pExecutor = (ExecutorRef)(controllerUnit->buildNativeCommand());
				program->addExecutor((ExecutorRef)(pExecutor));

				if (_beginExecutor == nullptr) {
					_beginExecutor = pExecutor;
				}
				_endExecutor = pExecutor;
			}
			//save the link between expression unit and its executor
			updateLaterMan->saveUpdateInfo(commandUnit.get(), _endExecutor.get());
		}

		int childrenBaseOffset = getBaseOffset() + getDataSize();
		
		const ScopeRefList& children = getChildren();

		for (auto it = children.begin(); it != children.end(); ++it) {
			auto& scope = *it;

			DestructorContextScope* destructorScope = dynamic_cast<DestructorContextScope*>(scope.get());
			if (destructorScope) {
				auto excutor =  (ExpUnitExecutor*)updateLaterMan->findUpdateInfo(destructorScope->getDestructorParentUnit());
				auto destructorCommandSize = excutor->getLocalSize();
				// to prevent the sub scope overwrite data to its parent command
				// it must has the base offset seperate from memory of the command
				destructorScope->setBaseOffset(childrenBaseOffset + destructorCommandSize);
			}
			else {
				scope->setBaseOffset(childrenBaseOffset);
			}

			if (scope->extractCode(program) == false) return false;
		}

		return true;
	}

	bool ContextScope::updateCodeForControllerCommands(Program* program) {
		CodeSegmentEntry* executorCode = program->getCode(_beginExecutor.get());
		this->setCodeBegin(executorCode->first);

		const ScopeRefList& children = getChildren();
		for (auto it = children.begin(); it != children.end(); ++it) {
			((ContextScope*)it->get())->updateCodeForControllerCommands(program);
		}

		auto updateLaterMan = CodeUpdater::getInstance(this);
		auto beginExitScopeExutor = updateLaterMan->findUpdateInfo(_beginExitScopeUnit);
		CodeSegmentEntry* beginExitCodeSeg = program->getCode(beginExitScopeExutor);
		_beginExitScopeCommand = beginExitCodeSeg->first;

		//if (children.size()) {
		//	//set code end is code end of last children
		//	this->setCodeEnd(((ContextScope*)children.back().get())->getCode()->second);
		//}
		//else {
			executorCode = program->getCode(_endExecutor.get());
			this->setCodeEnd(executorCode->second);
		//}

		//this->setCodeBegin(executorCode->first);

		return true;
	}

	void ContextScope::buildExitScopeCodeCommands(CommandList& commandList) const {
		auto endCommand = getCode()->second;
		endCommand++;
		for (auto command = _beginExitScopeCommand; command != endCommand; command++) {
			commandList.push_back(*command);
		}
	}

	CommandPointer ContextScope::getBeginExitScopeCommand() const {
		return _beginExitScopeCommand;
	}

	Function* ContextScope::checkAndGenerateDestructor(ScriptCompiler* scriptCompiler, const ScriptType& type) {
		auto getDestructorFunction = std::bind(&ScriptCompiler::getDestructor, scriptCompiler, std::placeholders::_1);

		auto destructorBuildInfoBlockRef = std::make_shared<ObjectBlock<OperatorBuidInfo>>();
		ObjectBlock<OperatorBuidInfo>* destructorBuildInfoBlock = destructorBuildInfoBlockRef.get();
		OperatorBuidInfo* destructorBuildInfo = (OperatorBuidInfo*)destructorBuildInfoBlock->getDataRef();
		checkAutoOperatorForChildren(scriptCompiler, getDestructorFunction, type, 0, &(destructorBuildInfo->buildItems));

		int destructorFunctionId = getDestructorFunction(type.iType());
		if (destructorFunctionId < 0 && destructorBuildInfo->buildItems.size() > 0) {
			destructorFunctionId = scriptCompiler->findFunction(DEFAULT_DUMMY_OPERATOR, "ref void");
		}

		Function* destructor = nullptr;
		if (destructorFunctionId >= 0) {
			destructor = scriptCompiler->createFunctionFromId(destructorFunctionId);

			destructorBuildInfo->operatorIndex = -1;
			destructor->setMask(destructor->getMask() | UMASK_DESTRUCTORFORRETURNDATA);
			destructor->setUserData(destructorBuildInfoBlockRef);
		}

		return destructor;
	}

	int ContextScope::checkAndGenerateDestructors(ScriptCompiler* scriptCompiler, ExecutableUnit* exeUnit, std::list<FunctionRef>& destructors) {
		if (!ISFUNCTION(exeUnit)) {
			return 0;
		}
		int iRes = 0;

		auto function = (Function*)exeUnit;
		//only generate destructor for function has not ExcludeFromDestructor in mask
		if (exeUnit->getType() != EXP_UNIT_ID_DEREF) {
			if (((unsigned int)function->getMask() & (unsigned int)UMASK_EXCLUDEFROMDESTRUCTOR) == 0) {
				auto& type = function->getReturnType();
				auto destructor = checkAndGenerateDestructor(scriptCompiler, type);
				if (destructor != nullptr) {
					destructors.push_back(FunctionRef(destructor));

					//the variable for this node will be update later
					auto pVariable = registTempVariable(exeUnit, -1);

					destructor->setSourceCharIndex(exeUnit->getSourceCharIndex());

					pVariable->setDataType(type);
					ExecutableUnitRef destructorUnit = std::make_shared<CXOperand>(this, pVariable, pVariable->getDataType());
					destructorUnit->setSourceCharIndex(exeUnit->getSourceCharIndex());

					if (!scriptCompiler->convertToRef(destructorUnit)) {
						((GlobalScope*)getRoot())->setErrorCompilerCharIndex(destructorUnit->getSourceCharIndex());
						return 1;
					}

					destructor->pushParam(destructorUnit);
				}
			}
		}
#if OPTIMIZE_CTOR_CALL
		if (function->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR) {
			//keep checking destructor for two direct children of default copy constructors
			//because destructor should not be run when the memory will be copy directly to new object
			auto& param1 = function->getChild(0);
			auto& param2 = function->getChild(1);

			//check destructor for children of children
			if (ISFUNCTION(param1)) {
				auto paramFunction = (Function*)param1.get();
				int n = paramFunction->getChildCount();
				for (int i = 0; i < n && iRes == 0; i++) {
					auto& exeUnitRef = paramFunction->getChild(i);
					iRes = checkAndGenerateDestructors(scriptCompiler, exeUnitRef.get(), destructors);
				}
			}
			if (ISFUNCTION(param2)) {
				auto paramFunction = (Function*)param2.get();
				int n = paramFunction->getChildCount();
				for (int i = 0; i < n && iRes == 0; i++) {
					auto& exeUnitRef = paramFunction->getChild(i);
					iRes = checkAndGenerateDestructors(scriptCompiler, exeUnitRef.get(), destructors);
				}
			}

			return 0;
		}
#endif
		int n = function->getChildCount();

		for (int i = 0; i < n && iRes == 0; i++) {
			auto& exeUnitRef = function->getChild(i);
			iRes = checkAndGenerateDestructors(scriptCompiler, exeUnitRef.get(), destructors);
		}

		return iRes;
	}

	int ContextScope::correctAndOptimize(Program* program) {
		ScriptCompiler* scriptCompiler = getCompiler();
		ScopeRefList childrenBackup = getChildren();
		auto updateMan = CodeUpdater::getInstance(this);

		int iRes = 0;
		for (auto it = _commandBuilder.begin(); it != _commandBuilder.end();) {
			CommandUnitRef& commandUnit = *it;
			ExecutableUnit* exeUnit = dynamic_cast<ExecutableUnit*>(commandUnit.get());

			if (exeUnit) {
				std::list<FunctionRef> destructors;
				iRes = checkAndGenerateDestructors(scriptCompiler, (Function*)exeUnit, destructors);
				if (iRes) {
					break;
				}
				if (destructors.size()) {
					it++;
					//create a context scope for destructors to guarantee that
					//the destructors command will not overwrite their data to current scope
					//and they can use data of current context to execute their code

					DestructorContextScope* subScope = new DestructorContextScope(this, _functionScope);
					subScope->setLoopScope(_loopScope);
					subScope->setDestructorParentUnit(exeUnit);
					// note to later task that this command unit need to map with excutor object
					updateMan->setUpdateInfo(exeUnit, nullptr);

					//apply enter scope command
					EnterScopeBuilder* enterScope = new EnterScopeBuilder(subScope);
					subScope->putCommandUnit(enterScope);

					for (auto& commandUnitRef : destructors) {
						//put destructor command to it
						subScope->putCommandUnit(commandUnitRef);
					}
					//apply exit scope command
					subScope->applyExitScopeCommand();

					//insert command after current command to run the scope automatically
					auto jumpToSubScope = std::make_shared<JumpToSubScopeCommandBuilder>(subScope);
					_commandBuilder.insert(it, jumpToSubScope);
					
					continue;
				}
			}

			it++;
		}

		for (auto it = childrenBackup.begin(); it != childrenBackup.end() && iRes == 0; ++it) {
			iRes = (*it)->correctAndOptimize(program);
		}
		return iRes;
	}

	void ContextScope::setParseBodyEventHandler(const ParseEventHandler& handler) {
		_parseContextBodyEventHandler = handler;
	}
}