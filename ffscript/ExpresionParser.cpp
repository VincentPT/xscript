#include "stdafx.h"
#include "ExpresionParser.h"
#include <functional>
#include <array>
#include "expressionunit.h"
#include "core_function.h"
#include "ScriptCompiler.h"
#include "ScriptScope.h"
#include "Expression.h"
#include "FactoryTree.h"
#include "FunctionFactory.h"
#include "..\Logger\remotelogger.h"
#include "Utils.h"
#include "BasicType.h"
#include "StructClass.h"
#include "Internal.h"
#include "DefaultCommands.h"
#include "FunctionScope.h"
#include "ScriptFunction.h"

#define DBG_PRINT_FUNTION()
#define DBG_INFO(x)
#define DBG_ERROR(x)


//static RemoteLogger::RLogger theLogger("localhost", 27015);

namespace ffscript {

	using namespace std;

	bool IsDisgit(const wchar_t* sNumber)
	{
		const wchar_t* pChar = sNumber;
		while (*pChar)
		{
			if (*pChar < '0' || *pChar > '9') {
				if (*pChar == 'f' || *pChar == 'l' || *pChar == '.') {
					if (pChar == sNumber) {
						return false;
					}
				}
				else {
					return false;
				}
			}
			pChar++;
		}
		return pChar != sNumber;
	}

	ExpressionParser::ExpressionParser(ScriptCompiler* scriptCompiler) : _scriptCompiler(scriptCompiler) {}


	ExpressionParser::~ExpressionParser(){}

