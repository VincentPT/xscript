/******************************************************************
* File:        ExpresionParser.cpp
* Description: implement ExpresionParser class. A class used to parse,
*              compile and link a expression.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#include "ExpresionParser.h"
#include <functional>
#include <array>
#include <algorithm>
#include "expresion_defs.h"
#include "expressionunit.h"
#include <stack>
#include "ScriptCompiler.h"
#include "ScriptScope.h"
#include "Expression.h"
#include "FactoryTree.h"
#include "FunctionFactory.h"
#include "../Logger/remotelogger.h"
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

	ExpressionParser::ExpressionParser(ScriptCompiler* scriptCompiler) : _scriptCompiler(scriptCompiler), _lastCompileChar(nullptr) {}

	ExpressionParser::~ExpressionParser(){}

	EExpressionResult ExpressionParser::makeExpression(OutputStack& output, OperatorStack& operators) {
		DynamicParamFunctionRef pExpFunction;
		ExecutableUnitRef expUnitTemp;
		EExpressionResult eResult = EE_SUCCESS;
		int iResult;

		while (eResult == EE_SUCCESS && operators.size() && output.size())
		{
			pExpFunction = static_pointer_cast<DynamicParamFunction>(operators.top());
			if ( !ISFUNCTION(pExpFunction) ) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), pExpFunction->GetName()));
				eResult = EE_TOKEN_UNEXPECTED;
				_lastErrorUnit = pExpFunction;
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
					eResult = EE_TOKEN_UNEXPECTED;
					_lastErrorUnit = expUnitTemp;
					break;
				}

				if (iResult == 0) {
					eResult = EE_SUCCESS;
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

	EExpressionResult ExpressionParser::tokenize(const std::wstring& sExpressionString, std::list<ExpUnitRef>& expUnitList) {
		const wchar_t* text = sExpressionString.c_str();
		const wchar_t* end = text + sExpressionString.size();
		return tokenize(text, end, expUnitList);
	}

	void ExpressionParser::numberedForUnit(std::list<ExpUnitRef>& expList) {
		int idx = 0;
		for (auto it = expList.begin(); it != expList.end(); it++, idx++) {
			(*it)->setIndex(idx);
		}
	}
/*
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
*/
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
			eResult = EE_TOKEN_UNEXPECTED;
			compiler->setErrorText("Lambda function is only allow when compile in a program context");
			return nullptr;
		}

		auto c = trimLeft(begin, end);
		unique_ptr<WCHAR, std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)begin, [this, &c](WCHAR*) {
			setLastCompilerChar(c);
		});

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
			eResult = EE_INCOMPLETED_EXPRESSION;
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

		// the expression [capture1, capture2, ...] become SYSTEM_LAMBDA_FUNCTION(capture1, capture2, ...)
		// so we need offset indicators to origin by substract to the additional sace
		constexpr int lamdaFunctionNameLen = sizeof(SYSTEM_LAMBDA_FUNCTION) - 1;
		constexpr int offset = lamdaFunctionNameLen - 0;

		std::list<ExpUnitRef> expUnitList;
		d = newExpression.c_str();
		d = readExpression(d, d + newExpression.size(), eResult, expUnitList);

		// convert the last compilied char from newExpression to current expression(begin, end)
		auto lastCharOffset = getLastCompileChar() - newExpression.c_str();
		if (lastCharOffset < lamdaFunctionNameLen) {
			lastCharOffset = 0;
		}
		else {
			lastCharOffset -= offset;
		}
		setLastCompilerChar(begin + lastCharOffset);
		if (d == nullptr || eResult != EE_SUCCESS) {
			return nullptr;
		}

		// also convert the unit's source chars to origin
		auto it = expUnitList.begin();
		// make the first unit is at begin
		it->get()->setSourceCharIndex(0);
		for (it++; it != expUnitList.end(); it++) {
			it->get()->setSourceCharIndex(it->get()->getSourceCharIndex() - offset);
		}

		list<ExpressionRef> temp;
		bool res = compile(expUnitList, temp, &eResult);
		if (res == false || temp.size() == 0) {
			eResult = EE_TYPE_UNKNOWN;
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
		// assign return value to d to keep c as its current value in return value is null
 		d = globalScope->parseAnonymous(c, end, params, lambaFunctionId);
		if (c == nullptr) {
			return nullptr;
		}
		// keep last compile character by c to storing last compile char when the scope exit
		c = d;

		//now we dont use function operator() to call create lambda function
		//but we use create lambda function directly
		DynamicParamFunction* createLambdaFunction = new DynamicParamFunction(SYSTEM_LAMBDA_FUNCTION, EXP_UNIT_ID_CREATE_LAMBDA, FUNCTION_PRIORITY_USER_FUNCTION, (int)params.size() + 1);
		for (auto it = params.begin(); it != params.end() ; it++) {
			createLambdaFunction->pushParam(*it);
		}
		auto unitIdRef = make_shared<CConstOperand<int>>(lambaFunctionId, "int");
		createLambdaFunction->pushParam(unitIdRef);

		// keep origin source char in new expression unit
		createLambdaFunction->setSourceCharIndex(lambdaExpression->getSourceCharIndex());

		lambdaExpression.reset(createLambdaFunction);
		return c;
	}

	inline bool checkPosibilityUnaryPrefix(ExpUnit* pLastUnit) {
		return pLastUnit == NULL ||
			pLastUnit->getType() == EXP_UNIT_ID_OPEN_ROUND_BRACKET ||
			pLastUnit->getType() == EXP_UNIT_ID_OPEN_CURLY_BRACKET ||
			pLastUnit->getType() == EXP_UNIT_ID_OPEN_SQUARE_BRACKET ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_CHOICE ||
			pLastUnit->getType() == EXP_UNIT_ID_FUNC_SEPERATOR;
	}

	void ExpressionParser::recursiveOffsetSourceCharIndex(Function* expFunc, int offset) {
		int n = expFunc->getChildCount();
		if (expFunc->getSourceCharIndex() >= 0) {
			expFunc->setSourceCharIndex(expFunc->getSourceCharIndex() + offset);
		}

		for (int i = 0; i < n; i++) {
			auto& child = expFunc->getChild(0);
			if (ISFUNCTION(child)) {
				recursiveOffsetSourceCharIndex((Function*)child.get(), offset);
			}
			else if(child->getSourceCharIndex() >= 0) {
				child->setSourceCharIndex(child->getSourceCharIndex() + offset);
			}
		}
	}

	/*
	(Hex codes assume an ASCII-compatible character encoding.)
		\a = \x07 = alert (bell)
		\b = \x08 = backspace
		\t = \x09 = horizonal tab
		\n = \x0A = newline (or line feed)
		\v = \x0B = vertical tab
		\f = \x0C = form feed
		\r = \x0D = carriage return
		\e = \x1B = escape (non-standard GCC extension)
	Punctuation characters:
		\" = quotation mark (backslash not required for '"')
		\' = apostrophe (backslash not required for "'")
		\? = question mark (used to avoid trigraphs)
		\\ = backslash
	*/

	inline char escapeChar(char c) {
		switch (c)
		{
		case 'a':
			return 7;
		case 'b':
			return 8;
		case 't':
			return 9;
		case 'n':
			return 0x0A;
		case 'v':
			return 0x0B;
		case 'f':
			return 0x0C;
		case 'r':
			return 0x0D;
		case 'e':
			return 0x1B;
		case '0':
			return 0;
		case '"':
		case '\'':
		case '?':
		case '\\':
			return c;
		default:
			return -1;
		}
	}

	inline int escapeChar(const wchar_t* xStr) {
		wchar_t buff[3] = { *xStr, *(xStr + 1), 0 };
		wchar_t * p;
		long n = wcstoul(buff, &p, 16);
		if (*p != 0) {
			// not a hex number
			return -1;
		}
		return (int)n;
	}

	const WCHAR* ExpressionParser::readExpression(const WCHAR* begin, const WCHAR* end, EExpressionResult& eResult, std::list<ExpUnitRef>& expUnitList) {
		const WCHAR* c;
		WCHAR* s1, *s2, *sToken;
		WCHAR sToken1[MAX_FUNCTION_LENGTH + 2];
		WCHAR sToken2[MAX_FUNCTION_LENGTH + 2];
		bool blHasDot, blHasChar;
		ExecutableUnit* pExpUnit;
		ExpUnit *pFixedExpUnit, *pLastUnit, *pStringConst, *pTBDExpUnit;
		ExpUnit *pArrayExpUnit = nullptr;

		ScriptCompiler* scriptCompiler = getCompiler();

		blHasDot = false;
		blHasChar = false;

		s1 = sToken1;
		s2 = sToken2;
		pLastUnit = NULL;

		eResult = EE_SUCCESS;

		unique_ptr<WCHAR,  std::function<void(WCHAR*)>> lastCompileCharScope((WCHAR*)begin, [this, &c](WCHAR*) {
			setLastCompilerChar(c);
		});

		static string key_create_array_func(SYSTEM_ARRAY_FUNCTION);
		for (c = begin; eResult == EE_SUCCESS; c++)
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
				*c == ',' || *c == '\"' || *c == '\n')
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
						pFixedExpUnit = new OpenRoundBracket();
						break;
					case ')':
						pFixedExpUnit = new ClosedRoundBracket();
						break;
					case '[':
						pFixedExpUnit = new OpenSquareBracket();
						break;
					case ']':
						pFixedExpUnit = new ClosedSquareBracket();
						break;
					case '}':
						pFixedExpUnit = new ClosedCurlyBracket();
						pArrayExpUnit = nullptr;
						break;
					case '{':
						//pArrayExpUnit = new DynamicParamFunction(key_create_array_func, EXP_UNIT_ID_OPERATOR_ARRAY, FUNCTION_PRIORITY_USER_FUNCTION, "TBD");
						//size = scriptCompiler->findDynamicFunctionOnly(key_create_array_func);
						//pArrayExpUnit = scriptCompiler->createFunctionFromId(size);
						pArrayExpUnit = new IncompletedUserFunctionUnit(key_create_array_func);
						pArrayExpUnit->setSourceCharIndex((int)(c - begin));
						expUnitList.push_back(ExpUnitRef(pArrayExpUnit));
						pFixedExpUnit = new OpenCurlyBracket();
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
					case '\"': {
						std::wstring constantString;
						auto beginConstantChar = c;
						c++;
						auto lastC = c;

						for (; c < end && *c != '\"'; c++) {
							if (constantString.size() > MAX_FUNCTION_LENGTH) {
								eResult = EE_TOKEN_OUTOFLENGTH;
								scriptCompiler->setErrorText("token is too long");
								break;
							}
							if (*c == '\\') {
								constantString.append(lastC, c);
								c++;
								if (c == end) {
									eResult = EE_INCOMPLETED_EXPRESSION;
									scriptCompiler->setErrorText("missing character after '\\'");
									break;
								}
								if (*c == 'x') {
									c++;
									if (c + 1 >= end) {
										eResult = EE_INCOMPLETED_EXPRESSION;
										scriptCompiler->setErrorText("lack of character after '\\'x");
										break;
									}
									auto res = escapeChar(c);
									if (res == -1) {
										eResult = EE_TOKEN_INVALID;
										string errorMsg("invalid escape character, token ");
										errorMsg += (char)*c;
										errorMsg += (char)*(c + 1);
										errorMsg += " is not a number";
										scriptCompiler->setErrorText(errorMsg);
										break;
									}
									c++;
									constantString += (wchar_t)res;
									lastC = c + 1;
								}
								else {
									auto cRes = escapeChar((char)*c);
									if (cRes == -1) {
										eResult = EE_TOKEN_INVALID;
										scriptCompiler->setErrorText("invalid escape character after '\\'");
										break;
									}
									constantString += (wchar_t)cRes;
									lastC = c + 1;
								}
							}
						}
						if (c > lastC) {
							constantString.append(lastC, c);
						}

						if (eResult == EE_SUCCESS) {
							if (*c != '\"') {
								eResult = EE_TOKEN_UNEXPECTED;
								scriptCompiler->setErrorText("missing '\"'");
							}
							else {
								if (sToken != NULL && *sToken == 'L' && *(sToken + 1) == 0) {
									pStringConst = new CConstOperand<std::wstring>(constantString, "wstring");
									if (sToken == sToken1) {
										s1 = sToken1;
									}
									else {
										s2 = sToken2;
									}
									sToken = nullptr;
								}
								else {
									pStringConst = new CConstOperand<std::string>(convertToAscii(constantString.c_str(), constantString.size()), "string");
								}

								pStringConst->setSourceCharIndex((int)(beginConstantChar - begin));
							}
						}
						break;
					}
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
			//else if (*c == '\n') {
			//	c = trimLeft(c, end);
			//	c--;
			//	continue;
			//}
			else if (c < end && !ScriptCompiler::isCommandBreakSign(*c)) {
				*s2++ = *c;
			}

			//if (blHasDot && blHasDot == blHasChar) {
			//	eResult = EE_TOKEN_INVALID;
			//	scriptCompiler->setErrorText("operator '.' is not supported now");
			//	DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
			//	break;
			//}
			if (s1 - sToken1 > MAX_FUNCTION_LENGTH) {
				eResult = EE_TOKEN_OUTOFLENGTH;
				scriptCompiler->setErrorText("token is too long");
				DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
				break;
			}
			if (s2 - sToken2 > MAX_FUNCTION_LENGTH) {
				eResult = EE_TOKEN_OUTOFLENGTH;
				scriptCompiler->setErrorText("token is too long");
				DBG_ERROR(_tprintf(__T("\n[#]Error at index %d"), c - sExpressionString));
				break;
			}
			// if this condition happens, it means
			// an one character like operator '.' follow an operator like '[]'
			// so we reset the stoken as null to avoid an ExpUnit is created by an empty token
			if (sToken && *sToken == 0) {
				sToken = NULL;
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
				int tokenLength = (int)wcslen(sToken);
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
					
					wchar_t* lastChar = sToken + tokenLength - 1;
					//if (sToken == sToken1) {
					//	lastChar = sToken1 + wcslen(sToken1) - 1;
					//}
					//else {
					//	lastChar = sToken2 + wcslen(sToken2) - 1;
					//}

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
							auto lVal = _wtoll(sToken);
							if (lVal > 0xFFFFFFFFll) {
								pExpUnit = new CConstOperand<__int64>(sign * lVal, "long");
							}
							else {
								pExpUnit = new CConstOperand<int>(sign * (int)lVal, "int");
							}
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
				else if (tokenLength >= 2 && sToken[0] == '0') {
					if (sToken[1] == 'x') {
						wchar_t* lastChar = sToken + tokenLength - 1;
						bool takeNumberAsLong = false;
						if (*lastChar == 'l') {
							*lastChar = 0;
							takeNumberAsLong = true;
						}
						wchar_t* p;
						unsigned long long n = wcstoull(&sToken[2], &p, 16);
						if (*p != 0) {
							std::string&& stdtoken = convertToAscii(sToken, tokenLength);
							eResult = EE_TOKEN_UNEXPECTED;
							scriptCompiler->setErrorText("token '" + stdtoken + "' is expected as a hexa number");
							break;
						}

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

						if (takeNumberAsLong == true || n > 0xFFFFFFFF) {
							pExpUnit = new CConstOperand<__int64>(sign * n, "long");
						}
						else {
							pExpUnit = new CConstOperand<int>(sign * (int)n, "int");
						}
					}
					else {
						std::string&& stdtoken = convertToAscii(sToken, tokenLength);
						eResult = EE_TOKEN_UNEXPECTED;
						scriptCompiler->setErrorText("token '" + stdtoken + "' is not expected here");
						break;
					}
				}
				else if (pLastUnit != nullptr && pLastUnit->getType() == EXP_UNIT_ID_MEMBER_ACCESS) {
					//expected member name here.
					//member name will be stored as string constant
					pStringConst = new CConstOperand<std::string>(convertToAscii(sToken), "string");
					pStringConst->setSourceCharIndex((int)(c - begin) - tokenLength);
				}
				else {
					std::string&& stdtoken = convertToAscii(sToken, tokenLength);
					bool foundRefType = false;
					if (stdtoken == "ref" && *c == ' ') {
						ScriptType stype;
						auto newC = scriptCompiler->readType(c + 1, end, stype);
						if (newC != nullptr && !stype.isUnkownType()) {
							// this mean pase type successfully.
							auto typeToken = stdtoken + " " + stype.sType();
							pExpUnit = new IncompletedUserFunctionUnit(typeToken);
							c = newC - 1;
							foundRefType = true;
						}
					}
					if (!foundRefType) {
						if (scriptCompiler->findKeyword(stdtoken) != KEYWORD_UNKNOWN) {
							eResult = EE_TOKEN_UNEXPECTED;
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
									eResult = EE_TYPE_UNKNOWN;
									scriptCompiler->setErrorText("type of '" + pVariable->getName() + "' is unknown");
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
										eResult = EE_TOKEN_UNEXPECTED;
										// keep last char in c
										c = ctemp;
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
										eResult = EE_TOKEN_UNEXPECTED;
										scriptCompiler->setErrorText("Unexpected token '" + convertToAscii(sToken) + "'");
										}*/
									}

									if (pTBDExpUnit) {
										pTBDExpUnit->setSourceCharIndex((int)(c - begin) - tokenLength);
									}
								}
							}
						}
					}
				}
				if (sToken == sToken1) {
					blHasDot = false;
					blHasChar = false;
				}
				if (pExpUnit) {
					pExpUnit->setSourceCharIndex((int)(c - begin) - tokenLength);
				}
			}
			if (pExpUnit) {
				pLastUnit = pExpUnit;
				expUnitList.push_back(ExpUnitRef((ExpUnit*)pExpUnit));
			}
			else if (pTBDExpUnit) {
				int tbdUnitIndex = pTBDExpUnit->getSourceCharIndex();
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
						//use index of last unit because two oprator '--' are become '+'
						tbdUnitIndex = pLastUnit->getSourceCharIndex();
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
					pTBDExpUnit->setSourceCharIndex(tbdUnitIndex);

					pLastUnit = pTBDExpUnit;
					expUnitList.push_back(ExpUnitRef(pTBDExpUnit));
				}
			}
			if (pStringConst) {
				pLastUnit = pStringConst;
				expUnitList.push_back(ExpUnitRef(pStringConst));
			}
			if (pFixedExpUnit) {
				pFixedExpUnit->setSourceCharIndex((int)(c - begin));

				bool isReplaced = false;
				if (pFixedExpUnit->getType() == EXP_UNIT_OPEN_SQUARE_BRACKET) {
					//check if the charactor is begin of a lambda expression
					if (checkPosibilityUnaryPrefix(pLastUnit) || pLastUnit->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT) {
						FunctionRef lambdaExpression;
						c = readLambdaExression(c, end, eResult, lambdaExpression);
						if (c == nullptr || !lambdaExpression) {
							return nullptr;
						}

						// since source char indcies generated by readLambdaExression are aligned by 'c',
						// they need to aligned by 'begin'
						recursiveOffsetSourceCharIndex(lambdaExpression.get(), (int)(c - begin));

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
			eResult = EE_INCOMPLETED_EXPRESSION;
		}

		if (eResult != EE_SUCCESS)
		{
			//clear and need to delete ExpUnit before clear
			expUnitList.clear();
		}		
		return c;
	}

	EExpressionResult ExpressionParser::tokenize(const WCHAR* sExpressionString, const WCHAR* end, std::list<ExpUnitRef>& expUnitList) {
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
				_lastErrorUnit = *it;
				return false;
			}

			//a operand can't be next to a user function
			//user function is a function use brackets to pass the param
			if (ISOPERAND(lastUnit) && ISUSERFUNC((*it))) {
				_lastErrorUnit = *it;
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

	const WCHAR* ExpressionParser::getLastCompileChar() const {
		return _lastCompileChar;
	}

	void ExpressionParser::setLastCompilerChar(const WCHAR* c) { 
		if (c != nullptr) {
			_lastCompileChar = c;
		}
	}

	ExpUnitRef ExpressionParser::getLastErrorUnit() const {
		return _lastErrorUnit;
	}

	EExpressionResult ExpressionParser::pickParamUnitsForFunction(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& functionRef) {
		EExpressionResult eResult = EE_SUCCESS;

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
			if (eResult != EE_SUCCESS || (int)params.size() >= maxParam) {
				break;
			}
		}

		if (eResult != EE_SUCCESS) {
			return eResult;
		}

		if ((int)params.size() < maxParam) {
			list<ExpressionRef> expList;
			eResult = makeExpressionList(inputList, expList);

			if (eResult == EE_SUCCESS && expList.size() != 1) {
				_lastErrorUnit = functionRef;
				eResult = EE_INCOMPLETED_EXPRESSION;
			}
			else if (_lastErrorUnit.get() == nullptr) {
				_lastErrorUnit = functionRef;
			}
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::pickParamUnitsElseClause(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& elseClauseUnitRef) {
		EExpressionResult eResult = EE_SUCCESS;

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
			if (eResult != EE_SUCCESS || (int)params.size() >= maxParam) {
				break;
			}
		}

		while (eResult != EE_SUCCESS) {
			if (it != end) {
				auto& expUnit = *it;
				if (expUnit->getType() == EXP_UNIT_ID_CLOSED_ROUND_BRACKET ||
					expUnit->getType() == EXP_UNIT_ID_CLOSED_CURLY_BRACKET ||
					expUnit->getType() == EXP_UNIT_ID_FUNC_SEPERATOR) {
					eResult = EE_SUCCESS;
					break;
				}
			}
			return eResult;
		}

		if ((int)params.size() < maxParam) {
			list<ExpressionRef> expList;
			eResult = makeExpressionList(inputList, expList);

			if (eResult == EE_SUCCESS && expList.size() != 1) {
				_lastErrorUnit = elseClauseUnitRef;
				eResult = EE_INCOMPLETED_EXPRESSION;
			}
			else if (_lastErrorUnit.get() == nullptr) {
				_lastErrorUnit = elseClauseUnitRef;
			}
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::compileConditionalExpression(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, ExecutableUnitRef& ifClauseUnit, ExecutableUnitRef& elseClauseUnit) {
		EExpressionResult eResult = EE_SUCCESS;
		//find if clause expression
		//use dummy function to collect the unit for if clause
		DynamicParamFunctionRef dummyFuncion = std::make_shared<DynamicParamFunction>(":", EXP_UNIT_ID_FUNC_CHOICE, FUNCTION_PRIORITY_CONDITIONAL, 1);
		// keep origin source char in new expression unit
		dummyFuncion->setSourceCharIndex(it->get()->getSourceCharIndex());

		eResult = pickParamUnitsForFunction(it, end, dummyFuncion);

		if (eResult != EE_SUCCESS) {
			return eResult;
		}

		if (it == end || (*it)->getType() != EXP_UNIT_ID_FUNC_CHOICE) {
			getCompiler()->setErrorText("Incompleted condition expression");
			eResult = EE_INCOMPLETED_EXPRESSION;
			_lastErrorUnit = dummyFuncion;
			return eResult;
		}

		ifClauseUnit = dummyFuncion->getFirstParam();

		//find else clause expression
		DynamicParamFunctionRef choiceOperatorRef = dynamic_pointer_cast<DynamicParamFunction>((*it));

		it++;
		eResult = pickParamUnitsElseClause(it, end, choiceOperatorRef);

		if (eResult == EE_SUCCESS) {
			elseClauseUnit = choiceOperatorRef->popParam();
		}
		else {

		}

		return eResult;
	}

	EExpressionResult ExpressionParser::putFunction(OperatorStack* pOperatorStack, OutputStack* pOutputStack, const DynamicParamFunctionRef& functionRef,
		list<ExpUnitRef>::const_iterator& iter, list<ExpUnitRef>::const_iterator end) {
		EExpressionResult eResult = EE_SUCCESS;
		ScriptCompiler* scriptCompiler = getCompiler();

		int iResult;

		//push the function or operator to Operator stack
		//// before push to stack compare with top of Operator stack
		while (eResult == EE_SUCCESS && pOperatorStack->size()) {

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
					eResult = EE_TOKEN_UNEXPECTED;
					_lastErrorUnit = pFuncTemp1;
					break;
				}

				if (iResult == 0) {
					break;
				}
			}
			pOutputStack->push(pExpFunction);
			pOperatorStack->pop();
		}
		if (eResult == EE_SUCCESS) {
			if (functionRef->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL) {
				ExecutableUnitRef ifClause, elseClause;

				iter++;
				eResult = compileConditionalExpression(iter, end, ifClause, elseClause);
				if (eResult != EE_SUCCESS) {
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
		EExpressionResult eResult = EE_SUCCESS;
		/*if (iter == end) {
			return EE_TYPE_END;
		}*/

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
				if (eResult == EE_SUCCESS) {
					eResult = putFunction(pOperatorStack, pOutputStack, functionRef, iter, end);
				}
			}
		}
		else if (ISOPERAND(expUnit)) {
			//push the operand to output stack
			pOutputStack->push(static_pointer_cast<ExecutableUnit>(expUnit));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_OPEN_ROUND_BRACKET || expUnit->getType() == EXP_UNIT_ID_OPEN_CURLY_BRACKET) {
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
			if (eResult != EE_SUCCESS) {
				return eResult;
			}

			pOutputStack = new OutputStack();
			pOperatorStack = new OperatorStack();
			inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));
		}
		else if (expUnit->getType() == EXP_UNIT_ID_CLOSED_ROUND_BRACKET || expUnit->getType() == EXP_UNIT_ID_CLOSED_CURLY_BRACKET) {
			auto openBracketType = expUnit->getType() == EXP_UNIT_ID_CLOSED_ROUND_BRACKET ? EXP_UNIT_ID_OPEN_ROUND_BRACKET : EXP_UNIT_ID_OPEN_CURLY_BRACKET;
			eResult = makeExpression(*pOutputStack, *pOperatorStack);
			if (/*pOutputStack->size() != 1 || */pOperatorStack->size() > 0) {
				_lastErrorUnit = expUnit;
				scriptCompiler->setErrorText("Incompleted expression");
				eResult = EE_INCOMPLETED_EXPRESSION;
			}
			if (eResult != EE_SUCCESS) {
				return eResult;
			}

			auto paramCollectionUnit = new ParamUnitCollection();
			ExecutableUnitRef paramCollectionUnitRef(paramCollectionUnit);
			auto& params = paramCollectionUnit->getParams();

			// temporary set source char index of paramCollectionUnit is source char index of close bracket
			paramCollectionUnit->setSourceCharIndex(expUnit->getSourceCharIndex());

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

				if (pOperatorStack->size() && pOperatorStack->top()->getType() == openBracketType) {
					break;
				}
				inputList.pop_back();

				if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
					scriptCompiler->setErrorText("Incompleted expression");
					eResult = EE_INCOMPLETED_EXPRESSION;
					break;
				}
				params.push_front (pOutputStack->top());

				delete pOutputStack;
				delete pOperatorStack;
			}
			if (eResult != EE_SUCCESS) {
				_lastErrorUnit = expUnit;
				return eResult;
			}
			//at least input list must contain open bracket
			//so it must be at least one layer
			if (inputList.size() == 0) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), expUnit->GetName()));
				scriptCompiler->setErrorText("Unexpected token '" + expUnit->toString() + "'");
				eResult = EE_INCOMPLETED_EXPRESSION;
				_lastErrorUnit = expUnit;
				return eResult;
			}

			if (pOperatorStack->size() == 0 || pOperatorStack->top()->getType() != openBracketType) {
				scriptCompiler->setErrorText("Unexpected token ')'");
				_lastErrorUnit = expUnit;
				return EE_INCOMPLETED_EXPRESSION;
			}
			
			//pop open bracket, we don't need it any more
			auto bracket = pOperatorStack->top().get();
			// set true source char index of paramCollectionUnit is source char index of open bracket
			// before it is destroyed
			paramCollectionUnit->setSourceCharIndex(bracket->getSourceCharIndex());
			pOperatorStack->pop();

			if (params.size() == 1 && openBracketType == EXP_UNIT_ID_OPEN_ROUND_BRACKET) {
				auto unitInsideBracket = params.front();
				auto& token = unitInsideBracket->toString();
				int typeId = DATA_TYPE_UNKNOWN;
				// check if there is a valid type inside the round brackets...
				ScriptType stype = ScriptType::parseType(scriptCompiler, token);
				if (!stype.isUnkownType()) {
					/// ... then convert all to type cast operator
					auto typeCastFunction = new DynamicParamFunction(TYPE_CAST_OPERATOR, EXP_UNIT_ID_OPERATOR_TYPE_CAST, FUNCTION_PRIORITY_TYPE_CAST, stype);
					typeCastFunction->setMaxParam(1);
					typeCastFunction->setSourceCharIndex(unitInsideBracket->getSourceCharIndex());
					typeCastFunction->setIndex(unitInsideBracket->getIndex());
					unitInsideBracket.reset(typeCastFunction);

					pOperatorStack->push(dynamic_pointer_cast<DynamicParamFunction>(unitInsideBracket));

					return eResult;
				}
			}

			//get previous unit before open bracket
			ExpUnit* previousUnit = nullptr;

			if (pOperatorStack->size() > 0) {
				if (pOperatorStack->top().get()->getIndex() < bracket->getIndex()) {
					previousUnit = pOperatorStack->top().get();
				}
			}
			if( pOutputStack->size() > 0) {
				if (previousUnit == nullptr) {
					if (pOutputStack->top().get()->getIndex() < bracket->getIndex()) {
						previousUnit = pOutputStack->top().get();
					}
				}
				else if (pOutputStack->top().get()->getIndex() < bracket->getIndex()) {
					if (pOutputStack->top().get()->getIndex() > previousUnit->getIndex()) {
						previousUnit = pOutputStack->top().get();
					}
				}
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
				auto functionOperator = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_OPERATOR_FUNCTIONCALL, FUNCTION_PRIORITY_FUNCTIONCALL, 2);
				// keep origin source char in new expression unit
				functionOperator->setSourceCharIndex(iter->get()->getSourceCharIndex());

				eResult = putFunction(pOperatorStack, pOutputStack, functionOperator, iter, end);
				if (eResult != EE_SUCCESS) {
					return eResult;
				}
				//pust param collection of subscript operator to output stack
				//it will be processed later
				pOutputStack->push(paramCollectionUnitRef);
				
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
				_lastErrorUnit = expUnit;
				eResult = EE_INCOMPLETED_EXPRESSION;
			}
			if (eResult != EE_SUCCESS) {
				return eResult;
			}
			auto paramCollectionUnit = new ParamUnitCollection();
			// temporary set source char index of paramCollectionUnit is source char index of close bracket
			paramCollectionUnit->setSourceCharIndex(expUnit->getSourceCharIndex());

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
					eResult = EE_INCOMPLETED_EXPRESSION;
					break;
				}
				params.push_front(pOutputStack->top());

				delete pOutputStack;
				delete pOperatorStack;
			}

			if (eResult != EE_SUCCESS) {
				_lastErrorUnit = expUnit;
				return eResult;
			}
			//at least input list must contain open square bracket and subscription's owner
			//so it must be at least one layer
			if (inputList.size() < 1) {
				DBG_ERROR(_tprintf(__T("\n[#]Unexpected token %s"), expUnit->GetName()));
				scriptCompiler->setErrorText("Unexpected token '" + expUnit->toString() + "'");
				eResult = EE_INCOMPLETED_EXPRESSION;
				_lastErrorUnit = expUnit;
				return eResult;
			}

			if (pOperatorStack->size() == 0 || pOperatorStack->top()->getType() != EXP_UNIT_ID_OPEN_SQUARE_BRACKET) {
				scriptCompiler->setErrorText("Unexpected token ')'");
				_lastErrorUnit = expUnit;
				return EE_INCOMPLETED_EXPRESSION;
			}

			// set true source char index of paramCollectionUnit is source char index of open bracket
			paramCollectionUnit->setSourceCharIndex(pOperatorStack->top()->getSourceCharIndex());

			//replace unit '[' by subscript operator
			pOperatorStack->pop();
			auto subscriptOperator = make_shared<DynamicParamFunction>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, 2);

			// set true source char index of operator '[]' is source char index of open bracket
			subscriptOperator->setSourceCharIndex(paramCollectionUnit->getSourceCharIndex());

			eResult = putFunction(pOperatorStack, pOutputStack, subscriptOperator, iter, end);
			if (eResult != EE_SUCCESS) {
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
			eResult = EE_TOKEN_TYPE_UNEXPECTED;
			_lastErrorUnit = expUnit;
		}

		return eResult;
	}

	EExpressionResult ExpressionParser::makeExpressionList(ExpressionInputList& inputList, list<ExpressionRef>& expList) {
		OperatorStack* pOperatorStack;
		OutputStack* pOutputStack;
		Expression* pExp = NULL;
		EExpressionResult eResult = EE_SUCCESS;
		ScriptCompiler* scriptCompiler = getCompiler();

		while (inputList.size()) {
			ExpressionEntry& entry = inputList.back();
			pOutputStack = entry.first;
			pOperatorStack = entry.second;
			inputList.pop_back();

			if (eResult == EE_SUCCESS) {
				eResult = makeExpression(*pOutputStack, *pOperatorStack);

				if (pOutputStack->size() != 1 || pOperatorStack->size() > 0) {
					eResult = EE_INCOMPLETED_EXPRESSION;
					scriptCompiler->setErrorText("Incompleted expression");
					if (pOperatorStack->size()) {
						_lastErrorUnit = pOperatorStack->top();
					}
				}
				if (eResult != EE_SUCCESS) {
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
		_lastErrorUnit.reset();

		numberedForUnit(expUnitList);
		if (precheck(expUnitList) == false) return false;
		ExpressionInputList inputList;

		EExpressionResult eResult1;
		EExpressionResult* pResult = &eResult1;
		if (peResult) pResult = peResult;
		EExpressionResult& eResult = *pResult;

		eResult = EE_SUCCESS;

		OperatorStack* pOperatorStack = new OperatorStack();;
		OutputStack* pOutputStack = new OutputStack();
		inputList.push_back(ExpressionEntry(pOutputStack, pOperatorStack));

		DBG_INFO(List_Enumerate(&expUnitList, printNode));
		
		const list<ExpUnitRef>& expUnitListConst = expUnitList;
		for (auto iter = expUnitListConst.begin(); iter != expUnitListConst.end() && eResult == EE_SUCCESS; iter++)
		{
			eResult = putAnExpUnit(iter, expUnitListConst.end(), inputList);
		}

		if (eResult != EE_SUCCESS) {
			return false;
		}

		eResult = makeExpressionList(inputList, expList);
		if (eResult != EE_SUCCESS && _lastErrorUnit.get() == nullptr) {
			if (expUnitList.size()) {
				_lastErrorUnit = expUnitList.back();
			}
		}

		return eResult == EE_SUCCESS && expList.size() > 0;
	}

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

	///
	/// check ability of assigning a dynamic array to a struct
	///
	bool checkAssigmentOperatorForStruct(ScriptCompiler* scriptCompiler, const StructClass* pStruct, const DynamicParamFunctionRef& function) {
		MemberInfo memberInfo;
		bool res = pStruct->getMemberFirst(nullptr, &memberInfo);
		auto& params = function->getParams();
		auto iter = params.begin();
		bool blError;
		while (res && iter != params.end()) {
			if (memberInfo.type != iter->get()->getReturnType()) {
				blError = true;
				//does not allow different type assigment for each member in the struct
				const StructClass* pSubStruct = scriptCompiler->getStruct(memberInfo.type.iType());
				if (pSubStruct != nullptr && iter->get()->getType() == EXP_UNIT_ID_DYNAMIC_FUNC) {
					if (checkAssigmentOperatorForStruct(scriptCompiler, pSubStruct, dynamic_pointer_cast<DynamicParamFunction>(*iter)))
						blError = false;

				}
				if (blError)
					return false;
			}
			res = pStruct->getMemberNext(nullptr, &memberInfo);
			iter++;
		}

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
				createLambdaCandidate->setSourceCharIndex(funcNode->getSourceCharIndex());
				
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
					/*if (param1Type == param2Type) {
						if (!scriptCompiler->convertToRef(param1)) {
							return nullptr;
						}

						auto defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param2Type.makeSemiRef());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
					else */
					if ((param1->getMask() & UMASK_DECLAREINEXPRESSION) && param1Type.isSemiRefType() && param1->getType() == EXP_UNIT_ID_XOPERAND) {
						if (param2->getType() != EXP_UNIT_ID_XOPERAND) {
							scriptCompiler->setErrorText(param2->toString() + " must be a variable");
							return nullptr;
						}
						if (param1Type.semiRefLevel() - param2Type.semiRefLevel() == 1) {
							auto defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_ASSIGMENT_SEMIREF, FUNCTION_PRIORITY_ASSIGNMENT, param1Type);
							defaultOperatorUnit->pushParam(param1);
							defaultOperatorUnit->pushParam(param2);
							// keep origin source char in new expression unit
							defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

							defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
							continue;
						}
					}
					if (param1Type.origin() == param2Type.origin() && param1Type.refLevel() == param2Type.refLevel()/* && param2Type.isSemiRefType()*/) {
						//default assigment operator for all types
						if (!param1Type.semiRefLevel()) {
							if (!scriptCompiler->convertToRef(param1)) {
								return nullptr;
							}
						}
						FixParamFunction<2>* defaultOperatorUnit;
						
						if (param2Type.isSemiRefType() && param2Type.semiRefLevel() >= param1Type.semiRefLevel()) {
							defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR_REF, FUNCTION_PRIORITY_ASSIGNMENT, param2Type);
						}
						else {
							defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param2Type.makeSemiRef());
						}
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);

						// keep origin source char in new expression unit
						defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
					else if (param1Type.refLevel() == 0 && param1Type.isFunctionType() && param2Type.iType() == basicTypes.TYPE_NULL) {
						if (!scriptCompiler->convertToRef(param1)) {
							return nullptr;
						}
					
						RuntimeFunctionInfo nullValueOfRuntimeFunctionInfo;
						defaultRuntimeFunctionInfoConstructor(&nullValueOfRuntimeFunctionInfo);
						auto nullValueRef = make_shared<CConstOperand<RuntimeFunctionInfo>>(nullValueOfRuntimeFunctionInfo, param1Type);
						// keep origin source char in new expression unit
						nullValueRef->setSourceCharIndex(param2->getSourceCharIndex());

						auto defaultOperatorUnit = new FixParamFunction<2>(DEFAULT_COPY_OPERATOR, EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR, FUNCTION_PRIORITY_ASSIGNMENT, param1Type.makeSemiRef());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(nullValueRef);

						// keep origin source char in new expression unit
						defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}

					const StructClass* pStructInfo = scriptCompiler->getStruct(param1Type.origin());
					if (pStructInfo != nullptr && param2Type.origin() == basicTypes.TYPE_VARIANTARRAY) {
						//default assigment operator for variant array to struct
						if (param1Type.isRefType() == false) {
							if (!scriptCompiler->convertToRef(param1)) {
								return nullptr;
							}
						}

						auto defaultOperatorUnit = new FixParamFunction<2>("=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, pStructInfo->getName());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);
						defaultOperatorUnit->setReturnType(param1Type);
						defaultOperatorUnit->setNative((DFunction2Ref)(new ArrayToStructCommand()));

						// keep origin source char in new expression unit
						defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
						continue;
					}
				}

				if (funcNode->getType() == EXP_UNIT_ID_OPERATOR_SUBSCRIPT) {
					if (param1Type.isRefType()) {
						//argument #2 is integer, so we need to find a way to convert current value to integer
						if (param2Type.iType() != basicTypes.TYPE_INT) {
							//the following code is casting before execute param 2 unit
							auto castingFunction = scriptCompiler->findCastingFunction(param2Type, ScriptType(basicTypes.TYPE_INT, scriptCompiler->getType(basicTypes.TYPE_INT)));
							if (!castingFunction) {
								continue;
							}
							castingFunction->pushParam(param2);
							castingFunction->setMask(castingFunction->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

							// keep origin source char in new expression unit
							castingFunction->setSourceCharIndex(param2->getSourceCharIndex());

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

						// Fix bug, unexpected access behaviour if the object has type like that 'ref T& arr'
						// in this case we should consider it as 'ref T arr' then the expression arr[0] will return
						// the exepceted type, T& or T.

						ScriptType returnType1 = param1Type.deRef();
						bool semiRefType = returnType1.isSemiRefType();
						ScriptType returnType2;
						if (semiRefType) {
							returnType2 = returnType1;
							returnType1 = returnType1.deSemiRef();
						} 
						else {
							returnType2 = returnType1.makeSemiRef();
						}
						
						int elementSize = scriptCompiler->getTypeSize(returnType1);

						auto defaultOperatorUnit2 = new FixParamFunction<2>(SUBSCRIPT_OPERATOR, EXP_UNIT_ID_OPERATOR_SUBSCRIPT, FUNCTION_PRIORITY_SUBSCRIPT, returnType2);
						defaultOperatorUnit2->pushParam(param1);
						defaultOperatorUnit2->pushParam(param2);
						if (semiRefType) {
							defaultOperatorUnit2->setNative((DFunction2Ref)(new ElementAccessCommand4(elementSize)));
						}
						else {
							defaultOperatorUnit2->setNative((DFunction2Ref)(new ElementAccessCommand2(elementSize)));
						}

						// keep origin source char in new expression unit
						defaultOperatorUnit2->setSourceCharIndex(funcNode->getSourceCharIndex());

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));
						
						continue;
					}
					if ( (param1Type.iType() & DATA_TYPE_ARRAY_MASK) ) {
						if (param1Type.isRefType() == false) {
							//argument #2 is integer, so we need to find a way to convert param2 to integer
							if (param2Type.iType() != basicTypes.TYPE_INT) {
								//the following code is casting before execute param 2 unit
								auto castingFunction = scriptCompiler->findCastingFunction(param2Type, ScriptType(basicTypes.TYPE_INT, scriptCompiler->getType(basicTypes.TYPE_INT)));
								if (!castingFunction) {
									return nullptr;
								}
								castingFunction->pushParam(param2);
								castingFunction->setMask(castingFunction->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

								// keep origin source char in new expression unit
								castingFunction->setSourceCharIndex(param2->getSourceCharIndex());

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

							// keep origin source char in new expression unit
							defaultOperatorUnit2->setSourceCharIndex(funcNode->getSourceCharIndex());

							defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit2));
							continue;
						}
					}
				}

				if (funcNode->getType() == EXP_UNIT_ID_USER_OPER && funcNode->getName() == "==") {
					if (param1Type.refLevel() == 0 && param1Type.isFunctionType() && param2Type.refLevel() == 0 && param2Type.isFunctionType()) {
						if (!scriptCompiler->convertToRef(param1)) {
							return nullptr;
						}
						if (!scriptCompiler->convertToRef(param2)) {
							return nullptr;
						}
						ScriptType boolType(basicTypes.TYPE_BOOL, "bool");
						auto defaultOperatorUnit = new FixParamFunction<2>("=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, FUNCTION_PRIORITY_ASSIGNMENT, boolType.sType());
						defaultOperatorUnit->pushParam(param1);
						defaultOperatorUnit->pushParam(param2);
						defaultOperatorUnit->setReturnType(boolType);
						defaultOperatorUnit->setNative((DFunction2Ref)(new CompareRuntimeFunction()));

						// keep origin source char in new expression unit
						defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

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
						if (param1Type.isSemiRefType() == false && !scriptCompiler->convertToRef(param1)) {
							return nullptr;
						}						
					}
					else if(param1Type.isSemiRefType()) {
						auto argumentType = param1Type.deSemiRef();
						auto deRefFunction = new FixParamFunction<1>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, argumentType);
						deRefFunction->setNative((DFunction2Ref)(new DeRefCommand(scriptCompiler->getTypeSize(argumentType))));
						deRefFunction->pushParam(param1);

						// keep origin source char in new expression unit
						deRefFunction->setSourceCharIndex(param1->getSourceCharIndex());

						param1.reset(deRefFunction);
					}

					//after link, param 2 is contain member info. Now, it's time to retreive it
					MemberInfo* memberInfo = (MemberInfo*)param2->Execute();
					int returnSize = scriptCompiler->getTypeSize(memberInfo->type);

					ScriptType typeInt(basicTypes.TYPE_INT, "int");
					ScriptType& memberType = memberInfo->type;

					CConstOperand<int>* param2New = new CConstOperand<int>(memberInfo->offset, typeInt);
					// keep origin source char in new expression unit
					param2New->setSourceCharIndex(param2->getSourceCharIndex());

					ExecutableUnitRef param2NewRef(param2New);

					/*auto defaultOperatorUnit1 = new FixParamFunction<2>(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, memberType);
					defaultOperatorUnit1->pushParam(param1);
					defaultOperatorUnit1->pushParam(param2NewRef);
					defaultOperatorUnit1->setNative((DFunction2Ref)(new MemberAccessCommand1(sizeof(void*))));
					defaultOperatorUnit1->setMask(UMASK_EXCLUDEFROMDESTRUCTOR);*/

					auto defaultOperatorUnit2 = new FixParamFunction<2>(".", EXP_UNIT_ID_MEMBER_ACCESS, FUNCTION_PRIORITY_MEMBER_ACCESS, memberType.makeSemiRef());
					defaultOperatorUnit2->pushParam(param1);
					defaultOperatorUnit2->pushParam(param2NewRef);
					defaultOperatorUnit2->setNative((DFunction2Ref)(new MemberAccessCommand2()));

					// keep origin source char in new expression unit
					defaultOperatorUnit2->setSourceCharIndex(funcNode->getSourceCharIndex());
					
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

						// keep origin source char in new expression unit
						defaultOperatorUnit->setSourceCharIndex(funcNode->getSourceCharIndex());

						defaultOperators->push_back(ExecutableUnitRef(defaultOperatorUnit));
					}
					continue;
				}
			}
		}

		return defaultOperators;
	}

	template<class T>
	T* makeRef(const T& val) {
		return (T*)&val;
	}

	void applyCastingAndOptimize(ExecutableUnitRef& unit, const FunctionRef& castingUnit) {
		if ((castingUnit->getType() == EXP_UNIT_ID_MAKE_REF && unit->getType() == EXP_UNIT_ID_DEREF) ||
			(castingUnit->getType() == EXP_UNIT_ID_SEMI_REF && unit->getType() == EXP_UNIT_ID_DEREF) ||
			(castingUnit->getType() == EXP_UNIT_ID_DEREF && unit->getType() == EXP_UNIT_ID_MAKE_REF) ||
			(castingUnit->getType() == EXP_UNIT_ID_DEREF && unit->getType() == EXP_UNIT_ID_SEMI_REF)) {
			//remove making ref function and deref function when it meet toghether
			unit = ((Function*)unit.get())->getChild(0);
		}
		else {
			ScriptCompiler::applyCasting(unit, castingUnit);
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
				auto& param = path[i];
				auto& paramType = param->getReturnType();
				auto& argumentType = *argTypes[i];
				if (scriptCompiler->findMatchingComposite(argumentType, param, paramCastingList.at(i))) {
					;
				}
				else if (scriptCompiler->findMatching(refVoidType, argumentType, paramType, paramCastingList.at(i), true)) {
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
				// keep origin source char in new expression unit
				castingF->setSourceCharIndex(paramRef->getSourceCharIndex());
				applyCastingAndOptimize(paramRef, castingF);
				paramRef->setReturnType(*(kt->get()));
			}
			jt++;
			kt++;
		}

		return std::make_pair(it->paramPath, it->candidate.totalAccurative);
	}

	CandidateCollectionRef filterCandidate(ScriptCompiler* scriptCompiler,
		const string& functionName, int functionType,
		const std::vector<CandidateCollectionRef>& candidatesForParams, EExpressionResult& eResult) {		
		const list<OverLoadingItem>* overloadingFuncs = scriptCompiler->findOverloadingFuncRoot(functionName);

		if (!overloadingFuncs) {
			eResult = EE_FUNCTION_NOT_FOUND;
			scriptCompiler->setErrorText("function " + functionName + " is not found");
			return nullptr;
		}

		return scriptCompiler->filterCandidate(functionName, functionType, overloadingFuncs, candidatesForParams, eResult);
	}	

	EExpressionResult checkCandidates(ScriptCompiler* scriptCompiler, const FunctionRef& function, const CandidateCollectionRef& candidates) {
		return EE_SUCCESS;
		for (auto it = candidates->begin(); it != candidates->end(); it++) {
			auto jt = it;
			ExecutableUnitRef& c1 = *it;
			auto& rt1 = c1->getReturnType();
			for (jt++; jt != candidates->end(); jt++) {
				ExecutableUnitRef& c2 = *jt;
				auto& rt2 = c2->getReturnType();
				if (rt1 == rt2) {
					scriptCompiler->setErrorText("ambious function call for function " + function->getName());
					return EE_TYPE_AMBIOUS_CALL;
				}
			}
		}

		return EE_SUCCESS;
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
			auto functionUnitRef = dynamic_pointer_cast<Function>(unit);
			unitCandidate = completeFunctionTree(scriptCompiler, functionUnitRef , eResult);
			unit = functionUnitRef;
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
					eResult = EE_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("token '" + unit->toString() + "' is not found");
					LOG_COMPILE_MESSAGE(scriptCompiler->getLogger(), MESSAGE_ERROR, scriptCompiler->formatMessage("token '%s' is not found", unit->toString().c_str()));
					_lastErrorUnit = unit;
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

				// keep origin source char in new expression unit
				functionPointerUnit->setSourceCharIndex(unit->getSourceCharIndex());

				unitCandidate->push_back(ExecutableUnitRef(functionPointerUnit));
			}			
		}
		else if (ISOPERAND(unit)) {
			//linking for operand
			auto& scriptType = unit->getReturnType();
			if (scriptType.isUnkownType()) {
				scriptType.updateType(scriptCompiler);
				if (scriptType.isUnkownType()) {
					eResult = EE_TYPE_UNKNOWN;
					scriptCompiler->setErrorText("data type of '" + unit->toString() + "' is unknown");
					_lastErrorUnit = unit;
					return nullptr;
				}
			}
			unitCandidate = std::make_shared<CandidateCollection>();
			unitCandidate->push_back(unit);
		}
		else {
			//other kinds is known as error
			eResult = EE_TOKEN_UNEXPECTED;
			scriptCompiler->setErrorText("Unexpected operator '" + unit->toString() + "'");
			_lastErrorUnit = unit;
		}

		return unitCandidate;
	}

	inline bool isLogicBuitInTypes(const BasicTypes& basicTypes, int originType) {
		return (originType == basicTypes.TYPE_INT ||
			originType == basicTypes.TYPE_DOUBLE ||
			originType == basicTypes.TYPE_FLOAT ||
			originType == basicTypes.TYPE_LONG ||
			originType == basicTypes.TYPE_BOOL);
	}

	inline bool isIntegerType(const BasicTypes& basicTypes, int originType) {
		return (originType == basicTypes.TYPE_INT ||
			originType == basicTypes.TYPE_CHAR ||
			originType == basicTypes.TYPE_LONG ||
			originType == basicTypes.TYPE_NULL ||
			originType == basicTypes.TYPE_WCHAR);
	}

	CandidateCollectionRef ExpressionParser::completeFunctionTree(ScriptCompiler* scriptCompiler, FunctionRef& function, EExpressionResult& eResult) {
		//LOG_I("begin update expression tree for " + POINTER2STRING(function.get()));
		eResult = EE_SUCCESS;

		Variable* variable;
		int n = function->getChildCount();

		CandidateCollectionRef functionCandidates;
		CandidateCollectionRef param1Candidates;
		std::vector<CandidateCollectionRef> candidatesForParams(n);
		int p = 0;
		const BasicTypes& basicType = scriptCompiler->getTypeManager()->getBasicTypes();
		
		bool check = false;
		bool candidateIsCompleted = false;

		ScriptScope* currentScope = scriptCompiler->currentScope();

		if (function->getType() == EXP_UNIT_ID_OPERATOR_FUNCTIONCALL) {
			ExecutableUnitRef& pExeUnit1 = function->getChild(0);
			ExecutableUnitRef& pExeUnit2 = function->getChild(1);

			DynamicParamFunction* paramCollector = (DynamicParamFunction*)pExeUnit2.get();
			auto& params = paramCollector->getParams();
			
			enum class FlowState
			{
				None,
				MEMBER_ACCESS,
			};

			FlowState prevState = FlowState::None;

			while (true) {
				if (pExeUnit1->getType() == EXP_UNIT_ID_INCOMPFUNC) {
					auto& functionName = pExeUnit1->toString();
					auto newFunction = scriptCompiler->createExpUnitFromName(functionName);
					if (newFunction == nullptr) {
						eResult = EE_TOKEN_UNEXPECTED;
						scriptCompiler->setErrorText("Unexpected token '" + functionName + "'");
						_lastErrorUnit = pExeUnit1;
						return nullptr;
					}

					//function operator is replaced by the real function
					newFunction->setSourceCharIndex(function->getSourceCharIndex());
					function.reset((Function*)newFunction);
					for (auto it = params.begin(); it != params.end(); it++) {
						if (function->pushParam(*it) < 0) {
							eResult = EE_FUNCTION_NOT_FOUND;
							scriptCompiler->setErrorText("function '" + function->getName() + "' is not compatible with its parameters");
							_lastErrorUnit = *it;
							return nullptr;
						}
					}

					//update parameter count
					n = function->getChildCount();
					candidatesForParams.resize(n);

					if (prevState == FlowState::MEMBER_ACCESS) {
						auto& contextObject = function->getChild(0);
						auto contextObjectRef = contextObject;
						scriptCompiler->convertToRef(contextObjectRef);
						auto candidatesRef = linkForUnit(scriptCompiler, contextObjectRef, eResult);
						if (eResult != EE_SUCCESS) {
							return nullptr;
						}
						candidatesRef->push_back(contextObject);
						candidatesForParams[0] = candidatesRef;
					}

					// the case is processed completly. So, it's time to break the flow
					break;
				}
			    if (pExeUnit1->getType() == EXP_UNIT_ID_MEMBER_ACCESS) {
					auto memberAccessFunction = (Function*)pExeUnit1.get();
					auto contextObject = memberAccessFunction->getChild(0);
					auto functionUnit = memberAccessFunction->getChild(1);

					auto& contextType = contextObject->getReturnType();
					auto& functionUnitType = functionUnit->getReturnType();
					functionUnitType.updateType(scriptCompiler);

					// check if the second unit is a string constant...
					if (functionUnitType.iType() == basicType.TYPE_STRING && functionUnit->getType() == EXP_UNIT_ID_CONST) {
						string& memberName = *((string*)functionUnit->Execute());
						bool processAndMethodCalling = true;
						auto struct1 = scriptCompiler->getStruct(contextType.origin());

						if (struct1) {
							MemberInfo memberInfo;
							// check if the struct contain the member
							if (struct1->getInfo(memberName, memberInfo)) {
								// then let further processing deal with this case.
								processAndMethodCalling = false;
							}
						}
						if (processAndMethodCalling) {
							auto methodUnitRef = make_shared<IncompletedUserFunctionUnit>(memberName);
							methodUnitRef->setSourceCharIndex(functionUnit->getSourceCharIndex());
							// replace the member access unit by the new function
							pExeUnit1 = methodUnitRef;
							// push the context object to param collector unit
							params.push_front(contextObject);
							// then goto the beginning of the flow
							prevState = FlowState::MEMBER_ACCESS;
							continue;
						}
						// if it is not a method calling case,
						// then let it flows to further processing.
					}
					else {
						// if not then this should not be considered at method calling case.
						// then let it flows to further processing.
					}
				}
				if (pExeUnit1->getReturnType().isFunctionType()) {
					candidatesForParams.resize(1 + params.size());
					auto forwardCallUnitRef = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_FORWARD_CALL, FUNCTION_PRIORITY_FUNCTIONCALL, 1 + (int)params.size());
					// keep origin source char in new expression unit
					forwardCallUnitRef->setSourceCharIndex(function->getSourceCharIndex());

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
					if (eResult != EE_SUCCESS) {
						return nullptr;
					}
					// first check the candidate is a function type...
					if (param1CandidatesTmp->size() == 1 && param1CandidatesTmp->front()->getReturnType().isFunctionType()) {
						candidatesForParams.resize(1 + params.size());
						auto forwardCallUnitRef = make_shared<DynamicParamFunction>(FUNCTION_OPERATOR, EXP_UNIT_ID_FORWARD_CALL, FUNCTION_PRIORITY_FUNCTIONCALL, 1 + (int)params.size());
						// keep origin source char in new expression unit
						forwardCallUnitRef->setSourceCharIndex(function->getSourceCharIndex());

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
										eResult = EE_FUNCTION_NOT_FOUND;
										scriptCompiler->setErrorText("Library error function operator for type " + checkType.sType() + " is missing");
										_lastErrorUnit = *it;
										return nullptr;
									}
									// keep origin source char in new expression unit
									functionUnit->setSourceCharIndex(function->getSourceCharIndex());

									functionCandidates->push_back(ExecutableUnitRef(functionUnit));
								}
							}
						}
						if (functionCandidates->size() <= 0) {
							eResult = EE_TOKEN_UNEXPECTED;
							scriptCompiler->setErrorText("Unexpected operator for token " + pExeUnit1->toString() + " '()'");
							_lastErrorUnit = pExeUnit1;
							return nullptr;
						}
						candidatesForParams[0] = param1CandidatesTmp;
					}

					//update parameter count
					//n = 1 + (int)params.size();
					//candidatesForParams.resize(n);
				}
				break;
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
				eResult = EE_FUNCTION_NOT_FOUND;
				scriptCompiler->setErrorText("function '" + function->getName() + "' is not compatible with its parameters");
				return nullptr;
			}*/

			
			check = true;
		}
		else if (function->getType() == EXP_UNIT_ID_SEMI_REF) {
			if (n != 1) {
				eResult = EE_TOKEN_INVALID;
				scriptCompiler->setErrorText("'&' operator can only take one parameter");
				_lastErrorUnit = function;
				return nullptr;
			}
			auto& refUnit = function->getChild(0);
			if (refUnit->getType() != EXP_UNIT_ID_XOPERAND) {
				eResult = EE_TOKEN_INVALID;
				scriptCompiler->setErrorText("'&' operator can only use for variable");
				_lastErrorUnit = function;
				return nullptr;
			}

			param1Candidates = linkForUnit(scriptCompiler, refUnit, eResult);
			if (eResult != EE_SUCCESS && param1Candidates->size() == 0) {
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

				// keep origin source char in new expression unit
				newCandidate->setSourceCharIndex(it->get()->getSourceCharIndex());

				functionCandidates->push_back(newCandidate);
			}

			return functionCandidates;
		}
		else if (function->getType() == EXP_UNIT_ID_OPERATOR_TYPE_CAST) {
			if (n != 1) {
				eResult = EE_TYPE_CONVERSION_ERROR;
				scriptCompiler->setErrorText("type cast operator can only take one parameter");
				_lastErrorUnit = function;
				return nullptr;
			}
			auto& returnType = function->getReturnType();
			if (returnType.isSemiRefType()) {
				eResult = EE_TYPE_CONVERSION_ERROR;
				scriptCompiler->setErrorText("type cast operator can not apply to " + returnType.sType());
				_lastErrorUnit = function;
				return nullptr;
			}
			
			auto& paramUnit = function->getChild(0);
			param1Candidates = linkForUnit(scriptCompiler, paramUnit, eResult);
			if (eResult != EE_SUCCESS && param1Candidates->size() == 0) {
				return nullptr;
			}

			functionCandidates = make_shared<CandidateCollection>();

			ScriptType typeLong(basicType.TYPE_LONG, scriptCompiler->getType(basicType.TYPE_LONG));
			ScriptType typeInt(basicType.TYPE_INT, scriptCompiler->getType(basicType.TYPE_INT));

			for (auto it = param1Candidates->begin(); it != param1Candidates->end(); it++) {
				auto& unit = *it;
				auto& param1TypeN = unit->getReturnType();
				ExecutableUnitRef newCandidate;

			    if (returnType.isRefType()) {
					if (param1TypeN.isRefType()) {
						newCandidate = unit;
					}
					else if(param1TypeN.isSemiRefType()) {
						auto param1TypeNOriginType = param1TypeN.deSemiRef();
						
						if (basicType.TYPE_INT == param1TypeNOriginType.iType() || basicType.TYPE_LONG == param1TypeNOriginType.iType()) {
							auto param1TypeNOriginSize = scriptCompiler->getTypeSize(param1TypeNOriginType.iType());

							auto derRefUnit = make_shared<FixParamFunction<1>>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, param1TypeNOriginType);
							derRefUnit->pushParam(unit);
							derRefUnit->setNative(make_shared<DeRefCommand>(param1TypeNOriginSize));

							newCandidate = derRefUnit;

							if (param1TypeNOriginSize != scriptCompiler->getTypeSize(returnType.iType())) {
								auto& targetType = typeInt.iType() == param1TypeNOriginType.iType() ?
									typeLong : typeInt;
								auto castingFunction = scriptCompiler->findCastingFunction(param1TypeNOriginType, targetType);
								if (castingFunction) {
									castingFunction->pushParam(derRefUnit);
								}

								newCandidate.reset(castingFunction);
							}
							if (newCandidate) {
								// keep origin source char in new expression unit
								newCandidate->setSourceCharIndex(function->getSourceCharIndex());
							}
						}
					}
					else if (scriptCompiler->getTypeSize(param1TypeN.iType()) == sizeof(void*)) {
						if (basicType.TYPE_INT == param1TypeN.iType() || basicType.TYPE_LONG == param1TypeN.iType()) {
							newCandidate = unit;
						}
					}
					else if (basicType.TYPE_INT == param1TypeN.iType() || basicType.TYPE_LONG == param1TypeN.iType()) {
						auto& targetType = typeInt.iType() == param1TypeN.iType() ?
							typeLong : typeInt;
						auto castingFunction = scriptCompiler->findCastingFunction(param1TypeN, targetType);
						if (castingFunction) {
							castingFunction->pushParam(unit);
							// keep origin source char in new expression unit
							castingFunction->setSourceCharIndex(function->getSourceCharIndex());
						}
						newCandidate.reset(castingFunction);
					}
					if (!newCandidate) {
						continue;
					}
				}
				else if(returnType == param1TypeN){
					newCandidate = unit;
				}
				else if (param1TypeN.isRefType()) {
					if (basicType.TYPE_INT == returnType.iType() || basicType.TYPE_LONG == returnType.iType()) {
						if (scriptCompiler->getTypeSize(returnType.iType()) != sizeof(void*)) {
							// for 32 bit app: int(long)
							// for 64 bit app: long(int)
							auto& sourceType = typeInt.iType() == returnType.iType() ?
								typeLong : typeInt;

							auto castingFunction = scriptCompiler->findCastingFunction(sourceType, returnType);
							if (castingFunction) {
								castingFunction->pushParam(unit);
							}
							newCandidate.reset(castingFunction);
						}
						else {
							newCandidate = unit;
						}
						if (newCandidate) {
							// keep origin source char in new expression unit
							newCandidate->setSourceCharIndex(function->getSourceCharIndex());
						}
					}
					if (!newCandidate) {
						continue;
					}
				}
				else if (param1TypeN.isSemiRefType()) {
					auto param1TypeNOriginType = param1TypeN.deSemiRef();

					auto param1TypeNOriginSize = scriptCompiler->getTypeSize(param1TypeNOriginType.iType());
					auto derRefUnit = make_shared<FixParamFunction<1>>(DEREF_OPERATOR, EXP_UNIT_ID_DEREF, FUNCTION_PRIORITY_UNARY_PREFIX, param1TypeNOriginType);
					derRefUnit->pushParam(unit);
					derRefUnit->setNative(make_shared<DeRefCommand>(param1TypeNOriginSize));

					newCandidate = derRefUnit;

					if (param1TypeNOriginType != returnType) {
						auto castingFunction = scriptCompiler->findCastingFunction(param1TypeNOriginType, returnType);
						if (castingFunction) {
							castingFunction->pushParam(derRefUnit);
						}

						newCandidate.reset(castingFunction);
					}
					// keep origin source char in new expression unit
					newCandidate->setSourceCharIndex(function->getSourceCharIndex());
					if (!newCandidate) {
						continue;
					}
				}
				else {
					auto castingFunction = scriptCompiler->findCastingFunction(param1TypeN, returnType);
					if (castingFunction) {
						castingFunction->pushParam(unit);
						// keep origin source char in new expression unit
						castingFunction->setSourceCharIndex(function->getSourceCharIndex());
					}
					newCandidate.reset(castingFunction);
					if (!newCandidate) {
						continue;
					}
				}
				if (!newCandidate) {
					newCandidate = make_shared<FixParamFunction<1>>(function->getName(), function->getType(), function->getPriority(), returnType);
					// keep origin source char in new expression unit
					newCandidate->setSourceCharIndex(function->getSourceCharIndex());
				}
				else {
					newCandidate->setReturnType(returnType);
				}

				functionCandidates->push_back(newCandidate);
			}

			if (functionCandidates->size() == 0) {
				eResult = EE_TYPE_CONVERSION_ERROR;
				scriptCompiler->setErrorText("type cast operator can not apply to any candidate of unit '" + paramUnit->toString() + "'");
				_lastErrorUnit = function;
				return nullptr;
			}

			return functionCandidates;
		}
		else if (function->getType() == EXP_UNIT_ID_OPERATOR_LOGIC_AND ||
			function->getType() == EXP_UNIT_ID_OPERATOR_LOGIC_OR) {
			if (n != 2) {
				eResult = EE_TOKEN_INVALID;
				scriptCompiler->setErrorText("operator '" + function->getName() + "' must has two parameters");
				_lastErrorUnit = function;
				return nullptr;
			}

			ExecutableUnitRef& pExeUnit1 = function->getChild(0);
			ExecutableUnitRef& pExeUnit2 = function->getChild(1);

			ScriptType typeBool(basicType.TYPE_BOOL, scriptCompiler->getType(basicType.TYPE_BOOL));

			param1Candidates = linkForUnit(scriptCompiler, pExeUnit1, eResult);
			if (param1Candidates && param1Candidates->size() && eResult == EE_SUCCESS) {
				auto param2Candidates = linkForUnit(scriptCompiler, pExeUnit2, eResult);
				if (param2Candidates && param2Candidates->size() && eResult == EE_SUCCESS) {
					functionCandidates = make_shared<CandidateCollection>();
					
					candidatesForParams[0] = param1Candidates;
					candidatesForParams[1] = param2Candidates;

					std::list<std::vector<ExecutableUnitRef>> paramPaths;
					std::list<pair<int, ExecutableUnitRef>> boolOpercandidates;
					ParamCastingInfo castingInfo;
					listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);
					int acurative1;
					int acurative2;
					int acurative;
					for (auto it = paramPaths.begin(); it != paramPaths.end(); it++) {
						auto& param1 = it->at(0);
						auto& param2 = it->at(1);

						auto& paramType1 = param1->getReturnType();
						auto& paramType2 = param2->getReturnType();

						if (paramType1.isRefType()) continue;
						if (paramType2.isRefType()) continue;

						if (!isLogicBuitInTypes(basicType, paramType1.origin())) {
							auto boolOperator = scriptCompiler->findBoolOperatorForType(paramType1.origin(), param1, &acurative1);
							if (!boolOperator) continue;

							param1 = boolOperator;
						}
						else {
							acurative1 = 0;
						}
						if (!isLogicBuitInTypes(basicType, paramType2.origin())) {
							auto boolOperator = scriptCompiler->findBoolOperatorForType(paramType2.origin(), param2, &acurative2);
							if (!boolOperator) continue;

							param2 = boolOperator;
						}
						else {
							acurative2 = 0;
						}
						acurative = acurative1 + acurative2;
						auto functionCandidate = make_shared<FixParamFunction<2>>(function->getName(), function->getType(), function->getPriority(), typeBool);
						functionCandidate->pushParam(param1);
						functionCandidate->pushParam(param2);
						functionCandidate->setMask(function->getMask());
						functionCandidate->setUserData(function->getUserData());
						// keep origin source char in new expression unit
						functionCandidate->setSourceCharIndex(function->getSourceCharIndex());

						if (acurative == 0) {
							functionCandidates->push_back(functionCandidate);
							return functionCandidates;
						}

						boolOpercandidates.push_back(make_pair(acurative, functionCandidate));
					}

					auto it = min_element(boolOpercandidates.begin(), boolOpercandidates.end(), [](auto& elm1, auto& elm2) {
						return elm1.first < elm2.first;
					});

					if (it != boolOpercandidates.end()) {
						functionCandidates->push_back(it->second);
						return functionCandidates;
					}
				}
			}
			scriptCompiler->setErrorText("no overloading of operator '" + function->getName() + "' found with given parameters");
			_lastErrorUnit = function;
			return nullptr;

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
					if (eResult != EE_SUCCESS) {
						return nullptr;
					}

					MaskType mask = (pExeUnit1->getMask() | UMASK_DECLAREINEXPRESSION);

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

					// keep origin source char in new expression unit
					defaultOperatorUnit->setSourceCharIndex(function->getSourceCharIndex());

					function.reset(defaultOperatorUnit);
					defaultOperatorUnit->pushParam(param1Candidates->front());
					defaultOperatorUnit->pushParam(pExeUnit2);

					for (auto it = param1Candidates->begin(); it != param1Candidates->end(); it++) {
						if ( (*it)->getReturnType().isUnkownType()) {
							eResult = EE_TYPE_UNKNOWN;
							scriptCompiler->setErrorText("data type of '" + pExeUnit1->toString() + "' is not set");
							//check later - bellow line was added
							_lastErrorUnit = pExeUnit1;
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
			for (int i = 0, k = 0; i < n && eResult == EE_SUCCESS; i++, k++) {
				//if candidate is already updated
				if (candidatesForParams[k]) {
					continue;
				}
				ExecutableUnitRef& pExeUnit = function->getChild(i);
				if (pExeUnit->getType() == EXP_UNIT_ID_COLLECTION) {
					DynamicParamFunctionRef paramCollection = static_pointer_cast<DynamicParamFunction>(pExeUnit);
					auto& paramsInCollection = paramCollection->getParams();
					candidatesForParams.resize(n + paramsInCollection.size() - 1);

					for (auto it = paramsInCollection.begin(); it != paramsInCollection.end() && eResult == EE_SUCCESS; it++, k++) {
						ExecutableUnitRef& pExeUnitTmp = *it;
						candidatesForParams[k] = linkForUnit(scriptCompiler, pExeUnitTmp, eResult);
					}
					k--;
				}
				else {
					candidatesForParams[k] = linkForUnit(scriptCompiler, pExeUnit, eResult);
				}
			}
			if (eResult != EE_SUCCESS) {
				return nullptr;
			}
			//update param count
			n = (int)candidatesForParams.size();
#pragma region linking for conditional operator
			if (n == 3 && function->getType() == EXP_UNIT_ID_FUNC_CONDITIONAL) {
				choosedFunctionId = scriptCompiler->getConditionalFunction();
				if (choosedFunctionId < 0) {
					eResult = EE_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("internal error: definition of operator ? is missing");
					_lastErrorUnit = function;
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
						eResult = EE_TYPE_CONVERSION_ERROR;
						error = "data type of if clause and else clause is not same";
						continue;
					}
					
					if (conditionDataType.iType() != basicType.TYPE_BOOL) {
						auto castingFunction = scriptCompiler->findCastingFunction(conditionDataType, ScriptType(basicType.TYPE_BOOL, scriptCompiler->getType(basicType.TYPE_BOOL)));
						if (!castingFunction) {
							eResult = EE_TYPE_CONVERSION_ERROR;
							error = "cannot cast condition type to bool";
							continue;
						}
						castingFunction->pushParam(conditionUnit);

						// keep origin source char in new expression unit
						castingFunction->setSourceCharIndex(conditionUnit->getSourceCharIndex());

						conditionUnit =  ExecutableUnitRef(castingFunction);
					}

					Function* conditionOperator = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
					conditionOperator->setReturnType(ifClauseReturnType);
					
					conditionOperator->pushParam(conditionUnit);
					conditionOperator->pushParam(ifUnit);
					conditionOperator->pushParam(elseUnit);

					// keep origin source char in new expression unit
					conditionOperator->setSourceCharIndex(function->getSourceCharIndex());

					functionCandidates->push_back( ExecutableUnitRef(conditionOperator) );
				}
				if (functionCandidates->size() == 0) {
					scriptCompiler->setErrorText(error);
					_lastErrorUnit = function;
				}
				eResult = checkCandidates(scriptCompiler, function, functionCandidates);
				if (eResult != EE_SUCCESS) {
					_lastErrorUnit = function;
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

						// keep origin source char in new expression unit
						refFunction->setSourceCharIndex(makeRefParamUnit->getSourceCharIndex());

						functionCandidates->push_back(ExecutableUnitRef(refFunction));
					}
				}
				if (functionCandidates->size() == 0) {
					eResult = EE_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("internal error: definition of operator 'ref' is missing");
					_lastErrorUnit = function;
					return nullptr;
				}
				eResult = checkCandidates(scriptCompiler, function, functionCandidates);
				if (eResult != EE_SUCCESS) {
					_lastErrorUnit = function;
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
				eResult = EE_FUNCTION_NOT_FOUND;
				scriptCompiler->setErrorText("internal error: definition of operator '" DEFAULT_COPY_OPERATOR "' is missing");
				_lastErrorUnit = function;
				return nullptr;
			}

			std::list<std::vector<ExecutableUnitRef>> paramPaths;
			listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

			string error;
			functionCandidates = std::make_shared<CandidateCollection>();
			bool needToCallConstructor = false;

			if (paramPaths.size()) {
				auto pit = paramPaths.begin();
				auto& param1 = pit->at(0);
				auto& param2 = pit->at(1);

				// check if param 1 is a variable and it is declared in expression...
				if (param1->getType() == EXP_UNIT_ID_XOPERAND &&
					(param1->getMask() & UMASK_DECLAREINEXPRESSION) != 0 &&
					((!ISFUNCTION(param2) && param2->getReturnType() == param1->getReturnType()) || (param2->getReturnType() != param1->getReturnType()))
					) {
                    // ...then it is must used constructor instead of default copy constructor if there is one of following cases:
                    // case 1: param1 = param2 if param2 is not a function
                    // case 2: param1 = param2 if param2 is a function but they have different return types
					
					auto newFunction = scriptCompiler->applyConstructor(param1, param2);

					// check if constructor was applied					
					if (newFunction) {
						functionCandidates->push_back(ExecutableUnitRef(newFunction));
						if (currentScope) {
							currentScope->applyConstructorDestructor(param1, newFunction);
						}

						needToCallConstructor = true;
					}
					else {						
						int defaultConstructor = scriptCompiler->getDefaultConstructor(param1->getReturnType().iType());
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
						if (param1Type == param2Type && scriptCompiler->convertToRef(param1)) {
							acceptPath = true;
						}
					}
					else if (param1Type.refLevel() - param2Type.refLevel() == 1) {
						if (param1Type.origin() == param2Type.origin()) {
							acceptPath = true;
						}
						else {
							auto castingFunction = scriptCompiler->findCastingFunction(param2Type, param1Type.deRef());
							if (castingFunction) {
								castingFunction->pushParam(param2);
								castingFunction->setMask(castingFunction->getMask() | UMASK_CASTINGUNITNOTINEXPRESSION);

								// keep origin source char in new expression unit
								castingFunction->setSourceCharIndex(param2->getSourceCharIndex());

								param2.reset(castingFunction);

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

						// keep origin source char in new expression unit
						defaultCopyFunction->setSourceCharIndex(function->getSourceCharIndex());

						auto& oldParam1 = ((Function*)param1.get())->getChild(0);
						// check if param 1 is a variable and
						// its was declared inside the expression...
						if (oldParam1->getType() == EXP_UNIT_ID_XOPERAND &&
							(oldParam1->getMask() & UMASK_DECLAREINEXPRESSION)) {
							// ...then generate destructor for it
							
							auto xOperand = (CXOperand*)oldParam1.get();				
							if (currentScope->checkVariableToRunDestructor(xOperand)) {
								// set construct build info to allow frame work set the object to be constructed
								// when the command is run
								currentScope->applyConstructBuildInfo(defaultCopyFunction);
								defaultCopyFunction->setMask(defaultCopyFunction->getMask() | UMASK_CONSTRUCT_FACTOR);
								// must generate next id for constructed object in current scope
								currentScope->generateNextConstructId();
							}
						}

						// check if param 2 is a function...
						if (ISFUNCTION(param2)) {
							// ...then exclude it from destructor
							// because the whole data of object return by a function will be copy directly to
							// the new object
							param2->setMask(param2->getMask() | UMASK_EXCLUDEFROMDESTRUCTOR);
						}

						functionCandidates->push_back(ExecutableUnitRef(defaultCopyFunction));
					}
				}
			}
			// check if not any operation was used to initialize the variable...
			if (functionCandidates->size() == 0) {
				// ...then we try to using assigment operator
				// find assgiment operator for params
				functionCandidates = filterCandidate(scriptCompiler, "=", EXP_UNIT_ID_OPERATOR_ASSIGNMENT, candidatesForParams, eResult);
				if (functionCandidates && functionCandidates->size() != 0) {
					//check if param 1 of the function is a X Operand
					//if param 1 is an X Operand, all candidates of its is also and X operand
					//so we just need to take the first one to check
					auto& param1 = candidatesForParams.at(0)->front();
					if (needToCallConstructor && currentScope) {
						//because param 1 is a X Operand/
						//we should call its constructor if exist first, then call assigment
						auto xOperand = (CXOperand*)param1.get();

						// this will check and add constructor and destructor for the variable
						if (currentScope->checkVariableToRunConstructor(xOperand)) {
							//add constructror to scope success, so we need to remove all other candidates
							auto itAfterBegin = functionCandidates->begin();
							itAfterBegin++;
							functionCandidates->erase(itAfterBegin, functionCandidates->end());
						}
					}
				}
			}

			// check if need call constructor but there is not candidate was create...
			if (needToCallConstructor && (!functionCandidates || functionCandidates->size() == 0)) {
				// ...then its must be and error case
				eResult = EE_FUNCTION_NOT_FOUND;
				scriptCompiler->setErrorText("there is no copy constructor or a combination of default copy constructor and assigment operator found");
				_lastErrorUnit = function;
				return nullptr;
			}

			candidateIsCompleted = true;
		}
#pragma endregion

		if (eResult != EE_SUCCESS) {
			//LOG_I("end update expression tree for " + POINTER2STRING(function.get()));
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
						eResult = EE_TYPE_UNKNOWN;
						scriptCompiler->setErrorText("function type is not specified for unit'" + functionPointerUnit->toString() + "'");
						_lastErrorUnit = functionPointerUnit;
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

						// keep origin source char in new expression unit
						forwardCallUnit->setSourceCharIndex(function->getSourceCharIndex());

						paramPathCandidate.push_back( std::make_pair(forwardCallUnit, res.second));
					}
				}

				if (paramPathCandidate.size() == 0) {
					scriptCompiler->setErrorText("Cannot convert from parameter types to argument types");
					eResult = EE_TYPE_CONVERSION_ERROR;
					_lastErrorUnit = function;
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
								eResult = EE_TOKEN_UNEXPECTED;
								scriptCompiler->setErrorText(memberName + " is not member of " + struct1->getName());
								_lastErrorUnit = pExeUnit2;
								return nullptr;
							}

							//change param 2 from string type to int type
							auto newExpUnit2 = new CConstOperand<MemberInfo>(memberInfo, scriptCompiler->getType(basicType.TYPE_ELEMENT_INFO));
							// keep origin source char in new expression unit
							newExpUnit2->setSourceCharIndex(pExeUnit2->getSourceCharIndex());
							pExeUnit2.reset(newExpUnit2);

							pExeUnit2->setReturnType(ScriptType(basicType.TYPE_ELEMENT_INFO, scriptCompiler->getType(basicType.TYPE_ELEMENT_INFO)));
							function->setReturnType(memberInfo.type);
						}
						else if (struct1 && dataType2.iType() == basicType.TYPE_ELEMENT_INFO && pExeUnit2->getType() == EXP_UNIT_ID_CONST) {
							;//do nothing
						}
						else {
							eResult = EE_TOKEN_UNEXPECTED;
							scriptCompiler->setErrorText("Wrong syntax of using operator '.'");
							_lastErrorUnit = function;
							return nullptr;
						}
					}
					//the code bellow is going to check assigment for structs
					else if (pExeUnit1->getType() == EXP_UNIT_ID_XOPERAND) {
						if (function->getType() == EXP_UNIT_ID_OPERATOR_ASSIGNMENT/* ||
							function->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR*/) {
							struct1 = scriptCompiler->getStruct(dataType1.iType());
							int variantArrayType = basicType.TYPE_VARIANTARRAY;
							if (variantArrayType == dataType2.iType() || variantArrayType == dataType2.origin()) {
								if (struct1) {

									//check variant array if valid or not in use to assign to a struct
									if (!checkAssigmentOperatorForStruct(scriptCompiler, struct1, dynamic_pointer_cast<DynamicParamFunction>(pExeUnit2))) {
										eResult = EE_TYPE_CONVERSION_ERROR;
										scriptCompiler->setErrorText("different type and different number of element for struct assigment does not allow");
										_lastErrorUnit = pExeUnit2;
										return nullptr;
									}
								}
							}
						}
						else if(function->getType() == EXP_UNIT_ID_DEFAULT_COPY_CONTRUCTOR) {
							int variantArrayType = basicType.TYPE_VARIANTARRAY;
							if (variantArrayType == dataType2.iType() || variantArrayType == dataType2.origin()) {							
								bool hasNoError;
								auto functionRef = scriptCompiler->applyConstructorForCompisiteType(pExeUnit1, pExeUnit2, hasNoError);
								if (!hasNoError) {
									eResult = EE_TYPE_CONVERSION_ERROR;
									scriptCompiler->setErrorText("different type and different number of element for struct assigment does not allow");
									_lastErrorUnit = pExeUnit2;
									return nullptr;
								}
								if (functionRef) {
									functionCandidates->push_back(functionRef);
									
									if (currentScope) {
										functionRef->setMask(functionRef->getMask() | UMASK_CONSTRUCT_FACTOR);
										currentScope->applyConstructBuildInfo(functionRef.get());
										currentScope->applyDestructor(pExeUnit1);
										currentScope->generateNextConstructId();
									}
								}
							}
						}
					}
