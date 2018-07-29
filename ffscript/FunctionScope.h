#pragma once
#include "ContextScope.h"
#include "Variable.h"
#include <vector>
#include "ScriptType.h"

namespace ffscript {
	class program;

	class FunctionScope :
		public ContextScope
	{
	protected:		
		std::string _name;
		ScriptType _returnType;
		int _functionId;
	public:
		FunctionScope(ScriptScope* parent, const std::string& name, const ScriptType& returnType);
		virtual ~FunctionScope();
		int getFunctionId() const;
		const std::string& getName() const;
		virtual bool updateCodeForControllerCommands(Program* program);
		const ScriptType& getReturnType() const;
	public:
		const wchar_t* parseFunctionParameters(const wchar_t* text, const wchar_t* end, std::vector<ScriptType>& paramTypes);
		virtual const wchar_t* parse(const wchar_t* text, const wchar_t* end);
		virtual const wchar_t* parseHeader(const wchar_t* text, const wchar_t* end, std::vector<ScriptType>& paramTypes);
		const wchar_t* parseBody(const wchar_t* text, const wchar_t* end, const ScriptType& returnType, const std::vector<ScriptType>& paramTypes);
		virtual bool extractCode(Program* program);
	protected:
		const wchar_t* parseFunctionParametersInternal(const wchar_t* text, const wchar_t* end, std::vector<ScriptType>& paramTypes, list<Variable*>& registeredVariables);
	};

	class AnonymousFunctionScope : public FunctionScope {
		const std::list<ExecutableUnitRef>* _captureList;
	public:
		AnonymousFunctionScope(ScriptScope* parent, const std::list<ExecutableUnitRef>& captureList);
		virtual ~AnonymousFunctionScope(); 
		const wchar_t* parse(const wchar_t* text, const wchar_t* end);
	};
}