	EExpressionResult makeExpression(OutputStack& output, OperatorStack& operators) {
		DynamicParamFunctionRef pExpFunction;
		ExecutableUnitRef expUnitTemp;
		EExpressionResult eResult = E_SUCCESS;
		int iResult;

		while (eResult == E_SUCCESS && operators.size() && output.size())
		{
			pExpFunction = static_pointer_cast<DynamicParamFunction>(operators.top());
			if ( !ISFUNCTION(pExpFunction) ) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), pExpFunction->GetName()));
				eResult = E_TOKEN_UNEXPECTED;
				break;
			}

			operators.pop();
			//processing for pushing and building expression tree
			while (output.size()) {
				expUnitTemp = output.top();
				output.pop();
				iResult = pExpFunction->pushParamFront(expUnitTemp);
				if (iResult < 0) {
					DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), expUnitTemp->GetName()));					
					eResult = E_TOKEN_UNEXPECTED;
					break;
				}

				if (iResult == 0) {
					eResult = E_SUCCESS;
					break;
				}
			}
			
			output.push(pExpFunction);
		}

		return eResult;
	}

	Variable* findVariable(ScriptScope* currentScope, const string& name) {
		Variable* pVariable = nullptr;
		ScriptScope* ownerScope = currentScope;
		while (ownerScope) {
			pVariable = ownerScope->findVariable(name);
			if (pVariable) break;
			ownerScope = ownerScope->getParent();
		}
		return pVariable;
	}

	Variable* parseStructVariable(ScriptCompiler* scriptCompiler,ScriptScope* currentScope, const string& name) {
		Variable* pVariable = findVariable(currentScope, name);
		if (pVariable) {
			return pVariable;
		}

		size_t idx = name.find_first_of('.');
		if (idx < 0) {
			scriptCompiler->setErrorText(name + " is not a struct(parser is error)");
			return nullptr;
		}

		string rootName = name.substr(0, idx);
		Variable* rootVariable = findVariable(currentScope, rootName);
		if (rootVariable == nullptr) {
			scriptCompiler->setErrorText(rootName + " is unrecognized");
			return nullptr;
		}

		currentScope = rootVariable->getScope();
		auto& varType = rootVariable->getDataType();
		auto structObj = varType.refLevel() <= 1 ? scriptCompiler->getStruct(varType.origin()) : nullptr;

		if (structObj == nullptr) {
			scriptCompiler->setErrorText(rootName + " is not a struct");
			return nullptr;
		}
		size_t idxTemp = idx + 1;
		MemberInfo memberInfo;
		memberInfo.offset = 0;

		while (idx < name.size())
		{
			idx = name.find_first_of('.', idx + 1);
			if ((int)idx < 0) {
				break;
			}
			rootName = name.substr(idxTemp , idx - idxTemp);
			if (!structObj->getInfo(rootName, memberInfo)) return nullptr;

			structObj = memberInfo.type.refLevel() <= 1 ? scriptCompiler->getStruct(memberInfo.type.origin()) : nullptr;

			if (structObj == nullptr) {
				scriptCompiler->setErrorText(rootName + " is not a struct");
				return nullptr;
			}
			rootName = name.substr(0, idx);
			pVariable = currentScope->registMemberVariable(rootVariable, rootName);
			if (pVariable) {
				pVariable->setDataType(memberInfo.type);
				pVariable->setOffset(memberInfo.offset);
				rootVariable = pVariable;
			}
			else
			{
				rootVariable = currentScope->findVariable(rootName);
			}

			idxTemp = idx + 1;
		}
		rootName = name.substr(idxTemp);
		if (!structObj->getInfo(rootName, memberInfo)) return  nullptr;
		pVariable = currentScope->registMemberVariable(rootVariable, name);
		pVariable->setDataType(memberInfo.type);
		pVariable->setOffset(memberInfo.offset);
		return pVariable;
	}

	EExpressionResult ExpressionParser::stringToExpList(const std::wstring& sExpressionString, std::list<ExpUnitRef>& expUnitList) {
		const wchar_t* text = sExpressionString.c_str();
		const wchar_t* end = text + sExpressionString.size();
		return stringToExpList(text, end, expUnitList);
	}

	void ExpressionParser::numberedForUnit(std::list<ExpUnitRef>& expList) {
		int idx = 0;
		for (auto it = expList.begin(); it != expList.end(); it++, idx++) {
			(*it)->setIndex(idx);
		}
	}

	template <typename T>
	T* detach(shared_ptr<T>& ptr) {
		auto sizePtr = sizeof(ptr);
		char* pptr = (char*)&ptr;
		_Ref_count_base* internalRef = *(_Ref_count_base**)(pptr + sizeof(void*));
		//increase ref one but no increase number of owner
		//so the pointer will not release after all owner are destroyed
		internalRef->_Incref();
		return ptr.get();
	}

	const WCHAR* ExpressionParser::readLambdaExression(const WCHAR* begin, const WCHAR* end, EExpressionResult& eResult, FunctionRef& lambdaExpression) {	

		auto compiler = getCompiler();
		auto currentScope = compiler->currentScope();
		auto contextScope = dynamic_cast<ContextScope*>(currentScope);
		GlobalScope* globalScope = nullptr;
		if (contextScope != nullptr) {
			globalScope = (GlobalScope*)(contextScope->getFunctionScope()->getParent());
		}
		else {
			globalScope = dynamic_cast<GlobalScope*>(currentScope);
		}
		if (globalScope == nullptr) {
			eResult = E_TOKEN_UNEXPECTED;
			compiler->setErrorText("Lambda function is only allow when compile in a program context");
			return nullptr;
		}

		auto c = trimLeft(begin, end);
		if (*c != '[') {
			return nullptr;
		}
		int openBracketCount = 1;
		auto d = c;
		c++;
		while (openBracketCount && c < end) {
			if (*c == '[') {
				openBracketCount++;
			}
			else if (*c == ']') {
				openBracketCount--;
			}
			c++;
		}
		
		if (c >= end) {
			eResult = E_INCOMPLETED_EXPRESSION;
			compiler->setErrorText("Unexpected lambda function, '[' and ']' does not match");
			return nullptr;
		}
		wstring newExpression(WIDEN(SYSTEM_LAMBDA_FUNCTION) L"(");
		newExpression.append(d + 1, c - d - 2);
		//add function id to as final parameter of create lambda function
		//if (newExpression.size() > 1) {
		//	newExpression.append(1, ',');
		//}
		newExpression.append(1, ')');

		std::list<ExpUnitRef> expUnitList;
		d = newExpression.c_str();
		d = readExpression(d, d + newExpression.size(), eResult, expUnitList);
		if (d == nullptr || eResult != E_SUCCESS) {
			return nullptr;
		}

		list<ExpressionRef> temp;
		bool res = compile(expUnitList, temp, &eResult);
		if (res == false || temp.size() == 0) {
			eResult = E_TYPE_UNKNOWN;
			return nullptr;
		}
		lambdaExpression = dynamic_pointer_cast<Function>(temp.back()->getRoot());

		//first param is create lambda function
		ExecutableUnitRef pExeUnit1 = lambdaExpression->getChild(0);

		//second param is contain capture list
		ExecutableUnitRef pExeUnit2 = lambdaExpression->getChild(1);

		DynamicParamFunction* paramCollector = (DynamicParamFunction*)pExeUnit2.get();
		auto& params = paramCollector->getParams();

		int lambaFunctionId;
 		c = globalScope->parseAnonymous(c, end, params, lambaFunctionId);
		if (c == nullptr) {
			return nullptr;
		}

		//now we dont use function operator() to call create lambda function
		//but we use create lambda function directly
		DynamicParamFunction* createLambdaFunction = new DynamicParamFunction(SYSTEM_LAMBDA_FUNCTION, EXP_UNIT_ID_CREATE_LAMBDA, FUNCTION_PRIORITY_USER_FUNCTION, (int)params.size() + 1);
		for (auto it = params.begin(); it != params.end() ; it++) {
			createLambdaFunction->pushParam(*it);
		}
		auto unitIdRef = make_shared<CConstOperand<int>>(lambaFunctionId, "int");
		createLambdaFunction->pushParam(unitIdRef);

		lambdaExpression.reset(createLambdaFunction);
		return c;
	}

	inline bool checkPosibilityUnaryPrefix(ExpUnit* pLastUnit) {
		return pLastUnit == NULL ||
			pLastUnit->getType() == EXP_UNIT_ID_OPEN_BRACKET ||
			pLastUnit->getType() == EXP_UNIT_ID_OPEN_SQUARE_BRACKET ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_CHOICE ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_SEPERATOR;
	}

	const WCHAR* ExpressionParser::readExpression(const WCHAR* begin, const WCHAR* end, EExpressionResult& eResult, std::list<ExpUnitRef>& expUnitList) {
		const WCHAR* c, *sMax;
		WCHAR* s1, *s2, *sToken;
		WCHAR sToken1[MAX_FUNCTION_LENGTH + 2];
		WCHAR sToken2[MAX_FUNCTION_LENGTH + 2];
		bool blHasDot, blHasChar;
		ExecutableUnit* pExpUnit;
		ExpUnit *pFixedExpUnit, *pLastUnit, *pStringConst, *pTBDExpUnit;
		ExpUnit *pArrayExpUnit = nullptr;
		int size;

		ScriptCompiler* scriptCompiler = getCompiler();

		blHasDot = false;
		blHasChar = false;
		s1 = sToken1;
		s2 = sToken2;
		pLastUnit = NULL;

		eResult = E_SUCCESS;

		static string key_create_array_func(SYSTEM_ARRAY_FUNCTION);
		for (c = begin; eResult == E_SUCCESS; c++)
		{
			sToken = NULL;
			pFixedExpUnit = NULL;
			pExpUnit = NULL;
			pStringConst = NULL;
			pTBDExpUnit = NULL;
			if (*c == '(' || *c == ')' ||
				*c == '{' || *c == '}' ||
				*c == '[' || *c == ']' ||				
				*c == ' ' || *c == '	' ||
				ScriptCompiler::isCommandBreakSign(*c) || c == end ||
				*c == ',' || *c == '\"')
			{
				if (s1 - sToken1 > 0)
				{
					//sToken1 is fully collected
					*s1 = 0;
					sToken = s1 = sToken1;
					DBG_INFO(_t_tprintf("%s ", sToken1));
				}
				else if (s2 - sToken2 > 0)
				{
					//sToken2 is fully collected
					*s2 = 0;
					sToken = s2 = sToken2;
					DBG_INFO(_tprintf("%s ", sToken2));
				}
				if (c < end && !ScriptCompiler::isCommandBreakSign(*c)) {
					DBG_INFO(_tprintf("%c ", *c));
					switch (*c)
					{
					case '(':
						pFixedExpUnit = new OpenBracket();
						break;
					case ')':
						pFixedExpUnit = new ClosedBracket();
						break;
					case '[':
						pFixedExpUnit = new OpenSquareBracket();
						break;
					case ']':
						pFixedExpUnit = new ClosedSquareBracket();
						break;
					case '}':
						pFixedExpUnit = new ClosedBracket();
						pArrayExpUnit = nullptr;
						break;
					case '{':
						//pArrayExpUnit = new DynamicParamFunction(key_create_array_func, EXP_UNIT_ID_OPERATOR_ARRAY, FUNCTION_PRIORITY_USER_FUNCTION, "TBD");
						//size = scriptCompiler->findDynamicFunctionOnly(key_create_array_func);
						//pArrayExpUnit = scriptCompiler->createFunctionFromId(size);
						pArrayExpUnit = new IncompletedUserFunctionUnit(key_create_array_func);
						expUnitList.push_back(ExpUnitRef(pArrayExpUnit));
						pFixedExpUnit = new OpenBracket();
						break;
						//case '+':
						//	pFixedExpUnit = new  DynamicParamFunction("+", EXP_UNIT_ID_OPERATOR_ADD, FUNCTION_PRIORITY_ADDITIVE, 2);
						//	break;
						//case '-':
						//	pFixedExpUnit = new DynamicParamFunction("-", EXP_UNIT_ID_OPERATOR_SUB, FUNCTION_PRIORITY_ADDITIVE, 2);
						//	break;
					case  ',':
						pFixedExpUnit = new ParamSeperator();
						break;
						/*			case  ':':
						pFixedExpUnit = new CConditionChoice();
						break;*/
					case '\"':
						c++;

						for (sMax = c + MAX_FUNCTION_LENGTH; *c && *c != '\"'; c++) {
							if (c > sMax) {
								eResult = E_TOKEN_OUTOFLENGTH;
								scriptCompiler->setErrorText("token is too long");
								DBG_ERROR(_tprintf(_u_T("\n[#]Error at index %d"), c - sExpressionString));
								break;
							}
							DBG_INFO(_tprintf("%c", *c));
						}

						if (eResult == E_SUCCESS) {
							size = (int)(c - (sMax - MAX_FUNCTION_LENGTH));
							if (*c != '\"') {
								eResult = E_TOKEN_UNEXPECTED;
								scriptCompiler->setErrorText("missing '\"'");
								DBG_ERROR(_tprintf(__T("\n[#]Unexpected token '\"' at index:%d"), c - sExpressionString - size));
							}
							else {
								DBG_INFO(_tprintf("\""));
								sMax -= MAX_FUNCTION_LENGTH;

								if (sToken != NULL && *sToken == 'L' && *(sToken + 1) == 0) {
									pStringConst = new CConstOperand<std::wstring>(std::wstring(sMax, size), "wstring");
									if (sToken == sToken1) {
										s1 = sToken1;
									}
									else {
										s2 = sToken2;
									}
									sToken = nullptr;
								}
								else {
									pStringConst = new CConstOperand<std::string>(convertToAscii(sMax, size), "string");
								}
							}
						}
						break;
					case 0:
						break;
					default:
						break;
					}
				}

			}
			else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || *c == '_') {
				blHasChar = true;
				*s1++ = *c;
			}
			else if (*c == '.') {
				*s1 = 0;
				sToken = sToken1;

				if (blHasChar) {
					blHasDot = false;
				}
				else {
					blHasDot = IsDisgit(sToken);
				}

				if (blHasDot == false) {
					pFixedExpUnit = new DynamicParamFunction(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, 2);
					//s1 now is fully collected, so it must reset to begin of buffer sToken1
					s1 = sToken1;
				}
				else {
					//revert sToken because it is not fully collected
					sToken = nullptr;
					*s1++ = *c;
				}
			}
			else if (*c >= '0'&& *c <= '9') {
				*s1++ = *c;
			}
			else if (c < end && !ScriptCompiler::isCommandBreakSign(*c)) {
				*s2++ = *c;
			}

			//if (blHasDot && blHasDot == blHasChar) {
			//	eResult = E_TOKEN_INVALID;
			//	scriptCompiler->setErrorText("operator '.' is not supported now");
			//	DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
			//	break;
			//}
			if (s1 - sToken1 > MAX_FUNCTION_LENGTH) {
				eResult = E_TOKEN_OUTOFLENGTH;
				scriptCompiler->setErrorText("token is too long");
				DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
				break;
			}
			if (s2 - sToken2 > MAX_FUNCTION_LENGTH) {
				eResult = E_TOKEN_OUTOFLENGTH;
				scriptCompiler->setErrorText("token is too long");
				DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
				break;
			}
			if (sToken == NULL) {
				if ((s2 - sToken2 == 1) && (s1 - sToken1 > 1 || (s1 - sToken1 == 1 && *(s2 - 1) == *c))) {
					//sToken1 is fully collected
					*s1 = 0;
					sToken = s1 = sToken1;
					DBG_INFO(_tprintf(__T("%s "), sToken1));
				}
				else if ((s1 - sToken1 == 1) && (s2 - sToken2 > 1 || (s2 - sToken2 == 1 && *(s1 - 1) == *c))) {
					//sToken2 is fully collected
					*s2 = 0;
					sToken = s2 = sToken2;
					DBG_INFO(_tprintf(__T("%s "), sToken2));
				}
			}
			if (sToken) {
				if ((blHasDot && !blHasChar) || IsDisgit(sToken)) {
					int sign = 1;
					if (expUnitList.size() > 0) {
						auto lastUnitIter = expUnitList.end();
						--lastUnitIter;
						if ((*lastUnitIter)->getType() == EXP_UNIT_ID_OPERATOR_NEG) {
							sign = -1;
							if (expUnitList.size() > 1) {
								auto beforeLastIter = lastUnitIter;
								--beforeLastIter;
								pLastUnit = beforeLastIter->get();
							}
							else {
								pLastUnit = nullptr;
							}
							expUnitList.erase(lastUnitIter);
						}
					}

					wchar_t* lastChar;
					if (sToken == sToken1) {
						lastChar = sToken1 + wcslen(sToken1) - 1;
					}
					else {
						lastChar = sToken2 + wcslen(sToken2) - 1;
					}

					if (blHasDot) {
						if (*lastChar == 'f') {
							*lastChar = 0;
							pExpUnit = new CConstOperand<float>(sign * (float)_wtof(sToken), "float");
						}
						else {
							pExpUnit = new CConstOperand<double>(sign * _wtof(sToken), "double");
						}
					}
					else {
						if (*lastChar == 'l') {
							*lastChar = 0;
							pExpUnit = new CConstOperand<__int64>(sign * _wtoll(sToken), "long");
						}
						else {
							pExpUnit = new CConstOperand<int>(sign * _wtoi(sToken), "int");
						}
					}
				}
#if 0 /*old member access way is removed*/
				else if (blHasChar && blHasDot) {
					//expect is a member of a struct
					std::string&& stdtoken = convertToAscii(sToken);
					ScriptScope* currentScope = scriptCompiler->currentScope();
					Variable* pVariable = parseStructVariable(scriptCompiler, currentScope, stdtoken);
					if (pVariable == nullptr) {
						eResult = E_TOKEN_UNEXPECTED;
						break;
					}

					pExpUnit = new CXOperand(currentScope, pVariable);
					pExpUnit->setReturnType(pVariable->getDataType());
				}
#endif
				else if (pLastUnit != nullptr && pLastUnit->getType() == EXP_UNIT_ID_MEMBER_ACCESS) {
					//expected member name here.
					//member name will be stored as string constant
					pStringConst = new CConstOperand<std::string>(convertToAscii(sToken), "string");
				}
				else {

					std::string&& stdtoken = convertToAscii(sToken);

					if (scriptCompiler->findKeyword(stdtoken) != KEYWORD_UNKNOWN) {
						eResult = E_TOKEN_UNEXPECTED;
						scriptCompiler->setErrorText("token '" + stdtoken + "' is not expected here");
						break;
					}

					//try search constant name first
					auto createConstantFunction = scriptCompiler->findConstantMap(stdtoken);
					if (createConstantFunction) {
						createConstantFunction->call();
						pExpUnit = (ExecutableUnit*)createConstantFunction->getReturnValAsVoidPointer();
					}
					else {
						//try to find token in variables
						ScriptScope* currentScope = scriptCompiler->currentScope();
						Variable* pVariable = findVariable(currentScope, stdtoken);

						if (pVariable) {
							if (pVariable->getDataType().isUnkownType()) {
								eResult = E_TYPE_UNKNOWN;
								scriptCompiler->setErrorText("type of '" + pVariable->getName() + "' is unknowns");
								DBG_ERROR(_tprintf(__T("\n[#]varible's type is unknown")));
								break;
							}
							pExpUnit = new CXOperand(pVariable->getScope(), pVariable, pVariable->getDataType());
						}
						else {
							auto ctemp = trimLeft(c, end);
							//for (ctemp = c; ctemp != end && (*ctemp == ' ' || *ctemp == '\t'); ctemp++);

							// varible is not declared yet but if after variable is assignment operator, it is an auto type variable
							if (*ctemp == '=' && *(ctemp + 1) != '=') {
								if (currentScope == nullptr) {
									scriptCompiler->setErrorText("internal error: the scope is missing");
									eResult = E_TOKEN_UNEXPECTED;
									return nullptr;
								}
								pVariable = currentScope->registVariable(stdtoken);
								//auto variable will be construct by default copy constructor
								//so we don't need find default construtor to init for it
								//so, the line below was removed
								//currentScope->checkVariableToRunConstructor(pVariable);

								//create a variable with default name, auto type variable
								pExpUnit = new CXOperand(currentScope, pVariable);
							}
							else {
								auto operatorEntry = scriptCompiler->findPredefinedOperator(stdtoken);
								if (operatorEntry
									//bellow double check to make sure the declaration in pre-defined operator talbe have at least one instant of implementation in the library
									/* && (scriptCompiler->findOverloadingFuncRoot(stdtoken) ||
									(operatorEntry->nameInExpression && scriptCompiler->findOverloadingFuncRoot(operatorEntry->nameInExpression))) */
									) {
									pTBDExpUnit = nullptr;
									if (operatorEntry->operatorType == EXP_UNIT_ID_OPERATOR_PREFIX_INC) {
										if (pLastUnit && pLastUnit->getType() == EXP_UNIT_ID_XOPERAND) {
											pTBDExpUnit = new DynamicParamFunction("post_fix_increase", EXP_UNIT_ID_OPERATOR_POSTFIX_INC, FUNCTION_PRIORITY_POSTFIX, 1);
										}
									}
									else if (operatorEntry->operatorType == EXP_UNIT_ID_OPERATOR_PREFIX_DEC) {
										if (pLastUnit && pLastUnit->getType() == EXP_UNIT_ID_XOPERAND) {
											pTBDExpUnit = new DynamicParamFunction("post_fix_decrease", EXP_UNIT_ID_OPERATOR_POSTFIX_DEC, FUNCTION_PRIORITY_POSTFIX, 1);
										}
									}
									if (!pTBDExpUnit) {
										pTBDExpUnit = new DynamicParamFunction(stdtoken, operatorEntry->operatorType, operatorEntry->priority, operatorEntry->maxParam);
									}
								}
								else {
									pTBDExpUnit = new IncompletedUserFunctionUnit(stdtoken);
									//finally find in registerd function
									/*pTBDExpUnit = scriptCompiler->createExpUnitFromName(stdtoken);
									if (pTBDExpUnit == NULL) {
									DBG_ERROR(_tprintf(__T("\n[#]Unexpected token '%s'"), sToken));
									eResult = E_TOKEN_UNEXPECTED;
									scriptCompiler->setErrorText("Unexpected token '" + convertToAscii(sToken) + "'");
									}*/
								}
							}
						}
					}
				}
				if (sToken == sToken1) {
					blHasDot = false;
					blHasChar = false;
				}
			}
			if (pExpUnit) {
				pLastUnit = pExpUnit;
				expUnitList.push_back(ExpUnitRef((ExpUnit*)pExpUnit));
			}
			else if (pTBDExpUnit) {
				if (pTBDExpUnit->getType() == EXP_UNIT_ID_OPERATOR_ADD) {
					if (checkPosibilityUnaryPrefix(pLastUnit)) {
						delete pTBDExpUnit;
						pTBDExpUnit = NULL;
					}
					else {
						if (ISFUNCTION(pLastUnit)) {
							if (((DynamicParamFunction*)pLastUnit)->getMaxParam() >= 2) {
								delete pTBDExpUnit;
								pTBDExpUnit = NULL;
							}
						}
					}
				}
				else if (pTBDExpUnit->getType() == EXP_UNIT_ID_OPERATOR_SUB) {
					if (checkPosibilityUnaryPrefix(pLastUnit)) {
						delete pTBDExpUnit;
						pTBDExpUnit = new DynamicParamFunction("-", EXP_UNIT_ID_OPERATOR_NEG, FUNCTION_PRIORITY_UNARY_PREFIX, 1);
					}
					else if (pLastUnit->getType() == EXP_UNIT_ID_OPERATOR_SUB) {
						expUnitList.pop_back();
						delete pTBDExpUnit;
						pTBDExpUnit = new DynamicParamFunction("+", EXP_UNIT_ID_OPERATOR_ADD, FUNCTION_PRIORITY_ADDITIVE, 2);
					}
					else if (pLastUnit->getType() == EXP_UNIT_ID_OPERATOR_ADD) {
						//remove last unit in the list and add new operator
						expUnitList.pop_back();
					}
					else if (ISFUNCTION(pLastUnit)) {
						delete pTBDExpUnit;
						pTBDExpUnit = new DynamicParamFunction("-", EXP_UNIT_ID_OPERATOR_NEG, FUNCTION_PRIORITY_UNARY_PREFIX, 1);
					}
				}
				else if (pTBDExpUnit->getType() == EXP_UNIT_ID_OPERATOR_MUL) {
					if (checkPosibilityUnaryPrefix(pLastUnit) || (ISFUNCTION(pLastUnit) && ((DynamicParamFunction*)pLastUnit)->getMaxParam() > 1)) {
						delete pTBDExpUnit;
						pTBDExpUnit = new DynamicParamFunction(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, 1);
					}
				}
				else if (pTBDExpUnit->getType() == EXP_UNIT_ID_BITWISE_AND) {
					if (checkPosibilityUnaryPrefix(pLastUnit) || (ISFUNCTION(pLastUnit) && ((DynamicParamFunction*)pLastUnit)->getMaxParam() > 1)) {
						delete pTBDExpUnit;
						pTBDExpUnit = new DynamicParamFunction(MAKING_SEMI_REF_FUNC, EXP_UNIT_ID_SEMI_REF, FUNCTION_PRIORITY_UNARY_PREFIX, 1);
					}
				}

				if (pTBDExpUnit) {
					pLastUnit = pTBDExpUnit;
					expUnitList.push_back(ExpUnitRef(pTBDExpUnit));
				}
			}
			if (pStringConst) {
				pLastUnit = pStringConst;
				expUnitList.push_back(ExpUnitRef(pStringConst));
			}
			if (pFixedExpUnit) {
				bool isReplaced = false;
				if (pFixedExpUnit->getType() == EXP_UNIT_OPEN_SQUARE_BRACKET) {
					//check if the charactor is begin of a lambda expression
					if (checkPosibilityUnaryPrefix(pLastUnit) || pLastUnit->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT) {
						FunctionRef lambdaExpression;
						c = readLambdaExression(c, end, eResult, lambdaExpression);
						if (c == nullptr || !lambdaExpression) {
							return nullptr;
						}
						c--;
						delete pFixedExpUnit;
						pFixedExpUnit = lambdaExpression.get();
						//pFixedExpUnit = detach(lambdaUnitRef);
						expUnitList.push_back(lambdaExpression);
						isReplaced = true;
					}
				}
				pLastUnit = pFixedExpUnit;
				if (!isReplaced) {
					expUnitList.push_back(ExpUnitRef(pFixedExpUnit));
				}
			}
			if (c == end || ScriptCompiler::isCommandBreakSign(*c)) {
				break;
			}
		}

		if (pArrayExpUnit) {
			eResult = E_INCOMPLETED_EXPRESSION;
		}

		if (eResult != E_SUCCESS)
		{
			//clear and need to delete ExpUnit before clear
			expUnitList.clear();
		}		
		return c;
	}

	EExpressionResult ExpressionParser::stringToExpList(const WCHAR* sExpressionString, const WCHAR* end, std::list<ExpUnitRef>& expUnitList) {
		EExpressionResult eResult;
		readExpression(sExpressionString, end, eResult, expUnitList);
		return eResult;
	}

	bool ExpressionParser::precheck(const list<ExpUnitRef>& expUnitList) {
		if (expUnitList.size() == 0) return false;
		if (expUnitList.size() == 1) return true;
		auto it = expUnitList.begin();
		ExpUnitRef lastUnit = *it;

		ScriptCompiler* scriptCompiler = getCompiler();

		for (++it; it != expUnitList.end(); ++it) {
			//a operand can't be next to another operand
			if (ISOPERAND(lastUnit) && ISOPERAND((*it))) {
				scriptCompiler->setErrorText("operator missing between '" + lastUnit->toString() + "' and '" + (*it)->toString() + "'");
				return false;
			}

			//a operand can't be next to a user function
			//user function is a function use brackets to pass the param
			if (ISOPERAND(lastUnit) && ISUSERFUNC((*it))) {
				scriptCompiler->setErrorText("operator missing between '" + lastUnit->toString() + "' and '" + (*it)->toString() + "'");
				return false;
			}

			lastUnit = *it;
		}
		return true;
	}

	ScriptCompiler* ExpressionParser::getCompiler() const {
		return _scriptCompiler;
	}

	EExpressionResult ExpressionParser::pickParamUnitsForFunction(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& functionRef) {
		EExpressionResult eResult = E_SUCCESS;

		OperatorStack* pOperatorStack = new OperatorStack();;
		OutputStack* pOutputStack = new OutputStack();

		pOperatorStack->push(functionRef);

		ExpressionInputList inputList;
		inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));

		auto& params = functionRef->getParams();
		int maxParam = functionRef->getMaxParam();
		
		for (; it != end; it++)
		{
			eResult = putAnExpUnit(it, end, inputList);
			if (eResult != E_SUCCESS || (int)params.size() >= maxParam) {
				break;
			}
		}

		if (eResult != E_SUCCESS) {
			return eResult;
		}

		if ((int)params.size() < maxParam) {
			list<ExpressionRef> expList;
			eResult = makeExpressionList(inputList, expList);

			if (eResult == E_SUCCESS && expList.size() != 1) {
				eResult = E_INCOMPLETED_EXPRESSION;
			}
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::pickParamUnitsElseClause(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& elseClauseUnitRef) {
		EExpressionResult eResult = E_SUCCESS;

		OperatorStack* pOperatorStack = new OperatorStack();;
		OutputStack* pOutputStack = new OutputStack();

		pOperatorStack->push(elseClauseUnitRef);

		ExpressionInputList inputList;
		inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));

		auto& params = elseClauseUnitRef->getParams();
		int maxParam = elseClauseUnitRef->getMaxParam();

		for (; it != end; it++)
		{
			eResult = putAnExpUnit(it, end, inputList);
			if (eResult != E_SUCCESS || (int)params.size() >= maxParam) {
				break;
			}
		}

		while (eResult != E_SUCCESS) {
			if (it != end) {
				auto& expUnit = *it;
				if (expUnit->getType() == EXP_UNIT_CLOSED_BRACKET ||
					expUnit->getType() == EXP_UNIT_COMMA) {
					eResult = E_SUCCESS;
					break;
				}
			}
			return eResult;
		}

		if ((int)params.size() < maxParam) {
			list<ExpressionRef> expList;
			eResult = makeExpressionList(inputList, expList);

			if (eResult == E_SUCCESS && expList.size() != 1) {
				eResult = E_INCOMPLETED_EXPRESSION;
			}
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::compileConditionalExpression(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, ExecutableUnitRef& ifClauseUnit, ExecutableUnitRef& elseClauseUnit) {
		EExpressionResult eResult = E_SUCCESS;
		//find if clause expression
		//use dummy function to collect the unit for if clause
		DynamicParamFunctionRef dummyFuncion = std::make_shared<DynamicParamFunction>(":", EXP_UNIT_ID_FUNC_CHOICE, FUNCTION_PRIORITY_CONDITIONAL, 1);
		eResult = pickParamUnitsForFunction(it, end, dummyFuncion);

		if (eResult != E_SUCCESS) {
			return eResult;
		}

		if (it == end || (*it)->getType() != EXP_UNIT_ID_FUNC_CHOICE) {
			getCompiler()->setErrorText("Incompleted condition expression");
			eResult = E_INCOMPLETED_EXPRESSION;
			return eResult;
		}

		ifClauseUnit = dummyFuncion->getFirstParam();

		//find else clause expression
		DynamicParamFunctionRef choiceOperatorRef = dynamic_pointer_cast<DynamicParamFunction>((*it));

		it++;
		eResult = pickParamUnitsElseClause(it, end, choiceOperatorRef);

		if (eResult == E_SUCCESS) {
			elseClauseUnit = choiceOperatorRef->popParam();
		}
		else {

		}

		return eResult;
	}

	EExpressionResult ExpressionParser::putFunction(OperatorStack* pOperatorStack, OutputStack* pOutputStack, const DynamicParamFunctionRef& functionRef,
		list<ExpUnitRef>::const_iterator& iter, list<ExpUnitRef>::const_iterator end) {
		EExpressionResult eResult = E_SUCCESS;
		ScriptCompiler* scriptCompiler = getCompiler();

		int iResult;

		//push the function or operator to Operator stack
		//// before push to stack compare with top of Operator stack
		while (eResult == E_SUCCESS && pOperatorStack->size()) {

			auto& pExpFunction = pOperatorStack->top();
			if (!ISFUNCTION(pExpFunction) || pExpFunction->IsOrdered(functionRef.get()) == true) {
				break;
			}

			while (pOutputStack->size()) {
				auto& pFuncTemp1 = pOutputStack->top();
				iResult = pExpFunction->pushParamFront(pFuncTemp1);
				pOutputStack->pop();
				if (iResult < 0) {
					DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), pFuncTemp1->GetName()));
					scriptCompiler->setErrorText("Unexpected token '" + pFuncTemp1->toString() + "' inside '" + pExpFunction->getName() + "'");
					eResult = E_TOKEN_UNEXPECTED;
					break;
				}

				if (iResult == 0) {
					break;
				}
			}
			pOutputStack->push(pExpFunction);
			pOperatorStack->pop();
		}
		if (eResult == E_SUCCESS) {
			if (functionRef->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL) {
				ExecutableUnitRef ifClause, elseClause;

				iter++;
				eResult = compileConditionalExpression(iter, end, ifClause, elseClause);
				if (eResult != E_SUCCESS) {
					return eResult;
				}
				iter--;

				//pop the last unit from output stack
				ExecutableUnitRef conditionUnitRef = pOutputStack->top();
				pOutputStack->pop();

				//the last operand in output stack is condition unit
				functionRef->pushParam(conditionUnitRef);
				functionRef->pushParam(ifClause);
				functionRef->pushParam(elseClause);

				//param of conditional param now is fully collected
				//so it is pushed to output stack
				pOutputStack->push(functionRef);
			}
			else {
				pOperatorStack->push(functionRef);
			}
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::putAnExpUnit(list<ExpUnitRef>::const_iterator& iter, list<ExpUnitRef>::const_iterator end, ExpressionInputList& inputList) {
		EExpressionResult eResult = E_SUCCESS;
		if (iter == end) {
			return E_TYPE_END;
		}

		ExpUnitRef expUnitTemp;
		DynamicParamFunctionRef pExpFunction;
		ExecutableUnitRef pFuncTemp1;
		ScriptCompiler* scriptCompiler = getCompiler();
		auto& compilingContex = inputList.back();

		OperatorStack* pOperatorStack = compilingContex.second;
		OutputStack* pOutputStack = compilingContex.first;

		const ExpUnitRef& expUnit = *iter;

		if (ISFUNCTION(expUnit)) {
			DynamicParamFunctionRef functionRef = dynamic_pointer_cast<DynamicParamFunction>(expUnit);
			if (functionRef->getMaxParam() == (int)(functionRef->getParams().size())) {
				//for some reason, the function's param is fully collected.
				//so it is consider as operand function
				pOutputStack->push(functionRef);
			}
			else {
				if (eResult == E_SUCCESS) {
					eResult = putFunction(pOperatorStack, pOutputStack, functionRef, iter, end);
				}
			}
		}
		else if (ISOPERAND(expUnit)) {
			//push the operand to output stack
			pOutputStack->push(static_pointer_cast<ExecutableUnit>(expUnit));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_OPEN_BRACKET) {
			//push open bracket to current opertor stack and create new output stack and operator stack for
			//expression inside brackets
			pOperatorStack->push(static_pointer_cast<DynamicParamFunction>(expUnit));
			pOutputStack = new OutputStack();
			pOperatorStack = new OperatorStack();
			inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_FUNC_SEPERATOR) {
			//make expression tree for current output stack and operator stack
			eResult = makeExpression(*pOutputStack, *pOperatorStack);
			if (eResult != E_SUCCESS) {
				return eResult;
			}

			pOutputStack = new OutputStack();
			pOperatorStack = new OperatorStack();
			inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_CLOSED_BRACKET) {

			eResult = makeExpression(*pOutputStack, *pOperatorStack);
			if (/*pOutputStack->size() != 1 || */pOperatorStack->size() > 0) {
				scriptCompiler->setErrorText("Incompleted expression");
				eResult = E_INCOMPLETED_EXPRESSION;
			}
			if (eResult != E_SUCCESS) {
				return eResult;
			}
#if 0

			list<ExecutableUnitRef> params;
			if (pOutputStack->size() == 0) {
				//remove empty entry (outout stack and operator stack)
				inputList.pop_back();

				delete pOutputStack;
				delete pOperatorStack;

				//with an empty entry, it should be case someFunc()
				//so the previous must be a user function
				//or an variable with operator () is registered
				ExpressionEntry& entry = inputList.back();
				pOutputStack = entry.first;
				pOperatorStack = entry.second;

				if (pOperatorStack->size() < 2 || pOperatorStack->top()->getType() != EXP_UNIT_ID_OPEN_BRACKET) {
					scriptCompiler->setErrorText("Incompleted expression");
					eResult = E_INCOMPLETED_EXPRESSION;
					return eResult;
				}
				auto openBracket = pOperatorStack->top();
				pOperatorStack->pop();

				//previous operator is not a user function
				if (!ISUSERFUNC(pOperatorStack->top())) {
					scriptCompiler->setErrorText("Incompleted expression");
					eResult = E_INCOMPLETED_EXPRESSION;
					return eResult;
				}

				//push open bracket to stack again for further processing
				pOperatorStack->push(openBracket);
			}
			else {
				while (inputList.size())
				{
					ExpressionEntry& entry = inputList.back();
					pOutputStack = entry.first;
					pOperatorStack = entry.second;

					if (pOperatorStack->size() && pOperatorStack->top()->getType() == EXP_UNIT_ID_OPEN_BRACKET) {
						break;
					}
					inputList.pop_back();

					if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
						scriptCompiler->setErrorText("Incompleted expression");
						eResult = E_INCOMPLETED_EXPRESSION;
						break;
					}

					params.push_back(pOutputStack->top());

					delete pOutputStack;
					delete pOperatorStack;
				}
			}
#else
			auto paramCollectionUnit = new ParamUnitCollection();
			ExecutableUnitRef paramCollectionUnitRef = ExecutableUnitRef(paramCollectionUnit);
			auto& params = paramCollectionUnit->getParams();

			if (pOutputStack->size() == 0) {
				//remove empty entry (outout stack and operator stack)
				inputList.pop_back();
				delete pOutputStack;
				delete pOperatorStack;
			}

			while (inputList.size())
			{
				ExpressionEntry& entry = inputList.back();
				pOutputStack = entry.first;
				pOperatorStack = entry.second;

				if (pOperatorStack->size() && pOperatorStack->top()->getType() == EXP_UNIT_ID_OPEN_BRACKET) {
					break;
				}
				inputList.pop_back();

				if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
					scriptCompiler->setErrorText("Incompleted expression");
					eResult = E_INCOMPLETED_EXPRESSION;
					break;
				}
				params.push_front (pOutputStack->top());

				delete pOutputStack;
				delete pOperatorStack;
			}
#endif // 0
			if (eResult != E_SUCCESS) {
				return eResult;
			}
			//at least input list must contain open bracket
			//so it must be at least one layer
			if (inputList.size() == 0) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), expUnit->GetName()));
				scriptCompiler->setErrorText("Unexpected token '" + expUnit->toString() + "'");
				eResult = E_INCOMPLETED_EXPRESSION;
				return eResult;
			}

			if (pOperatorStack->size() == 0 || pOperatorStack->top()->getType() != EXP_UNIT_ID_OPEN_BRACKET) {
				scriptCompiler->setErrorText("Unexpected token ')'");
				return E_INCOMPLETED_EXPRESSION;
			}
			
			//pop open bracket, we don't need it any more
			auto bracket = pOperatorStack->top();
			pOperatorStack->pop();

			//get previous unit before open bracket
			list<ExpUnit*> tempUnits;
			tempUnits.push_back(bracket.get());
			if (pOperatorStack->size() > 0) {
				tempUnits.push_back(pOperatorStack->top().get());
			}
			if( pOutputStack->size() > 0) {
				tempUnits.push_back(pOutputStack->top().get());
			}
			tempUnits.sort([](ExpUnit* expUnit1, ExpUnit* expUnit2) {
				return expUnit1->getIndex() < expUnit2->getIndex();
			});

			ExpUnit* previousUnit = nullptr;
			auto it = std::find(tempUnits.begin(), tempUnits.end(), bracket.get());
			if (it == tempUnits.begin()) {
				previousUnit = nullptr;
			}
			else
			{
				it--;
				previousUnit = *it;
			}

			bool completedFunction = false;
			if (previousUnit != nullptr && ISOPERATOR(previousUnit)) {
				ExecutableUnitRef dummy;				
				if (((Function*)previousUnit)->pushParam(dummy) < 0) {
					completedFunction = true;
				}
				else {
					((Function*)previousUnit)->popParam();
				}
			}
			
			if (previousUnit != nullptr &&
				(!ISOPERATOR(previousUnit) || completedFunction ||
					previousUnit->getType() == EXP_UNIT_ID_OPERATOR_FUNCTIONCALL ||
					previousUnit->getType() == EXP_UNIT_ID_OPERATOR_SUBSCRIPT)) {
				//collect param inside brackets () for user functions like sin(a), sum(...)
				//Ex: 1 + 2 * sum(3,4,5)
#if  0
				pExpFunction = pOperatorStack->top();
				iResult = 0;
				for (auto it = params.begin(); it != params.end(); it++) {
					iResult = pExpFunction->pushParamFront(*it);
					if (iResult < 0) {
						DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), (*it)->GetName()));
						scriptCompiler->setErrorText("Unexpected token '" + (*it)->toString() + "'");
						break;
					}
				}
				if (iResult < 0) {
					eResult = E_TOKEN_UNEXPECTED;
					return eResult;
				}
				pOperatorStack->pop();
				pOutputStack->push(pExpFunction);