#pragma endregion
				}
				if (functionCandidates && functionCandidates->size()) {
					return functionCandidates;
				}
			}

			functionCandidates = filterCandidate(scriptCompiler, function->getName(), function->getType(), candidatesForParams, eResult);
			if (eResult == EE_SUCCESS && functionCandidates != nullptr && functionCandidates->size()) {
				return functionCandidates;
			}
			
			choosedFunctionId = scriptCompiler->findDynamicFunctionOnly(function->getName());
			if (choosedFunctionId > 0) {
#pragma region linking for dynamic function
				Function* dynamicFunction = (Function*)scriptCompiler->createFunctionFromId(choosedFunctionId);
				// keep origin source char in new expression unit
				dynamicFunction->setSourceCharIndex(function->getSourceCharIndex());

				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

				if(paramPaths.size() != 0) {
					//check later - bellow code was commented
					/*
					if (paramPaths.size() != 1) {
						eResult = EE_TYPE_AMBIOUS_CALL;
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
				eResult = EE_SUCCESS;
#pragma endregion
			}
			else {
#pragma region linking for default operators
				//we cannot find a registered coressponding function here
				//now it is time to check some default operators
				functionCandidates = findApproxiateDefaultOperator(scriptCompiler, function, candidatesForParams);
				if (functionCandidates == nullptr || !functionCandidates->size()) {
					eResult = EE_FUNCTION_NOT_FOUND;

					std::list<std::vector<ExecutableUnitRef>> paramPaths;
					listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

					for (auto pathIt = paramPaths.begin(); pathIt != paramPaths.begin(); pathIt++) {
						auto& path = *pathIt;
						int n = (int)path.size();
						std::vector<ScriptType> paramTypes(n);
						for (int i = 0; i < n; i++) {
							paramTypes[i] = path[i]->getReturnType();
						}

						auto functionSignature = buildFunctionSign(function->getName(), paramTypes);
						LOG_COMPILE_MESSAGE(scriptCompiler->getLogger(), MESSAGE_INFO, scriptCompiler->formatMessage("function '%s' is not found", functionSignature.c_str()));
					}
					scriptCompiler->setErrorText("no overloadding found for function '" + function->getName() + "'");
					_lastErrorUnit = function;
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
					eResult = EE_FUNCTION_NOT_FOUND;
					scriptCompiler->setErrorText("library error: information of '" + function->getName() + "' is missing");
					_lastErrorUnit = function;
					return nullptr;
				}
				if (scriptCompiler->findDynamicFunctionOnly(*functionInfo->itemName) == functionId) {
					dynamicFunctionCandidates.push_back(functionId);
				}
				else {
					candidatesInfo.push_back(*functionInfo);
				}
			}

			functionCandidates = scriptCompiler->filterCandidate(function->getName(), function->getType(), &candidatesInfo, candidatesForParams, eResult);
			if (functionCandidates.get() == nullptr && dynamicFunctionCandidates.size()) {
				functionCandidates = std::make_shared<CandidateCollection>();
			}
#pragma region linking for dynamic function
			if (dynamicFunctionCandidates.size()) {
				std::list<std::vector<ExecutableUnitRef>> paramPaths;
				listPaths<ExecutableUnitRef, CandidateCollection, ExecutableUnitRef>(candidatesForParams, paramPaths);

				for (auto functionid : dynamicFunctionCandidates) {
					Function* dynamicFunction = (Function*)scriptCompiler->createFunctionFromId(functionid);
					// keep origin source char in new expression unit
					dynamicFunction->setSourceCharIndex(function->getSourceCharIndex());

					auto& paramPath = paramPaths.front();
					for (auto pit = paramPath.begin(); pit != paramPath.end(); pit++) {
						dynamicFunction->pushParam(*pit);
					}					
					functionCandidates->push_back(ExecutableUnitRef(dynamicFunction));
				}
			}
#pragma endregion
		}
		eResult = checkCandidates(scriptCompiler, function, functionCandidates);
		if (eResult != EE_SUCCESS) {
			_lastErrorUnit = function;
			return nullptr;
		}
		scriptCompiler->setErrorText("");
		return functionCandidates;
	}

	EExpressionResult ExpressionParser::link(Expression* pExp) {
		ExecutableUnitRef& root = pExp->getRoot();
		EExpressionResult eResult = EE_SUCCESS;
		if (ISFUNCTION(root)) {
			auto unitFunctionRef = dynamic_pointer_cast<Function>(root);
			auto candidateFounds = completeFunctionTree(getCompiler(), unitFunctionRef, eResult);
			root = unitFunctionRef;
			if (eResult == EE_SUCCESS && candidateFounds && candidateFounds->size()) {
				if (candidateFounds->size() > 1) {
					_scriptCompiler->setErrorText("ambitious call for function '" + root->toString() + "'");
					_lastErrorUnit = root;
					return EE_TYPE_AMBIOUS_CALL;
				}
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
			eResult = root->getReturnType().isUnkownType() ? EE_TYPE_UNKNOWN : EE_SUCCESS;
		}
		return eResult;
	}

	EExpressionResult ExpressionParser::link(Expression* pExp, CandidateCollectionRef& candidates) {
		ExecutableUnitRef& root = pExp->getRoot();
		EExpressionResult eResult = EE_SUCCESS;
		_lastErrorUnit.reset();

		candidates = linkForUnit(getCompiler(), root, eResult);
		if (eResult == EE_SUCCESS) {
			if (candidates->size() > 1) {
				_scriptCompiler->setErrorText("ambitious call for function '" + root->toString() + "'");
				_lastErrorUnit = root;
				return EE_TYPE_AMBIOUS_CALL;
			}
		}
		return eResult;
	}
}
