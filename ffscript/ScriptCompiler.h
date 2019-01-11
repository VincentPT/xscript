/******************************************************************
* File:        ScriptCompiler.h
* Description: declare ScriptCompiler class. A class play a role in
*              script compiling progress.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#pragma once
#include "ffscript.h"
#include "expressionunit.h"
#include "FactoryTree.h"
#include "MemoryBlock.h"
#include "FuncLibrary.h"
#include "StructClass.h"
#include "TypeManager.h"
#include "BasicFunctionFactory.hpp"
#include "Template.h"
#include "function/DynamicFunction.h"

#include <stack>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <memory>

#define CONDITIONAL_FUNCTION "_SYSTEM_FUNCTION_CONDITIONAL"
#define LOG_COMPILE_MESSAGE(logger, type, message) if(logger) logger->log(type, message)

namespace ffscript {
	class ScriptScope;
	class FactoryTree;
	class FunctionFactory;
	class Program;
	class ScriptType;

	using namespace std;

	enum EKeyword {
		KEYWORD_IF,
		KEYWORD_ELSE,
		KEYWORD_WHILE,
		KEYWORD_FOR,
		KEYWORD_RETURN,
		KEYWORD_BREAK,
		KEYWORD_CONTINUE,
		KEYWORD_UNKNOWN,
	};

	enum MessageType
	{
		MESSAGE_INFO,
		MESSAGE_WARNING,
		MESSAGE_ERROR
	};

	struct OperatorEntry {
		string name;
		const char* nameInExpression;
		UNIT_TYPE operatorType;
		int priority;
		int maxParam;
	};
	
	struct CandidatePathInfo {
		CandidateInfo candidate;
		std::vector<ExecutableUnitRef>* paramPath;
	};

	class CompilationLogger {
	public:
		virtual void log(MessageType type, const wchar_t* message) = 0;
	};
	
	class ScriptCompiler
	{
		typedef map<uint64_t, int> TypeCompatibilityMap;
		typedef shared_ptr<FunctionFactory> FunctionFactoryRef;

		typedef std::map<std::string, int> BinaryFunctionParamMap;
		typedef std::shared_ptr<BinaryFunctionParamMap> BinaryFunctionParamMapRef;
		typedef vector<int> ConstructorIDList;
		typedef std::shared_ptr<ConstructorIDList> ConstructorIDListRef;

		FuncLibraryRef _functionLibRef;
		TypeManagerRef _typeManagerRef;

		stack<ScriptScope*> _scopeStack;
		vector<FunctionFactory*> _functionFactories;
		map<string, EKeyword > _keywordMap;
		list<FunctionFactoryRef> _factoriesStorage;
		TypeCompatibilityMap _typeConversionMap;
		typedef std::map<string, OperatorEntry*>  OperatorMap;
		OperatorMap _preCompileOperators; /*map operator name to operator information, pre-defined operator is only allow type overloading, not param count overloading*/
		map<int, int> _constructorMap;
		map<int, int> _destructorMap;
		map<int, BinaryFunctionParamMapRef> _copyConstructorMap;
		map<int, ConstructorIDListRef> _constructorsMap; // map a data type to its constructor list
		map<string, TemplateRef> _templates;
		map<string, DFunctionRef> _constantMap;
		map<int, int> _functionCallMap;

		Program* _program;
		CompilationLogger* _logger;

		struct LibraryMarkInfo {
			int functionIdx;			
		};
		typedef std::shared_ptr<LibraryMarkInfo> LibraryMarkInfoRef;
		LibraryMarkInfoRef _systemLibMarkEnd;

		std::string _lastError;
		std::vector<wchar_t> _messageBuffer;

		int _refFunctionId = -1;
		int _functionInfoConstructorId = -1;
		int _functionInfoDestructorId = -1;
		int _functionInfoCopyConstructorId = -1;
		
	public:
		ScriptCompiler();
		virtual ~ScriptCompiler();

		void setErrorText(const std::string& errorMsg);
		const std::string& getLastError() const;
		void setLogger(CompilationLogger*);
		CompilationLogger* getLogger() const;

		void pushScope(ScriptScope* scope);
		ScriptScope* popScope();
		ScriptScope* currentScope() const;

		int registType(const std::string&, int mask = 0);
		int getTypeSize(int) const;
		int getTypeSizeInStack(int) const;
		int getTypeSize(const ScriptType&) const;
		void setTypeSize(int typeId, int size);
		std::string getType(int) const;
		int getType(const std::string&) const;
		bool isRefSign(const std::string&) const;
		int getMakingRefFunction();
		int getConditionalFunction();		

		void setRuntimeInfoConstructor(int functionId);
		int getRuntimeInfoConstructor() const;
		void setRuntimeInfoDestructor(int functionId);
		int getRuntimeInfoDestructor() const;
		void setRuntimeInfoCopyConstructor(int functionId);
		int getRuntimeInfoCopyConstructor() const;		
		
		int registDefaultCopyFunction(FunctionFactory* factory);
		int getDefaultCopyFunction();

		int registStruct(StructClass* pStruct);
		const StructClass* getStruct(int type);

		ExpUnit* createExpUnitFromName(const std::string&);
		Function* createFunctionFromId(int functionId);
		int registFunction(const std::string& name, const std::string& sargs, FunctionFactory* factory);
		int registFunction(const std::string& name, const std::vector<ScriptType>& args, FunctionFactory* factory);
		int registConditionalFunction(FunctionFactory* factory);
		void unregisterFunction(int functionId);
		int registDynamicFunction(const std::string& name, FunctionFactory* factory);
		const list<OverLoadingItem>* findOverloadingFuncRoot(const std::string& name);
		int findFunction(const std::string& name, const std::string& sargs);
		int findDynamicFunctionOnly(const std::string& name);
		int findFunction(const std::string& name, const std::vector<ScriptType>& paramTypes);
		FunctionFactory* getFunctionFactory(int functionId) const;
		 
		bool registerTypeConversionAccurative(int sourceType, int targetType, int accurative);
		int findConversionAccurative(int sourceType, int targetType);

		EKeyword findKeyword(const std::string& keyword) const;
		const OperatorEntry* findPredefinedOperator(const std::string& keyword) const;

		TemplateRef registTemplate(const std::string& name, const vector<std::string>& args);
		TemplateRef findTemplate(const std::string& name, int argCount);
		bool registTypeInfo(int type, MemoryBlockRef typeInfo);
		void* getTypeInfo(int type);

		bool registConstructor(int type, int functionId);
		int getDefaultConstructor(int type);
		std::shared_ptr<list<CandidateInfo>> getConstructor(const ScriptType& rootType, const ExecutableUnitRef& paramUnit);
		int getBinaryConstructor(int rootType, const ScriptType& paramType);
		std::shared_ptr<list<CandidateInfo>> getBinaryConstructor(const ScriptType& rootType, const ExecutableUnitRef& paramUnit, bool findExtracly = true);
		void getConstructors(int iType, list<OverLoadingItem*>& overloadingItems) const;
		bool registDestructor(int type, int functionId);
		int getDestructor(int type);

		bool findMatchingLevel1(const ScriptType& refVoidType, const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo);
		bool findMatchingLevel2(const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo);
		bool findMatchingConstructor(const ExecutableUnitRef& objectParam, const ExecutableUnitRef& secondParam, ParamCastingInfo& paramInfo);
		Function* findCastingFunction(const ScriptType& sourceType, const ScriptType& targetType);
		///
		/// return 0 if not find matching, 1 if found matching by using findMatchingLevel1
		/// 2 if found matching by using findMatchingLevel2
		///
		int findMatching(const ScriptType& refVoidType, const ScriptType& argumentType, const ScriptType& paramType, ParamCastingInfo& paramInfo, bool tryFindingLevel2);
		std::shared_ptr<list<CandidateInfo>> selectMultiCandidates(const list<OverLoadingItem*>& overloadingItems, const std::list<ExecutableUnitRef>& params);
		FunctionRef applyParamToCandidate(const CandidateInfo& item, std::list<ExecutableUnitRef>& params);
		static CandidateInfo* selectSingleCandidate(const std::shared_ptr<list<CandidateInfo>>& candidates);
		ExecutableUnitRef findBoolOperatorForType(int type, const ExecutableUnitRef& paramUnit, int* pAccurative = nullptr);
		Function* applyConstructor(const ExecutableUnitRef& variableUnit, const  ExecutableUnitRef& argUnit, int* pAccurative = nullptr);
		bool hasConstructor(int iType) const;
		bool convertToRef(ExecutableUnitRef& param);
		static void applyCasting(ExecutableUnitRef& unit, const FunctionRef& castingUnit);

		// return
		bool breakCompositeConstructor(const ExecutableUnitRef& variableUnit,
			const ExecutableUnitRef& secondOperand, list<pair<Variable*,
			ExecutableUnitRef>>& assigments, int& accurative);

		bool breakCompositeAssigment(const ExecutableUnitRef& variableUnit,
			const ExecutableUnitRef& secondOperand, list<pair<Variable*,
			ExecutableUnitRef>>&assigments, int& accurative);

		CandidateCollectionRef filterCandidate(const string& functionName, int functionType,
			const list<OverLoadingItem>* overloadingFuncs,
			const std::vector<CandidateCollectionRef>& candidatesForParams, EExpressionResult& eResult);

		FunctionRef applyConstructorForCompisiteType(const ExecutableUnitRef& xOperand, const ExecutableUnitRef& secondOperand, bool& hasNoError);
		FunctionRef applyAssigmentForCompisiteType(const ExecutableUnitRef& xOperand, const ExecutableUnitRef& secondOperand, bool& hasNoError);
		bool findMatchingComposite(const ScriptType& argumentType, const ExecutableUnitRef& unit, ParamCastingInfo& paramInfo);

		bool registFunctionOperator(int type, int functionId);
		int getFunctionOperator(int type);

		Program* bindProgram(Program* program);
		Program* getProgram() const;
		void beginUserLib();
		void clearUserLib();
		void takeOwnership(FunctionFactory* factory);

		void setConstantMap(const string& constantName, const DFunctionRef& createConstantObjFunc);
		DFunctionRef findConstantMap(const string& constantName) const;

		const FuncLibraryRef& getFunctionLib() const;
		const TypeManagerRef& getTypeManager() const;
		//parse functions
		int buildFunctionType(const OverLoadingItem& item);
		const wchar_t* parseFunctionType(const wchar_t* text, const wchar_t* end, ScriptType& returnType, std::list<ScriptType>& argTypes, bool& isDynamicFunction);
		bool parseFunctionType(int type, ScriptType& returnType, std::list<ScriptType>& argType, bool& isDynamicFunction);

		const wchar_t* parseArrayType(const wchar_t* text, const wchar_t* end, ScriptType& elmType, std::vector<int>& dimensions);
		const wchar_t* readType(const wchar_t* text, const wchar_t* end, ScriptType& stype);

		int registFunctionType(const std::string& functionType);
		int registArrayType(const std::wstring& arrayType);
		const wchar_t* formatMessage(const wchar_t* format, ...);
		const wchar_t* formatMessage(const char* format, ...);

	protected:
		int registArrayType(const ScriptType& elmType, const std::vector<int>& dimensions);
		bool registDefaultConstructor(int type, int functionId);
		bool registBinaryConstructor(int type, int functionId);
	public:
		inline static bool isCommandBreakSign(wchar_t c) {
			return c == ';';
		}
		inline static bool isOpenScopeSign(wchar_t c) {
			return c == '{';
		}
		inline static bool isCloseScopeSign(wchar_t c) {
			return c == '}';
		}
	};

	typedef std::shared_ptr<ScriptCompiler> ScriptCompilerRef;

	template<class T>
	class ContantFactory : public DFunction {
	protected:
		std::string _type;
		T _val;
		ConstOperandBase* _retStorage;
	public:
		ContantFactory(const T& val, const std::string& type) : _val(val), _type(type) {
			_ret = &_retStorage;
		}

		virtual void call() {
			_retStorage = new CConstOperand<T>(_val, _type);
		}
		bool pushParam(void* param) { return false; }
		void* popParam() { return nullptr; }
		DFunction* clone() {
			return new ContantFactory(_val, _type);
		}
	};

	template<class T>
	class ContantFactory2 : public DFunction {
		std::string _type;
		T _val;
		ConstOperandBase* _retStorage;
		std::string _valInString;
	public:
		ContantFactory2(const T& val, const std::string& type, const std::string& valInString) : _val(val), _type(type) {
			_ret = &_retStorage;
			_valInString = valInString;
		}
		bool pushParam(void* param) { return false; }
		void* popParam() { return nullptr; }
		void call() {
			_retStorage = new CConstOperand<T>(_val, _type, _valInString);
		}
		DFunction* clone() {
			return new ContantFactory2(_val, _type, _valInString);
		}
	};

	template<class T>
	void setConstantMap(ScriptCompiler* compiler, const std::string& constantName, const std::string& constantType, const T& val) {
		auto constantFactoryRef = std::make_shared<ContantFactory<T>>(val, constantType);
		compiler->setConstantMap(constantName, constantFactoryRef);
	}

	template<class T>
	void setConstantMap(ScriptCompiler* compiler, const std::string& constantName, const std::string& constantType, const T& val, const std::string& valInString) {
		auto constantFactoryRef = std::make_shared<ContantFactory2<T>>(val, constantType, valInString);
		compiler->setConstantMap(constantName, constantFactoryRef);
	}
}

