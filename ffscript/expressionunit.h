/******************************************************************
* File:        expressionunit.h
* Description: Declare expression unit classes. Each class is a
*              expression unit kind and play a role in compiling 
*              progress.
* Author:      Vincent Pham
*
* Copyright (c) 2018 VincentPT.
** Distributed under the MIT License (http://opensource.org/licenses/MIT)
**
*
**********************************************************************/

#ifndef _EXPRESSION_UNIT_H_
#define _EXPRESSION_UNIT_H_

#include "ffscript.h"
#include "expresion_defs.h"
#include "unitType.h"
#include "Variable.h"
#include "function/DynamicFunction2.h"
#include "CommandUnitBuilder.h"
#include "ScriptType.h"

#include <string>
#include <stack>
#include <list>
#include <vector>
#include <memory>
#include "StructClass.h"

namespace ffscript {	

	using namespace std;
	static const std::string AUTO_VARIABLE_TYPE = "auto data type";

	template <class T>
	inline std::string toString(const T& val) {
		return std::to_string(val);
	}
	template <>
	inline std::string toString(const MemberInfo& val) {
		return val.type.sType() + ":" + std::to_string(val.offset);
	}
	template <>
	inline std::string toString<std::string>(const std::string& val) {
		return val;
	}
	template <>
	inline std::string toString<std::wstring>(const std::wstring& val) {
		return std::string((char*)val.c_str(), val.size() * 2);
	}
	template <>
	inline std::string toString<RuntimeFunctionInfo>(const RuntimeFunctionInfo& val) {
		return std::to_string((size_t)val.address);
	}

	class FFSCRIPT_API ExpUnit : public CommandUnitBuilder
	{
	private:
	protected:
		short _indexInExpression;
		unsigned short _sourceCharIndex;
	public:
		ExpUnit();
		virtual ~ExpUnit();
		void setIndex(int);
		int getIndex() const;
		void setSourceCharIndex(int);
		int getSourceCharIndex() const;
	};

