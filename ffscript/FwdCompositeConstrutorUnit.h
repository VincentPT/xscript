#pragma once
#include "ExpresionParser.h"


namespace ffscript {
	class FwdConstrutorUnit :
		public Function
	{
	protected:
		FunctionRef _constructorUnit;
		FwdConstrutorUnit(const FunctionRef& constructorUnit, int functionType);
	public:
		FwdConstrutorUnit(const FunctionRef& constructorUnit);
		virtual ~FwdConstrutorUnit();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
		virtual ExecutableUnitRef popParam();
		virtual const ExecutableUnitRef& getChild(int index) const;
		virtual ExecutableUnitRef& getChild(int index);
		virtual int getChildCount();
		FunctionRef& getConstructorUnit();
	};

	class FwdCompositeConstrutorUnit :
		public FwdConstrutorUnit
	{
		std::vector<ScriptTypeRef> _argumentTypes;
		ParamCastingList _castingList;
	public:
		FwdCompositeConstrutorUnit(const FunctionRef& constructorUnit, const std::vector<ScriptTypeRef>& argumentTypes, const ParamCastingList& castingList);
		virtual ~FwdCompositeConstrutorUnit();

		virtual int pushParam(ExecutableUnitRef pExeUnit);
	};
}