#else
				auto functionOperator = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_OPERATOR_FUNCTIONCALL, FUNCTION_PRIORITY_FUNCTIONCALL, 2);
				eResult = putFunction(pOperatorStack, pOutputStack, functionOperator, iter, end);
				if (eResult != E_SUCCESS) {
					return eResult;
				}
				//pust param collection of subscript operator to output stack
				//it will be processed later
				pOutputStack->push(paramCollectionUnitRef);
#endif //  0
				
			}
			else {
				//collect param inside brackets () for operators like *, +, ...
				//Ex: 6 + (1 + 2) * (5 - 1)
				//result inside (5-1) will be pushed to output stack to be judged and decidded what operator is its owner
				//in this case * is owner of (5-1), not the first operator +
				for (auto it = params.begin(); it != params.end(); it++) {
					pOutputStack->push(*it);
				}
			}
		}
		else if (expUnit->getType() == EXP_UNIT_ID_OPEN_SQUARE_BRACKET) {
			//push open bracket to current opertor stack and create new output stack and operator stack for
			//expression inside brackets
			pOperatorStack->push(static_pointer_cast<DynamicParamFunction>(expUnit));
			pOutputStack = new OutputStack();
			pOperatorStack = new OperatorStack();
			inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_CLOSED_SQUARE_BRACKET) {

			eResult = makeExpression(*pOutputStack, *pOperatorStack);
			if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
				scriptCompiler->setErrorText("Incompleted expression");
				eResult = E_INCOMPLETED_EXPRESSION;
			}
			if (eResult != E_SUCCESS) {
				return eResult;
			}
			auto paramCollectionUnit = new ParamUnitCollection();
			ExecutableUnitRef paramCollectionUnitRef = ExecutableUnitRef(paramCollectionUnit);
			auto& params = paramCollectionUnit->getParams();
			while (inputList.size())
			{
				ExpressionEntry& entry = inputList.back();
				pOutputStack = entry.first;
				pOperatorStack = entry.second;

				if (pOperatorStack->size() && pOperatorStack->top()->getType() == EXP_UNIT_ID_OPEN_SQUARE_BRACKET) {
					break;
				}
				inputList.pop_back();

				if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
					scriptCompiler->setErrorText("Incompleted expression");
					eResult = E_INCOMPLETED_EXPRESSION;
					break;
				}
				params.push_front(pOutputStack->top());

				delete pOutputStack;
				delete pOperatorStack;
			}

			if (eResult != E_SUCCESS) {
				return eResult;
			}
			//at least input list must contain open square bracket and subscription's owner
			//so it must be at least one layer
			if (inputList.size() < 1) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), expUnit->GetName()));
				scriptCompiler->setErrorText("Unexpected token '" + expUnit->toString() + "'");
				eResult = E_INCOMPLETED_EXPRESSION;
				return eResult;
			}

			if (pOperatorStack->size() == 0 || pOperatorStack->top()->getType() != EXP_UNIT_ID_OPEN_SQUARE_BRACKET) {
				scriptCompiler->setErrorText("Unexpected token ')'");
				return E_INCOMPLETED_EXPRESSION;
			}

			//replace unit '[' by subscript operator
			pOperatorStack->pop();
			auto subscriptOperator = make_shared<DynamicParamFunction>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, 2);
			eResult = putFunction(pOperatorStack, pOutputStack, subscriptOperator, iter, end);
			if (eResult != E_SUCCESS) {
				return eResult;
			}
			//pust param collection of subscript operator to output stack
			//it will be processed later
			pOutputStack->push(paramCollectionUnitRef);
		}
		else {
			//unknown expression unit type
			DBG_ERROR(_tprintf(__T("\n[#]unknown type %X of token '%s'"), expUnit->GetType(), expUnit->GetName()));
			scriptCompiler->setErrorText("internal error: unknow type of token '" + expUnit->toString() + "'");
			eResult = E_TOKEN_TYPE_UNEXPECTED;
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::makeExpressionList(ExpressionInputList& inputList, list<ExpressionRef>& expList) {
		OperatorStack* pOperatorStack;
		OutputStack* pOutputStack;
		Expression* pExp = NULL;
		EExpressionResult eResult = E_SUCCESS;
		ScriptCompiler* scriptCompiler = getCompiler();

		while (inputList.size()) {
			ExpressionEntry& entry = inputList.back();
			pOutputStack = entry.first;
			pOperatorStack = entry.second;
			inputList.pop_back();

			if (eResult == E_SUCCESS) {
				eResult = makeExpression(*pOutputStack, *pOperatorStack);

				if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
					eResult = E_INCOMPLETED_EXPRESSION;
					scriptCompiler->setErrorText("Incompleted expression");
				}
				if (eResult != E_SUCCESS) {
					expList.clear();
				}
				else {
					pExp = new Expression(dynamic_pointer_cast<ExecutableUnit>(pOutputStack->top()));
					expList.push_back(ExpressionRef(pExp));
				}
			}

			delete pOutputStack;
			delete pOperatorStack;
		}
		return eResult;
	}

	bool ExpressionParser::compile(list<ExpUnitRef>& expUnitList, list<ExpressionRef>& expList, EExpressionResult* peResult) {
		numberedForUnit(expUnitList);
		if (precheck(expUnitList) == false) return false;
		ExpressionInputList inputList;

		EExpressionResult eResult1;
		EExpressionResult* pResult = &eResult1;
		if (peResult) pResult = peResult;
		EExpressionResult& eResult = *pResult;

		eResult = E_SUCCESS;

		OperatorStack* pOperatorStack = new OperatorStack();;
		OutputStack* pOutputStack = new OutputStack();
		inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));

		DBG_INFO(List_Enumerate(&expUnitList, printNode));

		for (auto iter = expUnitList.begin(); iter != expUnitList.end() && eResult == E_SUCCESS; iter++)
		{
			eResult = putAnExpUnit(iter, expUnitList.end(), inputList);
		}

		if (eResult != E_SUCCESS) {
			return false;
		}

		eResult = makeExpressionList(inputList, expList);

		return eResult == E_SUCCESS && expList.size() > 0;
	}

