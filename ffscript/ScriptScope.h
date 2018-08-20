#pragma once
#include <map>
#include <list>
#include <string>
#include <memory>
#include "Variable.h"
#include "function\DynamicFunction2.h"
#include "CommandUnit.h"
#include "Program.h"
#include "ScriptCompiler.h"
#include "ScriptType.h"
#include "ExpresionParser.h"
#include "ObjectBlock.hpp"

namespace ffscript {

	class ScriptScope;
	class Expression;
	class ExpressionParser;
	struct OperatorItem;
	typedef std::shared_ptr<ScriptScope> ScriptScopeRef;
	class CXOperand;

	typedef std::list<CommandUnitRef > ComandRefList;
	typedef ComandRefList::const_iterator CommandConstRefIter;
	typedef ComandRefList::iterator CommandRefIter;

	typedef std::list<ScriptScopeRef> ScopeRefList;

	class ScriptScope
	{
		typedef std::map<std::string, DFunction2Ref> KeywordProcessingMap;

		std::map<std::string, Variable*> _variableNameMap;
		std::map<CommandUnit*, std::shared_ptr<Variable>> _variableUnitMap;
		KeywordProcessingMap _keywordProcessingMap;
		std::list<Variable> _varibles;
		std::list<MemberVariable> _memberVaribles;
		ComandRefList _destructors;
		ScopeAutoRunList _constructors;
		ScriptScope* _parent;
		ScopeRefList _children;
		int _scopeSize;
		int _dataSize;
		int _scopeBaseOffset;
		int _constructorCount;
		ScriptCompiler* _scriptCompiler;
		ScriptCompilerRef _internalCompiler;
	protected:
		ComandRefList _commandBuilder;
	protected:
		ExecutableUnitRef chooseCandidate(const CandidateCollectionRef& candidates, const ScriptType& expectedReturnType);
		void constructObjectForReturning(ExecutableUnitRef& candidate, const ScriptType& expectedReturnType);
	public:
		ScriptScope(ScriptCompiler* scriptCompiler);
		virtual ~ScriptScope();
		Variable* findVariable(const std::string&);
		Variable* registVariable(const std::string&);
		Variable* registVariable();
		void removeVariable(Variable*);
		Variable* registTempVariable(CommandUnit* parentUnit, int offset);
		Variable* applyTemporaryVariableFor(CommandUnit* parentUnit, Variable* pVariable);
		bool deleteTempVariable(CommandUnit* parentUnit);
		Variable* findTempVariable(CommandUnit* parentUnit);
		CommandUnit* checkVariableToRunConstructor(CXOperand* xOperand);
		Function* generateDefaultAutoOperator(int operatorId, Variable* obj);
		OperatorBuidInfo* applyDefaultConstructor(const ScriptType& type, Function* constructor);
		/*void checkVariableToRunConstructorNonRecursive(Variable* pVariable, Function* constructor);*/
        void applyConstructorDestructor(const ExecutableUnitRef& variableUnit, Function* constructor);
		bool applyDestructor(const ExecutableUnitRef& variableUnit);
		bool checkVariableToRunDestructor(CXOperand* pVariable);
		int getConstructorCommandCount() const;
		void generateNextConstructId();
		OperatorBuidInfo* applyConstructBuildInfo(Function* constructFactor);
		std::shared_ptr<ObjectBlock<OperatorBuidInfo>> generateConstructBuildInfo() const;
		ComandRefList* getDestructorList();
		ScopeAutoRunList* getConstructorList();
		MemberVariable* registMemberVariable(Variable* parent, const std::string&);
		void updateVariableOffset();

		int getScopeSize() const;
		int getDataSize() const;
		int getBaseOffset() const;
		void setBaseOffset(int offset);
		void allocate(int size);
		virtual void addChild(ScriptScope* child);
		ScriptCompiler* getCompiler() const;

	public:
		DFunction2Ref getProcessingFunc(const std::string& keyword);
		bool setProcessingFunc(const std::string& keyword, DFunction2Ref function);
		ScriptScope* getParent() const;
		ScriptScope* getRoot() const;
		const ScopeRefList& getChildren() const;
		void setParent(ScriptScope* parent);
		void clear();

		//parser functions
	private:
		const wchar_t* parseExpressionInternal(const wchar_t* text, const wchar_t* end, std::wstring& expression, const ScriptType* expectedReturnType = nullptr);	
	protected:
		EExpressionResult parseExpressionInternal(ExpressionParser* parser, std::list<ExpUnitRef>& unitList, const ScriptType* expectedReturnType = nullptr);
		EExpressionResult parseExpressionInternal(ExpressionParser* parser, std::list<ExpressionRef>& expList , const ScriptType* expectedReturnType = nullptr);
	public:
		const wchar_t* parseType(const wchar_t* text, const wchar_t* end, ScriptType& type);
		const wchar_t* parseExpression(const wchar_t* text, const wchar_t* end, const ScriptType* expectedReturnType = nullptr);
		const wchar_t* parseDeclaredExpression(const wchar_t* text, const wchar_t* end, const ScriptType* expectedReturnType = nullptr);
		virtual const wchar_t* parse(const wchar_t* text, const wchar_t* end) = 0;
		virtual int correctAndOptimize(Program* program) = 0;
		virtual bool extractCode(Program* program) = 0;

		//void setBeginExpression(CommandConstRefIter expressionIter);
		//CommandConstRefIter getBeginExpression() const;
		//void setEndExpression(CommandConstRefIter expressionIter);
		//CommandConstRefIter getEndExpression() const;

		int getExpressionCount() const;
		void putCommandUnit(CommandUnit* commandUnit);
		void putCommandUnit(const CommandUnitRef& commandUnitRef);
		void insertCommandUnitBefore(CommandConstRefIter commandRefIter, CommandUnit* commandUnit);		
		void remove(CommandConstRefIter commandRefIter);
		CommandConstRefIter getLastExpression() const;
		CommandConstRefIter getFirstExpression() const;
		CommandRefIter getFirstExpression();
		CommandRefIter getLastExpression();
		CommandUnitRef* getLastCommandUnit();

		static int checkAutoOperatorForChildren(ScriptCompiler* scriptCompiler, const std::function<int(int)>& getOperator, const ScriptType& type, int beginOffset, std::list<OperatorBuidItemInfo>* operatorInfoList);

	protected:
		ScriptCompilerRef getInternalCompiler();		
	};
}