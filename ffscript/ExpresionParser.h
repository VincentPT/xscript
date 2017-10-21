#pragma once
#include "ffscript.h"
#include "expresion_defs.h"
#include "expressionunit.h"
#include <list>
#include <vector>
#include <string>
#include "Expression.h"
#include "ScriptType.h"

namespace ffscript {

	using namespace std;
	class ScriptCompiler;
	class FunctionFactory;

	typedef stack<ExecutableUnitRef> OutputStack;
	typedef stack<DynamicParamFunctionRef> OperatorStack;
	typedef std::pair<OutputStack*, OperatorStack*> ExpressionEntry;
	typedef list<ExpressionEntry> ExpressionInputList;
	typedef list<ExecutableUnitRef> CandidateCollection;
	typedef std::shared_ptr<CandidateCollection> CandidateCollectionRef;

	struct OverLoadingItem {
		int functionId;
		std::vector<ScriptTypeRef> paramTypes;
		const std::string* itemName; //optional
		int mask;
	};

#define ITEM_MASK_DYNAMIC_FUNCTION 1

	struct ParamCastingInfo
	{
		FunctionRef castingFunction;
		int accurative;
	};

	typedef vector<ParamCastingInfo> ParamCastingList;
	struct CandidateInfo
	{
		const OverLoadingItem* item;
		ParamCastingList paramCasting;
		int totalAccurative;
	};

	bool findMatchingLevel1(
		ScriptCompiler* scriptCompiler,
		const ScriptType& refVoidType,
		const ScriptType& argumentType, const ScriptType& paramType,
		ParamCastingInfo& paramInfo);

	bool findMatchingLevel2(
		ScriptCompiler* scriptCompiler,
		const ScriptType& argumentType, const ScriptType& paramType,
		ParamCastingInfo& paramInfo);

	void applyCasting(ExecutableUnitRef& unit, const FunctionRef& castingUnit);
	
	class FFSCRIPT_API ExpressionParser
	{
		ScriptCompiler* _scriptCompiler;
	public:
		ExpressionParser(ScriptCompiler* scriptCompiler);
		virtual ~ExpressionParser();
		bool precheck(const list<ExpUnitRef>& expUnitList);
		ScriptCompiler* getCompiler() const;
	public:
		//EExpressionResult stringToExpList(const WCHAR* sExpressionString, std::list<ExpUnitRef>& expList);
		const WCHAR* readExpression(const WCHAR* begin, const WCHAR* end, EExpressionResult& eResult, std::list<ExpUnitRef>& expList);
		const WCHAR* readLambdaExression(const WCHAR* begin, const WCHAR* end, EExpressionResult& eResult, FunctionRef& lambdaExpression);
		EExpressionResult stringToExpList(const std::wstring& sExpressionString, std::list<ExpUnitRef>& expList);
		EExpressionResult stringToExpList(const WCHAR* sExpressionString, const WCHAR* end, std::list<ExpUnitRef>& expList);
		static void numberedForUnit(std::list<ExpUnitRef>& expList);
		bool compile(list<ExpUnitRef>& expUnitList, list<ExpressionRef> &expList, EExpressionResult* peResult = nullptr);
		EExpressionResult link(Expression* pExp);
		EExpressionResult link(Expression* pExp, CandidateCollectionRef& candidates);
		static CandidateCollectionRef linkForUnit(ScriptCompiler* scriptCompiler, ExecutableUnitRef& unit, EExpressionResult& eResult);
		static CandidateCollectionRef completeFunctionTree(ScriptCompiler* scriptCompiler, FunctionRef& function, EExpressionResult& eResult );
		static Function* applyConstructor(ScriptCompiler* compiler, ExecutableUnitRef& variableUnit, ExecutableUnitRef& argUnit, bool& blHasError);
	protected:
		EExpressionResult compileConditionalExpression(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, ExecutableUnitRef& ifClauseUnit, ExecutableUnitRef& elseClauseUnit);
		EExpressionResult putAnExpUnit(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, ExpressionInputList& inputList);
		EExpressionResult makeExpressionList(ExpressionInputList& inputList, list<ExpressionRef>& expList);
		EExpressionResult pickParamUnitsForFunction(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& functionRef);
		EExpressionResult pickParamUnitsElseClause(list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end, const DynamicParamFunctionRef& elseClauseUnitRef);
		EExpressionResult putFunction(OperatorStack* operStack, OutputStack* operandStack, const DynamicParamFunctionRef& functionUnit, list<ExpUnitRef>::const_iterator& it, list<ExpUnitRef>::const_iterator end);
	};

	class FFSCRIPT_API ExpressionReader {
	public:
		ExpressionReader();
		virtual ~ExpressionReader();
		virtual const WCHAR* read(const WCHAR* text, const WCHAR* end, EExpressionResult& eResult, std::list<ExpUnitRef>& expList);
	};
}