#if 0 /*following function is not used any more*/
	bool findAccuracyForCandidate(
		ScriptCompiler* scriptCompiler,
		const ScriptType& refVoidType,
		const ScriptType& expectedType, const ScriptType& actualType,
		ParamCastingInfo& paramInfo) {

		bool valid = true;
		FunctionFactory* functionFactory;
		int accurative;
		int functionId;
		//expected type and actual type is not same
		//but actual type is not ref of specific type and expected type is not ref void
		//in c++ we can see the case, int* will not cast to void* automatically
		if (actualType != expectedType && !(actualType.isRefType() && refVoidType == expectedType)) {

			const list<OverLoadingItem>* castingFuncs = nullptr;
			bool makingRef = false;
			//check argument type is reference but param type is not
			if (expectedType.refLevel() - actualType.refLevel() == 1 && expectedType.origin() == expectedType.origin()) {
				//consider ref function as a casting function
				makingRef = true;
			}
			else if (!actualType.isRefType() && refVoidType == expectedType ) {
				//param type is ref void but actual type is another ref type
				makingRef = true;
			}
			else {
				//find a function can cast from actual type to expected type
				castingFuncs = scriptCompiler->findOverloadingFuncRoot(expectedType.sType());
			}
			if (makingRef) {
				functionId = scriptCompiler->getMakingRefFunction();
				if (functionId == -1) {
					valid = false;
				}
				else {
					paramInfo.castingFunction = scriptCompiler->getFunctionFactory(functionId);
					paramInfo.accurative = 2;
				}
			}
			else {
				if (castingFuncs == nullptr) {
					valid = false;
				}
				else {
					//check casting function is valid or not
					//a valid casting function receive must receive only one parameter in actualType
					auto it2 = castingFuncs->begin();
					for (; it2 != castingFuncs->end(); ++it2) {
						if (it2->paramTypes.size() == 1) {
							auto& arg0Type = *(it2->paramTypes[0]);
							if (arg0Type == actualType) {
								break;
							}
							if (arg0Type == refVoidType && !actualType.isRefType()) {
								break;
							}
						}
						
					}
					if (it2 == castingFuncs->end()) {
						//not a valid function
						valid = false;
					}
					else {
						//check return type of the function
						functionFactory = scriptCompiler->getFunctionFactory(it2->functionId);
						if (functionFactory == nullptr || functionFactory->getReturnType() != expectedType &&
							!(refVoidType == expectedType && functionFactory->getReturnType().isRefType())) {
							//return type is not match or function factory not found
							//if function factory not found this is an error of the library
							valid = false;
						}
						else {
							accurative = scriptCompiler->findConversionAccurative(actualType.iType(), expectedType.iType());
							if (accurative < 0) {
								valid = false;
							}
							else {
								paramInfo.accurative = accurative;
								paramInfo.castingFunction = functionFactory; 
							}
						}
					}
				}
			}
		}

		return valid;
	}
#endif


	bool inline canMakeSemiRef(const ScriptType& argType, const ScriptType& paramType) {
		return argType.isSemiRefType() && !paramType.isSemiRefType() && argType.refLevel() == paramType.refLevel() && argType.origin() == paramType.origin();
	}

	bool findMatchingLevel1(
		ScriptCompiler* scriptCompiler,
		const ScriptType& refVoidType,
		const ScriptType& argumentType, const ScriptType& paramType,
		ParamCastingInfo& paramInfo) {
		//same type => perfect match
		if (argumentType == paramType) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}
		if (argumentType.refLevel() == 1 && paramType.isSemiRefType() && argumentType.origin() == paramType.origin()) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}

		if (argumentType == refVoidType && paramType.isRefType()) {
			paramInfo.accurative = 2; //change a specific ref type to general ref type(Ex in c++: int* -> void*
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}

		//not same origin type
		if ( argumentType.origin() != paramType.origin()) {
			if (argumentType.isRefType() && (paramType.iType() & DATA_TYPE_ARRAY_MASK)) {
				auto arrayInfo = (StaticArrayInfo*)scriptCompiler->getTypeInfo(paramType.origin());
				ScriptType elmType(arrayInfo->elmType, "");
				if (arrayInfo && elmType.origin() == argumentType.origin() && argumentType.refLevel() - (int)arrayInfo->refLevel == 1) {
					if (paramType.isSemiRefType()) {
						paramInfo.accurative = 2; //change a specific ref type of static array to its element's ref type(Ex: array<int>& -> ref int)
						paramInfo.castingFunction = nullptr; //no need to casting
					}
					else {
						int functionId = scriptCompiler->getMakingRefFunction();
						if (functionId == -1) {
							scriptCompiler->setErrorText("Internal error: ref operator is not defined");
							return false;
						}
						paramInfo.castingFunction = FunctionRef(scriptCompiler->createFunctionFromId(functionId));
						paramInfo.castingFunction->setReturnType(argumentType);
						paramInfo.accurative = 2;
					}
				}
				return true;
			}
			return false;
		}
		// now two types have the same origin, check if they are not same ref level
		if (paramType.refLevel() - argumentType.refLevel() >= 1) {
			//param type is deeper level of ref than argument type
			return false;
		}

		if (argumentType.refLevel() - paramType.refLevel() == 1) {
			//argument type is ref type but param type is not
			//we can make reference to the param type
			//so this case is acceptable
			int functionId = scriptCompiler->getMakingRefFunction();
			if (functionId == -1) {
				scriptCompiler->setErrorText("Internal error: ref operator is not defined");
				return false;
			}
			paramInfo.castingFunction = FunctionRef(scriptCompiler->createFunctionFromId(functionId));
			paramInfo.castingFunction->setReturnType(argumentType);
			paramInfo.accurative = 2;
			
			return true;
		}
		if (canMakeSemiRef(argumentType, paramType)) {
			int functionId = scriptCompiler->getMakingRefFunction();
			if (functionId == -1) {
				scriptCompiler->setErrorText("Internal error: ref operator is not defined");
				return false;
			}
			paramInfo.castingFunction = FunctionRef(scriptCompiler->createFunctionFromId(functionId));
			paramInfo.castingFunction->setReturnType(argumentType);
			paramInfo.accurative = 2;

			return true;
		}
		if (canMakeSemiRef(paramType, argumentType)) {
			auto nativeFunction = new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentType);
			paramInfo.castingFunction = FunctionRef(nativeFunction);
			nativeFunction->setNative((DFunction2Ref)(new DeRefCommand(scriptCompiler->getTypeSize(argumentType))));
			paramInfo.accurative = 2;

			return true;
		}

		return false;
	}

	bool findMatchingLevel2(
		ScriptCompiler* scriptCompiler,
		const ScriptType& argumentType, const ScriptType& paramType,
		ParamCastingInfo& paramInfo) {
		//same type => perfect match
		if (argumentType == paramType) {
			paramInfo.accurative = 0; //best match
			paramInfo.castingFunction = nullptr; //no need to casting
			return true;
		}

		string castingFunction = scriptCompiler->getType(argumentType.iType());
		int functionId = scriptCompiler->findFunction(castingFunction, paramType.sType());
		if (functionId < 0) {
			return false;
		}

		paramInfo.accurative = scriptCompiler->findConversionAccurative(paramType.iType(), argumentType.iType());
		paramInfo.castingFunction = FunctionRef(scriptCompiler->createFunctionFromId(functionId));

		return true;
	}