	class FFSCRIPT_API OpenRoundBracket : public ExpUnit
	{
	public:
		virtual ~OpenRoundBracket();
		OpenRoundBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	class FFSCRIPT_API ClosedRoundBracket : public ExpUnit
	{
	private:
	protected:
	public:
		virtual ~ClosedRoundBracket();
		ClosedRoundBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	class FFSCRIPT_API OpenCurlyBracket : public ExpUnit
	{
	public:
		virtual ~OpenCurlyBracket();
		OpenCurlyBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	class FFSCRIPT_API ClosedCurlyBracket : public ExpUnit
	{
	private:
	protected:
	public:
		virtual ~ClosedCurlyBracket();
		ClosedCurlyBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	/************************************************************************/
	/* Operator ','                                                         */
	/************************************************************************/
	class FFSCRIPT_API ParamSeperator : public ExpUnit
	{
	private:
	protected:
	public:
		virtual ~ParamSeperator();
		ParamSeperator();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};
	typedef shared_ptr<ExpUnit> ExpUnitRef;

	class FFSCRIPT_API OpenSquareBracket : public ExpUnit
	{
	public:
		virtual ~OpenSquareBracket();
		OpenSquareBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	class FFSCRIPT_API ClosedSquareBracket : public ExpUnit
	{
	private:
	protected:
	public:
		virtual ~ClosedSquareBracket();
		ClosedSquareBracket();
		const std::string& toString() const;
		UNIT_TYPE getType() const;
		static const std::string& getFuncName();
	};

	class FFSCRIPT_API ExecutableUnit : public ExpUnit
	{
	private:
	protected:
		ScriptType _returnType;
	public:
		ExecutableUnit(const std::string& returnType);
		ExecutableUnit(const std::string& returnType, const int returnTypeInt);
		ExecutableUnit(const ScriptType& returnType);

		virtual ~ExecutableUnit();
		virtual void* Execute() = 0;
		virtual const ScriptType& getReturnType() const;
		virtual ScriptType& getReturnType();
		virtual void setReturnType(const ScriptType&);
	};

	typedef shared_ptr<ExecutableUnit> ExecutableUnitRef;

	class FFSCRIPT_API ConstOperandBase : public ExecutableUnit
	{
	public:
		std::string _valueInString;
	public:
		ConstOperandBase(const std::string& valueType);
		ConstOperandBase(const ScriptType& valueType);
		virtual ~ConstOperandBase();
		virtual const std::string& toString()const;
		virtual UNIT_TYPE getType() const;
		virtual int getDataSize() const = 0;
	};

	template <class T>
	class FFSCRIPT_API CConstOperand : public ConstOperandBase
	{
	public:
		T _value;
	public:
		CConstOperand(const T& constValue, const std::string& valueType) :
			_value(constValue),
			ConstOperandBase(valueType)
		{
			_valueInString = ffscript::toString(_value);
		}
		CConstOperand(const T& constValue, const ScriptType& valueType) :
			_value(constValue),
			ConstOperandBase(valueType)
		{
			_valueInString = ffscript::toString(_value);
		}
		CConstOperand(const T& constValue, const std::string& valueType, const char* constValueInString) :
			_value(constValue),
			ConstOperandBase(valueType)
		{
			_valueInString = constValueInString;
		}
		virtual ~CConstOperand(){}
		virtual void* Execute() {
			return &_value;
		}		

		int getDataSize() const {
			return sizeof(_value);
		}
	};

	class FFSCRIPT_API IncompletedUserFunctionUnit : public ConstOperandBase
	{
	public:
		IncompletedUserFunctionUnit(const std::string& functionName);
		virtual ~IncompletedUserFunctionUnit();		
		UNIT_TYPE getType() const;
		int getDataSize() const;
		void* Execute();
	};

	class ScriptScope;

	class FFSCRIPT_API CXOperand : public ExecutableUnit
	{
	private:
	public:
		Variable* _variable;
		ScriptScope* _currentScope;
	public:
		CXOperand(ScriptScope* scope, Variable* variable);
		CXOperand(ScriptScope* scope, Variable* variable, const std::string& valueType);
		CXOperand(ScriptScope* scope, Variable* variable, const ScriptType& valueType);
		virtual ~CXOperand();
		virtual const std::string& toString()const;
		virtual void* Execute();
		virtual UNIT_TYPE getType()const;
		virtual ScriptScope* getScope() const;
		virtual Variable* getVariable() const;
		virtual void setVariable(Variable* pVariable);
		virtual ExecutableUnit* clone();
	};

	class Function : public ExecutableUnit
	{
	protected:
		void* storedData;
		const std::string _name;
		const int _priority;
		const UNIT_TYPE _functionType;
		int _functionId;
		int _paramSize;		
	public:
		Function(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType);
		Function(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType);
		virtual ~Function();

		virtual int pushParam(ExecutableUnitRef pExeUnit) = 0;
		virtual ExecutableUnitRef popParam() = 0;
		virtual const ExecutableUnitRef& getChild(int index) const = 0;
		virtual ExecutableUnitRef& getChild(int index) = 0;
		virtual int getChildCount() = 0;

		virtual int getPriority();
		virtual const std::string& toString()const;
		virtual const std::string& getName()const;
		virtual bool IsOrdered(Function* pFunc);
		virtual void* Execute();
		virtual UNIT_TYPE getType()const;		
		virtual int getId()const;
		virtual void setId(int functionId);		
		int getParamsDataSize() const;
		void setParamsDataSize(int paramSize);
	};

	class NativeFunction : public Function
	{
	protected:
		DFunction2Ref _nativeFunction;
	public:
		NativeFunction(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType);
		NativeFunction(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType);
		virtual ~NativeFunction();
		virtual const DFunction2Ref& getNative()const;
		virtual void setNative(const DFunction2Ref& nativeFunction);
	};
	typedef shared_ptr<Function> FunctionRef;

	template <int ParamSize>
	class FixParamFunction : public NativeFunction
	{
	private:
	public:
		ExecutableUnitRef params[ParamSize];
		int _paramCount;
	public:

		FixParamFunction(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType) :
			NativeFunction(name, functionType, iPriority, returnType) {
			_paramCount = 0;			
		}

		FixParamFunction(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType) :
			NativeFunction(name, functionType, iPriority, returnType) {
			_paramCount = 0;
		}

		virtual ~FixParamFunction() {}

		virtual int pushParam(ExecutableUnitRef pExeUnit) {
			if (_paramCount < ParamSize) {
				params[_paramCount++] = pExeUnit;
			}
			return ParamSize - _paramCount;
		}
		virtual ExecutableUnitRef popParam() {
			if (_paramCount > 0) {
				return params[--_paramCount];
			}
			return nullptr;
		}

		virtual const ExecutableUnitRef& getChild(int index) const {
			return params[index];
		}

		virtual ExecutableUnitRef& getChild(int index) {
			return params[index];
		}

		virtual int getChildCount() {
			return _paramCount;
		}
	};

	class DynamicParamFunction : public NativeFunction
	{
	private:
	public:
		std::list<ExecutableUnitRef> _params;
		int _maxParam;
		std::list<ExecutableUnitRef>::iterator _currentPos;
	public:
		DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, const std::string& returnType);
		DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, const ScriptType& returnType);
		DynamicParamFunction(const std::string& name, unsigned int functionType, int iPriority, int maxParam);
		virtual ~DynamicParamFunction();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual int pushParamFront(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParamFront();
		virtual int getChildCount();
		virtual int getMaxParam() const;
		virtual void setMaxParam(int maxParam);

		virtual ExecutableUnitRef& getFirstParam();
		virtual ExecutableUnitRef& getNextParam();
		std::list<ExecutableUnitRef>& getParams();
		const std::list<ExecutableUnitRef>& getParams() const;

	private:
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
	};

	typedef shared_ptr<DynamicParamFunction> DynamicParamFunctionRef;

	class ParamUnitCollection : public DynamicParamFunction
	{
	public:
		virtual ~ParamUnitCollection();
		ParamUnitCollection();
	};

}

#endif //_EXPRESSION_UNIT_H_