#if 0 /*following function are no longer used*/
	EExpressionResult findCandidates(ScriptCompiler* scriptCompiler,
		const FunctionRef& function,
		const list<OverLoadingItem>& overloadingFuncs,
		list<CandidateInfo>& overloadingCandidates) {
		const string& functionName = function->getName();
		int n = function->getChildCount();
		ParamCastingInfo paramInfoTemp;
		paramInfoTemp.accurative = 0;
		paramInfoTemp.castingFunction = nullptr;

		//filter overloading functions by number of parameter
  		for (auto it = overloadingFuncs.begin(); it != overloadingFuncs.end(); ++it) {
			if ((*it).paramTypes.size() == (size_t)n) {
				const OverLoadingItem& item = *it;

				CandidateInfo candidateInfo;
				overloadingCandidates.push_back(candidateInfo);
				CandidateInfo& candidateInfoRef = overloadingCandidates.back();
				candidateInfoRef.item = &item;
				candidateInfoRef.paramCasting.resize(n, paramInfoTemp);
			}
		}

		ScriptType refVoidType(scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");
		int overloadingSize = (int)overloadingCandidates.size();
		//filter overloading functions by number of parameter types
		for (int i = 0; i < n; i++) {
			auto& paramType = function->getChild(i)->getReturnType();
			auto it = overloadingCandidates.begin();
			decltype(it) itTemp;

			while (it != overloadingCandidates.end()) {
				auto& argumentType = *(it->item->paramTypes[i].get());
				if (findMatchingLevel1(scriptCompiler, refVoidType, argumentType, paramType, it->paramCasting.at(i))) {
					++it;
				}
				else if (overloadingSize == 1 && findMatchingLevel2(scriptCompiler, argumentType, paramType, it->paramCasting.at(i))) {
					++it;
				}
				else {
					itTemp = it;
					++it;
					overloadingCandidates.erase(itTemp);
				}
			}
		}

		return E_SUCCESS;
	}

	EExpressionResult findCandidates(ScriptCompiler* scriptCompiler, const FunctionRef& function, list<CandidateInfo>& overloadingCandidates) {
		const string& functionName = function->getName();
		const list<OverLoadingItem>* overloadingFuncs = scriptCompiler->findOverloadingFuncRoot(functionName);

		if (!overloadingFuncs) {
			return E_FUNCTION_NOT_FOUND;
		}

		return findCandidates(scriptCompiler, function, *overloadingFuncs, overloadingCandidates);
	}
#endif
	const CandidateInfo* chooseFunction(ScriptCompiler* scriptCompiler, const list<CandidateInfo>& candidates, int paramCount, EExpressionResult& eResult) {
		int bestAcurative, totalAcurative;

		bestAcurative = 0x7FFFFFFF;
		const CandidateInfo* bestCandidate = nullptr;

		for (auto it = candidates.begin(); it != candidates.end(); ++it) {
			totalAcurative = 0;
			for (int i = 0; i < paramCount; i++) {
				totalAcurative += it->paramCasting.at(i).accurative;
			}
			if (bestAcurative > totalAcurative) {
				bestCandidate = &(*it);
				bestAcurative = totalAcurative;
			}
		}

		return bestCandidate;
	}

	int checkAssigmentOperatorForStruct(ScriptCompiler* scriptCompiler, const StructClass* pStruct, const DynamicParamFunctionRef& function) {
		MemberInfo memberInfo;
		bool res = pStruct->getMemberFirst(nullptr, &memberInfo);
		auto& params = function->getParams();
		auto iter = params.begin();
		int i = 0;
		bool blError;
		while (res && iter != params.end())
		{
			if (memberInfo.type != iter->get()->getReturnType()) {
				blError = true;
				//does not allow different type assigment for each member in the struct
				const StructClass* pSubStruct = scriptCompiler->getStruct(memberInfo.type.iType());
				if (pSubStruct != nullptr && iter->get()->getType() == EXP_UNIT_ID_DYNAMIC_FUNC) {
					if (checkAssigmentOperatorForStruct(scriptCompiler, pSubStruct, dynamic_pointer_cast<DynamicParamFunction>(*iter)) < 0)
						blError = false;

				}
				if(blError)
					return i;
			}
			res = pStruct->getMemberNext(nullptr, &memberInfo);
			iter++;
			i++;
		}
		if (i != (int)params.size()) {
			return i;
		}

		return -1;
	}

	bool convertToRef(ScriptCompiler* scriptCompiler, ExecutableUnitRef& param) {
		if (param->getType() == EXP_UNIT_ID_DEREF) {
			//make ref meet deref then both is removed from the expression
			auto& paramOfDeref = ((Function*)param.get())->getChild(0);
			param = paramOfDeref;
			return true;
		}
		int refFunctionId = scriptCompiler->getMakingRefFunction();
		if (refFunctionId < 0) {
			scriptCompiler->setErrorText("Internal error: making ref function is missing");
			return false;
		}

		Function* refFunction = (Function*)scriptCompiler->createFunctionFromId(refFunctionId);
		refFunction->pushParam(param);
		refFunction->setMask(MaskType::CastingUnitNotInExpression);
		param.reset((ExecutableUnit*)(refFunction));

		return true;
	}

	CandidateCollectionRef findApproxiateDefaultOperator(ScriptCompiler* scriptCompiler, FunctionRef funcNode, const std::vector<CandidateCollectionRef>& candidatesForParams) {
		int childCount = funcNode->getChildCount();

		std::list<std::vector<ExecutableUnitRef>> paramPaths;
		listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

		auto& basicTypes = scriptCompiler->getTypeManager()->getBasicTypes();
		CandidateCollectionRef defaultOperators = std::make_shared<CandidateCollection>();

		//CandidateCollectionRef defaultMemberAssigmentOperators1;
		//CandidateCollectionRef defaultMemberAssigmentOperators2;

		for (auto pathIt = paramPaths.begin(); pathIt != paramPaths.end(); pathIt++) {
			auto& pathParam = *pathIt;

			if (funcNode->getType() == EXP_UNIT_ID_CREATE_LAMBDA) {
				DynamicParamFunction* createLambdaCandidate = new DynamicParamFunction(SYSTEM_LAMBDA_FUNCTION, EXP_UNIT_ID_CREATE_LAMBDA, FUNCTION_PRIORITY_USER_FUNCTION, (int)pathParam.size());
				
				for (auto it = pathParam.begin(); it != pathParam.end(); it++) {
					createLambdaCandidate->pushParam(*it);
				}
				auto& lastParam = pathParam[pathParam.size() - 1];
				int anoynymousFunctionId = *(int*)(lastParam->Execute());
				auto functionInfo = scriptCompiler->getFunctionLib()->findFunctionInfo(anoynymousFunctionId);
			
				int iType = scriptCompiler->buildFunctionType(*functionInfo);
				ScriptType returnType(iType, scriptCompiler->getType(iType));				
				createLambdaCandidate->setReturnType(returnType);

				defaultOperators->push_back(ExecutableUnitRef(createLambdaCandidate));
				continue;
			}

			if (pathParam.size() == 2) {
				auto& param1 = pathParam.at(0);
				auto& param2 = pathParam.at(1);

				auto& param1Type = param1->getReturnType();
				auto& param2Type = param2->getReturnType();

				//check default copy constructor
				if (funcNode->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT || funcNode->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR) {
					if (param1Type == param2Type) {
						//default assigment operator for all types
						if (!convertToRef(scriptCompiler, param1)) {
							return nullptr;
						}						

						auto defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param2Type.makeSemiRef());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
					else if (param1Type.origin() == param2Type.origin() && param1Type.refLevel() == param2Type.refLevel()/* && param2Type.isSemiRefType()*/) {
						//default assigment operator for all types
						if (!param1Type.isSemiRefType()) {
							if (!convertToRef(scriptCompiler, param1)) {
								return nullptr;
							}
						}
						FixParamFunction<2>* defaultOperatorUnit;
						if (param2Type.isSemiRefType()) {
							defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR_REF, FUNCTION_PRIORITY_ASSIGNMENT, param2Type);
						}
						else {
							defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param2Type.makeSemiRef());
						}
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
					else if (param1Type.refLevel() == 0 && param1Type.isFunctionType() && param2Type.iType() == basicTypes.TYPE_NULL) {
						if (!convertToRef(scriptCompiler, param1)) {
							return nullptr;
						}
					
						RuntimeFunctionInfo nullValueOfRuntimeFunctionInfo;
						defaultRuntimeFunctionInfoConstructor(&nullValueOfRuntimeFunctionInfo);
						auto nullValueRef = make_shared<CConstOperand<RuntimeFunctionInfo>>(nullValueOfRuntimeFunctionInfo, param1Type);
						auto defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param1Type.makeSemiRef());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(nullValueRef);

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}

					const StructClass* pStructInfo = scriptCompiler->getStruct(param1Type.origin());
					if (pStructInfo != nullptr && param2Type.origin() == basicTypes.TYPE_VARIANTARRAY) {
						//default assigment operator for variant array to struct
						if (param1Type.isRefType() == false) {
							if (!convertToRef(scriptCompiler, param1)) {
								return nullptr;
							}
						}

						auto defaultOperatorUnit = new FixParamFunction<2>("=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, pStructInfo->getName());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);
						defaultOperatorUnit->setReturnType(param1Type);
						defaultOperatorUnit->setNative((DFunction2Ref)(new ArrayToStructCommand()));

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
				}

				if (funcNode->getType() == EXP_UNIT_ID_OPERATOR_SUBSCRIPT) {
					if (param1Type.isRefType()) {
						//argument #2 is integer, so we need to find a way to convert current value to integer
						if (param2Type.iType() != basicTypes.TYPE_INT) {
							int integerCasting = scriptCompiler->findFunction(scriptCompiler->getType(basicTypes.TYPE_INT), param2Type.sType());
							if (integerCasting < 0) {
								continue;
							}
							//the following code is casting before execute param 2 unit
							auto castingFunction = scriptCompiler->createFunctionFromId(integerCasting);
							castingFunction->pushParam(param2);
							castingFunction->setMask(MaskType::CastingUnitNotInExpression);
							//replace param 2 by casting function
							param2.reset(castingFunction);
						}

						//ScriptType returnType1 = param1Type.deRef();
						//ScriptType returnType2 = param1Type;
						//int elementSize = scriptCompiler->getTypeSize(returnType1);

						//auto defaultOperatorUnit1 = new FixParamFunction<2>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, returnType1);
						//defaultOperatorUnit1->pushParam(param1);
						//defaultOperatorUnit1->pushParam(param2);
						//defaultOperatorUnit1->setNative((DFunction2Ref)(new ElementAccessCommand1(elementSize)));

						//auto defaultOperatorUnit2 = new FixParamFunction<2>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, returnType2);
						//defaultOperatorUnit2->pushParam(param1);
						//defaultOperatorUnit2->pushParam(param2);
						//defaultOperatorUnit2->setNative((DFunction2Ref)(new ElementAccessCommand2(elementSize)));

						//defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit1));
						//defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));

						ScriptType returnType1 = param1Type.deRef();
						ScriptType returnType2 = returnType1.makeSemiRef();
						int elementSize = scriptCompiler->getTypeSize(returnType1);

						auto defaultOperatorUnit2 = new FixParamFunction<2>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, returnType2);
						defaultOperatorUnit2->pushParam(param1);
						defaultOperatorUnit2->pushParam(param2);
						defaultOperatorUnit2->setNative((DFunction2Ref)(new ElementAccessCommand2(elementSize)));

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));
						
						continue;
					}
					if ( (param1Type.iType() & DATA_TYPE_ARRAY_MASK) ) {
						if (param1Type.isRefType() == false) {
							//argument #2 is integer, so we need to find a way to convert param2 to integer
							if (param2Type.iType() != basicTypes.TYPE_INT) {
								int integerCasting = scriptCompiler->findFunction(scriptCompiler->getType(basicTypes.TYPE_INT), param2Type.sType());
								if (integerCasting < 0) {
									continue;
								}
								//the following code is casting before execute param 2 unit
								auto castingFunction = scriptCompiler->createFunctionFromId(integerCasting);
								castingFunction->pushParam(param2);
								castingFunction->setMask(MaskType::CastingUnitNotInExpression);
								//replace param 2 by casting function
								param2.reset(castingFunction);
							}
							auto pArrayInfo = (StaticArrayInfo*)scriptCompiler->getTypeInfo(param1Type.origin());
							if (pArrayInfo == nullptr) {
								scriptCompiler->setErrorText("Internal error: array type's information is missing");
								return nullptr;
							}							
							ScriptType returnType1(pArrayInfo->elmType, scriptCompiler->getType(pArrayInfo->elmType));
							ScriptType returnType2 = returnType1.makeSemiRef();

							auto defaultOperatorUnit2 = new FixParamFunction<2>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_STATIC_ARRAY_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, returnType2);
							defaultOperatorUnit2->pushParam(param1);
							defaultOperatorUnit2->pushParam(param2);

							defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));
							continue;
						}
					}
				}

				if (funcNode->getType() == EXP_UNIT_ID_USER_OPER && funcNode->getName() == "==") {
					if (param1Type.refLevel() == 0 && param1Type.isFunctionType() && param2Type.refLevel() == 0 && param2Type.isFunctionType()) {
						if (!convertToRef(scriptCompiler, param1)) {
							return nullptr;
						}
						if (!convertToRef(scriptCompiler, param2)) {
							return nullptr;
						}
						ScriptType boolType(basicTypes.TYPE_BOOL, "bool");
						auto defaultOperatorUnit = new FixParamFunction<2>("=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, boolType.sType());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);
						defaultOperatorUnit->setReturnType(boolType);
						defaultOperatorUnit->setNative((DFunction2Ref)(new CompareRuntimeFunction()));

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
				}

				const StructClass* pStructInfo = scriptCompiler->getStruct(param1Type.origin());
				if (funcNode->getType() == EXP_UNIT_ID_MEMBER_ACCESS && pStructInfo != nullptr && param2Type.iType() == basicTypes.TYPE_ELEMENT_INFO) {
					if (param1Type.refLevel() > 1) {
						return nullptr;
					}
					//default member assign operator for variant array to struct
					if (param1Type.isRefType() == false) {
						if (param1Type.isSemiRefType() == false && !convertToRef(scriptCompiler, param1)) {
							return nullptr;
						}						
					}
					else if(param1Type.isSemiRefType()) {
						auto argumentType = param1Type.deSemiRef();
						auto deRefFunction = new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentType);
						deRefFunction->setNative((DFunction2Ref)(new DeRefCommand(scriptCompiler->getTypeSize(argumentType))));
						deRefFunction->pushParam(param1);
						param1.reset(deRefFunction);
					}

					//after link, param 2 is contain member info. Now, it's time to retreive it
					MemberInfo* memberInfo = (MemberInfo*)param2->Execute();
					int returnSize = scriptCompiler->getTypeSize(memberInfo->type);

					ScriptType typeInt(basicTypes.TYPE_INT, "int");
					ScriptType& memberType = memberInfo->type;

					CConstOperand<int>* param2New = new CConstOperand<int>(memberInfo->offset, typeInt);
					ExecutableUnitRef param2NewRef(param2New);

					//auto defaultOperatorUnit1 = new FixParamFunction<2>(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, memberType);
					//defaultOperatorUnit1->pushParam(param1);
					//defaultOperatorUnit1->pushParam(param2NewRef);
					//defaultOperatorUnit1->setNative((DFunction2Ref)(new MemberAccessCommand1(returnSize)));

					//auto defaultOperatorUnit2 = new FixParamFunction<2>(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, memberType.makeRef());
					auto defaultOperatorUnit2 = new FixParamFunction<2>(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, memberType.makeSemiRef());
					defaultOperatorUnit2->pushParam(param1);
					defaultOperatorUnit2->pushParam(param2NewRef);
					defaultOperatorUnit2->setNative((DFunction2Ref)(new MemberAccessCommand2()));

					//defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit1));
					defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));
					continue;
				}
			}

			if (pathParam.size() == 1) {
				auto& param1 = pathParam.at(0);
				auto& param1Type = param1->getReturnType();

				if (funcNode->getType() == EXP_UNIT_ID_DEREF && param1Type.isRefType()) {
					if (param1->getType() == EXP_UNIT_ID_MAKE_REF) {

						//deref and makeref function will lead to remove all of them from expression
						auto& paramOfRef = ((Function*)param1.get())->getChild(0);
						defaultOperators->push_back(paramOfRef);
					}
					else {
						ScriptType returnType = param1Type.deRef();
						int typeSize = scriptCompiler->getTypeSize(returnType);

						auto defaultOperatorUnit = new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, returnType);
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->setNative((DFunction2Ref)(new DeRefCommand(typeSize)));

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
					}
					continue;
				}
			}
		}

		return defaultOperators;
	}
	
	struct CandidatePathInfo {
		CandidateInfo candidate;
		std::vector<ExecutableUnitRef>* paramPath;
	};

	template<class T>
	T* makeRef(const T& val) {
		return (T*)&val;
	}

	void applyCasting(ExecutableUnitRef& unit, const FunctionRef& castingUnit) {
		if ((castingUnit->getType() == EXP_UNIT_ID_MAKE_REF && unit->getType() == EXP_UNIT_ID_DEREF) ||
			(castingUnit->getType() == EXP_UNIT_ID_SEMI_REF && unit->getType() == EXP_UNIT_ID_DEREF) ||
			(castingUnit->getType() == EXP_UNIT_ID_DEREF && unit->getType() == EXP_UNIT_ID_MAKE_REF) ||
			(castingUnit->getType() == EXP_UNIT_ID_DEREF && unit->getType() == EXP_UNIT_ID_SEMI_REF)) {
			//remove making ref function and deref function when it meet toghether
			unit = ((Function*)unit.get())->getChild(0);
		}
		else {
			castingUnit->pushParam(unit);
			unit = castingUnit;
		}
	}

	//	return std::shared_ptr<list<OverLoadingItem>>(candidates);
	//}
	/* return best param path for expected argument
		param count and argument count must be equal
	*/
	std::pair<std::vector<ExecutableUnitRef>*,int> filterCandidate2(ScriptCompiler* scriptCompiler, const std::vector<ScriptTypeRef>& argTypes,
		std::list<std::vector<ExecutableUnitRef>>& paramPaths, EExpressionResult& eResult) {		
		
		ParamCastingInfo paramInfoTemp;
		paramInfoTemp.accurative = 0;
		paramInfoTemp.castingFunction = nullptr;
		int n = (int)argTypes.size();
		int i;

		ScriptType refVoidType(scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");		

		std::list<CandidatePathInfo> candidatePaths;
		for (auto pit = paramPaths.begin(); pit != paramPaths.end(); pit++) {
			auto& path = *pit;

			CandidatePathInfo tmp;
			candidatePaths.push_back(tmp);
			auto& candidatePathInfo = candidatePaths.back();
			auto& paramCastingList = candidatePathInfo.candidate.paramCasting;
			paramCastingList.resize(n);

			for (i = 0; i < n; i++) {
				auto& paramType = path[i]->getReturnType();
				auto& argumentType = *argTypes[i];
				if (findMatchingLevel1(scriptCompiler, refVoidType, argumentType, paramType, paramCastingList.at(i))) {
					;
				}
				else if (findMatchingLevel2(scriptCompiler, argumentType, paramType, paramCastingList.at(i))) {
					;
				}
				else {
					break;
				}
			}
			if (i < n) {
				candidatePaths.pop_back();
			}
			else {
				candidatePathInfo.paramPath = &path;
				candidatePathInfo.candidate.item = nullptr;
				auto& totalAccurative = candidatePathInfo.candidate.totalAccurative;
				totalAccurative = 0;
				for (auto it = paramCastingList.begin(); it != paramCastingList.end(); it++) {
					totalAccurative += it->accurative;
				}
			}
		}

		if (candidatePaths.size() == 0) {
			return std::make_pair(nullptr, -1);
		}

		auto it = std::min_element(candidatePaths.begin(), candidatePaths.end(), [](CandidatePathInfo& p1, CandidatePathInfo& p2) { return p1.candidate.totalAccurative < p2.candidate.totalAccurative; });

		auto& bestCandidateInfo = *it;
		auto& path = *bestCandidateInfo.paramPath;
		auto& castingList = bestCandidateInfo.candidate.paramCasting;

		auto jt = path.begin();
		auto kt = argTypes.begin();
		for (auto it = castingList.begin(); it != castingList.end(); it++) {
			auto& castingInfo = *it;
			auto& castingF = castingInfo.castingFunction;
			if (castingF) {
				auto& paramRef = *jt;
				applyCasting(paramRef, castingF);
				paramRef->setReturnType(*(kt->get()));
			}
			jt++;
			kt++;
		}

		return std::make_pair(it->paramPath, it->candidate.totalAccurative);
	}

	CandidateCollectionRef filterCandidate(ScriptCompiler* scriptCompiler,
		const string& functionName, int functionType,
		const list<OverLoadingItem>* overloadingFuncs,
		const std::vector<CandidateCollectionRef>& candidatesForParams, EExpressionResult& eResult) {

		int n = (int)candidatesForParams.size();
		ParamCastingInfo paramInfoTemp;
		paramInfoTemp.accurative = 0;
		paramInfoTemp.castingFunction = nullptr;

		list<CandidateInfo> overloadingCandidatesOrigin;

		//filter overloading functions by number of parameter
		for (auto it = overloadingFuncs->begin(); it != overloadingFuncs->end(); ++it) {
			if ((*it).paramTypes.size() == (size_t)n) {
				const OverLoadingItem& item = *it;

				CandidateInfo candidateInfo;
				overloadingCandidatesOrigin.push_back(candidateInfo);
				CandidateInfo& candidateInfoRef = overloadingCandidatesOrigin.back();
				candidateInfoRef.item = &item;
				candidateInfoRef.paramCasting.resize(n, paramInfoTemp);
			}
		}

		ScriptType refVoidType(scriptCompiler->getTypeManager()->getBasicTypes().TYPE_VOID | DATA_TYPE_POINTER_MASK, "ref void");
		int overloadingSize = (int)overloadingCandidatesOrigin.size();

		std::list<std::vector<ExecutableUnitRef>> paramPaths;
		listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

		std::map<int, CandidatePathInfo> candidateMap;
		for (auto pit = paramPaths.begin(); pit != paramPaths.end(); pit++) {
			auto& path = *pit;
			//prevent following expression to be executed by a native assigment function
			// int b = 1;
			// ref int a = b;
			//but it will be processed by default assignment operator
			if (functionType == EXP_UNIT_ID_OPERATOR_ASSIGNMENT && path.size() == 2) {
				auto& param1Type = path[0]->getReturnType();
				auto& param2Type = path[1]->getReturnType();

				if (path[0]->getType() == EXP_UNIT_ID_XOPERAND && param2Type.origin() == param1Type.origin() && param1Type.refLevel() != param2Type.refLevel()) {
					//continue
					//following assigment is not allow
					//ref int b;
					//int a;
					//b = a;
					continue;
				}
			}

			list<CandidateInfo> overloadingCandidates = overloadingCandidatesOrigin;
			for (int i = 0; i < n; i++) {
				auto& paramType = path[i]->getReturnType();
				auto it = overloadingCandidates.begin();
				decltype(it) itTemp;

				while (it != overloadingCandidates.end()) {
					auto& argumentType = *(it->item->paramTypes[i]);
					if (findMatchingLevel1(scriptCompiler, refVoidType, argumentType, paramType, it->paramCasting.at(i))) {
						++it;
					}
					else if (overloadingSize == 1 && findMatchingLevel2(scriptCompiler, argumentType, paramType, it->paramCasting.at(i))) {
						++it;
					}
					else {
						itTemp = it;
						++it;
						overloadingCandidates.erase(itTemp);
					}
				}
			}

			//copy candidate to map but no duplicate candidate(function) id
			for (auto cit = overloadingCandidates.begin(); cit != overloadingCandidates.end(); cit++) {
				CandidatePathInfo candidate;
				candidate.candidate = *cit;
				candidate.paramPath = &path;
				auto it = candidateMap.insert(std::make_pair(candidate.candidate.item->functionId, candidate));
				CandidatePathInfo& insertedCandidate = it.first->second;
				//if the canidate id is exits, compare accurative to choose the best one
				if (it.second == false) {
					//calcuate total accurative for each
					int acc1 = insertedCandidate.candidate.totalAccurative;
					int acc2 = 0;
					for (int j = 0; j < n; j++) {
						acc2 += candidate.candidate.paramCasting.at(j).accurative;
					}
					//the better found, so we repelace it
					if (acc1 > acc2) {
						candidate.candidate.totalAccurative = acc2;
						it.first->second = candidate;
					}
				}
				else {
					int acc1 = 0;
					for (int j = 0; j < n; j++) {
						acc1 += insertedCandidate.candidate.paramCasting.at(j).accurative;
					}
					insertedCandidate.candidate.totalAccurative = acc1;
				}
			}
		}

		//now we have all candidate, we remove duplicate by function id
		//but if two function return the same type, it is an ambious call.
		//so, now we check ambious call
		for (auto it = candidateMap.begin(); it != candidateMap.end(); it++) {
			FunctionFactory* fp1 = scriptCompiler->getFunctionFactory(it->first);
			auto jt = it;
			for (++jt; jt != candidateMap.end(); jt++) {

				FunctionFactory* fp2 = scriptCompiler->getFunctionFactory(jt->first);
				if (fp1->getReturnType() == fp2->getReturnType()) {
					eResult = E_TYPE_AMBIOUS_CALL;
					scriptCompiler->setErrorText("ambious call for function " + functionName);
					return nullptr;
				}
			}
		}
		vector<std::pair<const int, CandidatePathInfo>*> candidateElems(candidateMap.size());
		auto elmIt = candidateElems.begin();
		for (auto it = candidateMap.begin(); it != candidateMap.end(); it++) {
			*elmIt++ = &(*it);
		}
		std::sort(candidateElems.begin(), candidateElems.end(),
			[](std::pair<const int, CandidatePathInfo>* elm1, std::pair<const int, CandidatePathInfo>* elm2) {
			return elm1->second.candidate.totalAccurative < elm2->second.candidate.totalAccurative;
		});

		//everything is ok now, it's time to build candidates		
		CandidateCollectionRef functionCandidates = std::make_shared<CandidateCollection>();
		for (auto it = candidateElems.begin(); it != candidateElems.end(); it++) {
			FunctionFactory* fp = scriptCompiler->getFunctionFactory((*it)->first);
			Function* f = fp->build(fp->getName());
			CandidatePathInfo& candidate = (*it)->second;

			std::vector<ExecutableUnitRef>& paramPath = *(candidate.paramPath);
			ParamCastingList& paramCastingList = candidate.candidate.paramCasting;			
			
			auto& argTypes = candidate.candidate.item->paramTypes;

			for (int i = 0; i < n; i++) {
				FunctionRef& castingF = paramCastingList[i].castingFunction;
				ExecutableUnitRef& param = paramPath[i];
				if (castingF == nullptr) {
					f->pushParam(param);
				}
				else {
					auto paramTemp = param;
					applyCasting(paramTemp, castingF);
					paramTemp->setReturnType(*argTypes[i]);

					f->pushParam(paramTemp);
				}
			}

			functionCandidates->push_back(ExecutableUnitRef(f));
		}

		return functionCandidates;
	}

	CandidateCollectionRef filterCandidate(ScriptCompiler* scriptCompiler,
		const string& functionName, int functionType,
		const std::vector<CandidateCollectionRef>& candidatesForParams, EExpressionResult& eResult) {		
		const list<OverLoadingItem>* overloadingFuncs = scriptCompiler->findOverloadingFuncRoot(functionName);

		if (!overloadingFuncs) {
			eResult = E_FUNCTION_NOT_FOUND;
			scriptCompiler->setErrorText("function " + functionName + " is not found");
			return nullptr;
		}

		return filterCandidate(scriptCompiler, functionName, functionType, overloadingFuncs, candidatesForParams, eResult);
	}	

	EExpressionResult checkCandidates(ScriptCompiler* scriptCompiler, const FunctionRef& function, const CandidateCollectionRef& candidates) {
		return E_SUCCESS;
		for (auto it = candidates->begin(); it != candidates->end(); it++) {
			auto jt = it;
			ExecutableUnitRef& c1 = *it;
			auto& rt1 = c1->getReturnType();
			for (jt++; jt != candidates->end(); jt++) {
				ExecutableUnitRef& c2 = *jt;
				auto& rt2 = c2->getReturnType();
				if (rt1 == rt2) {
					scriptCompiler->setErrorText("ambious function call for function " + function->getName());
					return E_TYPE_AMBIOUS_CALL;
				}
			}
		}

		return E_SUCCESS;
	}

	inline bool isScriptFunction(ScriptCompiler* scriptCompiler, int functionId) {
		auto factory = scriptCompiler->getFunctionFactory(functionId);
		auto scriptFunctionFactory = dynamic_cast<ScriptFunctionFactory*>(factory);
		return scriptFunctionFactory != nullptr;
	}

	CandidateCollectionRef ExpressionParser::linkForUnit(ScriptCompiler* scriptCompiler, ExecutableUnitRef& unit, EExpressionResult& eResult) {
		CandidateCollectionRef unitCandidate;
		if (ISFUNCTION(unit)) {
			//linking for functions
			unitCandidate = completeFunctionTree(scriptCompiler, dynamic_pointer_cast<Function>(unit), eResult);
		}
		else if (EXP_UNIT_ID_INCOMPFUNC == unit->getType()) {
			auto overLoadingFunction = scriptCompiler->findOverloadingFuncRoot(unit->toString());
			auto& typeManager = scriptCompiler->getTypeManager();
			const OverLoadingItem* overloadingItem = nullptr;
			list<const OverLoadingItem*> items;
			if (overLoadingFunction == nullptr || overLoadingFunction->size() == 0) {
				int functionId = scriptCompiler->findDynamicFunctionOnly(unit->toString());
				if (functionId >= 0) {
					overloadingItem = scriptCompiler->getFunctionLib()->findFunctionInfo(functionId);
					items.push_back(overloadingItem);
				}
				else {
					return unitCandidate;
				}
			}
			else {
				for (auto it = overLoadingFunction->begin(); it != overLoadingFunction->end(); it++) {
					items.push_back(&*it);
				}
			}
			unitCandidate = std::make_shared<CandidateCollection>();
			for (auto it = items.begin(); it != items.end(); it++) {
				auto item = *it;
				int type = scriptCompiler->buildFunctionType(*item);
				RuntimeFunctionInfo runtimeInfoTemp;
				memset(&runtimeInfoTemp, 0, sizeof(runtimeInfoTemp));
				runtimeInfoTemp.address = (void*)(size_t)item->functionId;
				runtimeInfoTemp.info.type = isScriptFunction(scriptCompiler, item->functionId) ? RuntimeFunctionType::ScriptFunction: RuntimeFunctionType::NativeFunction;
				auto functionPointerUnit = new CConstOperand<RuntimeFunctionInfo>(runtimeInfoTemp, ScriptType(type, scriptCompiler->getType(type)));
				unitCandidate->push_back(ExecutableUnitRef(functionPointerUnit));
			}			
		}
		else if (ISOPERAND(unit)) {
			//linking for operand
			auto& scriptType = unit->getReturnType();
			if (scriptType.isUnkownType()) {
				scriptType.updateType(scriptCompiler);
				if (scriptType.isUnkownType()) {
					eResult = E_TYPE_UNKNOWN;
					scriptCompiler->setErrorText("data type of '" + unit->toString() + "' is unknown");
					return nullptr;
				}
			}
			unitCandidate = std::make_shared<CandidateCollection>();
			unitCandidate->push_back(unit);
		}
		else {
			//other kinds is known as error
			eResult = E_TOKEN_UNEXPECTED;
			scriptCompiler->setErrorText("Unexpected operator '" + unit->toString() + "'");
		}

		return unitCandidate;
	}

	Function* ExpressionParser::applyConstructor(ScriptCompiler* scriptCompiler, ExecutableUnitRef& variableUnit, ExecutableUnitRef& argUnit, bool& blHasError) {
		//find registered copy contructor for current types
		list<std::pair<const OverLoadingItem*, ParamCastingInfo>> constructorCandidates;

		auto& param1 = variableUnit;
		auto& param2 = argUnit;

		auto& param1Type = param1->getReturnType();
		auto& param2Type = param2->getReturnType();

		auto currentScope = scriptCompiler->currentScope();
		scriptCompiler->getCopyConstructor(param1Type.iType(), param2Type, constructorCandidates);

		blHasError = false;

		//copy constructor found
		if (constructorCandidates.size()) {
			auto& functionInfo = constructorCandidates.front().first;
			auto& castingInfo = constructorCandidates.front().second;

			//convert param 1 to ref
			if (param1Type.refLevel() == param2Type.refLevel()) {
				if (!convertToRef(scriptCompiler, param1)) {
					blHasError = true;
					return nullptr;
				}
			}

			//param 2 is not availble for using imediately and need to be cast before use.
			if (castingInfo.castingFunction) {
				auto& castingFunction = castingInfo.castingFunction;
				castingFunction->setMask(MaskType::CastingUnitNotInExpression);
				castingFunction->pushParam(param2);

				param2 = castingFunction;
			}
			auto newFunction = scriptCompiler->createFunctionFromId(functionInfo->functionId);
			newFunction->pushParam(param1);
			newFunction->pushParam(param2);
			newFunction->setMask(MaskType::CopyConstructor);

			return newFunction;
		}

		return nullptr;
	}

	CandidateCollectionRef ExpressionParser::completeFunctionTree(ScriptCompiler* scriptCompiler, FunctionRef& function, EExpressionResult& eResult) {
		LOG_I("begin update expression tree for " + POINTER2STRING(function.get()));
		eResult = E_SUCCESS;

		Variable* variable;
		int n = function->getChildCount();

		CandidateCollectionRef functionCandidates;
		CandidateCollectionRef param1Candidates;
		std::vector<CandidateCollectionRef> candidatesForParams(n);
		int p = 0;
		const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();
		
		bool check = false;
		bool candidateIsCompleted = false;

		if (function->getType() == EXP_UNIT_ID_OPERATOR_FUNCTIONCALL) {
			ExecutableUnitRef& pExeUnit1 = function->getChild(0);
			ExecutableUnitRef& pExeUnit2 = function->getChild(1);

			DynamicParamFunction* paramCollector = (DynamicParamFunction*)pExeUnit2.get();
			auto& params = paramCollector->getParams();

			if (pExeUnit1->getType() == EXP_UNIT_ID_INCOMPFUNC) {
				auto& functionName = pExeUnit1->toString();
				auto newFunction = scriptCompiler->createExpUnitFromName(functionName);
				if (newFunction == nullptr) {
					eResult = E_TOKEN_UNEXPECTED;
					scriptCompiler->setErrorText("Unexpected token '" + functionName + "'");
					return nullptr;
				}

				//function operator is replaced by the real function
				function.reset((Function*)newFunction);
				for (auto it = params.begin(); it != params.end(); it++) {
					if (function->pushParam(*it) < 0) {
						eResult = E_FUNCTION_NOT_FOUND;
						scriptCompiler->setErrorText("function '" + function->getName() + "' is not compatible with its parameters");
						return nullptr;
					}
				}

				//update parameter count
				n = function->getChildCount();
				candidatesForParams.resize(n);
			}
			else if(pExeUnit1->getReturnType().isFunctionType()){
				candidatesForParams.resize(1 + params.size());
				auto forwardCallUnitRef = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_FORWARD_CALL, FUNCTION_PRIORITY_FUNCTIONCALL, 1 + params.size());
				forwardCallUnitRef->pushParam(pExeUnit1);
				for (auto it = params.begin(); it != params.end(); it++) {
					forwardCallUnitRef->pushParam(*it);
				}
				function = forwardCallUnitRef;
				n = (int)candidatesForParams.size();
			}
			else {
				//try to find function operator for type
				//before do that, we need to know data type of first parameter by linking it
				auto param1CandidatesTmp = linkForUnit(scriptCompiler, pExeUnit1, eResult);
				if (eResult != E_SUCCESS) {
					return nullptr;
				}
				// first check the candidate is a function type...
				if (param1CandidatesTmp->size() == 1 && param1CandidatesTmp->front()->getReturnType().isFunctionType()) {
					candidatesForParams.resize(1 + params.size());
					auto forwardCallUnitRef = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_FORWARD_CALL, FUNCTION_PRIORITY_FUNCTIONCALL, 1 + params.size());
					forwardCallUnitRef->pushParam(pExeUnit1);
					for (auto it = params.begin(); it != params.end(); it++) {
						forwardCallUnitRef->pushParam(*it);
					}
					function = forwardCallUnitRef;
					n = (int)candidatesForParams.size();
				}
				else {
					// ... or not, then check function operator for type
					int functionOperator = -1;
					functionCandidates = make_shared<CandidateCollection>();
					for (auto it = param1CandidatesTmp->begin(); it != param1CandidatesTmp->end(); it++) {
						auto& param1Type = (*it)->getReturnType();
						if (param1Type.refLevel() == 0) {
							auto checkType = param1Type.isSemiRefType() ? param1Type.deSemiRef() : param1Type;							
							if ((functionOperator = scriptCompiler->getFunctionOperator(checkType.iType())) >= 0) {
								auto functionUnit = scriptCompiler->createFunctionFromId(functionOperator);
								if (functionUnit == nullptr) {
									eResult = E_FUNCTION_NOT_FOUND;
									scriptCompiler->setErrorText("Library error function operator is missing");
									return nullptr;
								}
								functionCandidates->push_back(ExecutableUnitRef(functionUnit));
							}
						}
					}
					if (functionCandidates->size() <= 0) {
						eResult = E_TOKEN_UNEXPECTED;
						scriptCompiler->setErrorText("Unexpected operator '()'");
						return nullptr;
					}
					candidatesForParams[0] = param1CandidatesTmp;
				}

				//update parameter count
				//n = 1 + (int)params.size();
				//candidatesForParams.resize(n);
			}

			//take all param of the functions back
			/*DynamicParamFunction* paramCollector = (DynamicParamFunction*)pExeUnit2.get();
			auto& params = paramCollector->getParams();
			for (auto it1 = functionCandidates->begin(); it1 != functionCandidates->end();) {
				auto candidateUnit = (Function*)(*it1).get();
				auto it2 = params.begin();
				for (; it2 != params.end(); it2++) {
					if (candidateUnit->pushParam(*it2) < 0) {
						auto itTmp = it1;
						it1++;
						functionCandidates->erase(itTmp);
						break;
					}
				}
				if (it2 == params.end()) {
					it1++;
				}
			}
			if (functionCandidates->size() == 0) {
				eResult = E_FUNCTION_NOT_FOUND;
				scriptCompiler->setErrorText("function '" + function->getName() + "' is not compatible with its parameters");
				return nullptr;
			}*/

			
			check = true;
		}
		else if (function->getType() == EXP_UNIT_ID_SEMI_REF) {
			if (n != 1) {
				eResult = E_TOKEN_INVALID;
				scriptCompiler->setErrorText("'&' operator can only take one parameter");
				return nullptr;
			}
			auto& refUnit = function->getChild(0);
			if (refUnit->getType() != EXP_UNIT_ID_XOPERAND) {
				eResult = E_TOKEN_INVALID;
				scriptCompiler->setErrorText("'&' operator can only use for variable");
				return nullptr;
			}

			param1Candidates = linkForUnit(scriptCompiler, refUnit, eResult);
			if (eResult != E_SUCCESS && param1Candidates->size() == 0) {
				return nullptr;
			}

			functionCandidates = make_shared<CandidateCollection>();
			
			auto it = param1Candidates->begin();
			auto& param1Type = (*it)->getReturnType();

			function->popParam();
			function->pushParam(*it);
			function->setReturnType(param1Type.makeSemiRef());

			functionCandidates->push_back(function);
			for (it++; it != param1Candidates->end(); it++) {
				auto& param1TypeN = (*it)->getReturnType();
				auto newCandidate = make_shared<FixParamFunction<1>>(MAKING_SEMI_REF_FUNC, EXP_UNIT_ID_SEMI_REF, FUNCTION_PRIORITY_UNARY_PREFIX, param1TypeN.makeSemiRef());
				functionCandidates->push_back(newCandidate);
			}

			return functionCandidates;
		}
		bool updated = false;
		if (check == false && n == 2) {
#pragma region update data type for auto variable
			ExecutableUnitRef& pExeUnit1 = function->getChild(0);
			ExecutableUnitRef& pExeUnit2 = function->getChild(1);

			if (ISOPERAND(pExeUnit1) && pExeUnit1->getReturnType().isUnkownType()) {
				if (pExeUnit1->getReturnType().sType().compare(AUTO_VARIABLE_TYPE) == 0) {
					CXOperand* xOperand = (CXOperand*)pExeUnit1.get();
					variable = xOperand->getVariable();

					auto param2Candidates = linkForUnit(scriptCompiler, pExeUnit2, eResult);
					if (eResult != E_SUCCESS) {
						return nullptr;
					}

					MaskType mask = (MaskType)((unsigned int)pExeUnit1->getMask() | (unsigned int)MaskType::DeclareInExpression);

					param1Candidates = std::make_shared<CandidateCollection>();
					pExeUnit1->setReturnType(param2Candidates->front()->getReturnType());
					variable->setDataType(pExeUnit1->getReturnType());
					pExeUnit1->setMask(mask);
					param1Candidates->push_back(pExeUnit1);
					auto it = param2Candidates->begin();
					it++;
					for (; it != param2Candidates->end(); it++) {
						ExecutableUnit* xOperandNew = xOperand->clone();
						variable = ((CXOperand*)xOperandNew)->getVariable();
						xOperandNew->setReturnType((*it)->getReturnType());
						xOperandNew->setMask(mask);
						variable->setDataType(xOperandNew->getReturnType());
						param1Candidates->push_back(ExecutableUnitRef(xOperandNew));
					}

					updated = true;

					//auto type variable will be construct by and default copy constructor, not assigment operator
					//so we replace current function object by default copy constructor
					auto defaultOperatorUnit = new DynamicParamFunction(DEFAULT_COPY_OPERATOR,
						EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, 2);
					function.reset(defaultOperatorUnit);
					defaultOperatorUnit->pushParam(param1Candidates->front());
					defaultOperatorUnit->pushParam(pExeUnit2);

					for (auto it = param1Candidates->begin(); it != param1Candidates->end(); it++) {
						if ( (*it)->getReturnType().isUnkownType()) {
							eResult = E_TYPE_UNKNOWN;
							scriptCompiler->setErrorText("data type of '" + pExeUnit1->toString() + "' is not set");
							//check later - bellow line was added
							return nullptr;
						}
					}
					candidatesForParams[0] = param1Candidates;
					candidatesForParams[1] = param2Candidates;
				}
			}
#pragma endregion
		}
		int choosedFunctionId = -1;
		Function* bestFunctionMatch = nullptr;

		if(updated == false && n > 0) {			
			for (int i = 0, k = 0; i < n && eResult == E_SUCCESS; i++, k++) {
				//if candidate is already updated
				if (candidatesForParams[k]) {
					continue;
				}
				ExecutableUnitRef& pExeUnit = function->getChild(i);
				if (pExeUnit->getType() == EXP_UNIT_ID_COLLECTION) {
					DynamicParamFunctionRef paramCollection = static_pointer_cast<DynamicParamFunction>(pExeUnit);
					auto& paramsInCollection = paramCollection->getParams();
					candidatesForParams.resize(n + paramsInCollection.size() - 1);

					for (auto it = paramsInCollection.begin(); it != paramsInCollection.end() && eResult == E_SUCCESS; it++, k++) {
						ExecutableUnitRef& pExeUnitTmp = *it;
						candidatesForParams[k] = linkForUnit(scriptCompiler, pExeUnitTmp, eResult);
					}
					k--;
				}
				else {
					candidatesForParams[k] = linkForUnit(scriptCompiler, pExeUnit, eResult);
				}
			}
			if (eResult != E_SUCCESS) {
				return nullptr;
			}
			//update param count
			n = (int)candidatesForParams.size();
#pragma region linking for conditional operator
			if (n == 3 && function->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL) {
				choosedFunctionId = scriptCompiler->getConditionalFunction();
				if (choosedFunctionId < 0) {
					eResult = E_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("internal error: definition of operator ? is missing");
					return nullptr;
				}

				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

				string error;
				functionCandidates = std::make_shared<CandidateCollection>();
				for (auto pit = paramPaths.begin(); pit != paramPaths.end(); pit++) {
					auto conditionUnit = pit->at(0);
					auto ifUnit = pit->at(1);
					auto elseUnit = pit->at(2);

					auto& conditionDataType = conditionUnit->getReturnType();
					auto& ifClauseReturnType = ifUnit->getReturnType();
					auto& elseClauseReturnType = elseUnit->getReturnType();

					if (ifClauseReturnType != elseClauseReturnType) {
						eResult = E_TYPE_CONVERSION_ERROR;
						error = "data type of if clause and else clause is not same";
						continue;
					}
					
					if (conditionDataType.iType() != basicType.TYPE_BOOL) {
						auto castingFuncName = scriptCompiler->getType(basicType.TYPE_BOOL);
						int castingFunctionId = scriptCompiler->findFunction(castingFuncName, conditionDataType.sType());
						if (castingFunctionId < 0) {
							eResult = E_TYPE_CONVERSION_ERROR;
							error = "cannot cast condition type to " + castingFuncName;
							continue;
						}
						auto castingFunction = (Function*)scriptCompiler->createFunctionFromId(castingFunctionId);
						castingFunction->pushParam(conditionUnit);
						conditionUnit =  ExecutableUnitRef(castingFunction);
					}

					Function* conditionOperator = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
					conditionOperator->setReturnType(ifClauseReturnType);
					
					conditionOperator->pushParam(conditionUnit);
					conditionOperator->pushParam(ifUnit);
					conditionOperator->pushParam(elseUnit);

					functionCandidates->push_back( ExecutableUnitRef(conditionOperator) );
				}
				if (functionCandidates->size() == 0) {
					scriptCompiler->setErrorText(error);
				}
				eResult = checkCandidates(scriptCompiler, function, functionCandidates);
				if (eResult != E_SUCCESS) {
					return nullptr;
				}
				return functionCandidates;
			}
#pragma endregion

#pragma region linking for ref operator
			if (n == 1 && function->getType() == EXP_UNIT_ID_MAKE_REF) {
				choosedFunctionId = scriptCompiler->getMakingRefFunction();
				functionCandidates = std::make_shared<CandidateCollection>();
				auto& paramCandidates = candidatesForParams.front();
				for(auto pit = paramCandidates->begin(); pit != paramCandidates->end(); pit++) {
					auto& makeRefParamUnit = (*pit);
					if (makeRefParamUnit->getType() == EXP_UNIT_ID_DEREF) {
						auto& paramOfDeref = ((Function*)makeRefParamUnit.get())->getChild(0);
						functionCandidates->push_back(paramOfDeref);
					}
					else if (makeRefParamUnit->getReturnType().isSemiRefType()) {
						auto& type = makeRefParamUnit->getReturnType();
						type = type.deSemiRef();
						type = type.makeRef();
						functionCandidates->push_back(makeRefParamUnit);
					}
					else if (choosedFunctionId >= 0) {
						Function* refFunction = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
						refFunction->pushParam(makeRefParamUnit);
						functionCandidates->push_back(ExecutableUnitRef(refFunction));
					}
				}
				if (functionCandidates->size() == 0) {
					eResult = E_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("internal error: definition of operator 'ref' is missing");
					return nullptr;
				}
				eResult = checkCandidates(scriptCompiler, function, functionCandidates);
				if (eResult != E_SUCCESS) {
					return nullptr;
				}
				return functionCandidates;
			}
#pragma endregion
		}

#pragma region linking for default copy operator
		if (n == 2 && function->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR) {
			choosedFunctionId = scriptCompiler->getDefaultCopyFunction();
			if (choosedFunctionId < 0) {
				eResult = E_FUNCTION_NOT_FOUND;
				scriptCompiler->setErrorText("internal error: definition of operator '" DEFAULT_COPY_OPERATOR "' is missing");
				return nullptr;
			}

			std::list<std::vector<ExecutableUnitRef>> paramPaths;
			listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

			auto currentScope = scriptCompiler->currentScope();

			string error;
			functionCandidates = std::make_shared<CandidateCollection>();
			bool needToCallConstructor = false;

			if (paramPaths.size()) {
				auto pit = paramPaths.begin();
				auto param1 = pit->at(0);
				auto param2 = pit->at(1);

				// check if param 1 is a variable and it is declared in expression...
				if (param1->getType() == EXP_UNIT_ID_XOPERAND && ((unsigned int)param1->getMask() & (unsigned int)MaskType::DeclareInExpression) != 0) {
					// ...then it is must used constructor instead of default copy constructor if there is
					// constructor for data type of the variable

					bool blHasError;

					//retreive varible object inside param 1 maybe changed in function applyConstructor
					auto xOperand = (CXOperand*)param1.get();
					auto pVariable = xOperand->getVariable();

					auto newFunction = applyConstructor(scriptCompiler, param1, param2, blHasError);
					if (blHasError) {
						return nullptr;
					}

					// check if constructor was applied					
					if (newFunction) {
						functionCandidates->push_back(ExecutableUnitRef(newFunction));

						if (currentScope != nullptr) {
							currentScope->checkVariableToRunConstructor(pVariable, newFunction);
						}

						needToCallConstructor = true;
					}
					else {
						int defaultConstructor = scriptCompiler->getConstructor(param1->getReturnType().iType());
						needToCallConstructor = defaultConstructor >= 0;
						if (!needToCallConstructor) {
							needToCallConstructor = scriptCompiler->getDestructor(param1->getReturnType().iType()) >= 0;
						}
					}
				}
			}

			// check if constructor was not used to initialize the variable...
			if (!needToCallConstructor) {
				// then try to use default copy constructor
				for (auto pit = paramPaths.begin(); pit != paramPaths.end(); pit++) {
					auto param1 = pit->at(0);
					auto param2 = pit->at(1);

					auto& param1Type = param1->getReturnType();
					auto& param2Type = param2->getReturnType();

					bool acceptPath = false;
					//default copy constructor should be run only when param 2 is a function
					//if (ISFUNCTION(param2)) {
					if (param1->getType() == EXP_UNIT_ID_XOPERAND) {
						if (param1Type == param2Type && convertToRef(scriptCompiler, param1)) {
							acceptPath = true;
						}
					}
					else if (param1Type.refLevel() - param2Type.refLevel() == 1) {
						if (param1Type.origin() == param2Type.origin()) {
							acceptPath = true;
						}
						else {
							int castingFunctionId = scriptCompiler->findFunction(param1Type.deRef().sType(), param2Type.sType());
							if (castingFunctionId >= 0) {
								auto castingFunction = (Function*)scriptCompiler->createFunctionFromId(castingFunctionId);
								castingFunction->pushParam(param2);
								castingFunction->setMask(MaskType::CastingUnitNotInExpression);
								param2 = ExecutableUnitRef(castingFunction);

								acceptPath = true;
							}
						}
					}
					//}

					if (acceptPath) {
						Function* defaultCopyFunction = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
						defaultCopyFunction->pushParam(param1);
						defaultCopyFunction->pushParam(param2);
						defaultCopyFunction->setReturnType(param2->getReturnType().makeSemiRef());

						functionCandidates->push_back(ExecutableUnitRef(defaultCopyFunction));
					}
				}
			}
			// check if not any operation was used to initialize the variable...
			if (functionCandidates->size() == 0) {
				// ...then we try to using assigment operator
				// find assgiment operator for params
				functionCandidates = filterCandidate(scriptCompiler, "=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, candidatesForParams, eResult);
				if (functionCandidates->size() != 0) {
					//check if param 1 of the function is a X Operand
					//if param 1 is an X Operand, all candidates of its is also and X operand
					//so we just need to take the first one to check
					auto& param1 = candidatesForParams.at(0)->front();
					if (needToCallConstructor && currentScope) {
						//because param 1 is a X Operand/
						//we should call its constructor if exist first, then call assigment
						auto xOperand = (CXOperand*)param1.get();
						auto pVariable = xOperand->getVariable();

						// this will check and add constructor and destructor for the variable
						if (currentScope->checkVariableToRunConstructor(pVariable)) {
							//add constructror to scope success, so we need to remove all other candidates
							auto itAfterBegin = functionCandidates->begin();
							itAfterBegin++;
							functionCandidates->erase(itAfterBegin, functionCandidates->end());
						}
					}
				}
			}

			candidateIsCompleted = true;
		}
#pragma endregion

		if (eResult != E_SUCCESS) {
			LOG_I("end update expression tree for " + POINTER2STRING(function.get()));
			return nullptr;
		}

		bool assignForStruct = false;
		if (functionCandidates == nullptr || functionCandidates->size() == 0) {
			if (function->getType() == EXP_UNIT_ID_FORWARD_CALL) {				
				decltype(functionCandidates) candidatesTmp;
				ScriptType returnType;
				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);
				list<std::pair<ExecutableUnit*, int>> paramPathCandidate;
				functionCandidates = make_shared<CandidateCollection>();
				param1Candidates = candidatesForParams[0];
				for (auto it = param1Candidates->begin(); it != param1Candidates->end(); it++) {
					auto& functionPointerUnit = *it;
					int functionType = functionPointerUnit->getReturnType().iType();
					list<ScriptType> argTypes;
					bool isDynamicFunction;
					argTypes.push_back(functionPointerUnit->getReturnType());
					scriptCompiler->parseFunctionType(functionType, returnType, argTypes, isDynamicFunction);
					if (returnType.isUnkownType()) {
						eResult = E_TYPE_UNKNOWN;
						scriptCompiler->setErrorText("function type is not specified for unit'" + functionPointerUnit->toString() + "'");
						return nullptr;
					}

					vector<ScriptTypeRef> argRefTypes;
					for (auto it = argTypes.begin(); it != argTypes.end(); it++) {
						argRefTypes.push_back(make_shared<ScriptType>(it->iType(), it->sType()));
					}

					auto res = filterCandidate2(scriptCompiler, argRefTypes, paramPaths, eResult);
					if (res.first) {
						auto forwardCallUnit = new DynamicParamFunction(FUNCTION_OPERATOR, EXP_UNIT_ID_FORWARD_CALL, FUNCTION_PRIORITY_FUNCTIONCALL, (int)argRefTypes.size());
						auto& path = *res.first;
						for (auto jt = path.begin(); jt != path.end(); jt++) {
							forwardCallUnit->pushParam(*jt);
						}
						forwardCallUnit->setReturnType(returnType);
						paramPathCandidate.push_back( std::make_pair(forwardCallUnit, res.second));
					}
				}

				if (paramPathCandidate.size() == 0) {
					scriptCompiler->setErrorText("Cannot convert from parameter types to argument types");
					eResult = E_TYPE_CONVERSION_ERROR;
					return nullptr;
				}
				paramPathCandidate.sort([](pair<ExecutableUnit*, int>& c1, pair<ExecutableUnit*, int>& c2) { return c1.second < c2.second; });
				for (auto it = paramPathCandidate.begin(); it != paramPathCandidate.end(); it++) {
					functionCandidates->push_back(ExecutableUnitRef(it->first));
				}

				return functionCandidates;
			}

			if (n == 2) {
				//we need to modify param unit, so we need use ExecutableUnitRef* here
				std::list<std::vector<ExecutableUnitRef*>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef*>(candidatesForParams, paramPaths, makeRef);
				for (auto pathIt = paramPaths.begin(); pathIt != paramPaths.end(); pathIt++) {
					auto& paramPath = *pathIt;
#pragma region checking some operators related to structs
					ExecutableUnitRef& pExeUnit1 = *(paramPath.at(0));
					ExecutableUnitRef& pExeUnit2 = *(paramPath.at(1));

					auto& dataType1 = pExeUnit1->getReturnType();
					auto& dataType2 = pExeUnit2->getReturnType();
					const StructClass* struct1;

					if (function->getType() == EXP_UNIT_ID_MEMBER_ACCESS) {
						struct1 = scriptCompiler->getStruct(dataType1.origin());
						if (struct1 && dataType2.iType() == basicType.TYPE_STRING && pExeUnit2->getType() == EXP_UNIT_ID_CONST) {
							string& memberName = *((string*)pExeUnit2->Execute());
							MemberInfo memberInfo;
							if (!struct1->getInfo(memberName, memberInfo)) {
								eResult = E_TOKEN_UNEXPECTED;
								scriptCompiler->setErrorText(memberName + " is not member of " + struct1->getName());
								return nullptr;
							}

							//change param 2 from string type to int type
							pExeUnit2.reset(
								new CConstOperand<MemberInfo>(memberInfo,
									scriptCompiler->getType(basicType.TYPE_ELEMENT_INFO))
							);
							pExeUnit2->setReturnType(ScriptType(basicType.TYPE_ELEMENT_INFO, scriptCompiler->getType(basicType.TYPE_ELEMENT_INFO)));
							function->setReturnType(memberInfo.type);
						}
						else if (struct1 && dataType2.iType() == basicType.TYPE_ELEMENT_INFO && pExeUnit2->getType() == EXP_UNIT_ID_CONST) {
							;//do nothing
						}
						else {
							eResult = E_TOKEN_UNEXPECTED;
							scriptCompiler->setErrorText("Wrong gramar of using operator '.'");
							return nullptr;
						}
					}
					//the code bellow is going to check assigment for structs
					else if (ISOPERAND(pExeUnit1) &&
						(function->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT ||
							function->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR)) {
						struct1 = scriptCompiler->getStruct(dataType1.iType());
						int variantArrayType = basicType.TYPE_VARIANTARRAY;
						if (variantArrayType == dataType2.iType() || variantArrayType == dataType2.origin()) {
							if (struct1) {

								//check variant array if valid or not in use to assign to a struct
								int errorIdx;
								if ((errorIdx = checkAssigmentOperatorForStruct(scriptCompiler, struct1, dynamic_pointer_cast<DynamicParamFunction>(pExeUnit2))) >= 0) {
									eResult = E_TYPE_CONVERSION_ERROR;
									scriptCompiler->setErrorText("different type and different number of element for struct assigment does not allow");
									return nullptr;
								}
							}
						}
					}
#pragma endregion
				}
			}

			functionCandidates = filterCandidate(scriptCompiler, function->getName(), function->getType(), candidatesForParams, eResult);
			if (eResult == E_SUCCESS && functionCandidates != nullptr && functionCandidates->size()) {
				return functionCandidates;
			}
			
			choosedFunctionId = scriptCompiler->findDynamicFunctionOnly(function->getName());
			if (choosedFunctionId > 0) {
#pragma region linking for dynamic function
				Function* dynamicFunction = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

				if(paramPaths.size() != 0) {
					//check later - bellow code was commented
					/*
					if (paramPaths.size() != 1) {
						eResult = E_TYPE_AMBIOUS_CALL;
						scriptCompiler->setErrorText("ambious function call for dynamic function");
						return nullptr;
					}*/
					auto& paramPath = paramPaths.front();
					for (auto pit = paramPath.begin(); pit != paramPath.end(); pit++) {
						dynamicFunction->pushParam(*pit);
					}
				}
				functionCandidates = std::make_shared<CandidateCollection>();
				functionCandidates->push_back(ExecutableUnitRef(dynamicFunction));
				eResult = E_SUCCESS;
#pragma endregion
			}
			else {
#pragma region linking for default operators
				//we cannot find a registered coressponding function here
				//now it is time to check some default operators
				functionCandidates = findApproxiateDefaultOperator(scriptCompiler, function, candidatesForParams);
				if (functionCandidates == nullptr || !functionCandidates->size()) {
					eResult = E_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("function '" + function->getName() + "' is not found");
					return nullptr;
				}
#pragma endregion
			}
		}
		else if(candidateIsCompleted == false){
			auto& functionLib = scriptCompiler->getFunctionLib();
			list<OverLoadingItem> candidatesInfo;
			list<int> dynamicFunctionCandidates;
			for (auto it = functionCandidates->begin(); it != functionCandidates->end(); it++) {
				auto functionId = ((Function*)(*it).get())->getId();				
				auto functionInfo = functionLib->findFunctionInfo(functionId);				
				if (functionInfo == nullptr) {
					eResult = E_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("library error: '" + function->getName() + "' is missing");
					return nullptr;
				}
				if (scriptCompiler->findDynamicFunctionOnly(*functionInfo->itemName) == functionId) {
					dynamicFunctionCandidates.push_back(functionId);
				}
				else {
					candidatesInfo.push_back(*functionInfo);
				}
			}

			functionCandidates = filterCandidate(scriptCompiler, function->getName(), function->getType(), &candidatesInfo, candidatesForParams, eResult);
			if (functionCandidates.get() == nullptr && dynamicFunctionCandidates.size()) {
				functionCandidates = std::make_shared<CandidateCollection>();
			}
#pragma region linking for dynamic function
			if (dynamicFunctionCandidates.size()) {
				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

				for (auto functionid : dynamicFunctionCandidates) {
					Function* dynamicFunction = (Function*)scriptCompiler->createFunctionFromId(functionid);
					auto& paramPath = paramPaths.front();
					for (auto pit = paramPath.begin(); pit != paramPath.end(); pit++) {
						dynamicFunction->pushParam(*pit);
					}					
					functionCandidates->push_back(ExecutableUnitRef(dynamicFunction));
				}
			}
#pragma endregion
		}

		//if (IS_UNKNOWN_TYPE(bestFunctionMatch->getReturnType())) {
		//	eResult = E_TYPE_UNKNOWN;
		//	scriptCompiler->setErrorText("Internal error, register function '" + bestFunctionMatch->getName() + "' is not specified return type");
		//	return nullptr;
		//}
		////re-assign return type for assigment struct because
		////the library has only one assigment operator for all truct and it return default is ref void
		//if (assignForStruct || EXP_UNIT_ID_MEMBER_ACCESS == function->getType()) {
		//	bestFunctionMatch->setReturnType(function->getReturnType());
		//}
		eResult = checkCandidates(scriptCompiler, function, functionCandidates);
		if (eResult != E_SUCCESS) {
			return nullptr;
		}
		scriptCompiler->setErrorText("");
		return functionCandidates;
	}

	EExpressionResult ExpressionParser::link(Expression* pExp) {
		ExecutableUnitRef& root = pExp->getRoot();
		EExpressionResult eResult = E_SUCCESS;
		if (ISFUNCTION(root)) {
			auto candidateFounds = completeFunctionTree(getCompiler(), dynamic_pointer_cast<Function>(root), eResult);
			if (eResult == E_SUCCESS && candidateFounds && candidateFounds->size()) {
				//check later - need consider choose r-value and l-value return of candidate
				root = candidateFounds->front();

				//below code need to consider again, due to performance
				//maybe we do it after all expression are linked
				//ScriptScope* scriptScope = ScriptCompiler::getInstance()->currentScope();
				//if (scriptScope) {
				//	scriptScope->updateVariableOffset();
				//}
			}
		}
		else {
			if (root->getReturnType().isUnkownType()) {
				root->getReturnType().updateType(getCompiler());
			}
			eResult = root->getReturnType().isUnkownType() ? E_TYPE_UNKNOWN : E_SUCCESS;
		}
		return eResult;
	}

	EExpressionResult ExpressionParser::link(Expression* pExp, CandidateCollectionRef& candidates) {
		ExecutableUnitRef& root = pExp->getRoot();
		EExpressionResult eResult = E_SUCCESS;
		candidates = linkForUnit(getCompiler(), root, eResult);
		return eResult;
	}
}